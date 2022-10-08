#pragma once
#include <vector>
#include <cstdint>

class serializable
{
public:
	virtual ~serializable() = 0;
	virtual std::vector<uint8_t> serialize() const = 0;
};
