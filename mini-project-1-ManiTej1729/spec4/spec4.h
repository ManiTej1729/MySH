#ifndef SPEC_4
#define SPEC_4

void path_formation(char **args, int n, int i);
int color_decider(char *path, char *filename);
void generate_directory_contents(int no_of_entities, int max_len, char items[][max_len + 1], int a, char *path);
int num_entities(char *path, int a, int *max_len);
void print_file_info(char *path, char *filename, int color);
void list_directory_contents(char *path, int a, int l);
void execute_reveal(char **args, int n);

#endif
