#include "../universal_header.h"

bool is_same_as_latest(char file_path[], char *command) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
        return true;
    }
    char buffer[MAX_STR_LEN];
    char line[MAX_STR_LEN];
    while ((fgets(line, MAX_STR_LEN, file)) != NULL) {
        strcpy(buffer, line);
    }
    fclose(file);
    replace_newline_with_null(buffer);
    return (strcmp(buffer, command) == 0);
}

void insert_command(char *command) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    FILE *file = fopen(file_path, "a+");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
        return;
    }
    if (!is_same_as_latest(file_path, command)) {
        fprintf(file, "%s\n", command);
    }
    fclose(file);
}

void remove_excess_commands() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
        return;
    }
    char ch;
    int count = 0;
    while ((ch = fgetc(file))!= EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fclose(file);
    if (count > 15) {
        FILE *file2 = fopen(file_path, "r");
        if (file2 == NULL) {
            printf("\033[31mError opening file\033[0m\n");
            return;
        }
        char buffer[count * MAX_STR_LEN];
        int start = 0;
        int curr_idx = 0;
        while ((ch = fgetc(file))!= EOF) {
            if (start == 1) {
                // strcat(buffer, ch);
                buffer[curr_idx++] = ch;
            }
            if (ch == '\n' && start == 0) {
                start = 1;
            }
        }
        buffer[curr_idx] = '\0';
        file2 = fopen(file_path, "w+");
        if (file2 == NULL) {
            printf("\033[31mError opening file\033[0m\n");
            return;
        }
        fprintf(file2, "%s", buffer);
        fclose(file2);
    }
}

void purge() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
        return;
    }
    fclose(file);
}

int num_commands_in_history() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
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

void execute_nth_command(int n, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("\033[31mError opening file\033[0m\n");
        return;
    }
    char line[MAX_STR_LEN];
    int num_lines = num_commands_in_history();
    if (num_lines < n) {
        printf("\033[31mMySH: log: Index out of bounds\033[0m\n");
        fclose(file);
        return;
    }
    int count = 0;
    while (count < num_lines - n) {
        if ((fgets(line, sizeof(line), file)) != NULL) {
            count++;
        }
    }
    char command[MAX_STR_LEN];
    if (fgets(command, sizeof(command), file) != NULL) {
        execute_command(command, oldwd, cwd, no_of_bkgd_processes);
    }
    else {
        printf("\033[31mMySH: log: Could not read command from file\033[0m\n");
    }
    fclose(file);
}

void execute_log(char **args, int n, char *oldwd, char *cwd, int *no_of_bkgd_processes) {

    char file_path[MAX_STR_LEN];
    snprintf(file_path, sizeof(file_path), "%s/spec5/history.txt", home_path);
    if (n == 2) {
        FILE *file = fopen(file_path, "r");
        if (file == NULL) {
            printf("\033[31mError opening file\033[0m\n");
            return;
        }
        char ch;
        while ((ch = fgetc(file))!= EOF) {
            printf("\033[34m%c\033[0m", ch);
        }
        fclose(file);
    }
    else if (n == 3) {
        if (strcmp(args[1], "purge") != 0) {
            printf("\033[31mMySH: log: Invalid command. Did you mean \"log purge\"?\033[0m\n");
            return;
        }
        purge();
    }
    else {
        // log execute <index>
        if (strcmp(args[1], "execute") != 0) {
            printf("\033[31mMySH: log: Invalid command.\033[0m\n\033[33mUsage: log execute <index>?\033[0m\n");
            return;
        }
        else {
            if (is_number(args[2])) {
                int index = atoi(args[2]);
                if (index < 1 || index > 15) {
                    printf("\033[31mMySH: log: Index out of range.\033[0m\n");
                }
                execute_nth_command(index, oldwd, cwd, no_of_bkgd_processes);
            }
            else {
                printf("\033[31mMySH: log: Invalid index. Please provide a valid integer for <index>.\033[0m\n");
            }
        }
    }
}
