#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


// Standard HTTP port
#define SERVER_PORT 80
#define MAXLINE 4096

typedef struct sockaddr sockaddr;

void err_n_die(const char *fmt, ...);

int main(int argc, char *argv[])
{
    
    int                 sockfd, n;
    int                 sendbytes;
    struct sockaddr_in  server_address;
    char                sendline[MAXLINE];
    char                recvline[MAXLINE];
    
    if (argc != 2)
        err_n_die("usage: %s <server address>", argv[0]);

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        err_n_die("Error while creating the socket!");

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family   = AF_INET;
    server_address.sin_port     = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0 )
        err_n_die("inet_pton error for %s ", argv[1]);

    if (connect(sockfd, (sockaddr *) &server_address, sizeof(server_address)) < 0)
        err_n_die("connect failed.");

    sprintf(sendline, "GET / HTTP/1.1\r\nHost:google.com\r\n\r\n");
    sendbytes = strlen(sendline);

    if (write(sockfd, sendline, sendbytes) < 0)
        err_n_die("write error");

    memset(recvline, 0, MAXLINE);

    while( (n = read(sockfd, recvline, MAXLINE-1)) > 0 )
    {
       printf("%s", recvline);
    }

    if (n < 0)
        err_n_die("read error");


    return 0;
}

void err_n_die(const char *fmt, ...)
{
    int     errno_save;
    va_list args;

    errno_save = errno;

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (errno_save != 0) 
    {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    
    va_end(args);

    exit(1);
}
