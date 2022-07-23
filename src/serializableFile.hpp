#pragma once
#include "serializable.hpp"
#include <string>

class serializableFile : public serializable
{
public:
    serializableFile() = delete;
    serializableFile(std::string const &fileName);
    ~serializableFile();

    std::vector<uint8_t> serialize() override;

private:
    size_t getFileSize() const;
    std::vector<uint8_t> readFileFromPosition(size_t position, size_t length);

    std::string const filePath;
};