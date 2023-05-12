add_library(serial-packets
	packet.c++
	messages.c++
)

target_link_libraries(serial-packets
#	pico_stdlib
	cpp-base64
	hash-library-sha1
)
