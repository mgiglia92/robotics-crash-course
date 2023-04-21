add_executable(picow_udp_beacon_background
        picow_udp_beacon.c
        )
target_compile_definitions(picow_udp_beacon_background PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        # PICO_CYW43_ARCH_POLL
        PICO_CYW43_ARCH_BACKGROUND
        CYW43_HOST_NAME=\"${CYW43_HOST_NAME}\"
        )
target_include_directories(picow_udp_beacon_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${PICO_SDK_PATH}
        ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include       
        ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include
        ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include
        ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_pwm/include
        ${PICO_SDK_PATH}/lib/cyw43-driver/src
        ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include
        ${PICO_SDK_PATH}/lib/lwip/src/include
        ../include
        ../lib
        )


target_link_libraries(picow_udp_beacon_background
        pico_cyw43_arch_lwip_threadsafe_background
        # pico_cyw43_arch_lwip_poll        
        pico_stdlib
        )

pico_add_extra_outputs(picow_udp_beacon_background)

pico_enable_stdio_usb(picow_udp_beacon_background 1)
pico_enable_stdio_uart(picow_udp_beacon_background 0)

