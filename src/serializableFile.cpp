#include "serializableFile.hpp"
#include <fstream>

serializableFile::serializableFile(std::string const &fileName) : filePath(fileName)
{
}

serializableFile::~serializableFile()
{
}

std::vector<uint8_t> serializableFile::serialize()
{
    size_t fileSize = this->getFileSize();

    return readFileFromPosition(0, fileSize);
}

size_t serializableFile::getFileSize() const
{
    std::ifstream fileStream(filePath, std::ios::ate | std::ios::binary);
    size_t fileSize = fileStream.tellg();
    fileStream.close();

    return fileSize;
}

std::vector<uint8_t> serializableFile::readFileFromPosition(size_t position, size_t length)
{
    std::ifstream workFile(this->filePath, std::ios::in | std::ios::binary);
    workFile.seekg(position);

    std::vector<uint8_t> fileContent;
    fileContent.resize(length);
    workFile.read(reinterpret_cast<char *>(fileContent.data()), length);
    workFile.close();

    return fileContent;
}