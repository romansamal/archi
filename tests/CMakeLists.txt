find_package(GTest REQUIRED)

set(SOURCES_UT_DIR ../src)

set(SOURCES_TEST
	serializableFile_test.cpp)

include_directories(${SOURCES_UT_DIR})

add_executable(serializableFile_test
	${SOURCES_UT_DIR}/serializableFile.cpp
	${SOURCES_UT_DIR}/serializable.cpp
	${SOURCES_TEST})

target_link_libraries(
	serializableFile_test
	GTest::GTest
)

add_test(serializableTest serializableFile_test)