include_directories(.)
add_library(comms comms/packet.c++ comms/messages.c++)
add_definitions(-D__BYTE_ORDER=99 -D__LITTLE_ENDIAN=99)


macro(make_exec exec_name exec_dir)
    add_executable(${exec_name} ${exec_dir})
    
    pico_add_extra_outputs(${exec_name})

    pico_enable_stdio_usb(${exec_name} 1)
    pico_enable_stdio_uart(${exec_name} 0)
    
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
                            # ${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include       
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/include
                            # ${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include
                            # ${PICO_SDK_PATH}/lib/cyw43-driver/src
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include
                            # ${PICO_SDK_PATH}/lib/lwip/src/include
                            # ${PICO_SDK_PATH}/src/rp2_common/pico_multicore/include
                            # ${PICO_SDK_PATH}/src/common/pico_util/include
                            ../include
                            ../lib
                            .
    )

    target_link_libraries(${exec_name}
                            pico_cyw43_arch_lwip_threadsafe_background
                            # pico_cyw43_arch_lwip_poll        
                            pico_stdlib
                            cpp-base64
                            hardware_adc
                            hardware_pwm
                            pico_multicore
                            comms
                            hash-library-sha1
    )



endmacro()

# make_exec(picow_udp_beacon_background picow_udp_beacon.cpp)
# make_exec(adc_test adc_test.cpp)
make_exec(wireless wireless_interface_test.cpp)