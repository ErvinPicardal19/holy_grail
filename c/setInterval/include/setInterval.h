#ifndef __SET_INTERVAL_H__
#define __SET_INTERVAL_H__

#include <stdbool.h>

typedef void (*callback_t)(void);
typedef unsigned long millis_t;

typedef struct
{
    callback_t callback;
    millis_t interval;
    bool *stop;

} IntervalInfo;

void *runCallback(void *arg);
void setInterval(callback_t callback, millis_t millis, bool *stopFlag);

#endif
