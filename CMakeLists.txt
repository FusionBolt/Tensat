cmake_minimum_required(VERSION 3.16)

project(Tensat)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)
add_subdirectory(test)