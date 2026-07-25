#include "filter.h"
#include <string.h>

FilterType current_filter = FILTER_NONE;

void parse_filter(int argc, char *argv[])
{
    if (argc < 2)
        return;

    if (strcmp(argv[1], "--tcp") == 0)
        current_filter = FILTER_TCP;

    else if (strcmp(argv[1], "--udp") == 0)
        current_filter = FILTER_UDP;

    else if (strcmp(argv[1], "--icmp") == 0)
        current_filter = FILTER_ICMP;

    else if (strcmp(argv[1], "--dns") == 0)
        current_filter = FILTER_DNS;

    else if (strcmp(argv[1], "--ipv4") == 0)
        current_filter = FILTER_IPV4;

    else if (strcmp(argv[1], "--ipv6") == 0)
        current_filter = FILTER_IPV6;
}