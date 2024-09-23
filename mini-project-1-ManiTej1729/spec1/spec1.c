#include "../universal_header.h"

void show_user_details(char *cwd, char *oldwd) {
    char *user = getlogin();
    char *path = strdup(home_path);
    int path_len = strlen(path);
    int cwd_len = strlen(cwd);
    char hostname[MAX_STR_LEN];
    hostname[MAX_STR_LEN - 1] = '\0';
    gethostname(hostname, MAX_STR_LEN - 1);
    if (cwd_len >= path_len) {
        if (strncmp(cwd, path, path_len) == 0) {
            char temp[MAX_STR_LEN];
            for (int i = path_len; i < cwd_len; i++) {
                temp[i - path_len] = cwd[i];
            }
            temp[cwd_len - path_len] = '\0';
            printf("<\033[31m%s\033[0m@%s:\033[32m~%s\033[0m> ", user, hostname, temp);
            free(path);
            return;
        }
    }
    printf("<\033[31m%s\033[0m@%s:\033[32m%s\033[0m> ", user, hostname, cwd);
    free(path);
}
