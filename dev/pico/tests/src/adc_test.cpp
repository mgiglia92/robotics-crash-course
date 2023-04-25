/**
 * Copyright (c) 2022 Andrew McDonnell
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <include_test.h>
#include <stdlib.h>
#include "pico/stdlib.h"    
#include "wireless_comms.h"
#include "Servo.h"
#include "hardware/adc.h"
// #include "hash-library/sha1.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.1.35"
#define IP_RECV "192.168.1.33"
#define BEACON_INTERVAL_MS 100

#define ADC0 26

// SHA1 sha1;
using namespace std;

bool led_state = true;
//Servo Init
Servo s1;

/* References for this implementation:
 * raspberry-pi-pico-c-sdk.pdf, Section '4.1.1. hardware_adc'
 * pico-examples/adc/adc_console/adc_console.c */
float convert_temperature(uint16_t raw) {
    
    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)raw * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}



int main() {
    stdio_init_all();

    sleep_ms(1000);

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    

    //ADC stuff
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_gpio_init(29);
    adc_set_temp_sensor_enabled(true);
    uint64_t t0, t1, dt;
    while(true)
    {   
        t0 = time_us_64();

        adc_select_input(2);
        auto x = adc_read();
        auto y = gpio_get(22);
        adc_select_input(3);
        auto temp = convert_temperature(adc_read());
        t1 = time_us_64();
        printf("ADC: %i DIGITAL: %i TEMP: %u TIME: %llu\n", x, y, temp,dt);
        dt = t1 - t0;
    }
    cyw43_arch_deinit();
    return 0;
}





























