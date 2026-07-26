#ifndef UDP4_H
#define UDP4_H

#include <stddef.h>

void parse_udp(const unsigned char* ip_buffer, size_t iph_len, size_t remaining_size);

#endif