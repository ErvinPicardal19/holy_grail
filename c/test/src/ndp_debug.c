#include "ndp_debug.h"

#include <stdio.h>
#include <stdint.h>

void debug_print(int loglevel, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    openlog("sc_neighbored", LOG_CONS | LOG_PID, LOG_USER);
    vsyslog(loglevel, fmt, args);
    closelog();

    va_end(args);
}

void hexdump(void *buf, long len)
{
    char *hexints = "0123456789ABCDEF";
    
    uint8_t *hex = buf;

    for(int i = 0; i < len; i++)
    {
        if((i % 8) == 0)
        {
            printf("\n");
        }

        printf("%c", hexints[((*hex >> 4) & 0x0F)]);
        printf("%c", hexints[(*hex & 0x0F)]);
        printf(" ");
        fflush(stdout);
        
        hex+=1;
    }
}
