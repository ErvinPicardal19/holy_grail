#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

FILE *log_file;
int incidents = 0;

void *logger();

int main(void)
{
    
    log_file = fopen("log.txt", "w");

    if (log_file == NULL)
    {
        perror("fopen");
        return 1;
    }

    pthread_t thread;
    int s;

    s = pthread_create(&thread, NULL, logger, NULL);
    if (s != 0) return 1;

    s = pthread_detach(thread);
    if (s != 0) return 1;

    int input = 0;

    do {
        printf("Enter -1 to quit.\n");
        printf("New incidents: ");
        scanf("%d", &input);

        if (input != -1) incidents += input;

    }while (input != -1);

    fclose (log_file);

    return 0;
}

void *logger()
{

    while (1)
    {
        time_t current_time = time(NULL);

        struct tm* tm = localtime(&current_time);

        char timestamp[256];
        strftime(timestamp, 256, "%x@%H:%M:%S", tm);
        
        fprintf(log_file, "%s: %d\n", timestamp, incidents);

        sleep(1);
    }

    return NULL;
}
