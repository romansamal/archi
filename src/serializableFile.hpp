#pragma once
#include "serializable.hpp"
#include <string>

namespace serializableFileConst
{
    enum blockSize
    {
        DEFAULT_SIZE = 256,
    };
};

class serializableFile : public serializable
{
public:
    serializableFile() = delete;
    explicit serializableFile(std::string const &fileName);
    ~serializableFile() override;

    std::vector<uint8_t> serialize() const override;

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::vector<uint8_t>;
        using pointer = std::vector<uint8_t> *;
        using reference = std::vector<uint8_t> &;
public:
        Iterator(
            std::string const &filePath, 
            std::streampos blockSize, 
            std::streampos position);

        value_type operator*() const;

        Iterator operator++();

        Iterator operator++(int);

        bool operator==(
            Iterator const &b) const;

        bool operator!=(
            Iterator const &b) const;

        private:
            std::streampos currentPosition;
            std::streampos const blockSize;
            std::string const filePath;
    };
    
    Iterator begin() const;

    Iterator end() const;

private:
    std::streampos getFileSize() const;
    std::vector<uint8_t> readFileFromPosition(
        std::streampos position, 
        std::streampos length) const;

    std::string const filePath;
};