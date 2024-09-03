
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

// Network Libraries
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

// Custom Libraries
#include <timerlib.h>

#define PING_PACKET_SIZE 64

struct ping_pckt {
    struct icmphdr hdr;
    char msg[PING_PACKET_SIZE - sizeof(struct icmphdr)];
};

void create_icmp_pckt(struct ping_pckt* ping_pckt, char *pckt, size_t pcktsize, int sequence);
unsigned short calculate_checksum(void *pckt, size_t pckt_len);

int main(void)
{
    struct ping_pckt ping_pckt;
    char pckt[PING_PACKET_SIZE];

    create_icmp_pckt(&ping_pckt, pckt, PING_PACKET_SIZE, 0);
    
    char bytes[32];
    memset(bytes, 0, 32);
    memset(bytes, 0x4500, 2);
    strcat(bytes, 0x0073);
    strcat(bytes, 0x0000);
    strcat(bytes, 0x4000);
    strcat(bytes, 0x4011);
    strcat(bytes, 0x0000);
    strcat(bytes, 0xc0a8);
    strcat(bytes, 0x0001);
    strcat(bytes, 0xc0a8);
    strcat(bytes, 0x00c7);
    strcat(bytes, 0x0035);
    strcat(bytes, 0xe97c);
    strcat(bytes, 0x005f);
    strcat(bytes, 0x279f);
    strcat(bytes, 0x1e4b);
    strcat(bytes, 0x8180);
    

#ifdef DEBUG
    struct icmphdr *hdr = (struct icmphdr *) pckt;
    char *message = (char *) (pckt + sizeof(struct icmphdr));
    printf("Type=%d\n", hdr->type);
    printf("Code=%d\n", hdr->code);
    printf("Echo ID=%d\n", ntohs(hdr->un.echo.id));
    printf("Echo Sequence=%d\n", ntohs(hdr->un.echo.sequence));
    printf("msg=%s\n", message);
#endif

    return 0;
}

unsigned short calculate_checksum(void *pckt, size_t pcktlen)
{
    unsigned short *twobyte = (unsigned short *) pckt;
    unsigned int sum = 0;
    unsigned short result;

    for (int i = 0; i < (pcktlen/2); i++)
        sum += *twobyte++;
    
    if ((pcktlen % 2) != 0)
        sum += *(unsigned char*) twobyte;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = (~sum);
}

void create_icmp_pckt(struct ping_pckt* pingpckt, char *pckt, size_t pcktsize, int sequence)
{
    memset(pingpckt, 0, pcktsize);
    memset(pckt, 0, pcktsize);
    
    pingpckt->hdr.type = ICMP_ECHO;
    pingpckt->hdr.code = 0;
    pingpckt->hdr.un.echo.id = htons(getpid());
    pingpckt->hdr.un.echo.sequence = htons(sequence);

    strcpy(pingpckt->msg, "Hello World!");
    
    pingpckt->hdr.checksum = 0;
    // ping_pckt->hdr.checksum = calculate_checksum(&ping_pckt, PING_PACKET_SIZE);
    
    memcpy(pckt, pingpckt, pcktsize);
}
