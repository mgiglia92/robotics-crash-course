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
#include "pico/multicore.h"
#include "pico/util/queue.h"
// #include "hash-library/sha1.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <atomic>

#define ADC0 26

// SHA1 sha1;
using namespace std;
typedef struct element{
    int16_t value;

}element_t;

element_t elem;

bool led_state = true;
int16_t value=1;
queue_t queue;

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

void blink_led()
{
    while(true){
    cyw43_arch_gpio_put(0,1);
    sleep_ms(100);
    cyw43_arch_gpio_put(0,0);
    sleep_ms(100);
    elem.value++;
    value++;
    queue_add_blocking(&queue, &value);
    }
}


int main() {
    stdio_init_all();
    queue_init(&queue, sizeof(&value), 200);

    sleep_ms(1000);
    printf("Starting!");
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    multicore_launch_core1(blink_led);
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
        element_t readVal;
        int16_t val;
        val=0;
        queue_remove_blocking(&queue, &val);
        cout << val << "\n";
        // printf("ADC: %i DIGITAL: %i TEMP: %u TIME: %llu VALUE: %d\n", x, y, temp, dt, val);
        dt = t1 - t0;
    }
    cyw43_arch_deinit();
    return 0;
}





























