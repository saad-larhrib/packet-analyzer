#ifndef TCP4_H
#define TCP4_H

#include <stddef.h>

void parse_tcp(const unsigned char *ip_buffer,
               size_t iphdr_len,
               size_t remaining_size);

#endif