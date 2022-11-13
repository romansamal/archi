#pragma once
#include <string>

struct Identificator
{
};

class objectID
{
public:
    virtual ~objectID() = 0;

    virtual std::string getName() = 0;

    virtual Identificator getID() = 0;
};