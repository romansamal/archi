#include <gtest/gtest.h>
#include "huffmanBinaryNode.hpp"

namespace
{
    class huffmanBinaryNodeTest : public testing::Test
    {
        protected:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
    };

    TEST_F(huffmanBinaryNodeTest, checkParrentFrequency1)
    {
        huffmanBinaryNode<int> nodeLeft(1, 1);
        huffmanBinaryNode<int> nodeRight(2, 4);
        huffmanBinaryNode<int> nodeParrent(std::move(nodeLeft), std::move(nodeRight));

        EXPECT_EQ(nodeParrent.getFrequency(), 5);
        EXPECT_EQ(nodeParrent.getData(), std::nullopt);
        EXPECT_EQ(nodeLeft.getData(), 1);
        EXPECT_EQ(nodeRight.getData(), 2);
    }

    TEST_F(huffmanBinaryNodeTest, checkParrentFrequency2)
    {
        huffmanBinaryNode<int> node1(1, 1);
        huffmanBinaryNode<int> node2(2, 4);
        huffmanBinaryNode<int> node3(3, 2);
        huffmanBinaryNode<int> node4(4, 8);
        huffmanBinaryNode<int> node5(5, 4);
        huffmanBinaryNode<int> node6(6, 16);
        huffmanBinaryNode<int> node7(7, 8);
        huffmanBinaryNode<int> node8(8, 32);

        huffmanBinaryNode<int> node12(std::move(node1), std::move(node2));
        huffmanBinaryNode<int> node34(std::move(node3), std::move(node4));
        huffmanBinaryNode<int> node56(std::move(node5), std::move(node6));
        huffmanBinaryNode<int> node78(std::move(node7), std::move(node8));

        huffmanBinaryNode<int> node1234(std::move(node12), std::move(node34));
        huffmanBinaryNode<int> node5678(std::move(node56), std::move(node78));

        huffmanBinaryNode<int> node12345678(std::move(node1234), std::move(node5678));

        EXPECT_EQ(node12345678.getFrequency(), 75);
    }

    TEST_F(huffmanBinaryNodeTest, checkIterator)
    {
        huffmanBinaryNode<int> node1(1, 1);
        huffmanBinaryNode<int> node2(2, 4);
        huffmanBinaryNode<int> node12(std::move(node1), std::move(node2));

        huffmanBinaryNode<int>::Iterator iter(node12);

        iter.begin();

        auto node = *iter;
        EXPECT_EQ(node.getData(), 2);
        ++iter;
        node = *iter;
        EXPECT_EQ(node.getData(), 1);    
    }
};