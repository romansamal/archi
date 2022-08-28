#pragma once
#include "serializable.hpp"
#include <string>

enum blockSize
{
    DEFAULT_SIZE = 256,
};

class serializableFile : public serializable
{
public:
    serializableFile() = delete;
    serializableFile(std::string const &fileName);
    ~serializableFile();

    std::vector<uint8_t> serialize() override;

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::vector<uint8_t>;
        using pointer = std::vector<uint8_t> *;
        using reference = std::vector<uint8_t> &;
public:
        Iterator(std::string filePath, std::streampos blockSize, std::streampos position);

        value_type operator*();

        Iterator operator++();

        Iterator operator++(int);

        bool operator==(Iterator b);

        bool operator!=(Iterator b);

        private:
            std::streampos currentPosition;
            std::streampos const blockSize;
            std::string const filePath;
    };
    
    Iterator begin();

    Iterator end();

private:
    std::streampos getFileSize() const;
    std::vector<uint8_t> readFileFromPosition(std::streampos position, std::streampos length);

    std::string const filePath;
};