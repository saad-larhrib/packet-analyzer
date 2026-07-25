#ifndef FILTER_H
#define FILTER_H

typedef enum
{
    FILTER_NONE,
    FILTER_TCP,
    FILTER_UDP,
    FILTER_ICMP,
    FILTER_DNS,
    FILTER_IPV4,
    FILTER_IPV6
} FilterType;

extern FilterType current_filter;

void parse_filter(int argc, char *argv[]);

#endif