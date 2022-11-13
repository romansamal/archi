#pragma once
#include <memory>
#include <vector>

class archiveAlgorithm
{
public:
    virtual std::vector<char> archive(
        std::vector<char> const &obj) const = 0;

    virtual std::vector<char> unarchive(
        std::vector<char> const &archivedData) const = 0;

    virtual ~archiveAlgorithm() = 0;
};
