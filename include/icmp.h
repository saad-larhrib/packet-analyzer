#ifndef ICMP_H
#define ICMP_H

#include <stddef.h>


void parse_icmp(const unsigned char* ip_buffer, size_t iph_len, size_t remaining_size);

#endif