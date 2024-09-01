#include <stdio.h>
#include <pthread.h>
#include <time.h>
void *computation(void *add);

int main(void)
{
    
    pthread_t thread1;
    pthread_t thread2;
    void *status = 0;

    long value1 = 1;
    long value2 = 2;

    pthread_create(&thread1, NULL, computation, &value1);
    pthread_create(&thread2, NULL, computation, &value2);
    

    puts("Doing something else...");
    time_t start = time(NULL);
    while(time(NULL)-start < 5) {}
    puts("Done!");
    
    pthread_join(thread1, &status);
    pthread_join(thread2, &status);
    if(status != 0) printf("Thread1 joined\n");
    if(status != 0) printf("Thread2 joined\n");
    

    return 0;
}

void *computation(void *add)
{
    long num = *((long*)add);
    printf("Add: %ld\n", num);
    return add;
}
