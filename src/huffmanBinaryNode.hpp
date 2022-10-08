#pragma once
#include <memory>
#include <optional>
#include <stack>
#include <boost/dynamic_bitset.hpp>

#ifdef DEBUG_OUTPUT
    #include <iostream>
    #define TRACE_DEBUG(MSG) (std::cout << MSG << std::endl)
#else
    #define TRACE_DEBUG(MSG)
#endif

template <typename T>
class huffmanBinaryNode
{
public:
    huffmanBinaryNode() : data(std::nullopt),
                          frequency(0),
                          leftChild(nullptr),
                          rightChild(nullptr)
    {
        TRACE_DEBUG("Default constructor is called");
    }

    explicit huffmanBinaryNode(T const &data) : data(data),
                                                frequency(0),
                                                leftChild(nullptr),
                                                rightChild(nullptr)
    {
        TRACE_DEBUG("Param constructor is called");
    }

    huffmanBinaryNode(T const &data, unsigned int frequency) : data(data),
                                                        frequency(frequency),
                                                        leftChild(nullptr),
                                                        rightChild(nullptr)
    {
        TRACE_DEBUG("Param constructor is called");
    }

    huffmanBinaryNode(huffmanBinaryNode<T> const &node) : data(node.data),
                                                          frequency(node.frequency)
    {
        TRACE_DEBUG("Copy constructor is called");

        if (node.leftChild != nullptr)
        {
            this->leftChild = std::make_shared<huffmanBinaryNode<T>>(*node.leftChild);
        }

        if (node.rightChild != nullptr)
        {
            this->rightChild = std::make_shared<huffmanBinaryNode<T>>(*node.rightChild);
        }
    }

    // copy and swap idiom
    huffmanBinaryNode<T> &operator=(huffmanBinaryNode<T> node2) noexcept
    {
        std::swap(this->leftChild, node2.leftChild);
        std::swap(this->rightChild, node2.rightChild);
        std::swap(this->frequency, node2.frequency);
        std::swap(this->data, node2.data);
        return *this;
    }

    huffmanBinaryNode(huffmanBinaryNode<T> &&node) : data(std::move(node.data)),
                                                     frequency(std::move(node.frequency)),
                                                     leftChild(std::move(node.leftChild)),
                                                     rightChild(std::move(node.rightChild))
    {
        TRACE_DEBUG("Move 1 constructor is called");
    }

    huffmanBinaryNode(huffmanBinaryNode<T> &&leftNode, huffmanBinaryNode<T> &&rightNode) : data(std::nullopt),
                                                                                           frequency(leftNode.frequency + rightNode.frequency),
                                                                                           leftChild(std::make_shared<huffmanBinaryNode<T>>(std::forward<huffmanBinaryNode<T>>(leftNode))),
                                                                                           rightChild(std::make_shared<huffmanBinaryNode<T>>(std::forward<huffmanBinaryNode<T>>(rightNode)))
    {
        TRACE_DEBUG("Move 2 constructor is called");
    }

    std::optional<T> const &getData() const
    {
        return this->data;
    }
    
    unsigned int const getFrequency() const
    {
        return this->frequency;
    }

    class Iterator
    {
    public:
        explicit Iterator(huffmanBinaryNode<T> const &rootNode) : root(std::make_shared<huffmanBinaryNode<T>>(rootNode))
        {
        }
        
        Iterator &begin()
        {
            boost::dynamic_bitset<uint8_t> code;
            this->eraseCodingTable();
            this->eraseContet();
            this->collectChildsAndCodes(root, code);
            return *this;
        }
                
        Iterator &operator++()
        {
            this->content.pop();
            return *this;
        }

        huffmanBinaryNode<T> &operator*()
        {
            return *(this->content.top());
        }

        bool hasNext()
        {
            return (this->content.size() != 0);
        }

        std::map<T, boost::dynamic_bitset<uint8_t>> getCodingTable()
        {
            boost::dynamic_bitset<uint8_t> code;
            this->eraseCodingTable();
            this->eraseContet();
            this->collectChildsAndCodes(this->root, code);
            return this->codingTable;
        }

    private:
        void collectChildsAndCodes(std::shared_ptr<huffmanBinaryNode<T>> const &node, boost::dynamic_bitset<uint8_t> &code)
        {
            if (node->leftChild == nullptr
                && node->rightChild == nullptr)
            {
                this->content.push(node);
                this->codingTable.insert( {node->getData().value(), code});
                return;
            }
            if (node->leftChild != nullptr)
            {
                code.push_back(0);
                this->collectChildsAndCodes(node->leftChild, code);
                code.pop_back();
            }
            if (node->rightChild != nullptr)
            {
                code.push_back(1);
                this->collectChildsAndCodes(node->rightChild, code);
                code.pop_back();
            }
        }

        void eraseContet()
        {
            while (this->content.size() > 0)
            {
                this->content.pop();
            }
        }

        void eraseCodingTable()
        {
            this->codingTable.clear();
        }

        std::stack<std::shared_ptr<huffmanBinaryNode<T>>> content;
        std::shared_ptr<huffmanBinaryNode<T>> root;

        std::map<T, boost::dynamic_bitset<uint8_t>> codingTable;
    };

private:
    std::optional<T> data;
    unsigned int frequency;
    std::shared_ptr<huffmanBinaryNode<T>> leftChild;
    std::shared_ptr<huffmanBinaryNode<T>> rightChild;
};