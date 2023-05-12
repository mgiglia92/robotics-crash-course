add_executable(inter-thread
	main.c++
)

target_link_libraries(inter-thread
	serial-packets
)
