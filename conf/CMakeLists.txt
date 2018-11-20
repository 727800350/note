## used by clion
cmake_minimum_required(VERSION 3.10)
project(projet)

set(CMAKE_CXX_STANDARD 11)

include_directories(./)

## add recursively source files to build list
file(GLOB_RECURSE src_file "*.cc" "*.cpp" "*.h")

add_executable(projet ${src_file})

