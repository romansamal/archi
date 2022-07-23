# Arch

## How to build
    mkdir build && cd build
    conan install .. --build=missing
    cmake -DCMAKE_MODULE_PATH="%cd%" ..
    cmake --build .

## Algorithm

### Huffman coding

1. Calculate frequency for every char from input data.

2. Create Huffman binary node for every char.

3. Build priority queue with Huffman binary nodes. The less frequency - the more priority.

4. Build Huffman binary tree (HBT). While (queue > 1) -> pop 2 elements, build new node with 2 childs -> push back to priority queue.

5. Build Huffman coding table from HBT.