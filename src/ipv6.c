#include "ipv6.h"
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <stdio.h>

void parse_ipv6(const unsigned char *ip_buffer, size_t remaining_size){

    if(remaining_size < sizeof(struct ip6_hdr)){
        printf("   ├─ [IPv6] Packet payload too small for IPv6 header\n");
        return;
    }


    const struct ip6_hdr* ip6_h = (const struct ip6_hdr *)ip_buffer;

    uint32_t flow = ntohl(ip6_h->ip6_flow);

    uint8_t version = (flow >> 28) & 0x0F;
    uint8_t traffic_class = (flow >> 20) & 0xFF;
    uint32_t flow_label = flow & 0xFFFFF;
    uint16_t payload_len = ntohs(ip6_h->ip6_plen);
    uint8_t next_header = ip6_h->ip6_nxt;
    uint8_t hop_limit = ip6_h->ip6_hlim;

    char src_ip[INET6_ADDRSTRLEN];
    char dst_ip[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6, &ip6_h->ip6_src, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET6, &ip6_h->ip6_dst, dst_ip, sizeof(dst_ip));

    const char* next = next_header_value(next_header);
    print_ipv6(version, traffic_class, flow_label, payload_len, next, hop_limit, src_ip, dst_ip);

}

const char* next_header_value(uint8_t next){
        switch(next){
        case 6 : return "TCP"; break;
        case 17: return "UDP"; break;
        case 58: return "ICMPv6"; break;
        case 43: return "Routing"; break;
        case 44: return "Fragment"; break;
        default: return "Unknown"; break;
    }
}

void print_ipv6(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint16_t payload_len, 
    const char* next, uint8_t hop_limit, const char *src_ip, const char *dst_ip){
    printf("\n");
    printf("IPv6 Header\n");
    printf(" ├─ Version                : %u\n", version);
    printf(" ├─ Traffic Class          : %u\n", traffic_class);
    printf(" ├─ Flow Label             : %u\n", flow_label);
    printf(" ├─ Payload Length         : %u\n", payload_len);
    printf(" ├─ Next Header            : %s\n", next);
    printf(" ├─ Hop Limit              : %u\n", hop_limit);
    printf(" ├─ Source IP              : %s\n", src_ip);
    printf(" ├─ Destination IP         : %s\n", dst_ip);
}