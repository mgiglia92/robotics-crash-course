#include <rcc_wireless_msg_interface.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

using namespace std;

bool init_cyw43(WirelessMsgInterface* infra)
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
        char * address;
        infra->lwip_infra.ip_recv = netif_list->ip_addr;
        address = ipaddr_ntoa(&infra->lwip_infra.ip_recv);
        printf("This PICOS IP address is: %s\n", address);
        return true;
    }

}

int main()
{
    stdio_init_all();    
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    lwip_infra_t lwip_infra;

    lwip_infra.pcb_recv = udp_new();
    lwip_infra.pcb_send = udp_new();
    ipaddr_aton(IP_SEND, &lwip_infra.ip_recv); 
    ipaddr_aton(IP_RECV, &lwip_infra.ip_send);
    lwip_infra.port_recv = PORT_RECV;
    lwip_infra.port_send = PORT_SEND;
    WirelessMsgInterface interface(lwip_infra);
    string ip = ipaddr_ntoa(&interface.lwip_infra.ip_send);

    init_cyw43(&interface);
    printf("DEBUG: ip is:  %s \n", ip);
    while(true)
    {
        Packet p;
        interface.send_msg(p);
        sleep_ms(1000);
        cyw43_arch_gpio_put(0, !cyw43_arch_gpio_get(0));
    }

    cyw43_arch_deinit();
    return 0;
}