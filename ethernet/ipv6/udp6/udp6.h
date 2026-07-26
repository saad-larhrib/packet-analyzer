#ifndef UDP6_H
#define UDP6_H

#include <stddef.h>
#include <stdint.h>

void parse_udp6(const unsigned char* ip_buffer, size_t remaining_size);
const char* identify_udp_protocol(uint16_t port);

#endif