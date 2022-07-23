#pragma once
#include <vector>
#include <cstdint>

class serializable
{
	class Iterator
	{
		virtual ~Iterator() = 0;
		virtual Iterator &begin() = 0;
		virtual Iterator &end() = 0;
		
		virtual std::vector<uint8_t> &operator*() = 0;
		virtual bool operator==(Iterator &operand) = 0;
	};

	virtual ~serializable() = 0;
	virtual std::vector<uint8_t> serialize() = 0;
};