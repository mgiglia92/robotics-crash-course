add_library(comms
	packet.c++
	messages.c++
)

target_link_libraries(comms pico_stdlib cpp-base64 hash-library-sha1)
