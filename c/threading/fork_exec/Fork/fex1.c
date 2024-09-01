#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
int main(void)
{
    fork();
    pid_t pid = getpid();

    printf("This is a process in fex1.c!\nPID = %d\n", pid);

    return 0;
}
