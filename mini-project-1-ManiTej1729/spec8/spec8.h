#ifndef SPEC_8
#define SPEC_8

#include "../universal_header.h"

bool is_executable(char *file_path);
void recursive_search(char *curr_path, char *prefix, char *path_to_be_stored, int *cnt, char *type, int d, int f, int *file_type);
void search_the_directory(char target[], char search_path[], int d, int e, int f, char *oldwd, char *cwd, int *no_of_bkgd_processes);
void execute_seek(char **args, int n, char *oldwd, char *cwd, int *no_of_bkgd_processes);

#endif
