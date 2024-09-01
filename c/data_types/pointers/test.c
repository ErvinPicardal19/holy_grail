#include <stdio.h>


void A()
{
    printf("Hello");
}

void B(void (*ptr)(void))
{
    (*ptr)();
}

int main()
{
    void (*p)(void (*)(void)) = &B;

    (*p)(&A);
    

    return 0;
}
