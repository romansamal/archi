#include "huffman.hpp"
#include <algorithm>
#include <queue>
#include <sstream>
#include <execution>
#include <boost/range/irange.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/crc.hpp>
#include <boost/iostreams/stream.hpp>

namespace huffman
{

    struct huffmanHeader_s
    {
        huffmanHeader_s()
        {
        }

        huffmanHeader_s(uint32_t sizeBits, uint32_t crc32) : dataSizeInBits(sizeBits), crc32(crc32)
        {
        }

        // CRTP boost serialization
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            switch (version)
            {
            case 0:
                ar &dataSizeInBits;
                ar &crc32;
                break;
            }
        }

        uint32_t dataSizeInBits;
        uint32_t crc32;
    };

    typedef std::vector<char> ArchivedBuffer;
    typedef boost::iostreams::stream<boost::iostreams::back_insert_device<ArchivedBuffer>>
        OBufferStream;
    typedef boost::archive::binary_oarchive OArchive;

    template <typename T>
    static void archiveValue(
        OArchive &arch,
        const T &value);

    template <typename First, class... Rest>
    static void archiveValue(
        OArchive &arch,
        const First &val,
        const Rest &...value);

    template <class... Rest>
    static void archiveValue(
        std::vector<char> &buf,
        const Rest &...value);

    template <typename T>
    static void unarchiveValue(
        boost::archive::binary_iarchive &arch,
        T &val);

    template <typename First, typename... Rest>
    static void unarchiveValue(
        boost::archive::binary_iarchive &arch,
        First &val,
        Rest &...value);

    template <typename... Rest>
    static void unarchiveValue(
        std::vector<char> const &buf,
        Rest &...value);

    static huffmanHeader_s calculateHeader(
        std::vector<char> const &buf,
        uint32_t sizeInBits);

    static uint32_t calculateCRC32(
        std::vector<char> const &buf);

};

huffman::blockFrequencyMap huffman::calculateBlockFrequency(huffman::blockBuffer const &buffer)
{
    huffman::blockFrequencyMap blockFrequencies;
    for (uint8_t i : boost::irange(0, 256))
    {
        if (std::find(std::execution::par, buffer.begin(), buffer.end(), i) != buffer.end())
        {
            blockFrequencies.insert(
                {i, static_cast<unsigned>(std::count(std::execution::par, buffer.begin(), buffer.end(), i))});
        }
    }
    return blockFrequencies;
}

huffman::hbt huffman::buildTree(huffman::blockFrequencyMap const &blockFrequencies)
{
    std::vector<huffman::hbt> huffmanVector;
    for (auto &node : blockFrequencies)
    {
        huffman::hbt huffmanNode(node.first, node.second);
        huffmanVector.push_back(std::move(huffmanNode));
    }

    struct huffmanComparer
    {
        bool operator()(huffman::hbt const &left, huffman::hbt const &right) const
        {
            return left.getFrequency() > right.getFrequency();
        }
    };

    std::priority_queue<huffman::hbt,
                        std::vector<huffmanBinaryNode<uint8_t>>,
                        huffmanComparer>
        huffmanPriorityQueue{huffmanComparer{}, std::move(huffmanVector)};

    while (huffmanPriorityQueue.size() > 1)
    {
        auto elemLeft = std::move(huffmanPriorityQueue.top());
        huffmanPriorityQueue.pop();
        auto elemRight = huffmanPriorityQueue.top();
        huffmanPriorityQueue.pop();
        huffman::hbt node(std::move(elemLeft), std::move(elemRight));
        huffmanPriorityQueue.push(node);
    }

    return huffmanPriorityQueue.top();
}

huffman::blockCodingTable huffman::getCodingTable(huffman::hbt const &tree)
{
    auto iter = hbt::Iterator(tree);

    return iter.getCodingTable();
}

huffman::blockDecodingTable huffman::getDecodingTable(huffman::blockCodingTable const &table)
{
    huffman::blockDecodingTable decodingTable;
    for (auto &code : table)
    {
        std::string codeString;
        boost::to_string(code.second, codeString);
        decodingTable.insert({codeString, code.first});
    }

    return decodingTable;
}

huffman::blockBuffer huffman::encode(
    huffman::blockBuffer const &buffer,
    huffman::blockCodingTable const &codingTable)
{
    boost::dynamic_bitset<uint8_t> encodedBits;
    encodedBits.reserve(buffer.size());

    for (auto &ch : buffer)
    {
        auto position = codingTable.find(ch);

        if (position != codingTable.end())
        {
            for (int bitNum = 0; bitNum < position->second.size(); bitNum++)
            {
                encodedBits.push_back(position->second[bitNum]);
            }
        }
    }

    std::vector<char> encodedBitsBuffer;
    encodedBitsBuffer.reserve(encodedBits.num_blocks());

    boost::to_block_range(encodedBits, std::back_inserter(encodedBitsBuffer));

    huffmanHeader_s header = huffman::calculateHeader(encodedBitsBuffer, encodedBits.size());

    huffman::blockDecodingTable decodingTable = huffman::getDecodingTable(codingTable);

    std::vector<char> archivedBuffer;
    archivedBuffer.reserve(buffer.size());

    archiveValue(archivedBuffer, header, decodingTable);

    archivedBuffer.insert(archivedBuffer.end(), encodedBitsBuffer.begin(), encodedBitsBuffer.end());

    return archivedBuffer;
}

huffman::blockBuffer huffman::decode(
    huffman::blockBuffer const &buffer)
{
    huffmanHeader_s header;
    huffman::blockDecodingTable decodingTable;

    unarchiveValue(buffer, header, decodingTable);

    boost::dynamic_bitset<uint8_t> bits;

    const int offset = header.dataSizeInBits % 8;
    const int bits_in_byte = 8;

    bits.init_from_block_range(buffer.end() - (header.dataSizeInBits / bits_in_byte) - (offset != 0), buffer.end());

    std::string word;
    huffman::blockBuffer result;
    result.reserve(buffer.size());

    for (unsigned index = 0; index < header.dataSizeInBits; index++)
    {
        unsigned bitVal = bits[index];
        word.insert(0, bits[index] ? "1" : "0");

        auto found = decodingTable.find(word);
        if (found != decodingTable.end())
        {
            word.erase();
            result.push_back(found->second);
        }
    }

    return result;
}

namespace huffman
{
    template <typename T>
    static void archiveValue(
        OArchive &arch,
        const T &value)
    {
        arch << value;
    }

    template <typename First, typename... Rest>
    static void archiveValue(
        OArchive &arch,
        const First &val,
        const Rest &...value)
    {
        arch << val;
        archiveValue(arch, value...);
    }

    template <typename... Rest>
    static void archiveValue(
        std::vector<char> &buf,
        const Rest &...value)
    {
        OBufferStream outStream(buf);
        OArchive archi(outStream);

        archiveValue(archi, value...);

        return;
    }

    template <typename T>
    static void unarchiveValue(
        boost::archive::binary_iarchive &arch,
        T &val)
    {
        arch >> val;
    }

    template <typename First, typename... Rest>
    static void unarchiveValue(
        boost::archive::binary_iarchive &arch,
        First &val,
        Rest &...value)
    {
        arch >> val;
        unarchiveValue(arch, value...);
    }

    template <typename... Rest>
    static void unarchiveValue(
        std::vector<char> const &buf,
        Rest &...value)
    {
        boost::iostreams::basic_array_source<char> device(buf.data(), buf.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char>> s(device);
        boost::archive::binary_iarchive ia(s);

        unarchiveValue(ia, value...);

        return;
    }

    static huffmanHeader_s calculateHeader(
        std::vector<char> const &buf,
        uint32_t sizeInBits)
    {
        return huffmanHeader_s(sizeInBits, huffman::calculateCRC32(buf));
    }

    static uint32_t calculateCRC32(
        std::vector<char> const &buf)
    {
        boost::crc_32_type crc_calc;

        crc_calc.process_bytes(buf.data(), buf.size());

        return crc_calc.checksum();
    }

};
