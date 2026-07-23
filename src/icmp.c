#define _GNU_SOURCE

#include "icmp.h"
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>

void parse_icmp(const unsigned char* ip_buffer, size_t iph_len, size_t remaining_size){
    size_t icmp_size = remaining_size - iph_len;

    if(icmp_size < sizeof(struct icmp)){
        printf("Incomplet ICMP header");
        return;
    }

    const struct icmp* icmphdr = (const struct icmp*)(ip_buffer + iph_len);

    unsigned short t_icmp = icmphdr->icmp_type;
    unsigned short c_icmp = icmphdr->icmp_code;
    
    printf("=================== ICMP PACKET ===================\n");
    printf("Type of ICMP                  : %u\n", t_icmp);
    printf("Code of ICMP                  : %u \n", c_icmp);
    printf("Checksum                      : 0x%04X\n", ntohs(icmphdr->icmp_cksum));
    printf("==================================================\n");

}
