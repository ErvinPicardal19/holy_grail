#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    
    char source[] = "john.smith@microsoft.com";
    char destination[20];

    memccpy(destination, source, '@', 20);

    for(int i = 0; i < 20; i++)
    {
        printf("destination[%d] = %c\n", i, destination[i]);
    }

    return 0;
}
