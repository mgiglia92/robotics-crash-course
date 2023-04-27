# add_executable(picow_udp_beacon_background
#         picow_udp_beacon.cpp
#         )
# # add_executable(test_main test_main.c)

# target_compile_definitions(picow_udp_beacon_background PRIVATE
#         WIFI_SSID=\"${WIFI_SSID}\"
#         WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
#         # PICO_CYW43_ARCH_POLL
#         PICO_CYW43_ARCH_BACKGROUND
#         CYW43_HOST_NAME=\"${CYW43_HOST_NAME}\"
#         )
# target_include_directories(picow_udp_beacon_background PRIVATE
#         ${CMAKE_CURRENT_LIST_DIR}
#         ${PICO_SDK_PATH}
#         ${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include       
#         ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
#         ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include
#         ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/include
#         ${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include
#         ${PICO_SDK_PATH}/lib/cyw43-driver/src
#         ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include
#         ${PICO_SDK_PATH}/lib/lwip/src/include
#         ../include
#         ../lib
#         )

# target_link_libraries(picow_udp_beacon_background
#         pico_cyw43_arch_lwip_threadsafe_background
#         # pico_cyw43_arch_lwip_poll        
#         pico_stdlib
#         cpp-base64
#         # hash-library-sha1
#         )

# pico_add_extra_outputs(picow_udp_beacon_background)

# pico_enable_stdio_usb(picow_udp_beacon_background 1)
# pico_enable_stdio_uart(picow_udp_beacon_background 0)

macro(make_exec exec_name exec_dir)
    add_executable(${exec_name} ${exec_dir})
    
    target_compile_definitions(${exec_name} PRIVATE
                            WIFI_SSID=\"${WIFI_SSID}\"
                            WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
                            # PICO_CYW43_ARCH_POLL
                            PICO_CYW43_ARCH_BACKGROUND
                            CYW43_HOST_NAME=\"${CYW43_HOST_NAME}\"
    )
    target_include_directories(${exec_name} PRIVATE
                            ${CMAKE_CURRENT_LIST_DIR}
                            ${PICO_SDK_PATH}
                            ${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include
                            ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include       
                            ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
                            ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include
                            ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/include
                            ${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include
                            ${PICO_SDK_PATH}/lib/cyw43-driver/src
                            ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include
                            ${PICO_SDK_PATH}/lib/lwip/src/include
                            ${PICO_SDK_PATH}/src/rp2_common/pico_multicore/include
                            ${PICO_SDK_PATH}/src/common/pico_util/include
                            ../include
                            ../lib
    )

    target_link_libraries(${exec_name}
                            pico_cyw43_arch_lwip_threadsafe_background
                            # pico_cyw43_arch_lwip_poll        
                            pico_stdlib
                            cpp-base64
                            hardware_adc
                            hardware_pwm
                            pico_multicore
                            
                            # hash-library-sha1
    )

    pico_add_extra_outputs(${exec_name})

    pico_enable_stdio_usb(${exec_name} 1)
    pico_enable_stdio_uart(${exec_name} 0)

endmacro()

make_exec(picow_udp_beacon_background picow_udp_beacon.cpp)
make_exec(adc_test adc_test.cpp)
