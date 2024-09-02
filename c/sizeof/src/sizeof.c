#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

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
