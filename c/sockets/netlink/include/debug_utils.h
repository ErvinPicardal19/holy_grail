#ifndef __DEBUG_UTILS_H__
#define __DEBUG_UTILS_H__

#include <stdarg.h>
#include <sys/syslog.h>

void log_msg(int loglevel, char *fmt, ...);
void hexdump(void *buf, unsigned long buf_size);

#define ERROR_LOG(fmt, ...)                                                      \
    log_msg(LOG_ERR, "[%s:%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, \
            ##__VA_ARGS__)

#define DEBUG_LOG(fmt, ...)                                                        \
    log_msg(LOG_DEBUG, "[%s:%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, \
            ##__VA_ARGS__)

#endif  //__DEBUG_UTILS_H__
