#include <stdio.h>

 // [char, char, char]

void foo(char *(*p)[])
{
    // Pointer to a Pointer - same as char **p
    *((*p)+1) = "HI";
}

void foa(char (*p)[])
{
    // Pointer to an array of char - same as char (*p)[];    
    *((*p)+1) = 'A';
}

void setChar(char *p, int pos, char c)
{
    *(p + pos) = c;
}

int main()
{
    char ch[] = "HELLO";
    char *str_arr[] = {"HELLO", "WORLD!"};
    
    //foa(&ch);
    setChar(ch, 1, 'A');
    foo(&str_arr);
     
    //printf("ch add: %c\n", *ch);
    printf("%s\n", ch);
    printf("%s\n", str_arr[1]);

    return 0;
}
