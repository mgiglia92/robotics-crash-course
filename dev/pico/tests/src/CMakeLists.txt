add_executable(picow_udp_beacon_background
        picow_udp_beacon.c
        )
target_compile_definitions(picow_udp_beacon_background PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(picow_udp_beacon_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${PICO_SDK_PATH}
        )
target_link_libraries(picow_udp_beacon_background
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        )

pico_add_extra_outputs(picow_udp_beacon_background)

pico_enable_stdio_usb(picow_udp_beacon_background 1)
pico_enable_stdio_uart(picow_udp_beacon_background 0)

