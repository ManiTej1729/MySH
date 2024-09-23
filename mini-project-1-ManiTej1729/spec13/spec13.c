#include "../universal_header.h"

void add_entry_to_file(int pid, char *command) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
    FILE *f = fopen(file_path, "a+");
    if (f == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    fprintf(f, "%d %s\n", pid, command);
    fclose(f);
}

void delete_entry(int pid) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
    char processes[MAX_STR_LEN];
    processes[0] = '\0';
    char line[MAX_STR_LEN];
    char pid_str[10];
    snprintf(pid_str, 10, "%d", pid);
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    while (1) {
        if (fgets(line, MAX_STR_LEN, f) == NULL) {
            break;
        }
        if (str_contains(line, pid_str)) {
            continue;
        }
        strcat(processes, line);
    }
    fclose(f);
    FILE *f2 = fopen(file_path, "w");
    if (f2 == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    fprintf(f2, "%s", processes);
    fclose(f2);
}

int compare(const void *a, const void *b) {
    char *a_str = *(char **)a;
    char *b_str = *(char **)b;
    // printf("a and b: %s, %s\n", a_str, b_str);
    int pid_a = atoi(a_str);
    int pid_b = atoi(b_str);
    return pid_a - pid_b;
}

void show_activities() {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/activities.txt", home_path);
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        printf("Error opening file: %s\n", file_path);
        return;
    }
    char line[MAX_STR_LEN];
    char *processes[MAX_STR_LEN];
    int curr_process = 0;
    while (fgets(line, MAX_STR_LEN, f)) {
        processes[curr_process++] = strdup(line);
        replace_newline_with_null(processes[curr_process - 1]);
    }
    qsort(processes, curr_process, sizeof(char *), compare);
    int pid;
    char *buffer;
    char status[20];
    for (int i = 0; i < curr_process; i++) {
        sscanf(processes[i], "%d %s", &pid, buffer);
        char path[MAX_STR_LEN];
        snprintf(path, MAX_STR_LEN, "/proc/%d/status", pid);
        FILE *fptr = fopen(path, "r");
        if (fptr == NULL) {
            printf("Error opening the file %s\n", path);
            // delete_entry(pid);
            continue;
        }
        while (fgets(line, sizeof(line), fptr)) {
            if (strncmp(line, "State:", 6) == 0) {
                printf("%s - ", processes[i]);
                if (line[7] == 'R' || line[7] == 'S') {
                    printf("\033[32mRunning\033[0m\n");
                }
                else {
                    printf("\033[31mStopped\033[0m\n");
                }
                break;
            }
        }
        fclose(fptr);
    }
    fclose(f);
    for (int i = 0; i < curr_process; i++) {
        free(processes[i]);
    }
}
