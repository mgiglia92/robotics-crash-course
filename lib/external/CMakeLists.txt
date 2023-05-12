cmake_minimum_required(VERSION 3.13)

# import the Pico SDK (with some options)
set(ENV{PICO_SDK_PATH} ../../pico-sdk/)

# allow using more than four shared IRQ handlers (necessary for ROS)
add_definitions(-DPICO_MAX_SHARED_IRQ_HANDLERS=16u)
include(pico_sdk_import.cmake)

# initialize the project
project(carrie-firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

# TODO: think about moving this definition
add_definitions(-DCARRIE_DEBUG)

include_directories(lib/)
add_subdirectory(lib)

add_subdirectory(src)
