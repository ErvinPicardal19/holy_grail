#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <strings.h>

#include "sizeof.h"

void err_n_die(const char *fmt, ...);

int main(int argc, char *argv[])
{

    if (argc != 2)
        err_n_die("usage: %s <data_type>", argv[0]);
    
    char *ctype = argv[1];
    size_t size;

    size = (strcasecmp(ctype, "short") == 0 || strcasecmp(ctype, "short int") == 0) ? sizeof(short) :
           (strcasecmp(ctype, "unsigned short") == 0 || strcasecmp(ctype, "unsigned short int") == 0) ? sizeof(unsigned short) :
           (strcasecmp(ctype, "unsigned int") == 0) ? sizeof(unsigned int) :
           (strcasecmp(ctype, "int") == 0) ? sizeof(int) :
           (strcasecmp(ctype, "long") == 0 || strcasecmp(ctype, "long int") == 1) ? sizeof(long int) : 
           (strcasecmp(ctype, "unsigned long") == 0 || strcasecmp(ctype, "unsigned long int") == 0) ? sizeof(unsigned long) :
           (strcasecmp(ctype, "long long") == 0 || strcasecmp(ctype, "long long int") == 0) ? sizeof(long long) :
           (strcasecmp(ctype, "unsigned long long") == 0 || strcasecmp(ctype, "unsigned long long int") == 0) ? sizeof(unsigned long long) :
           (strcasecmp(ctype, "char") == 0) ? sizeof(char) :
           (strcasecmp(ctype, "unsigned char") == 0) ? sizeof(unsigned char) :
           (strcasecmp(ctype, "float") == 0) ? sizeof(float) :
           (strcasecmp(ctype, "double") == 0) ? sizeof(double) :
           (strcasecmp(ctype, "long double") == 0) ? sizeof(long double) : 0;
    
    if (size == 0)
        return 1;

    printf("Size: %ld\n", size);

	return 0;
}

