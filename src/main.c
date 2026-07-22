#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <linux/ip.h> // Required for struct iphdr
#include <linux/if_ether.h> // for strcut ethhdr 

#define BUFFER_SIZE 65536


/*
struct Ethernet {
    unsigned char destination[6];
    unsigned char source[6];
    unsigned short protocol;
};
*/


const char* get_the_type_of_protocol(unsigned short protocol){
    protocol = ntohs(protocol);
    switch(protocol){
        case ETH_P_IP: return "IPv4";
        case ETH_P_IPV6: return "IPv6";
        case ETH_P_ARP: return "ARP";
        default: return "Unknown";
    }
}

const char* get_protocol_payload_name(int protocol_num) {
    switch (protocol_num) {
        case IPPROTO_ICMP: // Maps to numeric value 1
            return "Payload: ICMP";
        
        case IPPROTO_IGMP: // Maps to numeric value 2
            return "Payload: IGMP";
        
        case IPPROTO_TCP: // Maps to numeric value 6
            return "Payload: TCP";
        
        case IPPROTO_UDP: // Maps to numeric value 17
            return "Payload: UDP";
        
        case IPPROTO_IPV6: // Maps to numeric value 41
            return "Payload: IPv6 encapsulation (IPv6 inside IPv4 tunneling)";
        
        case 47: // GRE (Generic Routing Encapsulation) - Rarely defined in standard in.h
            return "Payload: GRE (VPN tunneling)";
        
        case IPPROTO_ESP: // Maps to numeric value 50 (IPsec Encapsulating Security Payload)
            return "Payload: ESP";
        
        case IPPROTO_AH:  // Maps to numeric value 51 (IPsec Authentication Header)
            return "Payload: AH";
        
        case 89: // OSPF (Open Shortest Path First) - Commonly left as numeric literal
            return "Payload: OSPF";
        
        default: 
            return "Payload: Other Protocol";
    }
}


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

void print_mac(const unsigned char* mac){
    printf(" %02x:%02x:%02x:%02x:%02x:%02x", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
        );
}

int main(){
    // Note: Running this requires sudo privileges (CAP_NET_RAW)
    int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(packet_socket < 0){
        perror("Failed to Create a Socket");
        return 1;
    }

    unsigned char buffer[BUFFER_SIZE];

    while(1){
        ssize_t packet_size = recv(packet_socket, buffer, sizeof(buffer), 0);
        if(packet_size < 0){
            perror("Failed Receive");
            break;
        }

        if(packet_size < sizeof(struct ethhdr))
            continue;

        struct ethhdr* eth = (struct ethhdr *)buffer; 
        
        printf("Packet Size : %zd bytes\n", packet_size);

        printf("Destination : ");
        print_mac(eth->h_dest);
        printf("\n");

        printf("Source      : ");
        print_mac(eth->h_source);
        printf("\n");
        
        unsigned short proto = ntohs(eth->h_proto);
        printf("Protocol    : %s (0x%04X)\n", get_the_type_of_protocol(eth->h_proto), proto);

        // If the payload is IPv4, parse it by shifting past the Ethernet header
        if (proto == ETH_P_IP) {
            const unsigned char* ip_payload = buffer + sizeof(struct ethhdr);
            size_t remaining_bytes = packet_size - sizeof(struct ethhdr);
            parse_ipv4(ip_payload, remaining_bytes);
        }

        printf("-----------------------------------------\n");
    }

    close(packet_socket);
    return 0;
}