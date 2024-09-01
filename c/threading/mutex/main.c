#include <stdio.h>
#include <pthread.h>
#include "bank.h"

void *deposit(void * amount);
pthread_mutex_t mutex;
static int lock = 0;

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;


    printf("Before: %d\n", read_balance());
    
    int deposit1 = 300;
    int deposit2 = 200;
    
    // pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, deposit, (void *)&deposit1);
    pthread_create(&thread2, NULL, deposit, (void *)&deposit2);
    // deposit(300);
    // deposit(200);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // pthread_mutex_destroy(&mutex);

    printf("After: %d\n", read_balance());

    return 0;
}

void *deposit(void *amount)
{
    // create lock
    // pthread_mutex_lock(&mutex);
    
    while(lock == 1){}
    lock = 1;

    int account_balance = read_balance();

    account_balance += *((int *)amount);

    write_balance(account_balance);

    // release lock
    // pthread_mutex_unlock(&mutex);
    lock = 0;

    return NULL;
}
