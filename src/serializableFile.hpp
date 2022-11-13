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

    std::vector<char> serialize() const override;

    std::string getName() override;

    Identificator getID() override;

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::vector<char>;
        using pointer = std::vector<char> *;
        using reference = std::vector<char> &;
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
    std::vector<char> readFileFromPosition(
        std::streampos position, 
        std::streampos length) const;

    std::string const filePath;
};