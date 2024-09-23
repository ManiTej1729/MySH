#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

void show_intro();
int num_lines_in_buffer();
void execute_command(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes);
void show_outro();
void trim_command(char *command);
char **parse_command(char *command);
int get_size_of_args(char **args);
void debug_print(int n);
void flush_the_buffer();

#endif
