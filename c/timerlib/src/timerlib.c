#include "timerlib.h"

#include <time.h>
#include <unistd.h>

millis_t millis()
{
    nanos_t ns = nanos();
    return ns2ms(ns);
}

millis_t micros()
{
    nanos_t ns = nanos();
    return ns2us(ns);
}

nanos_t nanos()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    nanos_t ns = 
        sec2ns((nanos_t)tp.tv_sec) + (nanos_t)tp.tv_nsec;
    return ns;
}
