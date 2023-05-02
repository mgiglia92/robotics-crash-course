#include <rcc_wireless_msg_interface.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"


using namespace std;

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
    printf("IN TIMER\n");
    cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
    return true;
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
    sleep_ms(500);
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    WirelessMsgInterface interface(IP_SEND, IP_RECV, PORT_SEND, PORT_RECV);
    interface.setup_wireless_interface();
    init_cyw43();
    add_repeating_timer_ms(500, timer_callback, &interface, &send_timer);


    char * address;
    interface.lwip_infra.ip_recv = netif_list->ip_addr;
    address = ipaddr_ntoa(&interface.lwip_infra.ip_recv);
    printf("This PICO's IP address is: %s\n", address);

    while(true)
    {
        // Packet p;
        // interface.send_msg(p);
        adc_select_input(2);
        delay_length = adc_read();
        sleep_ms(delay_length);
        uint8_t data = 255;
        queue_remove_blocking(&interface.recv_queue, &data);
        // cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
        printf("Recv Queue: %c\n", data);
    }

    cyw43_arch_deinit();
    return 0;
}