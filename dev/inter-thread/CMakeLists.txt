cmake_minimum_required(VERSION 3.13)

# initialize the project
project(inter-thread C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# pull in external libraries
add_subdirectory(../../lib lib)

add_subdirectory(src)
