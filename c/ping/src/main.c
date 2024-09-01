#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>

const char *dns_lookup(char *host_name, struct sockaddr_in* socket_info)
{
    struct hostent *host_entity;
    char *ip = (char *) malloc(sizeof(INET_ADDRSTRLEN));
   
    
    host_entity = gethostbyname(host_name);

    if (host_entity == NULL)
    {
        fprintf(stderr, "No IP found for hostname");
        exit(1);
    }

    const char *s = inet_ntop(AF_INET, ((struct in_addr_t **) host_entity->h_addr_list)[0], ip, INET_ADDRSTRLEN);
    if (s == NULL)
    {
        perror("inet_ntop");
        exit(1);
    }

    socket_info->sin_family = host_entity->h_addrtype;
    socket_info->sin_port = htons(0);
    socket_info->sin_addr.s_addr = *(in_addr_t *) (host_entity->h_addr_list[0]);
    return ip;
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in sock_add;
    char iif[INET_ADDRSTRLEN];
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <ip_address>\n", argv[0]);
        exit(1);
    }   


    const char *ip = dns_lookup(argv[1], &sock_add);
    
    printf("%s\n", ip);

    free((void *)ip);
	return 0;
}
