#ifndef DNS_H
#define DNS_H

#include <stddef.h>


void parse_dns(const unsigned char* dns_pylod, size_t dsn_size);
const unsigned char *parse_question(const unsigned char *question);
const unsigned char *parse_answer(const unsigned char *answer);

#endif