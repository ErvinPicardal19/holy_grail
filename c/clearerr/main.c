#include <stdio.h>

int main(void)
{
    
    FILE *file;

    file = fopen("file.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fputc('d', file);

    if (ferror(file)) printf("Write error!\n");

    clearerr(file);

    int c;
    while(1)
    {
        c = fgetc(file);
        if (feof(file) || ferror(file)) break;

        printf("%c\n", c);
    }

    fclose(file);

    return 0;
}
