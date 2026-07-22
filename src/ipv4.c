#include <ipv4.h>
#include "protocol.h"
#include <linux/ip.h>
#include <arpa/inet.h>


void parse_ipv4(const unsigned char* ip_buffer, size_t remaining_size) {
    if (remaining_size < sizeof(struct iphdr)) {
        printf("   [IPv4] Packet payload too small for IPv4 header\n");
        return;
    }

    const struct iphdr *iph = (const struct iphdr *)ip_buffer;

    if(iph->version != 4){
        printf("IPv4 invalide\n");
        return;
    }

    char src_ip[INET_ADDRSTRLEN];
    char dest_ip[INET_ADDRSTRLEN];

    // Convert binary IP addresses to human-readable strings
    if(inet_ntop(AF_INET, &(iph->saddr), src_ip, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop error\n");
        return;
    }

    if(inet_ntop(AF_INET, &(iph->daddr), dest_ip, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop error\n");
        return;
    }

    if(iph->ihl < 5){
        printf("Invalid IPv4 Header\n");
        return;
    }

    unsigned short iphdr_len = iph->ihl * 4;

    if (iphdr_len > remaining_size){
        printf("Incomplete IPv4 header\n");
        return;
    }

    const char* protocol_name = get_protocol_payload_name(iph->protocol);

    printf("   --- IPv4 Header ---\n");
    printf("   |- Version      : %d\n", iph->version);
    printf("   |- Header Length: %d Bytes\n", iphdr_len);
    printf("   |- Protocol     : %s\n", protocol_name);
    printf("   |- Total Length : %d Bytes\n", ntohs(iph->tot_len));
    printf("   |- TTL          : %d\n", iph->ttl);
    printf("   |- Source IP    : %s\n", src_ip);
    printf("   |- Destination  : %s\n", dest_ip);
}