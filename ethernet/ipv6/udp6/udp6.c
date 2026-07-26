#define _GNU_SOURCE

#include "udp6.h"

#include <netinet/udp.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>


const char* identify_udp_protocol(uint16_t port);


void parse_udp6(const unsigned char* ip_buffer, size_t remaining_size){

     size_t ipv6_len = sizeof(struct ip6_hdr);

    if(remaining_size < ipv6_len){
        printf("Incomplete UDP6 packet");
        return;
    }

    size_t udp6_size = remaining_size - ipv6_len;

    if (udp6_size < sizeof(struct udphdr)) {
        printf("Incomplete UDP6 header\n");
        return;
    }

    const struct udphdr* udph = (const struct udphdr*)(ip_buffer + ipv6_len);

    unsigned short src_port = ntohs(udph->uh_sport);
    unsigned short dst_port = ntohs(udph->uh_dport);
    unsigned short udp_len  = ntohs(udph->uh_ulen);

    
    printf("\n");
    printf("UDP\n");
    printf(" ├─ Source Port                   : %u (%s)\n", src_port, identify_udp_protocol(src_port));
    printf(" ├─ Destination Port              : %u (%s)\n", dst_port, identify_udp_protocol(dst_port));
    printf(" ├─ UDP Length                    : %u\n", udp_len);
    printf(" └─ Checksum                      : 0x%04X\n", ntohs(udph->uh_sum));
    printf("\n");


}

const char* identify_udp_protocol(uint16_t port) {
    if (port == 53)  return "DNS (Domain Name System)";
    if (port == 67 || port == 68) return "DHCP";
    if (port == 123) return "NTP (Network Time Protocol)";
    if (port == 161 || port == 162) return "SNMP";
    if (port == 443) return "QUIC / HTTP3";
    if (port == 500) return "ISAKMP / IKE (IPsec)";
    
    return "Unknown UDP Protocol";
}
