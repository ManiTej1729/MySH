#ifndef SPEC_5
#define SPEC_5

#include <stdbool.h>

bool is_same_as_latest(char file_path[], char *command);
void insert_command(char *command);
void remove_excess_commands();
void purge();
int num_commands_in_history();
void execute_nth_command(int n, char *oldwd, char *cwd, int *no_of_bkgd_processes);
void execute_log(char **args, int n, char *oldwd, char *cwd, int *no_of_bkgd_processes);

#endif
