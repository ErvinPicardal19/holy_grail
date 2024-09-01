#include <stdio.h>
#include <errno.h>

int check_value(int value)
{
    
    if (value > 1000) return (errno = EINVAL, -1);

    return 0;
}

int main(void)
{
    
    int m = 4;
    int n;

    n = (m+=1, m*2);

    printf("m: %d\nn: %d\n", m, n);

    if (check_value(1001) == -1)
        perror("error");

    return 0;
}
