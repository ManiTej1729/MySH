#include "../universal_header.h"

int color_decider(char *path, char *filename) {
    char full_path[MAX_STR_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        printf("stat error");
        return -1;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        return 34; // Blue for directories
    }
    if ((file_stat.st_mode & S_IXUSR) || (file_stat.st_mode & S_IXGRP) || (file_stat.st_mode & S_IXOTH)) {
        return 32; // Green for executables
    }
    if (S_ISREG(file_stat.st_mode)) {
        return 35; // Blue for regular files
    }
    return 0;
}

void generate_directory_contents(int no_of_entities, int max_len, char items[][max_len + 1], int a, char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        printf("\033[31mMySH: reveal: %s: No such file/directory.\033[0m\n", path);
        return;
    }
    int j = 0;
    while ((entry = readdir(dp)) != NULL) {
        if (a == 0 && entry -> d_name[0] == '.') {
            continue;
        }
        strcpy(items[j++], entry -> d_name);
    }
    closedir(dp);
}

int num_entities(char *path, int a, int *max_len) {
    DIR *dir;
    struct dirent *ent;
    int count = 0;

    if ((dir = opendir(path)) == NULL) {
        printf("opendir() error\n");
        return -1;
    }

    while ((ent = readdir(dir))!= NULL) {
        if (ent->d_name[0] == '.' && a == 0) {
            continue;
        }
        count++;
        *(max_len) = (strlen(ent -> d_name) > *(max_len)) ? strlen(ent -> d_name) : *(max_len);
    }

    closedir(dir);
    return count;
}

void print_file_info(char *path, char *filename, int color) {
    char full_path[MAX_STR_LEN];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        printf("stat error");
        return;
    }

    // 1. File permissions
    printf("\033[%dm", color);
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    // 2. Number of links
    printf("%hu ", file_stat.st_nlink);

    // 3. Owner
    struct passwd *pw = getpwuid(file_stat.st_uid);
    printf("%s ", pw ? pw->pw_name : "unknown");

    // 4. Group
    struct group *gr = getgrgid(file_stat.st_gid);
    printf("%s ", gr ? gr->gr_name : "unknown");

    // 5. File size
    printf("%lld ", file_stat.st_size);

    // 6. Last modified date
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", localtime(&file_stat.st_mtime));
    printf("%s ", timeStr);

    // 7. File/Directory name
    printf("%s", filename);
    printf(" \033[0m\n");
}

int compare_item_names(const void *name1, const void *name2) {
    return strcmp((const char *) name1, (const char *) name2);
}

void list_directory_contents(char *path, int a, int l) {
    int max_len = 0;
    int no_of_entities = num_entities(path, a, &max_len);
    printf("Number of entitites: %d\n", no_of_entities);

    char items[no_of_entities][max_len + 1];
    generate_directory_contents(no_of_entities, max_len, items, a, path);
    // sort the items by name
    qsort(items, no_of_entities, sizeof(items[0]), compare_item_names);
    for (int i = 0; i < no_of_entities; i++) {
        if (items[i][0] != '\0') {
            int color = color_decider(path, items[i]);
            if (l == 0) {
                if (a == 0) {
                    if (items[i][0] != '.') {
                        printf("\033[%dm%s\033[0m\n", color, items[i]);
                    }
                }
                else {
                    printf("\033[%dm%s\033[0m\n", color, items[i]);
                }
            }
            else {
                if (a == 0) {
                    if (items[i][0] != '.') {
                        print_file_info(path, items[i], color);
                    }
                }
                else {
                    print_file_info(path, items[i], color);
                }
            }
        }
    }
}

void execute_reveal(char **args, int n) {
    if (n == 2) {
        // reveal cwd
        list_directory_contents(".", 0, 0);
        return;
    }
    // getting flags
    int a = 0;
    int l = 0;
    int i;
    for (i = 1; i < n - 1; i++) {
        if (args[i][0] == '-') {
            int templen = strlen(args[i]);
            for (int j = 1; j < templen; j++) {
                if (args[i][j] == 'a') {
                    // reveal all files
                    a = 1;
                }
                else if (args[i][j] == 'l') {
                    // reveal file details
                    l = 1;
                }
                else {
                    // invalid flag
                    printf("\033[31mMySH: reveal: Invalid flag: -%c\033[0m\n", args[i][j]);
                    return;
                }
            }
        }
        else {
            break;
        }
    }
    if (i != n - 1) {
        // there is some path provided
        if (args[i][0] == '~') {
            char newpath[MAX_STR_LEN];
            snprintf(newpath, MAX_STR_LEN, "%s/%s", home_path, args[i]);
            strcpy(args[i], newpath);
        }
        path_formation(args, n, i);
        list_directory_contents(args[i], a, l);
    }
    else {
        // no path provided, reveal cwd
        list_directory_contents(".", a, l);
    }
}
