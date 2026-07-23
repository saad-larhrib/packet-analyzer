#define _GNU_SOURCE

#include "udp.h"
#include "dns.h"
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdio.h>



void parse_udp(const unsigned char* ip_buffer, size_t iph_len, size_t remaining_size){
    size_t udp_size = remaining_size - iph_len;

    if(udp_size < sizeof(struct udphdr)){
        printf("Incomplet UDP header\n");
        return;
    }

    const struct udphdr* udph = (const struct udphdr*)(ip_buffer + iph_len);

    unsigned short src_port = ntohs(udph->uh_sport);
    unsigned short dst_port = ntohs(udph->uh_dport);
    unsigned short udp_len  = ntohs(udph->uh_ulen);

    
    
    printf("=================== UDP PACKET ===================\n");
    printf("Source Port                   : %u\n", src_port);
    printf("Destination port              : %u \n", dst_port);
    printf("UDP Length                    : %u\n", udp_len);
    printf("Checksum                      : 0x%04X\n", ntohs(udph->uh_sum));
    printf("==================================================\n");

    if(src_port || dst_port){
        unsigned char* dns_pylod = ip_buffer + iph_len + sizeof(struct udphdr);
        size_t dsn_size = remaining_size - iph_len - sizeof(struct udphdr);

        parse_dns(dns_pylod, dsn_size);
    }

}