#include "huffmanArchiveAlgorithm.hpp"
#include "huffman.hpp"

huffmanArchiveAlgorithm::huffmanArchiveAlgorithm()
{
}

huffmanArchiveAlgorithm::~huffmanArchiveAlgorithm()
{
}

std::vector<char> huffmanArchiveAlgorithm::archive(
        std::vector<char> const &obj) const
{
    auto resultTree = huffman::buildTree(obj);
    auto codingTable = huffman::getCodingTable(resultTree);
    return huffman::encode(buffer, codingTable);
}

std::vector<char> huffmanArchiveAlgorithm::unarchive(
        std::vector<char> const &obj) const
{
    return huffman::decode(obj);
}