add_library(sensors mpu6050.cpp
)

target_link_libraries(sensors hardware_i2c)