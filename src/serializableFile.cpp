#include "serializableFile.hpp"
#include <fstream>

serializableFile::serializableFile(
    std::string const &fileName) : filePath(fileName)
{
}

serializableFile::~serializableFile()
{
}

std::vector<uint8_t> serializableFile::serialize() const
{
    std::streampos fileSize = this->getFileSize();

    return readFileFromPosition(0, fileSize);
}

std::streampos serializableFile::getFileSize() const
{
    std::ifstream fileStream(filePath, std::ios::ate | std::ios::binary);
    std::streampos fileSize = fileStream.tellg();
    fileStream.close();

    return fileSize;
}

std::vector<uint8_t> serializableFile::readFileFromPosition(
    std::streampos position,
    std::streampos length) const
{
    std::ifstream workFile(this->filePath, std::ios::in | std::ios::binary);
    workFile.seekg(position);

    std::vector<uint8_t> fileContent;
    fileContent.resize(length);
    workFile.read(reinterpret_cast<char *>(fileContent.data()), length);
    workFile.close();

    return fileContent;
}

serializableFile::Iterator::value_type serializableFile::Iterator::operator*() const
{
    serializableFile::Iterator::value_type fileContent;
    fileContent.resize(this->blockSize);

    std::ifstream file(this->filePath, std::ios::binary);

    file.seekg(this->currentPosition);
    file.read(reinterpret_cast<char *>(fileContent.data()), this->blockSize);
    file.close();

    return fileContent;
}

serializableFile::Iterator serializableFile::Iterator::operator++()
{
    this->currentPosition += this->blockSize;
    return *this;
}

serializableFile::Iterator serializableFile::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool serializableFile::Iterator::operator==(
    serializableFile::Iterator const &b) const
{
    return (this->currentPosition == b.currentPosition);
}

bool serializableFile::Iterator::operator!=(
    serializableFile::Iterator const &b) const
{
    return (this->currentPosition != b.currentPosition);
}

serializableFile::Iterator::Iterator(
    std::string const &filePath,
    std::streampos blockSize,
    std::streampos position) : filePath(filePath),
                               blockSize(blockSize),
                               currentPosition(position)
{
}

serializableFile::Iterator serializableFile::begin()
{
    return serializableFile::Iterator(
        this->filePath,
        serializableFileConst::blockSize::DEFAULT_SIZE,
        0);
}

serializableFile::Iterator serializableFile::end()
{
    return serializableFile::Iterator(
        this->filePath,
        serializableFileConst::blockSize::DEFAULT_SIZE,
        this->getFileSize());
}