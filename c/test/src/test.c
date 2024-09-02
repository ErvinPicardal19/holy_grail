#include <timerlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    
    unsigned long start = millis();

    sleep(1);
    
    unsigned long stop = millis();

    printf("%ld\n", stop - start);

    return 0;
}
