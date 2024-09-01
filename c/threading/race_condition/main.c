#include <stdio.h>
#include <pthread.h>
#include "bank.h"

void *deposit(void * amount);

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    printf("Before: %d\n", read_balance());
    
    int deposit1 = 300;
    int deposit2 = 200;

    pthread_create(&thread1, NULL, deposit, (void *)&deposit1);
    pthread_create(&thread2, NULL, deposit, (void *)&deposit2);
    // deposit(300);
    // deposit(200);
   
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    

    printf("After: %d\n", read_balance());

    return 0;
}

void *deposit(void *amount)
{

    int account_balance = read_balance();

    account_balance += *((int *)amount);

    write_balance(account_balance);

    return NULL;
}
