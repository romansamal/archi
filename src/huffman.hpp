#pragma once
#include <vector>
#include <map>
#include "huffmanBinaryNode.hpp"

namespace huffman
{
    using blockFrequencyMap = std::map<uint8_t, unsigned int>;
    using blockBuffer = std::vector<uint8_t>;
    using hbt = huffmanBinaryNode<uint8_t>;
    using blockCodes = std::map<uint8_t, boost::dynamic_bitset<uint8_t>>;

    blockFrequencyMap calculateBlockFrequency(blockBuffer const &buffer);

};
