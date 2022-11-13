#pragma once
#include "archiveAlgorithm.hpp"

class huffmanArchiveAlgorithm : public archiveAlgorithm
{
public:
    huffmanArchiveAlgorithm();

    std::vector<char> archive(
        std::vector<char> const &obj) const override;

    std::vector<char> unarchive(
        std::vector<char> const &archivedData) const override;

    ~huffmanArchiveAlgorithm() override;
};