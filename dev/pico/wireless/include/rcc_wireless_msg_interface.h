#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "serial-packets/packet.h"
#include "serial-packets/serialize.h"
#include "serial-packets/messages.h"
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
#define IP_SEND "192.168.1.101"
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
        WirelessMsgInterface(string, string, uint32_t, uint32_t);
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
        void packet_receiver();
        lwip_infra_t lwip_infra;
        queue_t recv_queue;
        queue_t send_queue;

};

WirelessMsgInterface::WirelessMsgInterface(string ip_send, string ip_recv, uint32_t port_send, uint32_t port_recv)
{
    this->lwip_infra.pcb_recv = udp_new();
    this->lwip_infra.pcb_send = udp_new();
    this->lwip_infra.port_send = 9999;
    this->lwip_infra.port_recv = 9900;
    ipaddr_aton(ip_recv.c_str(), &(this->lwip_infra.ip_recv)); 
    ipaddr_aton(ip_send.c_str(), &(this->lwip_infra.ip_send));
    printf("DEBUG: ip_send %s | %s\n", IP_SEND, ipaddr_ntoa(&this->lwip_infra.ip_send));
    printf("DEBUG: ip_recv %s | %s\n", IP_RECV, ipaddr_ntoa(&this->lwip_infra.ip_recv));

    //Get the ip address we've been given
    ip_addr_t ipnetif = netif_list->ip_addr;
    lwip_infra.ip_recv = ipnetif;
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
    stringstream indata;
    uint8_t data[p->tot_len]; //char array to place udp packet charaters into
    //break the above rule right away
    printf("%i chars from: %s | ", p->len, ipaddr_ntoa(addr));
    for(int i = 0; i < p->len; i++)
    {   
        data[i] = *(tmpPtr++);
        indata << data[i];
        printf("%c", data[i]);
    }

    Packet pack;
    printf("string stream data: %s", indata.str().c_str());
    printf("RECV CALLBACK packet data: %s", pack.data().c_str());
    queue_add_blocking(&obj->recv_queue, &pack);

    // Must free receive pbuf before return
    pbuf_free(p);
}



void WirelessMsgInterface::setup_wireless_interface()
{
    //Initialize udp receive and callback
    // const ip_addr_t ip_recv = lwip_infra.ip_recv;
    udp_bind(lwip_infra.pcb_recv, &lwip_infra.ip_recv, lwip_infra.port_recv); //Bind the pico ipaddr to port 9990
    udp_recv(lwip_infra.pcb_recv, this->recv_msg, this); //Setup recv callback fcn
    queue_init(&recv_queue, sizeof(Packet*), 100);
}

bool WirelessMsgInterface::send_msg(Packet pack)
{
    //place holder variables for data
    int32_t packet_id;
    float field_1, field_2, field_3;
    field_1 = 133;
    field_2 = 233;
    field_3=444;

    // stringify the data
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
    Packet packet(88, ack.c_str());

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    snprintf(req, BEACON_MSG_LEN_MAX, "%s\r\n", packet.data().c_str()); //instead get the package stsring
    err_t er = udp_sendto(lwip_infra.pcb_send, p, &lwip_infra.ip_send, PORT_SEND); //Send the string over udp
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        printf("Sent packet %s\n", ack.c_str()); 
    }
    return true;
}


void WirelessMsgInterface::packet_receiver() {
	bool switch_modes = false;
	while (!switch_modes) {
		Packet p;
		cin >> p;

		switch (p.id()) {
		case 0:
			switch_modes = true;
			break;

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
			break;
		}

		// just so something comes back
		Test_Outbound tout {
			exp(-1.0f),
			1,
			exp(1.0f)
		};
        Packet newp;
        newp = tout.pack();
		send_msg(newp);
	}
}