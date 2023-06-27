// #define private public
#include "rcc_stdlib.h"
using namespace std;


PID_control_config_t con;

int main(void)
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(28);
    sleep_ms(1000);

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 20000);
    MotorsOn(&motors);

    Odom<12> left;
    Odom<13> right;

    con.kp = 1;
    con.ki = 1;
    con.kd = 0.01;
    con.ts = 0.1; // 1ms
    con.sigma = 0.25;
    con.lowerLimit = 0;
    con.upperLimit = 100;
    con.antiWindupEnabled = true;
    
    PID_control controller(con);
    controller.setDeadbands(0, 30);
    Differentiator diff(con.sigma, con.ts);

    adc_select_input(2);
    cout << "STARTING! " << '\n';

    while(true)
    {   sleep_ms(con.ts*1000);
        int32_t pot_val = adc_read();
        float vel_reference = pot_val / 10.0;

        //Calculate velocity
        float vel = diff.differentiate(left.getCount());

        //Get controller output
        float output = controller.pid(vel_reference, vel);

        //Send controller output to motors
        MotorPower(&motors, output, 0);
        
        cout << "ref: " << vel_reference << " vel: " << vel <<
            " out: " << output << '\n';
    }
}