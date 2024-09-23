#ifndef SPEC10_11_12
#define SPEC10_11_12

#include <stdbool.h>

void execute_input_redirection(char *command, int start1, int end1, int start2, int end2);
void execute_output_redirection(char *command, int start1, int end1, int start2, int end2, int input, char *oldwd, char *cwd, int *no_of_bkgd_processes, int trunc);
void redirection(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes);
bool preprocessing(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes);

#endif