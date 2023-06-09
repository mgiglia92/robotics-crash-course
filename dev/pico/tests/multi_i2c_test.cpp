#include "rcc_stdlib.h"
using namespace std;

void rcc_init_i2c_bus(void)
{
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
}

void rcc_init_lidar(VL53L0X* l)
{
    l->setBus(i2c1);
    l->setTimeout(200);
    if(!l->init())
    {
        printf("Failed to detect and initalize VL53L0X, infinite loop");
        while(1){}
    //IDEA: extend pololu class to allow robot to run with uninitialized lidar 
    //Remove above while loop if implemented
    //return;
    }
    l->startContinuous(0);
    return;
}

uint16_t getFastReading(VL53L0X* l)
{

  uint16_t range = l->readReg16Bit(l->RESULT_RANGE_STATUS + 10);
  //  sensor.writeReg(sensor.SYSTEM_INTERRUPT_CLEAR, 0x01);
  return range;
}

int main() {
    stdio_init_all();

    sleep_ms(1000);
    rcc_init_i2c_bus();
    
    //Instantiate IMU and Lidar classes
    MPU6050 imu;
    VL53L0X lidar;
    imu.begin(i2c1);
    rcc_init_lidar(&lidar);
    printf("Both Sensors initialized!, Calibrating IMU");
    imu.calibrate();

    while (1) {
        imu.update_pico();

        uint16_t dist = getFastReading(&lidar);
        printf("Accel: %f, %f, %f Distance (mm): %hu\n", imu.getAccelX(), imu.getAccelY(), imu.getAccelZ(), dist);
        sleep_ms(100);
    }

    return 0;
}
