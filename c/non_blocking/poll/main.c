#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
    
    char name[255];
    int counter = 0;

    struct pollfd mypoll;

    /* let's init mypoll */
    memset(&mypoll, 0, sizeof(mypoll));
    // the file descriptor is set to stdin
    mypoll.fd = 0;

    mypoll.events = POLLIN;

    printf("Type in your name:\n");


    // if (poll(&mypoll, 1, -1) == 1){
    //     read(0, name, sizeof(name));
    //     printf("Hello, %s\n", name);
    // } else {
    //     counter++;
    // }
    while(1)
    {
        poll(&mypoll, 1, 100);

        if ((mypoll.revents & POLLIN) == 1){
            read(0, name, sizeof(name));
            printf("Hello, %s\n", name);
            break;
        } else {
            counter++;
        }
    }
    
    printf("It took %d ms to type in your name.\n", 100 * counter);

    return 0;
}
