#ifndef __TIMERLIB_H__
#define __TIMERLIB_H__

#include <stdbool.h>

#define sec2ms(s) (s * 1000)
#define sec2us(s) (s * 1000000L)
#define sec2ns(s) (s * 1000000000L)
#define ns2ms(ns) (ns / 1000000)
#define ns2us(ns) (ns / 1000)

typedef void (*callback_t)(void);
typedef double millis_t;
typedef double micros_t;
typedef unsigned long nanos_t;

typedef struct
{
    callback_t callback;
    millis_t interval;
    bool *stop;
} IntervalInfo;

double millis();
double micros();
unsigned long nanos();

void *runCallback(void *arg);
void setInterval(callback_t callback, millis_t millis, bool *stopFlag);

#endif
