#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <regex.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 0 // Automatically allocate available port

const char *dns_lookup(const char *domainName, struct sockaddr_in *sockInfo);
int check_domain_name(const char *domainName);
void err_n_die(const char *fmt, ...);

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in sockDest;
    const char *ip;

    if (argc != 2)
        err_n_die("usage: %s <destination>", argv[0]);

    if ( (ip = dns_lookup(argv[1], &sockDest)) == NULL)
        err_n_die("Could not resolve domain name %s", argv[1]);
    
    printf("%s\n", ip);

    free((void *)ip);

	return 0;
}

const char *dns_lookup(const char *domainName, struct sockaddr_in *sockDest)
{
    struct hostent* host;
    char *ip = (char *) malloc(sizeof(char) * INET_ADDRSTRLEN);

    if ( check_domain_name(domainName) < 0 )
        return NULL;


    if ( (host = gethostbyname(domainName)) == NULL )
        return NULL;
     
    if ( inet_ntop(AF_INET, (in_addr_t *) host->h_addr_list[0], ip, INET_ADDRSTRLEN) == NULL )
        return NULL;

    sockDest->sin_family = host->h_addrtype;
    sockDest->sin_port = htons(PORT);
    sockDest->sin_addr.s_addr = *(in_addr_t *) host->h_addr_list[0];

    return ip;
}

int check_domain_name(const char *domainName)
{
    regex_t reg;
    const char *pattern = "^([a-z0-9][-a-z0-9]*\\.)+[a-z0-9][-a-z0-9]*";
    if ( regcomp(&reg, pattern, REG_EXTENDED) != 0 )
        err_n_die("regcomp error");

    if ( regexec(&reg, domainName, 0, NULL, 0) != 0 ) {
        regfree(&reg);
        return -1;
    }

    regfree(&reg);

    return 0;
}

void err_n_die(const char *fmt, ...)
{
    va_list args;
    int errnoSave = 0;

    errnoSave = errno;

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

