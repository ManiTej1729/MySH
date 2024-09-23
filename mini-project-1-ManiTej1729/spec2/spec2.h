#ifndef SPEC_2
#define SPEC_2

void tokenize_command(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes);
void execute_chlid(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes);

#endif
