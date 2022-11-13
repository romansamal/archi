#pragma once
#include "serializable.hpp"
#include "archiveAlgorithm.hpp"
#include <vector>

class archiver
{
public:
    explicit archiver(archiveAlgorithm const *algorithm);

    Identificator add(serializable const &item);

    void remove(Identificator const &item);

    std::vector<Identificator> list() const;


private:
    archiveAlgorithm const *algorithm;

};