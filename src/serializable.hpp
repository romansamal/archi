#pragma once
#include <vector>
#include <cstdint>
#include "objectID.hpp"

class serializable : public objectID
{
public:
	virtual ~serializable() = 0;
	virtual std::vector<char> serialize() const = 0;
};
