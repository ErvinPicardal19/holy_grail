#include <errno.h>
#include <netinet/ip.h>
#include <regex.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

// Network Libraries
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

// Custom Libraries
#include <timerlib.h>

#define PORT 0
#define PACKET_SIZE 64

int ping_loop = 1;

// struct ping_pckt {
//     struct icmphdr hdr;
//     char msg[64 - sizeof(struct icmphdr)];
// };

void int_handler(int temp);
void ping(int sockfd, struct sockaddr_in *sockDest, const char *ip, const char *domainName, const char *reverseDomain);
char *dns_lookup(const char *domainName, struct sockaddr_in *sockDest);
char *reverse_dns_lookup(const char *ip);
void err_n_die(const char *fmt, ...);

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in sockDest;
    char *ip, *domainName = argv[1];

    if (argc != 2) err_n_die("usage: %s <destination>", argv[0]);

    if ((ip = dns_lookup(domainName, &sockDest)) == NULL)
        err_n_die("Could not resolve domain name %s", domainName);
    
    char *reverseDomain = reverse_dns_lookup(ip);
   
    signal(SIGINT, int_handler);

    if ( (sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0 )
        err_n_die("Socket file descriptor not recieved");
    
    ping(sockfd, &sockDest, ip, domainName, reverseDomain);
    // printf("%s\n", ip);
    // printf("%s\n", reverseDomain);


    free(ip);
    free(reverseDomain);
    return 0;
}

void int_handler(int temp)
{
    ping_loop = 0;
}

// Calculate the checksum (RFC 1071)
unsigned short checksum(void *b, size_t len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;

    return result;
}

void ping(int sockfd, struct sockaddr_in *sockDest, const char *ip, const char *domainName, const char *reverseDomain)
{   
    int ttl = 64, flag = 1, i = 0, msg_count = 0, msg_recieve_count = 0;
    
    struct icmphdr hdr;
    struct sockaddr_in r_addr;
    char packet[PACKET_SIZE];
    char rbuffer[PACKET_SIZE];
    socklen_t addr_len;
    
    struct timeval timeout;
    long start, end;
    long ping_start, ping_end;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) != 0)
        err_n_die("Setting socket option to TTL failed!\n");

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0)
        err_n_die("Setting socket option recieve timeout failed.");
    
    ping_start = millis();
    long temp;
    while (ping_loop)
    {
        flag = 1;

        memset(&hdr, 0, sizeof(hdr));
        hdr.type = ICMP_ECHO;
        hdr.un.echo.id = htons(getpid()); //setting the identifier of the request
        hdr.code = htons(0); // CODE FOR ECHO REQUEST
        hdr.un.echo.sequence = msg_count++;
        hdr.checksum = checksum(&hdr, sizeof(hdr));
        
        memcpy(packet, &hdr, sizeof(hdr));
        
        usleep(1000000);

        start = millis();

        if (sendto(sockfd, packet, sizeof(hdr), 0, (struct sockaddr *) sockDest, sizeof(*sockDest)) <= 0) {
            flag = 0;
            printf("\nPacket Sending Failed.\n");
        }

        addr_len = sizeof(r_addr);
        if (recvfrom(sockfd, rbuffer, sizeof(rbuffer), 0, (struct sockaddr *) &r_addr, &addr_len) <= 0 && msg_count > 1) {
            printf("\nPacket recieve failed!\n");
        } else {
            end = millis();

            long timeElapsed = end - start;

            if (flag) {
                struct icmphdr *recv_hdr = (struct icmphdr *) (rbuffer + sizeof(struct iphdr));
                if (!(recv_hdr->type == ICMP_ECHOREPLY || recv_hdr->code == 0))
                    printf("Error... Packet recieved with ICMP type %d code %d\n", recv_hdr->type, recv_hdr->code);
                else {
                    printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%ldms\n", sizeof(packet), reverseDomain, ip, msg_count, ttl, timeElapsed);
                }
                msg_recieve_count++;
            }
        }

    }

    ping_end = millis();

    long totalTimeElapsed = ping_end - ping_start;

    printf("\n--- %s ping statistics ---\n", domainName);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms", 
           msg_count, msg_recieve_count, ((msg_count - msg_recieve_count) /  msg_count), totalTimeElapsed);
}

int check_domain(const char *domainName)
{
    regex_t reg;
    
    if (strlen(domainName) > HOST_NAME_MAX) return -1;

    if ((regcomp(&reg, "^([a-z0-9][-a-z0-9]*\\.)+[a-z0-9][-a-z0-9]*", REG_EXTENDED) != 0))
        return -1;

    if ((regexec(&reg, domainName, 0, NULL, 0) != 0))
        return -1;

    regfree(&reg);
    return 0;
}

char *dns_lookup(const char *domainName, struct sockaddr_in *sockDest)
{
    struct hostent *host;
    char *ip = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));

    if (check_domain(domainName) < 0) return NULL;

    if ((host = gethostbyname(domainName)) == NULL) return NULL;

    if ((inet_ntop(AF_INET, host->h_addr_list[0], ip, INET_ADDRSTRLEN)) == NULL)
        return NULL;

    sockDest->sin_family = host->h_addrtype;
    sockDest->sin_port = htons(PORT);
    sockDest->sin_addr.s_addr = *(in_addr_t *)host->h_addr_list[0];

    return ip;
}

char *reverse_dns_lookup(const char *ip)
{
    struct sockaddr_in temp;
    size_t len;
    char buf[NI_MAXHOST], *retBuf;

    temp.sin_family = AF_INET;
    temp.sin_addr.s_addr = inet_addr(ip);
    len = sizeof(temp);

    if ( (getnameinfo((struct sockaddr *) &temp, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD)) != 0 )
        return NULL;

    retBuf = (char *) malloc((strlen(buf)+1) * sizeof(char));
    strcpy(retBuf, buf);

    return retBuf;
}

void err_n_die(const char *fmt, ...)
{
    va_list args;
    int errnoSave = errno;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    fflush(stderr);

    if (errnoSave != 0) {
        fprintf(stderr, "(errno = %d): %s\n", errnoSave, strerror(errnoSave));
        fflush(stderr);
    }

    va_end(args);
    exit(1);
}
