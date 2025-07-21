#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include "hal_wlan.h"

int main(void)
{
    char line[1024] = {0};
    FILE *fp = fopen("of_output.txt", "r");
    int start_parse = 0;
    WLAN_MAP_TOPOLOGY h;
    while(fgets(line, sizeof(line)-1, fp) != NULL)
    {
        if(strstr(line, "Agent") != NULL)
        {
            start_parse = 1;
        }
        else if(!strncmp(line, "\n", sizeof(line)))
        {
            start_parse = 0;
        }

        if(start_parse)
        {
            printf("%s", line);
        }
        memset(line, 0, sizeof(line));
    }
    fclose(fp);

    return 0;
}
