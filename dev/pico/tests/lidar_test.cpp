#include "sensors/VL53L0X.h"
#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
using namespace std;

uint16_t getFastReading(VL53L0X* l)
{

  uint16_t range = l->readReg16Bit(l->RESULT_RANGE_STATUS + 10);
  //  sensor.writeReg(sensor.SYSTEM_INTERRUPT_CLEAR, 0x01);
  return range;
}

int main()
{   
    stdio_init_all();
    sleep_ms(100);
    // This example will use i2c1 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);

    VL53L0X lidar;
    lidar.setBus(i2c1);
    lidar.setTimeout(200);
    if (!lidar.init())
    {
        printf("Failed to detect and initialize sensor!");
        while (1) {}
    }
    lidar.startContinuous(0);
    while(true)
    {
        uint16_t dist = getFastReading(&lidar);
        printf("Distance (mm): %hu\n", dist);
        cout << lidar.timeoutOccurred() << '\n';
        sleep_ms(100);
    }
}