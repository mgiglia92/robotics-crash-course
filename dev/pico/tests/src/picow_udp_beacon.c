/**
 * Copyright (c) 2022 Andrew McDonnell
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <include_test.h>
#include "comms/serialize.h"

#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "Servo.h"

#define UDP_PORT 9999
#define BEACON_MSG_LEN_MAX 500
#define BEACON_TARGET "192.168.1.26"
#define BEACON_INTERVAL_MS 100

bool led_state = true;
int servo_position = 0;
//Servo Init
Servo s1;

bool try_connect();

//UDP Callback Fcn
void udp_receive_callback( void* arg,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb* upcb,   // Receiving Protocol Control Block
                           struct pbuf* p,         // Pointer to Datagram
                           const ip_addr_t* addr,  // Address of sender
                           u16_t port )            // Sender port 
{

    // Process datagram here (non-blocking code)
    servo_position = p->tot_len;

    printf("%i chars from: %s, %s\n", p->len, ipaddr_ntoa(addr), p->payload);

    // Must free receive pbuf before return
    pbuf_free(p);
}

void run_udp_beacon() {
    //Create a udp protocol control block
    struct udp_pcb* pcb = udp_new();

    //Setup two ip addresses
    ip_addr_t addr, addr2;
    ipaddr_aton(BEACON_TARGET, &addr); //This is the ip address ofo my computer ( to send msg to from pico)
    ipaddr_aton("192.168.1.33", &addr2); //This one is the ip address of the pico itself

    udp_bind(pcb, &addr2, 9900); //Bind the pico ipaddr to port 9990
    udp_recv(pcb, udp_receive_callback, NULL); //Setup recv callback fcn

    int counter = 0;
    while (true) {
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
        char *req = (char *)p->payload;
        memset(req, 0, BEACON_MSG_LEN_MAX+1);
        snprintf(req, BEACON_MSG_LEN_MAX, "%d%s\r\n", counter, "I'm trying to make this message very long and see how fast I can send it AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
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
        ServoPosition(&s1, servo_position * 10);
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
    

    printf("Connecting to Wi-Fi...\n");
    //if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
    while(!try_connect()){}
    run_udp_beacon();
    cyw43_arch_deinit();
    return 0;
}



bool try_connect()
{
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



























