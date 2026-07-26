#define _GNU_SOURCE

#include <netinet/tcp.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "tcp4.h"


void parse_tcp(const unsigned char *ip_buffer, size_t iphdr_len, size_t remaining_size){

    size_t tcp_size = remaining_size - iphdr_len;

    if (tcp_size < sizeof(struct tcphdr)) {
        printf("Incomplete TCP header\n");
        return;
    }

    const struct tcphdr* tcph = (const struct tcphdr *)(ip_buffer + iphdr_len);

    unsigned short source_port = ntohs(tcph->th_sport);
    unsigned short destination_port = ntohs(tcph->th_dport);
    uint32_t seq_num = ntohl(tcph->th_seq);
    uint32_t ack_num = ntohl(tcph->th_ack);
    unsigned short tcp_header_len = tcph->th_off * 4;


    printf("\n");
    printf("TCP\n");
    printf(" ├─ Header Length                : %u Bytes\n", tcp_header_len);
    printf(" ├─ Source Port                  : ");
    switch(source_port){
        case 80: printf("HTTP\n"); break;
        case 443: printf("HTTPS\n"); break;
        case 22: printf("SSH\n"); break;
        case 21: printf("FTP\n"); break;
        case 25: printf("SMTP\n"); break;
        case 53: printf("DNS\n"); break;
        case 110: printf("POP3\n"); break;
        case 143: printf("IMAP\n"); break;
        default: printf("%u\n", source_port); break;
    }

    printf(" ├─ Destination Port             : ");
    switch(destination_port){
        case 80: printf("HTTP\n"); break;
        case 443: printf("HTTPS\n"); break;
        case 22: printf("SSH\n"); break;
        case 21: printf("FTP\n"); break;
        case 25: printf("SMTP\n"); break;
        case 53: printf("DNS\n"); break;
        case 110: printf("POP3\n"); break;
        case 143: printf("IMAP\n"); break;
        default: printf("%u\n", destination_port); break;
    }

    printf(" ├─ Sequence Number              : %u\n", seq_num);
    printf(" ├─ Acknowledgement Number       : %u\n", ack_num);
    printf(" ├─ Window Size                  : %u\n",ntohs(tcph->th_win));
    printf(" ├─ CheckSum                     : 0x%04X\n", ntohs(tcph->th_sum));
    printf(" ├─ Point Urgent                 : %u\n", ntohs(tcph->th_urp));
    printf(" └─ Flags                        : ");
    if (tcph->th_flags & TH_SYN) printf("SYN ");
    if (tcph->th_flags & TH_ACK) printf("ACK ");
    if (tcph->th_flags & TH_FIN) printf("FIN ");
    if (tcph->th_flags & TH_RST) printf("RST ");
    if (tcph->th_flags & TH_PUSH) printf("PSH ");
    if (tcph->th_flags & TH_URG) printf("URG ");
    printf("\n");

}