#include "icmp6.h"
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <stdio.h>


void parse_icmp6(const unsigned char *ip_buffer, size_t remaining_size){
    size_t ipv6_len = sizeof(struct ip6_hdr);

    if(remaining_size < ipv6_len){
        printf("Incomplete ICMPv6 packet");
        return;
    }

    size_t icmp6_size = remaining_size - ipv6_len;

    if (icmp6_size < sizeof(struct icmp6_hdr)){
        printf("Incomplete ICMPv6 header\n");
        return;
    }

    const struct icmp6_hdr* icmp6hdr = (const struct icmp6_hdr *)(ip_buffer + ipv6_len);
    
    uint8_t  type_field = icmp6hdr->icmp6_type;
    uint8_t  Code_field = icmp6hdr->icmp6_code;
    uint16_t checksum_field = ntohs(icmp6hdr->icmp6_cksum);

    printf("\n");
    printf("ICMPv6 \n");
    printf(" ├─  Type Field      : %u\n", type_field);
    printf(" ├─  Code Field      : %u\n", Code_field);
    printf(" └─  Checksum Field  : %u\n", checksum_field);
}