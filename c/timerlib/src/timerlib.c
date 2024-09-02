#include "timerlib.h"

#include <time.h>
#include <unistd.h>

millis_t millis()
{
<<<<<<< HEAD
    nanos_t ns = nanos();
    return ns2ms(ns);
=======
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    unsigned long ms =
        (sec2ms((unsigned long)tp.tv_sec) + ns2ms((unsigned long)tp.tv_nsec));
    return ms;
>>>>>>> 70dba4cadc00b4ddbc4de248311405f136b369a5
}

millis_t micros()
{
<<<<<<< HEAD
    nanos_t ns = nanos();
    return ns2us(ns);
=======
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    long unsigned int us =
        (sec2us((unsigned long)tp.tv_sec) + ns2us((unsigned long)tp.tv_nsec));
    return us;
>>>>>>> 70dba4cadc00b4ddbc4de248311405f136b369a5
}

nanos_t nanos()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
<<<<<<< HEAD
    nanos_t ns = 
        sec2ns((nanos_t)tp.tv_sec) + (nanos_t)tp.tv_nsec;
=======
    long unsigned int ns = 
        sec2ns((unsigned long)tp.tv_sec) + (unsigned long)tp.tv_nsec;
>>>>>>> 70dba4cadc00b4ddbc4de248311405f136b369a5
    return ns;
}
