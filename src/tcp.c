#define _GNU_SOURCE

#include <netinet/tcp.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "tcp.h"


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


    printf("================= TCP PACKET ===================\n");
    printf("Header Length                : %u Bytes\n", tcp_header_len);
    printf("Source Port                  : %u\n", source_port);
    printf("Destination Port             : %u\n", destination_port);
    printf("Sequence Number              : %u\n", seq_num);
    printf("Acknowledgement Number       : %u\n", ack_num);
    printf("Window Size                  : %u\n",ntohs(tcph->th_win));
    printf("CheckSum                     : 0x%04X\n", ntohs(tcph->th_sum));
    printf("Point Urgent                 : %u\n", ntohs(tcph->th_urp));
    printf("Flags : \n");
    if (tcph->th_flags & TH_SYN) printf("SYN ");
    if (tcph->th_flags & TH_ACK) printf("ACK ");
    if (tcph->th_flags & TH_FIN) printf("FIN ");
    if (tcph->th_flags & TH_RST) printf("RST ");
    if (tcph->th_flags & TH_PUSH) printf("PSH ");
    if (tcph->th_flags & TH_URG) printf("URG ");
    printf("\n");
    printf("================================================\n");

}