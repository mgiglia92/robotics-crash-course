add_library(sensors mpu6050.cpp VL53L0X.cpp
)

target_link_libraries(sensors hardware_i2c)