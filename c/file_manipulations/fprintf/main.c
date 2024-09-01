#include <stdio.h>

int main(void)
{
    
    FILE *file;
    file = fopen("file.txt", "w");

    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(file, "\n");
    fprintf(file, "\t ABC \n");
        
    // Format Specifiers
    // %[flags][width][.precision][length]specifier
    
    float f = 2.125;
    fprintf(file, "%f\n", f);

    double d = 9.82;
    fprintf(file, "%.2f\n", d);

    char string[] = "abc";
    fprintf(file, "%s\n", string);

    //Field
    fprintf(file, "%4d\n", 22);
    fprintf(file, "%-4d<-end\n", 22);
    fprintf(file, "%04d\n", 22);
    
    fprintf(file, "%d %d\n", 1, -1);
    fprintf(file, "%+d %+d\n", 1, -1);

    fprintf(file, "%.*f\n", 1, 1.2345);

    fclose(file);

    return 0;
}
