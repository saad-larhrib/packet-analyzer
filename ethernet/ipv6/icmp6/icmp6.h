#ifndef ICMP6_H
#define ICMP6_H

#include <stddef.h>


void parse_icmp6(const unsigned char *ip_buffer, size_t remaining_size);

#endif