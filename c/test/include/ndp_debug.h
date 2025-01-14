#ifndef __DEBUG_UTIL_H__
#define __DEBUG_UTIL_H__

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>

void debug_print(int loglevel, char *fmt, ...);
void hexdump(void *buf, long len);

#define ERR_LOG(fmt, ...) \
    debug_print(LOG_ERR, "[%s][%s:%d] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#define DEBUG_LOG(fmt, ...) \
    debug_print(LOG_INFO, "[%s][%s:%d] " fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#endif //__DEBUG_UTIL_H__
