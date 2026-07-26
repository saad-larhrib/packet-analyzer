#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include "protocol.h"

const char* get_ethertype_name(unsigned short protocol){
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