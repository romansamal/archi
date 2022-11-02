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

};