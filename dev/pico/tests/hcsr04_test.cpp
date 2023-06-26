#include "rcc_stdlib.h"
using namespace std;

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting hcsr04 test!\n");
    HC_SR04_async* dist;
    HC_SR04_async dist_sensor = *dist;
    dist_sensor.begin(12, 13);
    while(true)
    {
        dist_sensor.pulse(100000);
        sleep_ms(100);
        cout << "dist: " << dist_sensor.getDuration() * RCC_SOUND_INCHES_PER_MICROSECOND/2.0 << '\n';
			
    }
    // gpio_set_function(13, GPIO_FUNC_SIO);
    // gpio_set_dir(13, true);
    // while(true)
    // {
    //     gpio_put(13, 1);
    //     sleep_ms(1000);
    //     gpio_put(13, 0);
    //     sleep_ms(1000);
    // }
}
