#include "ndp_debug.h"

#include <stdio.h>

void debug_print(int loglevel, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    openlog("sc_neighbored", LOG_CONS | LOG_PID, LOG_USER);
    vsyslog(loglevel, fmt, args);
    closelog();

    va_end(args);
}
