#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "configs.h"

#define SHOW_USAGE() fprintf(stdout, "Create a C Project Directory.\n\n" \
                                     "usage: %s [<options>] <project_name>\n\n" \
                                     "-h\t\t- Show help.\n" \
                                     "-L\t\t- Will turn on MAKE_SHARED_LIBRARY on Makefile.\n" \
                                     "-v\t\t- Show version.\n", argv[0])


int main(int argc, char *argv[])
{
    int option;

    if (argc <= 1) 
    {
        SHOW_USAGE();
        exit(1);
    }
    
    while((option = getopt(argc, argv, "hvL:")) != -1)
    {
        switch(option)
        {
            case 'h':
                SHOW_USAGE();
                exit(0);
            case 'v':
                fprintf(stdout, "v1.0\n");
                exit(0);
            case 'L':
                set_shared_lib();
            break;
            default:
                exit(1);        
        }
    }

    char project_dir[PATH_MAX];
    if (getcwd(project_dir, sizeof(project_dir)) == NULL) {
        fprintf(stderr, "Cannot access current directory.");
        exit(1);
    }
    if(optind > 1)
    {
        set_project_name(argv[optind-1]);
        strcat(project_dir, "/");
        strcat(project_dir, argv[optind-1]);
        create_project_dir(project_dir);
    }
    else
    {
        set_project_name(argv[1]);
        strcat(project_dir, "/");
        strcat(project_dir, argv[1]);
        create_project_dir(project_dir);
    }

    create_config(project_dir, MAKEFILE);
    create_config(project_dir, CLANG_FORMAT);
    create_config(project_dir, CLANG_D);
    create_config(project_dir, CLANG_TIDY);

    return 0;
}
