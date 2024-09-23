#include "../universal_header.h"

void execute_ping(char **args, int n) {
    int pid = atoi(args[1]);
    int signal_number = atoi(args[2]);
    signal_number = signal_number % 32;
    if (kill(pid, 0) == -1) {
        printf("\033[33mThe process with pid \"%d\" is not found\033[0m\n", pid);
    }
    else if (kill(pid, signal_number) == -1) {
        printf("Error in sending the signal \"%d\" to the process with pid \"%d\".\n", signal_number, pid);
    }
    else if (signal_number == 9) {
        printf("\033[31mThe process with pid \"%d\" was killed using SIGINT\033[0m\n", pid);
        delete_entry(pid);
    }
}

void handle_ctrl_c() {
    if (curr_fg_proc != 0) {
        if (kill(curr_fg_proc, SIGKILL) == -1) {
            printf("Error in sending the signal SIGINT to the process with pid %d\n", curr_fg_proc);
        }
        curr_fg_proc = 0;
    }
}

void handle_ctrl_d() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
    char line[MAX_STR_LEN];
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    int pid;
    while (1) {
        if (fgets(line, MAX_STR_LEN, f) == NULL) {
            break;
        }
        pid = atoi(line);
        if (kill(pid, 0) == -1) {
            printf("\033[33mProcess with pid %d is not found\033[0m\n", pid);
        }
        else {
            if (kill(pid, SIGKILL) == -1) {
                printf("\033[32mError in killing process with pid %d\033[0m\n", pid);
            }
            else {
                printf("\033[31mKilled the process with pid %d\033[0m\n", pid);
            }
        }
    }
    // handle_ctrl_c();
    int shell_pid = getpid();
    fclose(f);
    char act_file_path[MAX_STR_LEN];
    snprintf(act_file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
    
    // empty out the activities file
    f = fopen(act_file_path, "w");
    fclose(f);
    printf("Exiting MySH...\n");
    exit(0);
}

void handle_ctrl_z() {
    // printf("Hello there\n");
    if (curr_fg_proc != 0) {
        if (kill(curr_fg_proc, SIGTSTP) == -1) {
            printf("Error in sending the signal SIGINT to the process with pid %d\n", curr_fg_proc);
        }
        // add_entry_to_file(curr_fg_proc)
        curr_fg_proc = 0;
    }
}
