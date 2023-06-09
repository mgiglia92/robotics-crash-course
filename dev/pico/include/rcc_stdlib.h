#pragma once

//arm-none-eabi stdlib.h?
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Pico SDK
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h" //Imported to blink LED
#include "hardware/adc.h"
#include "hardware/i2c.h"

//Rcc made
#include "pwm_helper.h"
#include "sensors/mpu6050.h"
#include "sensors/VL53L0X.h"
