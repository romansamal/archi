#pragma once
#include <memory>
#include <vector>

class archiveAlgorithm
{
public:
    virtual std::vector<uint8_t> archive(
        std::vector<uint8_t> const &obj) const = 0;
    virtual std::vector<uint8_t> unarchive(
        std::vector<uint8_t> const &archivedData) const = 0;
    virtual ~archiveAlgorithm() = 0;
};
