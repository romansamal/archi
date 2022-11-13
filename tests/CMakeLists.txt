find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

set(SOURCES_UT_DIR ../src)

set(SOURCES_SERIALIZABLE_TEST
	serializableFile_test.cpp)

set(SOURCES_HUFFMANBN_TEST
	huffmanBinaryNode_test.cpp)

set(SOURCES_HUFFMAN_TEST
	huffman_test.cpp)

include_directories(${SOURCES_UT_DIR})

add_executable(serializableFile_test
	${SOURCES_UT_DIR}/serializableFile.cpp
	${SOURCES_UT_DIR}/serializable.cpp
	${SOURCES_UT_DIR}/objectID.cpp
	${SOURCES_SERIALIZABLE_TEST})

add_executable(huffmanBinaryNode_test
	${SOURCES_HUFFMANBN_TEST})

add_executable(huffman_test
	${SOURCES_UT_DIR}/huffman.cpp
	${SOURCES_HUFFMAN_TEST})

target_link_libraries(
	serializableFile_test
	GTest::GTest
)

target_link_libraries(
	huffmanBinaryNode_test
	GTest::GTest
	Boost::Boost
)

target_link_libraries(
	huffman_test
	GTest::GTest
	Boost::Boost
)

add_test(serializableTest serializableFile_test)
add_test(huffmanBinaryNodeTest huffmanBinaryNode_test)
add_test(huffman huffman_test)