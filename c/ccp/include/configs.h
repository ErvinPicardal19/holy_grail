#ifndef __CONFIGS_H__
#define __CONFIGS_H__

extern const char *makefile_template;
extern const char *clang_format_config;
extern const char *clangd_config;
extern const char *clang_tidy_config;
extern const char *main_file;

typedef enum { MAKEFILE = 0, CLANG_FORMAT, CLANG_TIDY, CLANG_D } Config;

void set_project_name(char *name);
void create_project_dir(char *project_dir);
void create_config(const char *project_dir, Config config);

#endif  //__CONFIGS_H__
