#include <stdio.h>
#include <stdlib.h>

void cleanup();

int main(void)
{
    atexit(cleanup);
    printf("Hello, World!\n");
    
    abort();
    
    // exit(0);

    return 0;
}

void cleanup()
{
    puts("Doing Cleanup...");
}
