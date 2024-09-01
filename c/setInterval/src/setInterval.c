#include <pthread.h>
#include <stdlib.h>

#include "setInterval.h"

void *runCallback(void *arg)
{
    IntervalInfo *intervalinfo = (IntervalInfo *)arg;
    millis_t start = millis();
    while (*(intervalinfo->stop) != true) {
        if ((millis() - start) >= intervalinfo->interval) {
            (*intervalinfo->callback)();
            start = millis();
        }
    }
    free(arg);
    return NULL;
}

void setInterval(callback_t callback, millis_t millis, bool *stopFlag)
{
    IntervalInfo *intervalinfo = (IntervalInfo *)malloc(sizeof(IntervalInfo));
    intervalinfo->callback = callback;
    intervalinfo->interval = millis;
    intervalinfo->stop = stopFlag;

    pthread_t thread;
    pthread_create(&thread, NULL, runCallback, intervalinfo);
    pthread_detach(thread);

}
