#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno;

int main(void)
{
    FILE *fh;

    printf("Errno before: %d\n", errno); 
    fh = fopen("dne.txt", "r");

    if (fh == NULL) {
        fprintf(stderr, "Errno after: %d\n", errno);
        fprintf(stderr, "Errno msg: %s\n", strerror(errno));
    } else {
        fclose(fh);
    }

    return 0;
}
