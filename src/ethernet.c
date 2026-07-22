#include "ethernet.h"
#include "protocol.h"
#include <stdio.h>
#include <arpa/inet.h>

void print_mac(const unsigned char* mac){
    printf(" %02x:%02x:%02x:%02x:%02x:%02x", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
        );
}

unsigned short parse_ethernet(const struct ethhdr* eth){
    printf("Destination : ");
    print_mac(eth->h_dest);
    printf("\n");

    printf("Source      : ");
    print_mac(eth->h_source);
    printf("\n");

    unsigned short ether_type = ntohs(eth->h_proto);
    printf("Protocol    : %s (0x%04X)\n", get_ethertype_name(eth->h_proto), ether_type);

    return ether_type;
}
