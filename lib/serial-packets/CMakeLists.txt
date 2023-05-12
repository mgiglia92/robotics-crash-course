add_library(serial-packets
	packet.c++
	messages.c++
)

target_include_directories(serial-packets INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(serial-packets
#	pico_stdlib
	cpp-base64
	hash-library-sha1
)
