#include "../universal_header.h"

void print_latest_pid() {
    FILE *f = fopen("/proc/loadavg", "r");
    if (f == NULL) {
        printf("Error opening /proc/loadavg\n");
        return;
    }
    char line[MAX_STR_LEN];
    if (fgets(line, MAX_STR_LEN, f) == NULL) {
        printf("Error reading from /proc/loadavg\n");
        fclose(f);
        return;
    }
    fclose(f);
    // printf("%s\n", line);
    char *token = strtok(line, " ");
    char token2[100];
    while (token != NULL) {
        strcpy(token2, token);
        token2[strlen(token) - 1] = '\0';
        token = strtok(NULL, " ");
    }
    printf("\033[36m%s\033[0m\n", token2);
}

struct termios original_terminos;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminos);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &original_terminos);
    atexit(disable_raw_mode);
    raw = original_terminos;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // non-blocking behaviour
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void execute_neonate(char **args, int n) {
    int interval = atoi(args[2]);
    char ch;
    enable_raw_mode();
    time_t start = time(NULL);
    while (1) {
        time_t end = time(NULL);
        if (end - start >= interval) {
            print_latest_pid();
            start = end;
        }
        scanf("%c", &ch);
        if (ch == 'x') {
            break;
        }
    }
    disable_raw_mode();
}
