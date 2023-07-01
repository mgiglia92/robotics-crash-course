// #define private public
#include "rcc_stdlib.h"
using namespace std;


PID_control_config_t con;
volatile uint mode;

const uint32_t button_events = GPIO_IRQ_EDGE_RISE;

void button_isr(void)
{
    if (gpio_get_irq_event_mask(RCC_PUSHBUTTON) & button_events) {
        gpio_acknowledge_irq(RCC_PUSHBUTTON, button_events);
        switch(mode){
            case 3:
                mode=1;
                break;
            default:
                mode++;
                break;
        }
    }

}

void init_button_isr(void)
{
    gpio_set_irq_enabled(RCC_PUSHBUTTON, button_events, true);
    gpio_add_raw_irq_handler(RCC_PUSHBUTTON, &button_isr);
}


void initialize_potentiometer()
{

    adc_init();
    adc_gpio_init(RCC_POTENTIOMETER);
    adc_select_input(2); //TODO: Macro for gpio -> adc chan 
}

void initialize_pushbutton()
{
    gpio_init(RCC_PUSHBUTTON);
    gpio_set_dir(RCC_PUSHBUTTON, false); //Make push button gpio an input
}


int main(void)
{
    stdio_init_all();
    initialize_potentiometer();
    initialize_pushbutton();
    // init_button_isr();
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
    controller.setDeadbands(0, 30);
    Differentiator diff(con.sigma, con.ts);
    cout << "STARTING! " << '\n';

    mode=0;
    while(true)
    {   
        sleep_ms(con.ts*1000);
        int32_t pot_val = adc_read();
        float vel_reference = (pot_val / 4097.0) * 270;
        if(gpio_get(RCC_PUSHBUTTON)) { vel_reference = 0; }

        //Calculate velocity
        float vel = diff.differentiate(right.getCount());

        //Get controller output
        float output = controller.pid(vel_reference, vel);

        //Send controller output to motors
        MotorPower(&motors, 0, output);
        
        cout << "mode: " << mode << " ref: " << vel_reference << " vel: " << vel <<
            " out: " << output << '\n';
    }
}