project(archi)
cmake_minimum_required(VERSION 3.10)

enable_language(CXX)
enable_testing()

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(tests)
