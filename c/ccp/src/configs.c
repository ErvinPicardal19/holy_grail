#include "configs.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char project_name[64];
static int is_shared_lib = 0;

void set_project_name(char *name)
{
    memset(project_name, 0, 64);
    strcpy(project_name, name);
}

void set_shared_lib()
{
    is_shared_lib = 1;
}


void create_config(const char *project_dir, Config config)
{
    FILE       *file;
    const char *output;
    char type[64];
    char temp[4096];
    int len = 0;
    char filepath[PATH_MAX];
    memset(filepath, 0, PATH_MAX);
    memset(type, 0, 64);
    memset(temp, 0, 4096);
    strcat(filepath, project_dir);
    switch (config) {
        case MAKEFILE:
            strcat(filepath, "/Makefile");
            memset(temp, 0, sizeof(temp));
            strcat(temp, "EXEC := ");
            strcat(temp, project_name);
            strcat(temp, "\n");
            strcat(temp, (is_shared_lib) ? "MAKE_SHARED_LIBRARY := 1" : "MAKE_SHARED_LIBRARY := 0");
            strcat(temp, "\n");
            strcat(temp, makefile_template);
            output = temp;
            strcpy(type, "Makefile");
            break;
        case CLANG_FORMAT:
            strcat(filepath, "/.clang-format");
            output = clang_format_config;
            strcpy(type, ".clang-format");
            break;
        case CLANG_D:
            strcat(filepath, "/.clangd");
            len = snprintf(temp + len, sizeof(temp) - len, 
                           "CompileFlags:\n"
                           "  Add: [-I%s/include]"
                           , project_dir);
            output = temp;
            strcpy(type, ".clangd");
            break;
        case CLANG_TIDY:
            strcat(filepath, "/.clang-tidy");
            output = clang_tidy_config;
            strcpy(type, ".clang-tidy");
            break;
        default:
            fprintf(stderr, "Invalid config %d\n", config);
            exit(1);
            break;
    }

    file = fopen(filepath, "w");

    if (file == NULL) {
        fprintf(stderr, "Cannot create %s\n", filepath);
        perror("fopen");
        exit(1);
    }

    fprintf(file, "%s", output);

    fclose(file);

    fprintf(stdout, "\033[32m%s created.\033[0m\n", type);
}

void create_project_dir(char *project_dir)
{
    FILE *file;
    char  src_dir[PATH_MAX];
    char  include_dir[PATH_MAX];
    memset(src_dir, 0, PATH_MAX);
    memset(include_dir, 0, PATH_MAX);

    strcat(src_dir, project_dir);
    strcat(include_dir, project_dir);
    strcat(src_dir, "/src/");
    strcat(include_dir, "/include");

    if ((mkdir(project_dir, S_IRWXU | S_IRWXG)) == -1 ||
        (mkdir(src_dir, S_IRWXU | S_IRWXG)) == -1 ||
        (mkdir(include_dir, S_IRWXU | S_IRWXG)) == -1) {
        fprintf(stderr, "Cannot create project directory.\n");
        perror("mkdir: ");
        exit(1);
    }

    strcat(src_dir, project_name);
    file = fopen(src_dir, "w");

    if (file == NULL) {
        fprintf(stderr, "Cannot create %s\n", src_dir);
        perror("fopen");
        exit(1);
    }

    fprintf(file, "%s", main_file);

    fclose(file);
}
