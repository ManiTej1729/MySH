#include "../universal_header.h"

int valid_wrt_flag(char **args, int n) {
    int tempSize;
    for (int i = 2; i < n - 1; i++) {
        tempSize = strlen(args[i]);
        for (int j = 0; j < tempSize; j++) {
            if (args[i][j] == '~') {
                return 0;
            }
        }
    }
    tempSize = strlen(args[1]);
    for (int j = 1; j < tempSize; j++) {
        if (args[1][j] == '~') {
            return 0;
        }
    }
    return 1;
}

void execute_hop(char **args, int n, char *oldwd, char *cwd) {
    // int n = get_size_of_args(args);
    if (n < 2) {
        printf("\033[31mIncorrect usage of hop\033[0m\n\033[33mex: hop <directory>\033[0m\n");
    }
    else if (n == 2) {
        // For the command: hop
        if (chdir(home_path)) {
            printf("\033[31mMySH: hop: %s No such file or directory\033[0m\n", home_path);
        }
        getcwd(cwd, MAX_STR_LEN - 1);
        printf("\033[32mHopped to directory: %s\033[0m\n", cwd);
    }
    else {
        int is_valid = valid_wrt_flag(args, n);
        path_formation(args, n, 1);
        if (!is_valid) {
            printf("\033[31mMySH: hop: Invalid flag usage\033[0m\n\033[33mFlags can only be at the beginning\033[0m\n");
        }
        else if (args[1][0] == '-') {
            // execute history
            if (n != 3) {
                printf("\033[31mMySH: hop: Incorrect usage of hop\033[0m\n\033[33mCommand usage: \"hop -\"\033[0m\n");
            }
            else {
                if (strlen(args[1]) > 1) {
                    printf("\033[31mMySH: hop: Incorrect usage of hop\033[0m\n\033[33mCommand usage: \"hop -\"\033[0m\n");
                }
                else {
                    // "History valid request"
                    if (strcmp(oldwd, "") == 0) {
                        printf("\033[36mMySH: hop: No history yet\033[0m\n");
                    }
                    else {
                        if (chdir(oldwd)) {
                            printf("\033[31mMySH: hop: %s No such file or directory\033[0m\n", oldwd);
                        }
                        else {
                            strcpy(oldwd, cwd);
                            getcwd(cwd, MAX_STR_LEN - 1);
                            // absolute_to_relative(cwd);
                            printf("\033[32mHopped to directory: %s\033[0m\n", cwd);
                        }
                    }
                }
            }
        }
        else if (args[1][0] == '~') {
            // execute ~ (for directing from ~ of the shell)
            // "Tilde valid request\n"
            char temp[MAX_STR_LEN];
            temp[MAX_STR_LEN - 1] = '\0';
            strcpy(temp, home_path);
            args[1][0] = '/';
            strcat(temp, args[1]);
            if (chdir(temp)) {
                printf("\033[31mMySH: hop: %s No such file or directory\033[0m\n", temp);
            }
            else {
                strcpy(oldwd, cwd);
                getcwd(cwd, MAX_STR_LEN - 1);
                // absolute_to_relative(cwd);
                printf("\033[32mHopped to directory: %s\033[0m\n", cwd);
            }
        }
        else {
            char *path = args[1];
            if (chdir(path)) {
                printf("\033[31mMySH: hop: %s No such file or directory\033[0m\n", path);
            }
            else {
                strcpy(oldwd, cwd);
                getcwd(cwd, MAX_STR_LEN - 1);
                // absolute_to_relative(cwd);
                printf("\033[32mHopped to directory: %s\033[0m\n", cwd);
            }
        }
    }
}
