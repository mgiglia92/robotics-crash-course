/**
 * Copyright (c) 2022 Andrew McDonnell
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <include_test.h>
#include <stdlib.h>
#include "pico/stdlib.h"    
#include "wireless_comms.h"
#include "Servo.h"
#include "hardware/adc.h"
#include "pico/util/queue.h"
// #include "hash-library/sha1.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#define ADC0 26

// SHA1 sha1;
using namespace std;

bool led_state = true;

//Servo Init
Servo s1;

queue_t queue;
typedef void* HANDLE;

int counter = 0;

void send_udp_packet(lwip_infra_t* infra, comms_data_t* data)
{

    // decode it (TODO: actually do this right)
    int32_t packet_id;
    float field_1, field_2, field_3;
    field_1 = 133;
    field_2 = 233;
    field_3=444;

    // send an acknowledgement back
    string ack = base64_encode(
        serialize<int32_t, float, float, float>(
            make_tuple(
                377l,
                field_2,
                field_1 + field_2,
                exp(1.0f)
            )
        )
    );
    printf("SERIALIZE LIB: %s\n", ack.c_str());


    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    snprintf(req, BEACON_MSG_LEN_MAX, "%s\r\n", ack.c_str());
    err_t er = udp_sendto(infra->pcb_send, p, &infra->ip_send, PORT_SEND);
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        printf("Sent packet %s\n", ack.c_str()); 
    }
}


void run_udp_beacon(lwip_infra_t* infra, comms_data_t* data, ip_addr_t hostname_addr, queue_t* dataQueue) {

    //Setup two ip addresses
    ipaddr_aton(IP_SEND, &infra->ip_send); 
    ipaddr_aton(IP_RECV, &infra->ip_recv);
    // ipaddr_aton(CYW43_HOST_NAME, &addr2); //This one is the ip address of the pico itself

    //Sometimes still can't ping pico using host name
    // dns_gethostbyname(CYW43_HOST_NAME, &hostname_addr, gotHostName, NULL);

    init_udp(infra, data, dataQueue);
    
    while (true) {

        send_udp_packet(infra, data);
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
    queue_init(&queue, sizeof(int16_t), 100);

    sleep_ms(1000);

    ServoInit(&s1, 18, false);
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
    u16_t port_recv = 9900;
    u16_t port_send = 9999;
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

    //ADC stuff
    printf("Tring to read adc...");
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);
    uint16_t x = adc_read();
    printf("ADC READING: %i", x);

    run_udp_beacon(&infra, &data, hostname_addr, &queue); //RUn blocking udp beacon
    cyw43_arch_deinit();
    return 0;
}





























