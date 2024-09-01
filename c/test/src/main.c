#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <timerlib.h>

void print_hello(void)
{
    static int i = 1;
    printf("Hello %d.\n", i++);
}

int main(void)
{
    bool stop = 0;

    unsigned long start = millis();
    setInterval(print_hello, 1000, &stop);

    sleep(3);
    
    unsigned long end = millis();
    stop = 1;

    printf("%ld\n", (end-start));

	return 0;
}
