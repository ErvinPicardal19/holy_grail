#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atoh.h"

int main(void)
{
    
    truncate("file.txt", 50);
    
    FILE *file;
    file = fopen("file.txt", "r");

    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }
    
    char c;
    char *hex;
    char buffer[1024];
    int i = 0;
    memset(buffer, 0, 1024);

    while (!((c = fgetc(file)) == '\0' || c == EOF))
    {
        buffer[i] = c;
        i++;
    }

    hex = atoh((unsigned char*) buffer, i);
    
    i = 0;
    while (hex[i] != '\0')
    {
        printf("%c", hex[i]);
        i++;
    }    

    free(hex);
    hex = NULL;

    return 0;
}
