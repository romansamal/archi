#include "huffman.hpp"
#include <algorithm>
#include <queue>
#include <sstream>
#include <execution>
#include <boost/range/irange.hpp>


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