cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
#include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
pico_sdk_init()

project(picow_udp_beacon_background)
add_subdirectory(./src)
# include_directories(./lib)
# include_directories(./include)