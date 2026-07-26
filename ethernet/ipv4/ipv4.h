#ifndef IPV4_H
#define IPV4_H

#include <stddef.h>

void parse_ipv4(const unsigned char* ip_buffer, size_t remaining_size);

#endif