add_library(sensors mpu6050.cpp VL53L0X.cpp hc-sr04.cpp
)

target_link_libraries(sensors pico_stdlib hardware_gpio hardware_i2c)