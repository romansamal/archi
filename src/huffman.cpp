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