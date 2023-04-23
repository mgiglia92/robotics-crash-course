
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/ip_addr.h"

#define PORT_SEND 9999
#define PORT_RECV 9900
#define BEACON_MSG_LEN_MAX 500
#define IP_SEND "192.168.160.124"
#define IP_RECV "192.168.1.33"
#define BEACON_INTERVAL_MS 100

ip_addr_t laptop_addr, pico_addr;
 
//Struct of data allowed tto be edited by incoming udp/serial messages
typedef struct comms_data_s
{
    int servo_position;
} comms_data_t;

//Generaet lwip infra from raw



//Struct of the comms lwip_infrastructure (ips, ports)
typedef struct lwip_infra_s
{
    struct udb_pcb pcb_recv;
    struct udb_pcb pcb_send;
    ip_addr_t ip_recv; //This is the picos ip addr
    ip_addr_t ip_send; //this is the lapttop ip addr
    uint16_t port_recv;
    uint16_t port_send;
} lwip_infra_t;

lwip_infra_t generate_lwip_infra(ip_addr_t ip_recv, uint16_t port_recv, ip_addr_t ip_send, uint16_t port_send)
{
    lwip_infra_t infra;
    infra.pcb_recv = udp_new();
    infra.pcb_send = udp_new();
    infra.ip_recv = ip_recv;
    infra.ip_send = ip_send;
    infra.port_recv = port_recv;
    infra.port_send = port_send;
    return infra;
}

bool try_connect()
{
    return cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000);
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
    tmpPtr = (uint8_t*)p->payload;
    uint8_t data[p->tot_len];
    printf("%i chars from: %s | ", p->len, ipaddr_ntoa(addr));
    for(int i = 0; i < p->len; i++)
    {   
        data[i] = *(tmpPtr++);
        printf("%c", data[i]);
    }
    int x = atoi(data);
    *(comms_data_t*)arg->servo_position = x;
    // Must free receive pbuf before return
    pbuf_free(p);
}


void run_udp_beacon(lwip_infra_t* lwip_infra, comms_data_t* data) {
    //Create a udp protocol control block
    struct udp_pcb* pcb = udp_new();

    //Setup two ip addresses
    ipaddr_aton(LAPTOP_IP_ADDR, &addr); //This is the ip address ofo my computer ( to send msg to from pico)

    dns_gethostbyname(CYW43_HOST_NAME, &hostnameAddr, gotHostName, NULL);
    
    int counter = 0;
    while (true) {
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
        char *req = (char *)p->payload;
        memset(req, 0, BEACON_MSG_LEN_MAX+1);
        snprintf(req, BEACON_MSG_LEN_MAX, "%d %s\r\n", counter, "I'm trying to make this message very long and see how fast I can send it AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
        err_t er = udp_sendto(pcb, p, &addr, UDP_PORT_TO_LAPTOP);
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

        }
}

void init_udp(lwip_infra_t* lwip_infra, comms_data_t* data)
{
    udp_bind(lwip_infra->pico_pcb_recv, &(netif_list->ip_addr), 9900); //Bind the pico ipaddr to port 9990
    udp_recv(pcb, udp_receive_callback, NULL); //Setup recv callback fcn
    return;
}