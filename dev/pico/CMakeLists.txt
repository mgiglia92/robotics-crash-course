cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11) 
set(CMAKE_CXX_STANDARD 17)


set(PICO_BOARD pico_w) # Force pico_w so onboard led and wireless sdk stuff is built
include(pico_sdk_import.cmake)
project(rcc_pico_dev C CXX ASM)
pico_sdk_init()

add_subdirectory(include)
add_subdirectory(../../lib lib)
add_subdirectory(sensors)
#add_subdirectory(actuators)
add_subdirectory(util)

macro(make_exec exec_name exec_dir)
    add_executable(${exec_name} ${exec_dir})
    
    pico_add_extra_outputs(${exec_name})

    pico_enable_stdio_usb(${exec_name} 1)
    pico_enable_stdio_uart(${exec_name} 0)
    
# For debugging with openocd (comment out above)
    # pico_enable_stdio_usb(${exec_name} 0)
    # pico_enable_stdio_uart(${exec_name} 1)

# Allow hash-library-sha1 to find endian.h which is in include directory
    target_include_directories(hash-library-sha1 PUBLIC include)

    target_compile_definitions(${exec_name} PUBLIC
                        # Set WIFI parameters with cmake cmd
                            WIFI_SSID=\"${WIFI_SSID}\"
                            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
                            CYW43_HOST_NAME=\"${CYW43_HOST_NAME}\"

                        # Forced values for WIFI params
                            # WIFI_SSID=<"MY_WIFI_ROUTER"
                            # WIFI_PASSWORD="MY_WIFI_ROUTER_PASS"
                            # CYW43_HOST_NAME="NAME_I_WANT MY PICO TO HAVE ON NETWORK"

                        # Pico Sdk
                            PICO_MAX_SHARED_IRQ_HANDLERS=6 #IMPORTANT!! It seems default shared irq is 2, we need more.
                            
                        # Related to wireless capabilities
                            # PICO_CYW43_ARCH_BACKGROUND
    )

    target_include_directories(${exec_name} PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ${PICO_SDK_PATH}
                            wireless/include #TODO: Turn into CMake compatible directory (lib)
                            include
                            
    )

    target_link_libraries(${exec_name}
                            # pico_cyw43_arch_lwip_threadsafe_background # For wireless
                            pico_cyw43_arch_none
                            pico_stdlib
                            hardware_adc
                            hardware_pwm
                            # pico_multicore
                            # serial-packets
                            hardware_i2c
                            sensors
                            util
    )



endmacro()

# make_exec(picow_udp_beacon_background picow_udp_beacon.cpp)
# make_exec(adc_test src/misc/adc_test.cpp)
# make_exec(wireless tests/wireless_interface_test.cpp)
make_exec(mpu6050_test tests/mpu_6050_test.cpp)
make_exec(lidar_test tests/lidar_test.cpp)
make_exec(multi_i2c_test tests/multi_i2c_test.cpp)
make_exec(hcsr04_test tests/hcsr04_test.cpp)
make_exec(odom_test tests/odom_test.cpp)
make_exec(servo_pot_test tests/servo_pot_test.cpp)
make_exec(motor_test tests/motor_test.cpp)
make_exec(frequency_test tests/frequency_test.cpp)
make_exec(control_test tests/control_test.cpp)
make_exec(all_sensor_test tests/all_sensor_test.cpp)
make_exec(vel_control_test tests/vel_control_test.cpp)
