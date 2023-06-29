#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();    
    sleep_ms(1500);
    if(cyw43_arch_init())
    {
        cout << "CYW43 INIT failed!" << '\n';
    }
    cout << "init worked!" << '\n';

    cyw43_arch_gpio_put(0,1);

    gpio_init(21);
    gpio_set_dir(21, true);

    adc_init();
    adc_gpio_init(28);
    //Init servo
    Servo s1;
    Servo s2;
    ServoInit(&s1, 16, false, 50);
    ServoInit(&s2, 17, false, 50);
    ServoOn(&s1);
    ServoOn(&s2);
    printf("AFTER SERVO");


    //Init motors
    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 1000);
    MotorsOn(&motors);
    printf("AFTER MOTORS");

    //INit i2c and mpu6050    
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
    MPU6050 imu;
    imu.begin(i2c1);
    imu.calibrate();
    float accelx, accely, angvelz;
    printf("AFTER IMU");

    // //Odom
    Odom<12> left;
    Odom<13> right;
    printf("AFTER ODOM");

    adc_select_input(2);
    while(true)
    {   
        uint16_t pot_val = adc_read();
        sleep_ms(100);

        //Servo Pos
        ServoPosition(&s1, 100*pot_val/4097);
        //Motor Power
        MotorPower(&motors, 100*pot_val/4097, 100*pot_val/4097);
        //MPU6050
        imu.update_pico();
        accelx  = imu.getAccelX();
        accely  = imu.getAccelY();
        angvelz = imu.getAngVelZ();

        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0)));
        gpio_put(21, !(gpio_get(21)));
        //Print everything
        cout << "mpu: " << accelx << " " << accely << " " << angvelz << 
        "Odom: " << left.getCount() << " " << right.getCount() << '\n';
    }

}