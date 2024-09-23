#include "../universal_header.h"

bool is_number(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    if ((s[len - 1] == ' ' || s[len - 1] == '\n' || s[len - 1] == '\0') && len > 1) {
        return true;
    }
    return isdigit(s[len - 1]);
}

bool is_not_inbuilt(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    // hop, reveal, log, proclore, seek
    char *command_copy = strdup(command);
    trim_command(command_copy);
    char **args = parse_command(command_copy);
    int n = get_size_of_args(args);
    bool ans = true;
    int a_or_f = is_alias_or_function(args[0]);
    // printf("prog_name: %s %s\n", args[0], command_copy);
    if (strcmp(args[0], "hop") == 0) { // spec3
        execute_hop(args, n, oldwd, cwd);
        ans = false;
    }
    else if (strcmp(args[0], "reveal") == 0) { // spec4
        execute_reveal(args, n);
        ans = false;
    }
    else if (strcmp(args[0], "log") == 0) { // spec5
        execute_log(args, n, oldwd, cwd, no_of_bkgd_processes);
        ans = false;
    }
    else if (strcmp(args[0], "proclore") == 0) { // spec7
        if (n > 3 || n < 2) {
            printf("\033[31mMySH: proclore: Invalid syntax.\033[0m\n\033[33mUsage: \"proclore <pid>\"\033[0m\n");
            return true;
        }
        else {
            execute_proclore(args, n);
        }
        ans = false;
    }
    else if (strcmp(args[0], "seek") == 0) { // spec8
        execute_seek(args, n, oldwd, cwd, no_of_bkgd_processes);
        ans = false;
    }
    else if (a_or_f == 1 || a_or_f == 2) { // spec9
        if (a_or_f == 1) { // alias
            if (n != 2) {
                printf("\033[31mMySH: %s is an alias\033[0m", args[0]);
                return true;
            }
            get_alias(args[0], command_copy);
            is_not_inbuilt(command_copy, oldwd, cwd, no_of_bkgd_processes);
        }
        else { // function
            get_function(args[0], command_copy);
            // printf("function line: %s\n", command_copy);
            char dummy[MAX_STR_LEN];
            int len = strlen(command_copy);
            int curr_idx = 0;
            for (int i = 0; i < len; i++) {
                if (command_copy[i] == '"') {
                    int start = i + 2;
                    int end = i + 2;
                    while (command_copy[end] >= '0' && command_copy[end] <= '9') {
                        end++;
                    }
                    i = end;
                    char arg_number[end - start + 1];
                    strncpy(arg_number, command_copy + start, end - start);
                    arg_number[end - start] = '\0';
                    int index = atoi(arg_number);
                    if (index >= n) {
                        printf("\033[31mMySH: %s: Invalid argument\033[0m\n", args[0]);
                        return true;
                    }
                    strcat(dummy, args[index]);
                    curr_idx += strlen(args[index]);
                }
                else {
                    dummy[curr_idx++] = command_copy[i];
                }
            }
            dummy[curr_idx] = '\0';
            trim_command(dummy);
            strcpy(command_copy, dummy);
            // printf("command copy: %s\n", command_copy);
            tokenize_command(command_copy, oldwd, cwd, no_of_bkgd_processes);
        }
        ans = false;
    }
    else if (strcmp(args[0], "activities") == 0) { // spec13
        show_activities();
        ans = false;
    }
    else if (strcmp(args[0], "ping") == 0) { // spec 14
        if (n != 4) {
            printf("\033[31mMySH: ping: Invalid syntax.\033[0m\n\033[33mUsage: \"ping <pid> <signal_number>\"\033[0m\n");
            return true;
        }
        execute_ping(args, n);
        ans = false;
    }
    else if (strcmp(args[0], "neonate") == 0) { // spec 16
        if (n != 4) {
            printf("\033[31mMySH: neonate: Invalid syntax.\033[0m\n\033[33mUsage: \"neonate -n [time_arg]\"\033[0m\n");
            return true;
        }
        execute_neonate(args, n);
        ans = false;
    }
    else if (strcmp(args[0], "iMan") == 0) { // spec 17
        get_man_page(args[1]);
        ans = false;
    }
    for (int i = 0; i < n; i++) {
        // free(args[i]);
    }
    // free(args);
    free(command_copy);
    return ans;
}

void path_formation(char **args, int n, int i) {
    for (int j = i + 1; j < n - 1; j++) {
        strcat(args[i], "/");
        strcat(args[i], args[j]);
    }
    // printf("path generated: %s\n", args[i]);
}

void replace_newline_with_null(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

void execute_simple_command(char *command) {
    int rc = fork();
    if (rc < 0) {
        printf("Fork failed\n");
        return;
    } 
    else if (rc == 0) {
        int len = strlen(command);
        if (len > 2 && strncmp(command, "./", 2) == 0) {
            execlp("sh", "sh", command, NULL);
        }
        exit(0);
    } 
    else {
        int status;
        waitpid(rc, &status, 0);
        return;
    }
}

void absolute_to_relative(char *path) {
    if (strncmp(path, home_path, strlen(home_path)) == 0 && strlen(path) >= strlen(home_path)) {
        snprintf(path, sizeof(path), "~%s", path + strlen(home_path));
    }
}

void tabs_and_spaces(char *command) {
    int len = strlen(command);
    for (int i = 0; i < len; i++) {
        if (command[i] == '\t') {
            command[i] = ' ';
        }
    }
}
