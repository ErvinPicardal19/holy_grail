#include <stdio.h>
#include <string.h>


typedef struct {
    int type;
    union {
        char x[1024];
        float y[1024];
        double z[1024];
    } Buffer;
} Info;

int main(void)
{
    
    
    Info info;

    memset(info.Buffer.x, 't', 1024);
    
    info.type = 1;

    if(info.type == 0)
        for(int i = 0; i < 1024; i++)
            printf("%c", info.Buffer.x[i]);
    

    return 0;
}
