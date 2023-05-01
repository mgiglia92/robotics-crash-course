#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "rcc_comms/serialize.h"
#include "rcc_comms/packet.h"
#include "cpp-base64/base64.h"
#include "pico/util/queue.h"
#include <stdlib.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <stdio.h>

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.1.38"
#define IP_RECV "192.168.1.37"
#define BEACON_INTERVAL_MS 100

using namespace std;

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

class WirelessMsgInterface
{
    public:
        WirelessMsgInterface(lwip_infra_t);
        void setup_wireless_interface();
        bool send_msg(Packet);
        static void recv_msg(void*,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb*,   // Receiving Protocol Control Block
                           struct pbuf* ,         // Pointer to Datagram
                           const ip_addr_t* ,  // Address of sender
                           u16_t );
        void* get_packet_from_queue(Packet);
        void* send_packet_to_queue(Packet);
        bool init_cyw43();
        lwip_infra_s lwip_infra;
        queue_t recv_queue;
        queue_t send_queue;

};

WirelessMsgInterface::WirelessMsgInterface(lwip_infra_t infra)
{
    lwip_infra=infra;
}


//UDP Callback Fcn
void WirelessMsgInterface::recv_msg( void* arg,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb* upcb,   // Receiving Protocol Control Block
                           struct pbuf* p,         // Pointer to Datagram
                           const ip_addr_t* addr,  // Address of sender
                           u16_t port )            // Sender port 
{

    WirelessMsgInterface* obj = (WirelessMsgInterface*)arg;

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

    // Must free receive pbuf before return
    pbuf_free(p);
}



void WirelessMsgInterface::setup_wireless_interface()
{
    //Initialize udp receive and callback
    udp_bind(lwip_infra.pcb_recv, &lwip_infra.ip_recv, lwip_infra.port_recv); //Bind the pico ipaddr to port 9990
    udp_recv(lwip_infra.pcb_recv, this->recv_msg, this); //Setup recv callback fcn
}

bool WirelessMsgInterface::send_msg(Packet pack)
{

    // decode it (TODO: actually do this right)
    int32_t packet_id;
    float field_1, field_2, field_3;
    field_1 = 133;
    field_2 = 233;
    field_3=444;

    // send an acknowledgement back
    std::string ack = base64_encode(
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
    err_t er = udp_sendto(lwip_infra.pcb_send, p, &lwip_infra.ip_send, PORT_SEND);
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        printf("Sent packet %s\n", ack.c_str()); 
    }
    return true;
}