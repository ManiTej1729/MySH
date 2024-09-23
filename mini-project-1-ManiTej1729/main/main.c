#include "../universal_header.h"

char oldwd[MAX_STR_LEN] = "";
char cwd[MAX_STR_LEN];

int curr_fg_proc = 0;

char home_path[MAX_STR_LEN];

int main() {
    // Signals
    curr_fg_proc = 0;
    getcwd(home_path, MAX_STR_LEN);
    // printf("home path: %s\n", home_path);
    signal(SIGINT, handle_ctrl_c);
    // signal(SIGTSTP, handle_ctrl_z);
    // show_intro();
    char command[MAX_STR_LEN];
    strcpy(cwd, home_path);
    int no_of_bkgd_processes = 0;
    int insert_flag = 1;
    int num_ended_processes = 0;
    char ch;
    int curr_idx;
    while (1) {
        show_user_details(cwd, oldwd);
        // fgets(command, 1000, stdin);
        curr_idx = 0;
        while ((ch = fgetc(stdin)) != '\n') {
            if (ch == EOF) {
                handle_ctrl_d();
                break;
            }
            command[curr_idx++] = ch;
            if (curr_idx >= MAX_STR_LEN) {
                break;
            }
        }
        command[curr_idx] = '\0';
        trim_command(command);
        getcwd(cwd, MAX_STR_LEN);
        tabs_and_spaces(command);
        int len = strlen(command);
        if (strcmp(command, "q") == 0) {
            // show_outro();
            break;
        }
        num_ended_processes = num_lines_in_buffer();
        no_of_bkgd_processes -= num_ended_processes;
        // push the buffer
        flush_the_buffer();
        if (strcmp(command, "\n") == 0) {
            continue;
        }
        // check for log
        insert_flag = 1;
        for (int i = 0; i < len; i++) {
            if (strncmp(command + i, "log", 3) == 0) {
                // check for back
                if ((i + 3 < len && (command[i + 3] == ' ' || command[i + 3] == '&' || command[i + 3] == ';' || command[i + 3] == '\n')) || i + 3 == len) {
                    // check for front
                    if ((i - 1 >= 0 && (command[i - 1] == ' ' || command[i - 1] == '&' || command[i - 1] == ';')) || i - 1 < 0) {
                        // this is a valid log command
                        insert_flag = 0;
                    }
                }
            }

            if (strncmp(command + i, "le", 2) == 0) { // le is alias for log execute command
                // check for back
                if ((i + 2 < len && (command[i + 2] == ' ' || command[i + 2] == '&' || command[i + 2] == ';' || command[i + 2] == '\n')) || i + 2 == len) {
                    // check for front
                    if ((i - 1 >= 0 && (command[i - 1] == ' ' || command[i - 1] == '&' || command[i - 1] == ';')) || i - 1 < 0) {
                        // this is a valid log command
                        insert_flag = 0;
                    }
                }
            }
        }
        if (insert_flag == 1) {
            // add the command to the file history.txt
            insert_command(command);
            remove_excess_commands();
        }
        execute_command(command, oldwd, cwd, &no_of_bkgd_processes);
    }

    return 0;
}
