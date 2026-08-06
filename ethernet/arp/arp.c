#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <linux/if_arp.h>
#include "../ethernet.h"

typedef struct __attribute__((packed)) {
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t  hardware_len;
    uint8_t  protocol_len;
    uint16_t operation;

    uint8_t sender_hard_add[6];
    uint8_t sender_prot_add[4];

    uint8_t target_hard_add[6];
    uint8_t target_prot_add[4];
} arp_hdr_t;


/* Print MAC Address 
void print_mac(const uint8_t *mac) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2],
           mac[3], mac[4], mac[5]);
}

*/

/* Print IPv4 Address */
void print_ip(const uint8_t *ip){
    printf("%u.%u.%u.%u",
        ip[0], ip[1], ip[2], ip[3]);
}


void parse_arp(const unsigned char *arp_payload, size_t remaining_size) {
    if (remaining_size < sizeof(arp_hdr_t))
    {
        printf("    ├─ [ARP] Packet payload too small for ARP header\n");
        return;
    }

    const arp_hdr_t *arp = (const arp_hdr_t *)arp_payload;

    uint16_t h_type = ntohs(arp->hardware_type);
    uint16_t p_type = ntohs(arp->protocol_type);
    uint8_t  h_len  = arp->hardware_len;
    uint8_t  p_len  = arp->protocol_len;
    uint16_t oper   = ntohs(arp->operation);

    printf("ARP\n");

    /* Hardware Type */
    printf(" ├─ Hardware Type          : ");
    switch (h_type)
    {
        case ARPHRD_ETHER:
            printf("Ethernet (%u)\n", h_type);
            break;

        default:
            printf("%u\n", h_type);
    }

    /* Protocol Type */
    printf(" ├─ Protocol Type          : ");
    switch (p_type)
    {
        case 0x0800:
            printf("IPv4 (0x0800)\n");
            break;

        default:
            printf("0x%04X\n", p_type);
    }

    printf(" ├─ Hardware Length        : %u\n", h_len);
    printf(" ├─ Protocol Length        : %u\n", p_len);

    /* Operation */
    printf(" ├─ Operation              : ");

    switch (oper)
    {
        case ARPOP_REQUEST:
            printf("Request (%u)\n", oper);
            break;

        case ARPOP_REPLY:
            printf("Reply (%u)\n", oper);
            break;

        default:
            printf("%u\n", oper);
    }

    printf(" ├─ Sender MAC             : ");
    print_mac(arp->sender_hard_add);
    printf("\n");

    printf(" ├─ Sender IP              : ");
    print_ip(arp->sender_prot_add);
    printf("\n");

    printf(" ├─ Target MAC             : ");
    print_mac(arp->target_hard_add);
    printf("\n");

    printf(" └─ Target IP              : ");
    print_ip(arp->target_prot_add);
    printf("\n");
}