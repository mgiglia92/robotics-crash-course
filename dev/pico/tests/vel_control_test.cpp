#define private public
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
                mode=0;
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
    irq_set_enabled(IO_IRQ_BANK0, true);
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
    gpio_pull_up(RCC_PUSHBUTTON);
}

typedef struct quantize_s
{
    uint32_t cur;
    uint32_t prev;

} quantize_t;

int main(void)
{
    stdio_init_all();
    initialize_potentiometer();
    initialize_pushbutton();
    // init_button_isr();
    sleep_ms(1000);

    quantize_t timer;

    Motor motors;
    MotorInit(&motors, RCC_ENA, RCC_ENB, 20000);
    MotorsOn(&motors);

    Left_Odom left;
    Right_Odom right;

    con.kp = 1.5;
    con.ki = 0.5;
    con.kd = 0.005;
    con.ts = 0.01; // 1ms
    con.sigma = 0.1;
    con.lowerLimit = 0;
    con.upperLimit = 100;
    con.antiWindupEnabled = true;
    
    PID_control lcon(con);
    PID_control rcon(con);
    lcon.setDeadbands(0, 30);
    rcon.setDeadbands(0, 30);
    Differentiator ldiff(con.sigma, con.ts);
    Differentiator rdiff(con.sigma, con.ts);
    cout << "STARTING! " << '\n';

    mode=0;
    while(true)
    {   
        timer.cur = time_us_64();
        if((timer.cur - timer.prev) >= (uint32_t)(con.ts*1000000))
        {
            float pot_val = adc_read()/4097.;
            lcon.kd = pot_val;
            rcon.kd = pot_val;
            float vel_reference = 100;//(pot_val / 4097.0) * 270;
            if(!gpio_get(RCC_PUSHBUTTON)){vel_reference = 0; }

            //Calculate velocity
            float lvel = ldiff.differentiate(left.getCount());
            float rvel = rdiff.differentiate(right.getCount());

            //Get controller output
            float loutput = lcon.pid(vel_reference, lvel);
            float routput = rcon.pid(vel_reference, rvel);

            //Send controller output to motors
            MotorPower(&motors, loutput, routput);

            
            cout << "kp: " << lcon.kp << " ki: " << lcon.ki << " kd: " << lcon.kd << " ref: " << vel_reference << 
            " --VELS: " << lvel << " | " << rvel << " --OUT: " << loutput << " | " << routput;
            cout << '\n';

            timer.prev = timer.cur;

        }
    }
}