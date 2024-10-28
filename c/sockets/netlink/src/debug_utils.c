#include "debug_utils.h"

#include <stdio.h>

void log_msg(int loglevel, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    openlog("netlink_socket", LOG_CONS | LOG_PID, LOG_USER);
    vsyslog(loglevel, fmt, args);
    closelog();
    va_end(args);
}

void hexdump(void *buf, unsigned long buf_size)
{
    char *hexints = "0123456789ABCDEF";
    char *octet = buf;

    for (int i = 0; i < buf_size; i++) {
        if ((i % 8) == 0) printf("\n");

        printf("%c", hexints[(*(octet + i) >> 4) & 0x0F]);
        printf("%c", hexints[*(octet + i) & 0x0F]);
        printf(" ");
        fflush(stdout);
    }

    printf("\n");
}
