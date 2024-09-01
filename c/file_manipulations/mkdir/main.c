#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

int main(void)
{
    if(mkdir("test4", S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        perror("Error");

        if (errno == EEXIST) {
            mkdir("test2", S_IRWXU);
        }

        return -1;
    }

    return 0;
}
