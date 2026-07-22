#include "ethernet.h"
#include <stdio.h>

void print_mac(const unsigned char* mac){
    printf(" %02x:%02x:%02x:%02x:%02x:%02x", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
        );
}