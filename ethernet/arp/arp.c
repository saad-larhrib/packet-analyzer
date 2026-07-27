#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <linux/if_arp.h>

typedef struct __attribute__((packed)){
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t  hardware_len;
    uint8_t  protocol_len;
    uint16_t operation;

    uint8_t sender_hard_add[6];
    uint8_t sender_prot_add[4];

    uint8_t target_hard_add[6];
    uint8_t target_prot_add[4];
}arp_hdr_t;

static void print_mac(const uint8_t* mac){

}

void parse_arp(unsigned char ip_pyload, size_t remaining_size){
    if(remaining_size < sizeof(arp_hdr_t)){

    }
    
    const struct arp_hdr_t* arp = (const struct arp_hdr_t*)ip_pyload;

    uint16_t h_type = ntohs(arp->hardware_type);
    uint16_t p_type = ntohs(arp->protocol_type);
    uint8_t  h_leng = arp->hardware_len;
    uint8_t  p_leng = arp->protocol_len;
    uint16_t operat = arp->operation;

    

}