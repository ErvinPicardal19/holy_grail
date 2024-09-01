#include <stdio.h>

int main(int argc, char *argv[])
{
    
    FILE *file;

    file = fopen("file.txt", "r");

    if (file == NULL) {
        perror("Error");
        return 1;
    }
    
    int c;
    char op1[256];
    int i = 0;

    while (!((c = fgetc(file)) == '\0' || c == EOF))
    {
        if (c == '+') 
        {
            ungetc(c, file);
            break;
        }
        else
            op1[i++] = c;
    }
    op1[i] = '\0';
    printf("op1: %s\n", op1);

    char rest[256];
    fgets(rest, 256, file);
    printf("rest: %s\n", rest);

    fclose(file);

    return 0;
}
