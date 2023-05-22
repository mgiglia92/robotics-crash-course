#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"
#include "serial-packets/packet.h"
#include "serial-packets/packet.c++"
#include "serial-packets/serialize.h"
#include "serial-packets/messages.h"
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
#include "pico/mutex.h"
#include <condition_variable>

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.1.35" //Computers IP
#define IP_RECV "192.168.1.37" //Picos IP
#define BEACON_INTERVAL_MS 100

using namespace std;

//ty cvw
struct inter_thread_message {
	inter_thread_message() = default;
	inter_thread_message(const Packet&);
	inter_thread_message(const string& m);

	Packet pack();

	string s;
    int32_t id;
};

inter_thread_message::inter_thread_message(const Packet& p) {
	s = p.data();
    id = p.id();
}

inter_thread_message::inter_thread_message(const string& s)
: s(s) {}

Packet inter_thread_message::pack() {
	return Packet(id, s);
}

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
        void packet_receiver(Packet);
        lwip_infra_t lwip_infra;
        stringstream msg_stream;
        string msg;
        mutex_t mtx;
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
    mutex_init(&mtx);
}


//UDP Callback Fcn
void WirelessMsgInterface::recv_msg( void* arg,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb* upcb,   // Receiving Protocol Control Block
                           struct pbuf* p,         // Pointer to Datagram
                           const ip_addr_t* addr,  // Address of sender
                           u16_t port )            // Sender port 
{

    WirelessMsgInterface* interface = (WirelessMsgInterface*)arg;

    // Process datagram here (non-blocking code)
    char* tmpPtr;
    tmpPtr = reinterpret_cast<char*>((uint8_t*)(p->payload));
    char data[p->tot_len]; //char array to place udp packet charaters into
    stringstream tmp;
    // auto lock = std::unique_lock{mtx};
    // interface->readingStarted.wait(lock);
    uint32_t ownerout;
    if(mutex_try_enter(&interface->mtx, &ownerout))
    {
        interface->msg_stream.str("");
        interface->msg_stream.clear();
        for(int i = 0; i < p->len; i++)
            {   
                data[i] = *(tmpPtr++);
                tmp << data[i];
                interface->msg_stream << data[i];
            }
            // inter_thread_message m(out.pack());
            // inter_thread_message m(tmp.str());
            // interface->msg_stream << m.s;
            printf("Received msg!\n");
            mutex_exit(&interface->mtx);
    }

    // Must free receive pbuf before return
    pbuf_free(p);
}



void WirelessMsgInterface::setup_wireless_interface()
{
    //Initialize udp receive and callback
    // const ip_addr_t ip_recv = lwip_infra.ip_recv;
    udp_bind(lwip_infra.pcb_recv, &lwip_infra.ip_recv, lwip_infra.port_recv); //Bind the pico ipaddr to port 9990
    udp_recv(lwip_infra.pcb_recv, this->recv_msg, this); //Setup recv callback fcn
}

bool WirelessMsgInterface::send_msg(Packet pack)
{
    // //place holder variables for data
    // int32_t packet_id;
    // float field_1, field_2, field_3;
    // field_1 = 133;
    // field_2 = 233;
    // field_3=444;

    // Twist out;
    // out.linear = 1.3;
    // out.angular = 0.5;

    // // stringify the data
    // std::string ack = base64_encode(
    //     serialize<float, float, float>(
    //         make_tuple(
    //             field_2,
    //             field_2,
    //             field_3
    //         )
    //     )
    // );
    // // Packet packet(Test_Outbound::id, ack.c_str());
    Packet packet;
    packet = pack;

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    // string s("TESTING");
    // snprintf(req, BEACON_MSG_LEN_MAX, "%x%s%x\r\n", start_tx, s.c_str(), end_tx); 
    stringstream s;
    s << packet;
    snprintf(req, BEACON_MSG_LEN_MAX, "%s", s.str().c_str()); //instead get the package stsring

    err_t er = udp_sendto(lwip_infra.pcb_send, p, &lwip_infra.ip_send, PORT_SEND); //Send the string over udp
    pbuf_free(p);

    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        // printf("Sent packet %s\n", packet.data().c_str()); 
        cout << "Sent packet: " << packet << '\n';
    }
    return true;
}
