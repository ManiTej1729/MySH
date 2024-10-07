#include "../universal_header.h"

void execute_input_redirection(char *command, int start1, int end1, int start2, int end2) {
    char left_command[end1 - start1 + 2];
    char right_command[end2 - start2 + 2];
    left_command[end1 - start1 + 1] = '\0';
    right_command[end2 - start2 + 1] = '\0';
    strncpy(left_command, command, end1 - start1 + 1);
    strncpy(right_command, command + start2, end2 - start2 + 1);
    trim_command(left_command);
    trim_command(right_command);

    // replace newline with null
    replace_newline_with_null(right_command);

    // ececute next part
    int fd_in = open(right_command, O_RDONLY);
    dup2(fd_in, STDIN_FILENO);
    close(fd_in);
    
    // create child process
    int rc = fork();
    if (rc == -1) {
        printf("Error: Failed to fork.\n");
        return;
    }
    else if (rc == 0) {
        execlp(left_command, left_command, (char *)NULL);
        printf("Error: Failed to execute command %s\n", left_command);
        exit(1);
    }
    else {
        waitpid(rc, NULL, 0);
    }
}

void execute_output_redirection(char *command, int start1, int end1, int start2, int end2, int input, char *oldwd, char *cwd, int *no_of_bkgd_processes, int trunc) {
    char left_command[end1 - start1 + 2];
    char right_command[end2 - start2 + 2];
    left_command[end1 - start1 + 1] = '\0';
    right_command[end2 - start2 + 1] = '\0';
    strncpy(left_command, command, end1 - start1 + 1);
    strncpy(right_command, command + start2, end2 - start2 + 1);
    trim_command(left_command);
    trim_command(right_command);

    // replace newline with null
    replace_newline_with_null(right_command);

    // ececute next part
    int fd_out;
    if (trunc == 1) {
        fd_out = open(right_command, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    else {
        fd_out = open(right_command, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }
    dup2(fd_out, 1);
    close(fd_out);
    if (input == -1) {
        tokenize_command(left_command, oldwd, cwd, no_of_bkgd_processes);
    }
    else {
        // parse the command cmd1 < cmd2 > cmd3
        end1 = input - 1;
        start2 = input + 1;
        end2 = strlen(left_command) - 1;
        execute_input_redirection(command, start1, end1, start2, end2);
    }
}

void redirection(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    replace_newline_with_null(command);
    trim_command(command);
    int len = strlen(command);
    int num_spl_chars = 0;
    // getting the indices of spl_chars (>, <, >>)
    int input = -1;
    int output = -1;
    for (int i = 0; i < len; i++) {
        if (command[i] == '>') {
            num_spl_chars++;
            output = i;
            if (i + 1 < len && command[i] == '>' && command[i + 1] == '>') {
                i++;
            }
        }
        if (command[i] == '<') {
            input = i;
            num_spl_chars++;
        }
    }
    // for (int i = 0; i < num_spl_chars + 1; i++) {
    //     printf("indices: %d\n", spl_chars[i]);
    // }
    if (num_spl_chars > 2) {
        printf("\033[31mInvalid command. More than two redirection operators found.\033[0m\n");
        return;
    }
    if (input > output && num_spl_chars == 2) {
        printf("\033[31mInvalid command. Input redirection operator is after output redirection operator.\033[0m\n");
        return;
    }
    // start doing the redirection
    int start1 = 0, end1, start2, end2 = strlen(command) - 1;
    if (output != -1) {
        // parse the command cmd1 > cmd2
        end1 = output - 1;
        start2 = (command[output + 1] == '>') ? output + 2 : output + 1;
        if (command[output + 1] == '>') {
            execute_output_redirection(command, start1, end1, start2, end2, input, oldwd, cwd, no_of_bkgd_processes, 0);
        }
        else {
            execute_output_redirection(command, start1, end1, start2, end2, input, oldwd, cwd, no_of_bkgd_processes, 1);
        }
    }
    else {
        if (input == -1) {
            tokenize_command(command, oldwd, cwd, no_of_bkgd_processes);
        }
        else {
            // parse the command cmd1 < cmd2
            end1 = input - 1;
            start2 = input + 1;
            execute_input_redirection(command, start1, end1, start2, end2);
        }
    }
}

void mid_processing(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    int len = strlen(command);
    int num_commands = 0;
    for (int i = 0; i < len; i++) {
        if (command[i] == '|') {
            num_commands++;
        }
    }
    if (command[strlen(command) - 1] != '|') {
        num_commands++;
    }
    else {
        printf("\033[31mMySH: Invalid usage of pipes: %s\033[0m\n", command);
    }
    char *commands[num_commands];
    char *token = strtok(command, "|");
    int index = 0;
    while (token!= NULL) {
        commands[index] = strdup(token);
        replace_newline_with_null(commands[index++]);
        // printf("token: %s\n", commands[index++]);
        token = strtok(NULL, "|");
    }
    int std_in_copy = dup(STDIN_FILENO);
    int std_out_copy = dup(STDOUT_FILENO);
    int prev_fd;
    int curr_fd[2];
    for (int i = 0; i < num_commands; i++) {
        if (i < num_commands - 1) {
            if (pipe(curr_fd) == -1) {
                printf("Pipe error\n");
                exit(1);
            }
        }
        int rc = fork();
        if (rc == -1) {
            printf("Error: Failed to fork.\n");
            exit(1);
        }
        else if (rc == 0) {
            if (i > 0) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i < num_commands - 1) {
                // Redirect output to the pipe for all but the last command
                dup2(curr_fd[1], STDOUT_FILENO);
                close(curr_fd[1]);  // Close the write-end of the current pipe
            }
            redirection(commands[i], foreground, background, oldwd, cwd, no_of_bkgd_processes);
            exit(0);
        }
        else {
            waitpid(rc, NULL, 0);
            if (i > 0) {
                close(prev_fd);
            }
            if (i < num_commands - 1) {
                close(curr_fd[1]);
                prev_fd = curr_fd[0];
            }
        }
    }
    dup2(std_in_copy, 0);
    dup2(std_out_copy, 1);
    close(std_in_copy);
    close(std_out_copy);
    for (int i = 0; i < num_commands; i++) {
        free(commands[i]);
    }
}

void inner_process_for_acknowledgement(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    int pid = getpid();
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "Fork failed\n");
        return;
    }
    else if (rc == 0) {
        mid_processing(command, foreground, background, oldwd, cwd, no_of_bkgd_processes);
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
        delete_entry(pid);
    }
}

bool preprocessing(char *command, int foreground, int background, char *oldwd, char *cwd, int *no_of_bkgd_processes) {
    bool involves_redirection = false;
    int num_commands = 0;
    int len = strlen(command);
    for (int i = 0; i < len; i++) {
        if (command[i] == '>' || command[i] == '<' || command[i] == '|') {
            involves_redirection = true;
            break;
        }
    }
    if (!involves_redirection) {
        return false;
    }
    int rc = fork();
    if (rc == -1) {
        printf("Error: Failed to fork.\n");
        exit(1);
    }
    else if (rc == 0) {
        if (background) {
            inner_process_for_acknowledgement(command, foreground, background, oldwd, cwd, no_of_bkgd_processes);
        }
        else {
            mid_processing(command, foreground, background, oldwd, cwd, no_of_bkgd_processes);
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
    return true;
}
