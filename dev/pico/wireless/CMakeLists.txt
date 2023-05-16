cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11) 
set(CMAKE_CXX_STANDARD 17)


include(pico_sdk_import.cmake)
project(picow_udp_beacon_background C CXX ASM)
pico_sdk_init()
# include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

add_subdirectory(../../../lib lib)

macro(make_exec exec_name exec_dir)
    add_executable(${exec_name} ${exec_dir})
    
    pico_add_extra_outputs(${exec_name})

    pico_enable_stdio_usb(${exec_name} 1)
    pico_enable_stdio_uart(${exec_name} 0)
    
    # Allow hash-library-sha1 to find endian.h which is in include directory
    target_include_directories(hash-library-sha1 PUBLIC include)

    target_compile_definitions(${exec_name} PUBLIC
                            WIFI_SSID=\"${WIFI_SSID}\"
                            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
                            # PICO_CYW43_ARCH_POLL
                            PICO_CYW43_ARCH_BACKGROUND
                            CYW43_HOST_NAME=\"${CYW43_HOST_NAME}\"
    )

    target_include_directories(${exec_name} PUBLIC
                            ${CMAKE_CURRENT_LIST_DIR}
                            ${PICO_SDK_PATH}
                            ./include
    )

    target_link_libraries(${exec_name}
                            pico_cyw43_arch_lwip_threadsafe_background
                            pico_stdlib
                            hardware_adc
                            hardware_pwm
                            pico_multicore
                            serial-packets
    )



endmacro()

# make_exec(picow_udp_beacon_background picow_udp_beacon.cpp)
# make_exec(adc_test src/misc/adc_test.cpp)
make_exec(wireless wireless_interface_test.cpp)