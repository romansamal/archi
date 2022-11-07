#include "huffman.hpp"
#include <algorithm>
#include <queue>
#include <sstream>
#include <execution>
#include <boost/range/irange.hpp>
#include <boost/dynamic_bitset.hpp>

typedef union _huffmanHeader_u
{
    typedef struct _huffmanHeader_s
    {
        uint32_t version;
        uint32_t codingTableOffset;
        uint32_t dataOffset;
        uint32_t crc32;
    } huffmanHeader_s;
    huffmanHeader_s header;

    uint8_t raw[sizeof(huffmanHeader_s)];
} huffmanHeader_u;

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

huffman::blockBuffer encode(
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

    
}