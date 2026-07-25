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


void parse_dns(const unsigned char* dns_payload, size_t dns_size){
    if(sizeof(dnshdr) > dns_size){
        printf("Incomplet DNS header\n");
        return;
    }

    const dnshdr* dns = (const dnshdr*)dns_payload;
    
    printf("\n");
    printf("DNS\n");
    printf(" ├─ Transaction ID          : 0x%04X\n", ntohs(dns->id));
    printf(" ├─ Flag                    : %u\n", ntohs(dns->flag));
    printf(" ├─ Questions               : %u\n", ntohs(dns->qdcount));
    printf(" ├─ Answers                 : %u\n", ntohs(dns->ancount));
    printf(" ├─ Num Authority RRS       : %u\n", ntohs(dns->num_authority_rrs));
    printf(" ├─ Num Additional RRS      : %u\n",ntohs(dns->num_additional_rrs));
    printf(" ├─ Question                : \n");

    const unsigned char *question = dns_payload + sizeof(dnshdr); 
    question = parse_question(question);

    const unsigned char* answer = question;

    printf(" ├─ Answer                  : \n");

    for (int i = 0; i < ntohs(dns->ancount); i++){
        answer = parse_answer(answer);
    }

    printf("\n");
}

const unsigned char *parse_question(const unsigned char *question){
    printf("       ├─ Domain : ");

    while (*question != 0)
    {
        unsigned char len = *question;
        question++;

        for (int i = 0; i < len; i++)
        {
            printf("%c", question[i]);
        }

        question += len;

        if (*question != 0)
            printf(".");
    }

    printf("\n");

    question++;      // skip terminating 0

    uint16_t qtype = ntohs(*(uint16_t *)question);
    question += 2;

    uint16_t qclass = ntohs(*(uint16_t *)question);
    question += 2;

    printf("       ├─ QTYPE  : %u\n", qtype);
    printf("       ├─ QCLASS : %u\n", qclass);

    return question;
}


const unsigned char *parse_answer(const unsigned char *answer){
    uint16_t name = ntohs(*(uint16_t *)answer);
    answer += 2;

    uint16_t type = ntohs(*(uint16_t *)answer);
    answer += 2;

    uint16_t class = ntohs(*(uint16_t *)answer);
    answer += 2;

    uint32_t ttl = ntohl(*(uint32_t *)answer);
    answer += 4;

    uint16_t rd_len = ntohs(*(uint16_t *)answer);
    answer += 2;

    printf("       ├─ NAME Pointer : 0x%04X\n", name);
    printf("       ├─ TYPE         : %u\n", type);
    printf("       ├─ CLASS        : %u\n", class);   
    printf("       ├─ TTL          : %u\n", ttl);
    printf("       ├─ RDLENGTH     : %u\n", rd_len);

    if (type == 1 && rd_len == 4) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, answer, ip, sizeof(ip));
        printf("       ├─ Address      : %s\n", ip);
    }

    answer += rd_len;

    return answer;
}