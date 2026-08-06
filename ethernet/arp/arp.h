#ifndef ARP_H
#define ARP_H

#include <linux/if_arp.h>
#include <stdint.h>

/*void print_mac(const uint8_t *mac);*/
void print_ip(const uint8_t *ip);
void parse_arp(const unsigned char *arp_payload, size_t remaining_size);

#endif
