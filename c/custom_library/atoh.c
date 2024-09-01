#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* atoh(unsigned char const *text, unsigned int len)
{
    
    char *hexits = "0123456789ABCDEF";
    int i = 0;


    if (text == NULL || len <= 0)
        return NULL;

    unsigned int output_size = ((sizeof(char) * 3) * (len))+1;
    char *output = (char *)malloc(output_size);
    if (output == NULL)
        return output;

    memset(output, 0, output_size);
    
    for (i = 0; i < len; i++)
    {
        output[i*3] = hexits[(text[i] >> 4)];
        output[(i*3) + 1] = hexits[(text[i] & 0x0F)];
        output[(i*3) + 2] = ' ';
    }

    return output;
}
