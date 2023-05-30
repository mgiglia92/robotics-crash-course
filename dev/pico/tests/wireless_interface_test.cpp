#define DEBUG
#define ENA 2
#define IN1 3
#define IN2 4
#define ENB 5
#define IN3 6
#define IN4 7
#include <rcc_wireless_msg_interface.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "pwm_helper.h"

using namespace std;

void WirelessMsgInterface::packet_receiver(Packet p) {
    this->send_msg(p);
    switch (p.id()) {
    case 0:
        break;
    case Test_Outbound::id: {
        #ifdef DEBUG
            printf("[DEBUG]: TEST OUTBOUND RECEIVED!\n");
        #endif
        break;
    }
    case Twist::id:
    {
        Twist twist(p);
        #ifdef DEBUG
            printf("[DEBUG]: %s", twist.repr().c_str());
            printf("Twist rep: %f, %f\n", twist.linear, twist.angular);
        #endif
        float lpwr = -1*(twist.linear - twist.angular);
        float rpwr = -1*(twist.linear + twist.angular);
        lpwr = min(max(-1.f, lpwr), 1.f);
        rpwr = min(max(-1.f, rpwr), 1.f);
        if(lpwr < 0){gpio_put(IN1,false); gpio_put(IN2, true);}
        else{gpio_put(IN1,true); gpio_put(IN2, false);}
        if(rpwr < 0){gpio_put(IN3,false); gpio_put(IN4, true);}
        else{gpio_put(IN3,true); gpio_put(IN4, false);}

        pwm_set_duty(ENA, int(abs(lpwr)*100));
        pwm_set_duty(ENB, int(abs(rpwr)*100));
        break;
    }
    case Position::id: {
        break;
    }

    case Simple_Move::id: {
        Simple_Move move(p);

        Test_Outbound tout {
            move.distance,
            move.curvature,
            move.velocity
        };
        cout << tout.pack();
        break;
    }

    case Stop::id: {
    }

    default:
        // nothing (yet, at least)
        #ifdef DEBUG
        printf("[DEBUG]: Invalid Packet | id: %u | data: %s\n", p.id(), p.data().c_str());
        #endif
        break;
    }

}

bool init_cyw43()
{
    // if (!cyw43_arch_init()) {
    //     printf("failed to initialise\n");
    //     return false;;
    // }

    // cyw43_arch_enable_sta_mode();

    //Attempt connection
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect.\n");
        printf(WIFI_SSID);
        printf("\n");
        printf(WIFI_PASSWORD);

        return false;
    } else {
        printf("Connected.\n");
        return true;
    }

}

bool timer_callback(struct repeating_timer *t)
{
    //get wireless interface pointer from user data 
    WirelessMsgInterface* interface;
    interface = (WirelessMsgInterface*)(t->user_data);
    Packet p;
    interface->send_msg(p);
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
    return true;
}

void setup_motors()
{

    uint8_t pwmpins[] = {2,5};
    uint8_t slices[] = {0,0};
    uint8_t channels[] = {0,0};
    uint8_t outpins[] = {3,4,6,7};

    for(int i=0; i<2; i++)
    {
        gpio_set_function(pwmpins[i], GPIO_FUNC_PWM);
        uint slice;
        uint channel;
        slices[i] = pwm_gpio_to_slice_num(pwmpins[i]);
        channels[i] = pwm_gpio_to_channel(pwmpins[i]);
        pwm_set_enabled(slices[i], true);
    }
    for(int i=0; i<4; i++)
    {
        gpio_init(outpins[i]);
        gpio_set_dir(outpins[i], GPIO_OUT);
    }

    gpio_put(IN1, true);
    gpio_put(IN2, false);
    gpio_put(IN3, true);
    gpio_put(IN4, false);

}



int main()
{
    uint delay_length;
    struct repeating_timer send_timer;
    adc_init();
    adc_gpio_init(28);
    adc_gpio_init(29);
    adc_set_temp_sensor_enabled(true);
    
    stdio_init_all();    
    sleep_ms(1000);
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    setup_motors();

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    WirelessMsgInterface interface(IP_SEND, IP_RECV, PORT_SEND, PORT_RECV);
    interface.setup_wireless_interface();
    init_cyw43();
    // add_repeating_timer_ms(500, timer_callback, &interface, &send_timer);


    char * address;
    interface.lwip_infra.ip_recv = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.ip_recv);
    printf("This PICO's IP address is: %s\n", address);

    while(true)
    {   
        mutex_enter_blocking(&interface.mtx);
        Packet p;
        interface.msg_stream >> p;
        inter_thread_message m(p);
        if(!interface.msg_stream)
        {   
            // printf("Pack failed!\n");  
        }
        else{
            interface.packet_receiver(p);
        }
        mutex_exit(&interface.mtx);
        sleep_ms(100);

    }

    cyw43_arch_deinit();
    return 0;
}