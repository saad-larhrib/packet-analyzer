#include <netinet/udp.h>
#include <arpa/inet.h>


void parse_udp(const unsigned char* ip_buffer, size_t iph_len, size_t remaining_size){
    size_t udp_size = remaining_size - iph_len;

    if(udp_size < sizeof(struct udph)){
        printf("Incomplet UDP header");
        return;
    }

    const struct udphdr* udph = (const struct *)(ip_buffer + iph_len);
    printf("================= TCP PACKET ===================\n");
    printf("Source Port ")
}