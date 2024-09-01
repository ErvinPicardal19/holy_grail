#include "timerlib.h"

#include <time.h>
#include <unistd.h>

unsigned long millis()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    unsigned long ms =
        sec2ms((unsigned long)tp.tv_sec) + ns2ms((unsigned long)tp.tv_nsec);
    return ms;
}

unsigned long micros()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    unsigned long us =
        sec2us((unsigned long)tp.tv_sec) + ns2us((unsigned long)tp.tv_nsec);
    return us;
}

unsigned long nanos()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    unsigned long ns = sec2ns((unsigned long)tp.tv_sec) + (unsigned long)tp.tv_nsec;
    return ns;
}
