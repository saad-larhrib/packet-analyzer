#define _GNU_SOURCE

#include "dns.h"
#include <stdio.h>
#include <arpa/inet.h>


typedef struct {
    uint16_t id;
    uint16_t flag;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t num_authority_rrs;
    uint16_t num_additional_rrs;
}dnshdr;


void parse_dns(unsigned char* dns_payload, size_t dns_size){
    if(sizeof(dnshdr) > dns_size){
        printf("Incomplet DNS header\n");
        return;
    }

    const dnshdr* dns = (const dnshdr*)dns_payload;
    
    printf("$$$$$$$$$$$$$$$$$$$$$$$$ DNS $$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("Transaction ID          : 0x%04X\n", ntohs(dns->id));
    printf("Flag                    : %u\n", ntohs(dns->flag));
    printf("Questions               : %u\n", ntohs(dns->qdcount));
    printf("Answers                 : %u\n", ntohs(dns->ancount));
    printf("Num Authority RRS       :%u\n", ntohs(dns->num_authority_rrs));
    printf("Num Additional RRS      : %u\n",ntohs(dns->num_additional_rrs));
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

}