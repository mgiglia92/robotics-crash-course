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

    Left_Odom left;
    Right_Odom right;

    con.kp = 0.1;
    con.ki = 1;
    con.kd = 0.01;
    con.ts = 0.1; // 1ms
    con.sigma = 0.1;
    con.lowerLimit = 0;
    con.upperLimit = 100;
    con.antiWindupEnabled = true;
    
    PID_control controller(con);
    controller.setDeadbands(0, 60);
    Differentiator diff(con.sigma, con.ts);

    adc_select_input(2);
    cout << "STARTING! " << '\n';

    while(true)
    {   sleep_ms(con.ts*1000);
        int32_t pot_val = adc_read();
        float vel_reference = (pot_val / 4097.0) * 270;

        //Calculate velocity
        float vel = diff.differentiate(right.getCount());

        //Get controller output
        float output = controller.pid(vel_reference, vel);

        //Send controller output to motors
        MotorPower(&motors, 0, output);
        
        cout << "ref: " << vel_reference << " vel: " << vel <<
            " out: " << output << '\n';
    }
}