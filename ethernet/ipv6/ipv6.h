#ifndef IPV6_H
#define IPV6_H

#include <stddef.h>
#include <stdint.h>

void parse_ipv6(const unsigned char *ip_buffer, size_t remaining_size);
const char* next_header_value(uint8_t next);
void print_ipv6(uint8_t version, uint8_t traffic_class, uint32_t flow_label, uint16_t Pyload_len, const char* next, uint8_t hop_limit, const char *src_ip, const char *dst_ip);


#endif