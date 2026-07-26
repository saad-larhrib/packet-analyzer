#ifndef TCP6_H
#define TCP6_H

#include <stddef.h>


void parse_tcp6(const unsigned char *ip_buffer, size_t remaining_size);

#endif