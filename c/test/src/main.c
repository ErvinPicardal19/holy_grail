#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#include <timerlib.h>

#define PING_PACKET_SIZE 64

struct ping_pckt {
    struct icmphdr hdr;
    char msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];
};

void create_icmp_pckt(struct ping_pckt* ping_pckt, char *pckt, size_t pcktsize, int sequence);

int main(void)
{
    struct ping_pckt ping_pckt;
    char pckt[PING_PACKET_SIZE];

    create_icmp_pckt(&ping_pckt, pckt, PING_PACKET_SIZE)

        
    return 0;
}

void create_icmp_pckt(struct ping_pckt* ping_pckt, char *pckt, size_t pcktsize, int sequence)
{
    memset(ping_pckt, 0, pcktsize);
    memset(pckt, 0, pcktsize);
    
    ping_pckt->hdr.type = htons(ICMP_ECHO);
    ping_pckt->hdr.code = 0;
    ping_pckt->hdr.un.echo.id = htons(getpid());
    ping_pckt->hdr.un.echo.sequence = htons(sequence);

    strcpy(ping_pckt->msg, "Hello World!");
    
    memcpy(pckt, ping_pckt, pcktsize);
}
