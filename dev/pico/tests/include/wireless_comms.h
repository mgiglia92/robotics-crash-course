
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "rcc_comms/serialize.h"
#include "cpp-base64/base64.h"
#include "pico/util/queue.h"
#include <stdlib.h>

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.1.101"
#define IP_RECV "192.168.1.123"
#define BEACON_INTERVAL_MS 100

//Struct of data allowed tto be edited by incoming udp/serial messages
typedef struct comms_data_s
{
    int servo_position;
    float leftPwr;
    float rightPwr;
} comms_data_t;

//Generaet lwip infra from raw



//Struct of the comms lwip_infrastructure (ips, ports)
typedef struct lwip_infra_s
{
    struct udp_pcb* pcb_recv;
    struct udp_pcb* pcb_send;
    ip_addr_t ip_recv; //This is the picos ip addr
    ip_addr_t ip_send; //this is the lapttop ip addr
    uint16_t port_recv;
    uint16_t port_send;
} lwip_infra_t;

bool try_connect(ip_addr_t ip_recv)
{
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect.\n");
        printf(WIFI_SSID);
        printf("\n");
        printf(WIFI_PASSWORD);

        return false;
    } else {
        printf("Connected.\n");
        char * address;
        ip_recv = netif_list->ip_addr;
        address = ipaddr_ntoa(&ip_recv);
        printf("This PICOS IP address is: %s\n", address);
        // IP4_ADDR(&addr2, 192, 168, 1, 169);
        // netif_add();
        return true;
    }
}


void gotHostName(const char *name, struct ip_addr *ipaddr, void *arg)
{
    char * tmp;
    printf("GOT HOST NAME ADDR: %s %s", name, ipaddr);
}


//UDP Callback Fcn
void udp_receive_callback( void* arg,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb* upcb,   // Receiving Protocol Control Block
                           struct pbuf* p,         // Pointer to Datagram
                           const ip_addr_t* addr,  // Address of sender
                           u16_t port )            // Sender port 
{

    // Process datagram here (non-blocking code)
    uint8_t* tmpPtr;
    tmpPtr = (uint8_t*)(p->payload);
    uint8_t data[p->tot_len]; //char array to place udp packet charaters into
    //break the above rule right away
    printf("%i chars from: %s | ", p->len, ipaddr_ntoa(addr));
    for(int i = 0; i < p->len; i++)
    {   
        data[i] = *(tmpPtr++);
        printf("%c", data[i]);
    }
    int x = atoi((char *)data);
    // //Manipulate data at pointer
    comms_data_t* dataPtr;
    dataPtr = (comms_data_t*)arg;
    dataPtr->servo_position = x;
    
    
    // int16_t y=16;
    // queue_try_add((queue_t*)arg, &y);
    // printf("DEBUG: Added to queue");

    // Must free receive pbuf before return
    pbuf_free(p);
}


void init_udp(lwip_infra_t* lwip_infra, comms_data_t* data, queue_t* queue)
{
    udp_bind(lwip_infra->pcb_recv, &lwip_infra->ip_recv, lwip_infra->port_recv); //Bind the pico ipaddr to port 9990
    udp_recv(lwip_infra->pcb_recv, udp_receive_callback, data); //Setup recv callback fcn
}



