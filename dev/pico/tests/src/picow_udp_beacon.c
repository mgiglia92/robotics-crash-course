/**
 * Copyright (c) 2022 Andrew McDonnell
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <include_test.h>

// #include "comms/serialize.h"

#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
// #include "pico/cyw43_arch.h"

// #include "lwip/pbuf.h"
// #include "lwip/udp.h"
// #include "lwip/dns.h"
// #include "lwip/ip_addr.h"
#include "wireless_comms.h"
#include "Servo.h"

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.1.35"
#define IP_RECV "192.168.1.33"
#define BEACON_INTERVAL_MS 100

bool led_state = true;
//Servo Init
Servo s1;




//UDP Callback Fcn
// void udp_receive_callback( void* arg,              // User argument - udp_recv `arg` parameter
//                            struct udp_pcb* upcb,   // Receiving Protocol Control Block
//                            struct pbuf* p,         // Pointer to Datagram
//                            const ip_addr_t* addr,  // Address of sender
//                            u16_t port )            // Sender port 
// {

//     // Process datagram here (non-blocking code)
//     uint8_t* tmpPtr;
//     tmpPtr = (uint8_t*)p->payload;
//     uint8_t data[p->tot_len];
//     printf("%i chars from: %s | ", p->len, ipaddr_ntoa(addr));
//     for(int i = 0; i < p->len; i++)
//     {   
//         data[i] = *(tmpPtr++);
//         printf("%c", data[i]);
//     }
//     servo_position = atoi(data);
//     // Must free receive pbuf before return
//     pbuf_free(p);
// }

void run_udp_beacon(lwip_infra_t* infra, comms_data_t* data, ip_addr_t hostname_addr) {

    //Setup two ip addresses
    ipaddr_aton(IP_SEND, &infra->ip_send); 
    ipaddr_aton(IP_RECV, &infra->ip_recv);
    // ipaddr_aton(CYW43_HOST_NAME, &addr2); //This one is the ip address of the pico itself

    udp_bind(infra->pcb_recv, &infra->ip_recv, 9900); //Bind the pico ipaddr to port 9990
    udp_recv(infra->pcb_recv, udp_receive_callback, data); //Setup recv callback fcn

    dns_gethostbyname(CYW43_HOST_NAME, &hostname_addr, gotHostName, NULL);
    
    int counter = 0;
    while (true) {
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
        char *req = (char *)p->payload;
        memset(req, 0, BEACON_MSG_LEN_MAX+1);
        snprintf(req, BEACON_MSG_LEN_MAX, "%d %s\r\n", counter, "I'm trying to make this message very long and see how fast I can send it AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        err_t er = udp_sendto(infra->pcb_send, p, &infra->ip_send, PORT_SEND);
        pbuf_free(p);
        if (er != ERR_OK) {
            printf("Failed to send UDP packet! error=%d", er);
        } else {
            printf("Sent packet %d\n", counter); 
            counter++;
        }

        // Note in practice for this simple UDP transmitter,
        // the end result for both background and poll is the same

    #if PICO_CYW43_ARCH_POLL
            // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
            // main loop (not from a timer) to check for Wi-Fi driver or lwIP work that needs to be done.
            cyw43_arch_poll();
            sleep_ms(BEACON_INTERVAL_MS);
    #else
            // if you are not using pico_cyw43_arch_poll, then WiFI driver and lwIP work
            // is done via interrupt in the background. This sleep is just an example of some (blocking)
            // work you might be doing.
            sleep_ms(BEACON_INTERVAL_MS);
    #endif
        led_state = !led_state;
        ServoPosition(&s1, data->servo_position);
        cyw43_arch_gpio_put(0,led_state);
        }
}

int main() {
    stdio_init_all();


    ServoInit(&s1, 20, false);
    ServoOn(&s1);


    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_gpio_put(0,1);
    cyw43_arch_enable_sta_mode();
    

    struct udp_pcb* pcb = udp_new();
    struct udp_pcb* pcb2 = udp_new();
    ip_addr_t hostname_addr;
    ip_addr_t ip_send, ip_recv;
    uint16_t port_recv = 9900;
    uint16_t port_send = 9999;
    lwip_infra_t infra;
    infra.pcb_recv = pcb;
    infra.pcb_send = pcb2;
    infra.ip_recv = ip_recv;
    infra.ip_send = ip_send;
    infra.port_recv = port_recv;
    infra.port_send = port_send;
    comms_data_t data;
    data.servo_position = 95;

    //Start connection
    printf("Connecting to Wi-Fi...\n");
    while(!try_connect(infra.ip_recv)){} //Keep trying till wifi connects
    run_udp_beacon(&infra, &data, hostname_addr); //RUn blocking udp beacon
    cyw43_arch_deinit();
    return 0;
}





























