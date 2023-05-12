add_library(rcc
	actuator/motor.cpp
	util/differentiator.cpp
	util/pid-control.cpp
)

target_link_libraries(rcc pico_stdlib hardware_pwm)
