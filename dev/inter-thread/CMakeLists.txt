cmake_minimum_required(VERSION 3.13)

# initialize the project
project(inter-thread C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Debug)
find_package(Threads REQUIRED)

# pull in external libraries
add_subdirectory(../../lib lib)

# set up for the program
add_executable(inter-thread
	inter-thread.c++
	random-words.c++
)

target_link_libraries(inter-thread
	serial-packets
	${CMAKE_THREAD_LIBS_INIT}
)
