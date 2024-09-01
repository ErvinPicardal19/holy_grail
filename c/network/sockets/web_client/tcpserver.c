#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <strings.h>
#include <sys/poll.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Standard HTTP port
#define SERVER_PORT 18000
#define MAXLINE 4096

typedef struct sockaddr sockaddr;

void err_n_die(const char *fmt, ...);
char *bin2hex(const unsigned char *input, size_t len);

int main(int argc, char *argv[])
{
    int                 socket_fd, connfd, n;
    struct sockaddr_in  server_address;
    uint8_t             buff[MAXLINE+1];
    uint8_t             recvline[MAXLINE+1];

    if ( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        err_n_die("socket error : %s %d", __FUNCTION__, __LINE__);

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( (bind(socket_fd, (sockaddr *) &server_address, sizeof(server_address))) < 0 )
        err_n_die("bind error : %s %d", __FUNCTION__, __LINE__);
    
    if ( (listen(socket_fd, 10)) < 0 )
        err_n_die("listen error : %s %d", __FUNCTION__, __LINE__);

    while (1)
    {
        struct sockaddr_in addr;
        socklen_t addr_len;
        
        printf("Waiting for a connection on port %d\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(socket_fd, (sockaddr *) &addr, &addr_len);

        if(connfd < 0)
            err_n_die("accept error.");

        memset(recvline, 0, MAXLINE);
        
        while ( (n = read(connfd, recvline, MAXLINE-1)) > 0 )
        {
            char *temp = bin2hex(recvline, n);
            fprintf(stdout, "\n%s\n\n%s", temp, recvline);
            
            if (recvline[n-1] == '\n')
                break;
        
            memset(recvline, 0, MAXLINE);
        }
        if (n < 0)
            err_n_die("read error");
        printf("Here.\n");
        snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

        write(connfd, (char*)buff, strlen((char *)buff));

        close(connfd);
    }
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

char *bin2hex(const unsigned char *input, size_t len)
{
    char *result;
    char *hexits = "0123456789ABCDEF";
    
    if (input == NULL || len <= 0)
        return NULL;

    int resultlength = (len * 3) + 1;


    result = malloc(resultlength);
    memset(result, 0, resultlength);

    for (int i = 0; i < len; i++)
    {
        result[i*3] = hexits[input[i] >> 4];
        result[(i*3)+1] = hexits[input[i] & 0x0F];
        result[(i*3)+2] = ' ';
    }

    return result;
}
