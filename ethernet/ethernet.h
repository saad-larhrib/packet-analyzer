#ifndef ETHERNET_H
#define ETHERNET_H

#include <linux/if_ether.h>


void print_mac(const unsigned char* mac);
unsigned short parse_ethernet(const struct ethhdr* eth);

#endif