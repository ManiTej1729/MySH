#include "../universal_header.h"

void inner_child_for_acknowledgement(char *command, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    int pid = getpid();
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        return;
    }
    else if (rc == 0) {
        if (!preprocessing(command, 0, 1, oldwd, cwd, no_of_bkgd_processes)) {
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
        int status;
        waitpid(rc, &status, 0);
        char file_path[MAX_STR_LEN];
        snprintf(file_path, sizeof(file_path), "%s/main/buffer.txt", home_path);
        FILE *f = fopen(file_path, "a+");
        if (f == NULL) {
            fprintf(stderr, "Error opening file\n");
            exit(1);
        }
        if (status == 0) {
            fprintf(f, "The process \"%s\" with ID %d has terminated without any erors\n", command, pid);
        }
        else {
            fprintf(f, "The process \"%s\" with ID %d has terminated abnormally\n", command, pid);
        }
        fclose(f);
        (*no_of_bkgd_processes) = (*no_of_bkgd_processes) - 1;
        delete_entry(pid);
    }
}
