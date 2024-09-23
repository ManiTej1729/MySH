#include "../universal_header.h"

bool is_executable(char *file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) == -1) {
        printf("stat error");
        return false;
    }
    return (file_stat.st_mode & S_IXUSR) || (file_stat.st_mode & S_IXGRP) || (file_stat.st_mode & S_IXOTH);
}

void recursive_search(char *curr_path, char *prefix, char *path_to_be_stored, int *cnt, char *type, int d, int f, int *file_type) {
    struct dirent *entry;
    DIR *dp = opendir(curr_path);
    if (dp == NULL) {
        printf("\033[31mMySH: seek: %s: No such directory/path\033[0m\n", curr_path);
        return;
    }
    char new_path[MAX_STR_LEN];
    while ((entry = readdir(dp)) != NULL) {
        // printf("name: %s, %s %lu %d\n", entry -> d_name, prefix, strlen(prefix), strncmp(entry -> d_name, prefix, strlen(prefix)) == 0);
        if (entry -> d_type == DT_DIR && entry -> d_name[0] != '.') {
            sprintf(new_path, "%s/%s", curr_path, entry -> d_name);
            if (f == 0 && (strncmp(entry -> d_name, prefix, strlen(prefix)) == 0)) {
                printf("\033[34mdirectory: %s\033[0m\n", new_path);
                (*cnt)++;
                if ((*cnt) == 1) {
                    strcpy(path_to_be_stored, new_path);
                    *(file_type) = 0;
                }
            }
            recursive_search(new_path, prefix, path_to_be_stored, cnt, type, d, f, file_type);
        }
        else {
            if (d == 0 && (strncmp(entry -> d_name, prefix, strlen(prefix)) == 0)) {
                (*cnt)++;
                sprintf(new_path, "%s/%s", curr_path, entry -> d_name);
                if (is_executable(new_path)) {
                    printf("\033[32m");
                }
                else {
                    printf("\033[35m");
                }
                printf("file: %s\n", new_path);
                printf("\033[0m");
                if ((*cnt) == 1) {
                    *(file_type) = 1;
                    strcpy(path_to_be_stored, new_path);
                    if (is_executable(new_path)) {
                        *(type) = 'x';
                    }
                }
            }
        }
    }
    closedir(dp);
}

void search_the_directory(char target[], char search_path[], int d, int e, int f, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    struct dirent *entry;
    DIR *dp = opendir(search_path);

    if (dp == NULL) {
        printf("\033[31mMySH: seek: %s: No such directory/path\033[0m\n", search_path);
        return;
    }
    closedir(dp);
    int cnt = 0;
    char path[MAX_STR_LEN];
    char type = '-';
    int file_type = -1; // 0 for directory and 1 for file
    recursive_search(search_path, target, path, &cnt, &type, d, f, &file_type);
    if (cnt == 1 && e == 1) {
        if (file_type == -1) {
            printf("No matching files found\n");
        }
        else if (file_type == 0) {
            char command[MAX_STR_LEN];
            snprintf(command, sizeof(command), "hop %s\n", path);
            execute_command(command, oldwd, cwd, no_of_bkgd_processes);
        }
        else {
            if (type == 'x') {
                char command[MAX_STR_LEN];
                snprintf(command, sizeof(command), "./%s", path);
                execute_simple_command(command);
            }
            else {
                FILE *f = fopen(path, "r");
                if (f == NULL) {
                    printf("Error opening file: %s\n", path);
                    return;
                }
                char line[MAX_STR_LEN];
                while (fgets(line, sizeof(line), f)!= NULL) {
                    printf("%s", line);
                }
                fclose(f);
            }
        }
    }
}

void execute_seek(char **args, int n, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    if (n == 2) {
        printf("MySH: seek\n\033[33mUsage: seek <flags> <str1> <str2>\nWhere <str1> is the prefix(filename) you want to search in the file/path <str2>\033[0m\n");
        return;
    }
    // getting flags
    int f = 0;
    int d = 0;
    int e = 0;
    int i;
    for (i = 1; i < n - 1; i++) {
        if (args[i][0] == '-') {
            int templen = strlen(args[i]);
            for (int j = 1; j < templen; j++) {
                if (args[i][j] == 'f') {
                    // only search for files
                    f = 1;
                    if (d == 1) {
                        printf("\033[31mMySH: seek: Cannot use both -f and -d flags together\033[0m\n");
                        return;
                    }
                }
                else if (args[i][j] == 'd') {
                    // only search for directories
                    d = 1;
                    if (f == 1) {
                        printf("\033[31mMySH: seek: Cannot use both -f and -d flags together\033[0m\n");
                        return;
                    }
                }
                else if (args[i][j] == 'e') {
                    // reveal file details
                    e = 1;
                }
                else {
                    // invalid flag
                    printf("\033[31mMySH: seek: Invalid flag: -%c\033[0m\n", args[i][j]);
                    return;
                }
            }
        }
        else {
            break;
        }
    }
    if (i != n - 1) {
        char target[MAX_STR_LEN];
        strcpy(target, args[i++]);
        char search_path[MAX_STR_LEN];
        if (i < n - 1) {
            // search in specified path
            path_formation(args, n, i);
            strcpy(search_path, args[i++]);
        }
        else {
            // search in cwd
            strcpy(search_path, ".");
        }
        search_the_directory(target, search_path, d, e, f, oldwd, cwd, no_of_bkgd_processes);
    }
    else {
        // invalid arguments
        printf("\033[31mMySH: seek: Invalid arguments\033[0m\n");
        return;
    }
}
