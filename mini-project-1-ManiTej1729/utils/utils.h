#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

bool is_number(char *s);
void path_formation(char **args, int n, int i);
void replace_newline_with_null(char *str);
void execute_simple_command(char *command);
void absolute_to_relative(char *path);
void tabs_and_spaces(char *command);
bool is_not_inbuilt(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes);
void remove_double_quotes(char *s);

#endif
