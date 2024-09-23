#include "spec2.h"
#include "../spec6/spec6.h"
#include "../spec13/spec13.h"
#include "../main/main.h"
#include "../utils/utils.h"
#include "../universal_header.h"
#include "../spec10_11_12/spec10_11_12.h"

void execute_chlid(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        return;
    }
    else if (rc == 0) {
        if (background) {
            inner_child_for_acknowledgement(command, oldwd, cwd, no_of_bkgd_processes);
        }
        else {
            // printf("command: %s\n", command);
            // char *args[] = {"/bin/sh", "-c", command, NULL};
            // execv("/bin/sh", args);
            trim_command(command);
            int num_commands = 1;
            int len = strlen(command);
            for (int i = 0; i < len; i++) {
                if (command[i] == ' ') {
                    num_commands++;
                }
            }
            char *args[num_commands + 1];
            int curr_idx = 0;
            char *token = strtok(command, " ");
            while (token != NULL) {
                args[curr_idx++] = token;
                token = strtok(NULL, " ");
            }
            args[curr_idx] = (char *) NULL;
            execvp(args[0], args);
            printf("Error in child process\n");
            exit(1);
        }
        exit(0);
    }
    else {
        if (foreground) {
            curr_fg_proc = rc;
            waitpid(rc, NULL, WUNTRACED);
            curr_fg_proc = 0;
        }
        else {
            (*no_of_bkgd_processes) = (*no_of_bkgd_processes) + 1;
            printf("\033[33m[%d] Process \"%s\" with ID %d is running in background\033[0m\n", *no_of_bkgd_processes, command, rc);
            add_entry_to_file(rc, command);
        }
    }
}

void tokenize_command(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    int num_semi_colons = 0;
    int len = strlen(command);
    for (int i = 0; i < len; i++) {
        if (command[i] == ';') {
            num_semi_colons++;
        }
    }
    char tokens[num_semi_colons + 1][strlen(command) + 1];
    char *token = strtok(command, ";");
    int curr_idx = 0;
    while (token != NULL) {
        strcpy(tokens[curr_idx], token);
        // printf("token: %s\n", tokens[curr_idx]);
        trim_command(tokens[curr_idx++]);
        token = strtok(NULL, ";");
    }
    // further tokenise based on '&' character
    for (int i = 0; i < curr_idx; i++) {
        char *temp = tokens[i];
        int num_ampersands = 0;
        for (int j = 0; j < strlen(temp); j++) {
            if (temp[j] == '&') {
                num_ampersands++;
            }
        }
        char subtokens[num_ampersands + 1][strlen(temp) + 1];
        char *subtoken = strtok(temp, "&");
        int sub_curr_idx = 0;
        while (subtoken != NULL) {
            strcpy(subtokens[sub_curr_idx], subtoken);
            // printf("subtoken: %s\n", subtokens[sub_curr_idx]);
            trim_command(subtokens[sub_curr_idx++]);
            subtoken = strtok(NULL, "&");
        }
        char act_file_path[MAX_STR_LEN];
        snprintf(act_file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
        FILE *fp = fopen(act_file_path, "a+");
        for (int j = 0; j < sub_curr_idx - 1; j++) {
            // background processes
            if (!preprocessing(subtokens[j], 0, 1, oldwd, cwd, no_of_bkgd_processes)) {
                execute_chlid(subtokens[j], 0, 1, oldwd, cwd, no_of_bkgd_processes);
            }
        }
        if (sub_curr_idx > num_ampersands) {
            // last one is a foreground process
            time_t start = time(NULL);
            if (!preprocessing(subtokens[sub_curr_idx - 1], 1, 0, oldwd, cwd, no_of_bkgd_processes)) {
                if (is_not_inbuilt(subtokens[sub_curr_idx - 1], oldwd, cwd, no_of_bkgd_processes)) {
                    execute_chlid(subtokens[sub_curr_idx - 1], 1, 0, oldwd, cwd, no_of_bkgd_processes);
                }
            }
            time_t end = time(NULL);
            double time_taken = difftime(end, start);
            // printf("Time taken by %s: %f seconds\n", subtokens[sub_curr_idx - 1], time_taken);
            if (time_taken > 1) {
                strcat(cwd, "(");
                strcat(cwd, subtokens[sub_curr_idx - 1]);
                strcat(cwd, ")");
            }
        }
        else {
            // last one is a background process
            if (!preprocessing(subtokens[i], 0, 1, oldwd, cwd, no_of_bkgd_processes)) {
                execute_chlid(subtokens[i], 0, 1, oldwd, cwd, no_of_bkgd_processes);
            }
        }
        fclose(fp);
    }
}
