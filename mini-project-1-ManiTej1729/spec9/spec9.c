#include "../universal_header.h"

bool str_contains(char * line, char *target) {
    int len = strlen(line);
    int target_len = strlen(target);
    for (int i = 0; i <= len - target_len; i++) {
        if (strncmp(line + i, target, target_len) == 0) {
            return true;
        }
    }
    return false;
}

int is_alias_or_function(char *arg) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/.myshrc", home_path);
    FILE *myshrc = fopen(file_path, "r");
    if (!myshrc) {
        fprintf(stderr, "Error: Could not open %s\n", file_path);
        return -1;
    }
    char line[MAX_STR_LEN];
    char target1[MAX_STR_LEN];
    char target2[MAX_STR_LEN];
    snprintf(target1, MAX_STR_LEN, "%s=", arg);
    snprintf(target2, MAX_STR_LEN, "%s()", arg);
    while (fgets(line, MAX_STR_LEN, myshrc)) {
        if (str_contains(line, target1)) {
            fclose(myshrc);
            return 1;  // returning 1 for alias
        }
        if (str_contains(line, target2)) {
            fclose(myshrc);
            return 2;  // returning 2 for functions
        }
    }
    fclose(myshrc);
    return 0;  // return 0 if nothing is found
}

void get_alias(char *arg, char *dest) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/.myshrc", home_path);
    FILE *myshrc = fopen(file_path, "r");
    if (!myshrc) {
        fprintf(stderr, "Error: Could not open %s\n", file_path);
        return;
    }
    char line[MAX_STR_LEN];
    char target[MAX_STR_LEN];
    snprintf(target, MAX_STR_LEN, "%s=", arg);
    while (fgets(line, MAX_STR_LEN, myshrc)) {
        if (str_contains(line, target)) {
            int i = 0;
            while (line[i] != '\n') {
                if (line[i] == '=') {
                    line[i] = '\0';
                    strncpy(dest, line + i + 1, MAX_STR_LEN - 1);
                    return;
                }
                i++;
            }
            fclose(myshrc);
            return;
        }
    }
    fclose(myshrc);
}

void get_function(char *arg, char *dest) {
    char file_path[MAX_STR_LEN];
    snprintf(file_path, MAX_STR_LEN, "%s/.myshrc", home_path);
    FILE *myshrc = fopen(file_path, "r");
    if (!myshrc) {
        fprintf(stderr, "Error: Could not open %s\n", file_path);
        return;
    }
    char line[MAX_STR_LEN];
    char target[MAX_STR_LEN];
    snprintf(target, MAX_STR_LEN, "%s()", arg);
    while (fgets(line, MAX_STR_LEN, myshrc) != NULL) {
        if (str_contains(line, target)) {
            char buffer[MAX_STR_LEN];
            while (!str_contains(line, "}")) {
                strcat(buffer, line);
                if (fgets(line, MAX_STR_LEN, myshrc) == NULL) {
                    break;
                }
            }
            int buf_len = strlen(buffer);
            int curr_idx = 0;
            int blocked = 0;
            int start = 0;
            for (int i = 0; i < buf_len; i++) {
                if (buffer[i] == '{') {
                    start = 1;
                    continue;
                }
                if (start == 1) {
                    if (buffer[i] == '\t') {
                        continue;
                    }
                    else if (buffer[i] == ' ') {
                        if (curr_idx > 0 && blocked == 0 && dest[curr_idx - 1] != ' ') {
                            dest[curr_idx++] = ' ';
                        }
                    }
                    else if (buffer[i] == '#') {
                        blocked = 1;
                    }
                    else if (buffer[i] == '\n') {
                        if (curr_idx > 0) {
                            blocked = 0;
                            dest[curr_idx++] = ';';
                        }
                    }
                    else {
                        if (blocked == 0) {
                            dest[curr_idx++] = buffer[i];
                        }
                    }
                }
            }
            dest[curr_idx - 1] = '\0';
            fclose(myshrc);
            return;
        }
    }
    fclose(myshrc);
}
