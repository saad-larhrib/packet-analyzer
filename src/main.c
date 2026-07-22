#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

#include "ethernet.h"
#include "protocol.h"
#include "ipv4.h"

#define BUFFER_SIZE 65536
#define RUN_FOREVER 1


int main(){
    // Note: Running this requires sudo privileges (CAP_NET_RAW)
    int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(packet_socket < 0){
        perror("Failed to Create a Socket");
        return 1;
    }

    unsigned char buffer[BUFFER_SIZE];

    while(RUN_FOREVER){
        ssize_t packet_size = recv(packet_socket, buffer, sizeof(buffer), 0);
        if(packet_size < 0){
            perror("Failed Receive");
            break;
        }

        if((size_t)packet_size < sizeof(struct ethhdr))
            continue;

        const struct ethhdr* eth = (const struct ethhdr *)buffer; 
        
        printf("Packet Size : %zd bytes\n", packet_size);

        unsigned short ether_type = parse_ethernet(eth);
        
        // If the payload is IPv4, parse it by shifting past the Ethernet header
        if (ether_type == ETH_P_IP) {
            const unsigned char* ip_payload = buffer + sizeof(struct ethhdr);
            size_t remaining_bytes = packet_size - sizeof(struct ethhdr);
            parse_ipv4(ip_payload, remaining_bytes);
        }

        printf("-----------------------------------------\n");
    }

    close(packet_socket);
    return 0;
}