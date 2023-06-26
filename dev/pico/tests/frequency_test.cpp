#include "rcc_stdlib.h"
using namespace std;

int main(void)
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(28);
    sleep_ms(1000);

    Motor motors;
    MotorInit(&motors, RCC_ENB, RCC_ENA, 500);
    MotorsOn(&motors);

    if(cyw43_arch_init())
    {
        cout << "CYW43 init failed!" << "\n";
    }

    cyw43_arch_gpio_put(0, true);

    adc_select_input(2);
    while(true)
    {   
        int32_t pot_val = adc_read();
        sleep_ms(10);
        cout << "r chan: " << pot_val <<
            "\n";
        // MotorPower(&motors, 100*pot_val/4097, 100*pot_val/4097);
        MotorInit(&motors, RCC_ENA, RCC_ENB, pot_val*4);
        MotorPower(&motors, 20, 20);
        cyw43_arch_gpio_put(0, !(cyw43_arch_gpio_get(0)));

    }

}