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
    
    printf("$$$$$$$$$$$$$$$$$$$$$$$$ DNS $$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("  * Transaction ID          : 0x%04X\n", ntohs(dns->id));
    printf("  * Flag                    : %u\n", ntohs(dns->flag));
    printf("  * Questions               : %u\n", ntohs(dns->qdcount));
    printf("  * Answers                 : %u\n", ntohs(dns->ancount));
    printf("  * Num Authority RRS       : %u\n", ntohs(dns->num_authority_rrs));
    printf("  * Num Additional RRS      : %u\n",ntohs(dns->num_additional_rrs));
    printf("  * Question                : \n");

    const unsigned char *question = dns_payload + sizeof(dnshdr); 
    question = parse_question(question);

    printf("  * Anwser                  : \n");
    /*
    for (int i = 0; i < ntohs(dns->ancount); i++){
        answer = parse_answer(answer);
    }
    */

    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

}

const unsigned char *parse_question(const unsigned char *question){
    printf("      - Domain : ");

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

    printf("      - QTYPE  : %u\n", qtype);
    printf("      - QCLASS : %u\n", qclass);

    return question;
}

/*
const unsigned char *parse_answer(const unsigned char *answer){
    // Answer
    question += 2;
}
*/