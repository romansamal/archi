#include <gtest/gtest.h>
#include "huffman.hpp"

namespace
{
    class huffmanTest : public testing::Test
    {
        protected:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
    };

    TEST_F(huffmanTest, calculateBlockFrequency_check)
    {
        constexpr unsigned buffer_size = 1024 * 10;
        constexpr unsigned buffer_period = 8;
        huffman::blockBuffer buffer(buffer_size);
        uint8_t charValue = 0;

        std::generate(buffer.begin(), buffer.end(), [&charValue]()
        {
            return (charValue++ % buffer_period);
        });

        huffman::blockFrequencyMap charMap = huffman::calculateBlockFrequency(buffer);

        for (auto &pair : charMap)
        {
            EXPECT_EQ(pair.second, buffer_size / buffer_period);
        }
    }

    TEST_F(huffmanTest, calculateBlockFrequency_empty)
    {
        huffman::blockBuffer buffer(0);

        huffman::blockFrequencyMap charMap = huffman::calculateBlockFrequency(buffer);

        EXPECT_EQ(charMap.size(), 0);
    }

    TEST_F(huffmanTest, calculateBlockFrequency_single)
    {
        constexpr uint8_t single_char = 14;
        constexpr unsigned buffer_size = 1024 * 10;
        huffman::blockBuffer buffer(buffer_size);

        std::fill(buffer.begin(), buffer.end(), single_char);

        huffman::blockFrequencyMap charMap = huffman::calculateBlockFrequency(buffer);

        EXPECT_EQ(charMap.size(), 1);
        EXPECT_EQ(charMap.begin()->first, single_char);
        EXPECT_EQ(charMap.begin()->second, buffer_size);
    }

    
    TEST_F(huffmanTest, buildTree_check)
    {
        huffman::blockFrequencyMap chars;

        chars['a'] = 1;
        chars['b'] = 2;
        chars['c'] = 3;

        auto resultTree = huffman::buildTree(chars);

        EXPECT_EQ(resultTree.getFrequency(), 6);
    }

    TEST_F(huffmanTest, encode_checkDecodedEqualInput)
    {
        huffman::blockBuffer buffer { 'a', 'b', 'c', 'a', 'b', 'c', 'c', 'c', 'c', 'c', 'a'};

        huffman::blockFrequencyMap chars;

        chars['a'] = std::count(buffer.begin(), buffer.end(), 'a');
        chars['b'] = std::count(buffer.begin(), buffer.end(), 'b');
        chars['c'] = std::count(buffer.begin(), buffer.end(), 'c');
        auto resultTree = huffman::buildTree(chars);

        auto codingTable = huffman::getCodingTable(resultTree);
        auto encoded = huffman::encode(buffer, codingTable);

        auto decoded = huffman::decode(encoded);

        EXPECT_EQ(decoded, buffer);
    }


    TEST_F(huffmanTest, encode_checkDecodedEqualInput_2)
    {
        huffman::blockBuffer buffer { 'h', 1, 2, 3, 4, 5};

        huffman::blockFrequencyMap chars;

        chars['h'] = std::count(buffer.begin(), buffer.end(), 'h');
        chars[1] = std::count(buffer.begin(), buffer.end(), 1);
        chars[2] = std::count(buffer.begin(), buffer.end(), 2);
        chars[3] = std::count(buffer.begin(), buffer.end(), 3);
        chars[4] = std::count(buffer.begin(), buffer.end(), 4);
        chars[5] = std::count(buffer.begin(), buffer.end(), 5);
        auto resultTree = huffman::buildTree(chars);

        auto codingTable = huffman::getCodingTable(resultTree);
        auto encoded = huffman::encode(buffer, codingTable);

        auto decoded = huffman::decode(encoded);

        EXPECT_EQ(decoded, buffer);
    }
};