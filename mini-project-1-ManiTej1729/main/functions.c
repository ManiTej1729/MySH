#include "../universal_header.h"

void debug_print(int n) {
    char temp[MAX_STR_LEN];
    temp[MAX_STR_LEN - 1] = '\0';
    getcwd(temp, MAX_STR_LEN - 1);
    printf("%d Current working directory: %s\n", n, temp);
}

int num_lines_in_buffer() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/main/buffer.txt", home_path);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("\033[31mError opening file for history\033[0m\n");
        return 0;
    }
    char ch;
    int count = 0;
    while ((ch = fgetc(file))!= EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fclose(file);
    return count;
}

void show_intro() {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } 
    else if (rc == 0) {
        execlp("sh", "sh", "./intro.sh", NULL);
        printf("Failed to run the intro.\n");
        exit(1);
    } 
    else {
        wait(NULL);
    }
}

void trim_command(char *command) {
    int n = strlen(command);
    if (command[n - 1] == ' ' || command[n - 1] == '\n') {
        command[n - 1] = '\0';
    }
    // char *trimmed_command = (char *)malloc(n * sizeof(char));
    char trimmed_command[n];
    int curr_idx = 0;
    for (int i = 0; i < n; i++) {
        if (command[i] != ' ') {
            trimmed_command[curr_idx++] = command[i];
        }
        else {
            if (curr_idx != 0) {
                if (trimmed_command[curr_idx - 1] != ' ') {
                    trimmed_command[curr_idx++] = ' ';
                }
            }
        }
    }
    trimmed_command[curr_idx] = '\0';
    strcpy(command, trimmed_command);
    // free(trimmed_command);
    return;
}

char **parse_command(char *command) {
    int num_strs = 0;
    int n = strlen(command);
    int len = 0;
    int max_len = 0;
    for (int i = 0; i < n; i++) {
        if (command[i] == ' ') {
            if (len > 0) {
                num_strs++;
                max_len = (len > max_len)? len : max_len;
                len = 0;
            }
        }
        else if (i == n - 1) {
            len++;
            num_strs++;
            max_len = (len > max_len)? len : max_len;
            len = 0;
        }
        else {
            len++;
        }
    }
    char **args = (char **)malloc((num_strs + 1) * sizeof(char *));
    for (int i = 0; i < num_strs + 1; i++) {
        args[i] = (char *)malloc(max_len * sizeof(char));
    }
    args[num_strs] = NULL;
    int curr_arg = 0;
    int curr_idx = 0;
    for (int i = 0; i < n; i++) {
        if (command[i] != ' ') {
            args[curr_arg][curr_idx++] = command[i];
            if (i == n - 1) {
                args[curr_arg][curr_idx] = '\0';
                curr_idx = 0;
                curr_arg++;
            }
        }
        else {
            args[curr_arg++][curr_idx] = '\0';
            curr_idx = 0;
        }
    }
    return args;
}

void flush_the_buffer() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/main/buffer.txt", home_path);
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file for buffer flushing\n");
        exit(1);
    }
    char line[MAX_STR_LEN];
    while (fgets(line, MAX_STR_LEN - 1, f) != NULL) {
        printf("\033[34m%s\033[0m", line);
    }
    fclose(f);
    // to empty the buffer
    FILE *f2 = fopen(file_path, "w");
    fclose(f2);
}

int get_size_of_args(char **args) {
    int n = 0;
    while (args[n] != NULL) {
        // printf("%s\n", args[n]);
        n++;
    }
    return n + 1;
}

void execute_command(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    // printf("Command is: %s\n", command);
    tokenize_command(command, oldwd, cwd, no_of_bkgd_processes);
    clearerr(stdin);
    fflush(stdout);
    return;
}

void show_outro() {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } 
    else if (rc == 0) {
        execlp("sh", "sh", "./outro.sh", NULL);
        printf("Failed to run the outro.\n");
        exit(1);
    } 
    else {
        wait(NULL);
    }
}
