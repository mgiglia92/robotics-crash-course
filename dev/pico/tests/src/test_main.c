#include "pico/cyw43_arch.h"
#include "Servo.h"
#include "comms.h"



int main() {
    //global scope vars (?)
    comms_data_t data;
    Servo s1;
    ip_addr_t ip_send, ip_recv;
    uint16_t port_send, port_recv;
    port_send = PORT_SEND;
    port_recv = PORT_RECV;
    ipaddr_aton(IP_SEND, &ip_send);
    ipaddr_aton(IP_RECV, &ip_recv);
    lwip_infra_t lwip_infra = generate_lwip_infra(ip_recv, port_recv, ip_send, port_send);
    //Init modules
    stdio_init_all();
    ServoInit(&s1, 20, false);
    ServoOn(&s1);
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    //Set led
    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");

    //loop()
    while(true){
        if (try_connect()) {
        printf("failed to connect.\n");
        printf(WIFI_SSID);
        printf("\n");
        printf(WIFI_PASSWORD);
    } else {
        printf("Connected.\n");
        char * address;
        memcpy(&pico_addr, &(netif_list->ip_addr), sizeof(ip_addr_t));
        address = ipaddr_ntoa(&pico_addr);
        printf("This PICOS IP address is: %s\n", address);

        init_udp(&lwip_infra, &data);
        
        // IP4_ADDR(&addr2, 192, 168, 1, 169);
        // netif_add();
        break;
    }
    }
    
    run_udp_beacon(&lwip_infra, &data);
    cyw43_arch_deinit();
    return 0;
}