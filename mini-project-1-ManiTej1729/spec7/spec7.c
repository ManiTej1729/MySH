#include "../universal_header.h"

#if defined(__APPLE__) && defined(__MACH__)
    int OS = 1;
#elif defined(__linux__)
    int OS = 0;
#endif

void fun_for_mac(int pid) {
    // 1. process ID
    printf("1. Process ID: %d\n", pid);

    // 2. process Group ID
    int pgid = getpgid(pid);
    printf("2. Process Group ID: %d\n", pgid);

    // 3. Process status
    // struct proc_bsdinfo proc;
    // if (proc_pidfdinfo(pid, PROC_PIDTBSDINFO, 0, &proc, sizeof(proc)) != sizeof(proc)) {
    //     perror("3. proc_pidfdinfo error\n");
    // }
    // else {
    //     printf("3. Process Status: %u\n", proc.pbi_status);
    // }
    // 4. Virtual memory
    // mach_port_t task;
    // if (task_for_pid(mach_task_self(), pid, &task) != KERN_SUCCESS) {
    //     perror("task_for_pid");
    //     return;
    // }

    // mach_vm_address_t address = MACH_VM_MIN_ADDRESS;
    // mach_vm_size_t size;
    // vm_region_basic_info_data_64_t info;
    // mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
    // memory_object_name_t object;
    
    // printf("Virtual Memory Map:\n");
    // while (vm_region_64(task, &address, &size, VM_REGION_BASIC_INFO, (vm_region_info_t)&info, &count, &object) == KERN_SUCCESS) {
    //     printf("Region: %p - %p\n", (void *)address, (void *)(address + size));
    //     address += size;
    // }

    // 5. executable's path
    // char exec_path[MAX_STR_LEN];
    // uint32_t sizePath = sizeof(exec_path);
    // if (_NSGetExecutablePath(exec_path, &sizePath) == 0) {
    //     printf("5. Executable's path: %s\n", exec_path);
    // }
    // else {
    //     printf("5. Executable's path is too long.\n");
    // }
}

void fun_for_linux(int pid) {
    FILE *fptr;
    char path[MAX_STR_LEN];
    char line[256];
    printf("\033[32m");
    // 1. process ID
    printf("1. Process ID: %d\n\n", pid);
    // 2. process Group ID
    snprintf(path, MAX_STR_LEN, "/proc/%d/status", pid);
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Process with pid %d not found\n", pid);
        return;
    }
    while (fgets(line, sizeof(line), fptr)) {
        if (strncmp(line, "Tgid:", 5) == 0) {
            printf("2. Process Group ID: %s\n", line + 6);
            break;
        }
    }
    fclose(fptr);
    fptr = fopen(path, "r");
    // 3. Process status
    while (fgets(line, sizeof(line), fptr)) {
        if (strncmp(line, "State:", 6) == 0) {
            printf("3. Process Status: %s\n", line + 7);
            break;
        }
    }
    fclose(fptr);
    // 4. Virtual memory
    snprintf(path, MAX_STR_LEN, "/proc/%d/maps", pid);
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        perror("fopen");
        return;
    }
    printf("4. Virtual Memory Map:\n");
    while (fgets(line, sizeof(line), fptr)) {
        printf("%s", line);
    }
    printf("\n");
    fclose(fptr);
    // 5. executable's path
    snprintf(path, MAX_STR_LEN, "/proc/%d/exe", pid);
    char exePath[MAX_STR_LEN];
    ssize_t len = readlink(path, exePath, sizeof(exePath) - 1);
    if (len != -1) {
        exePath[len] = '\0';
        printf("5. Executable Path: %s\n", exePath);
    } else {
        printf("readlink\n");
    }
    printf("\033[0m");
}

void execute_proclore(char **args, int n) {
    if (n == 3) { // proclore of mentioned process
        if (is_number(args[1])) {
            int pid = atoi(args[1]);
            if (OS == 1) {
                fun_for_mac(pid);
            }
            else {
                fun_for_linux(pid);
            }
        }
        else {
            printf("\033[31mMySH: proclore: Invalid process ID.\033[0m\n");
        }
    }
    else { // proclore of shell/our current program
        if (OS == 1) {
            fun_for_mac(getpid());
        }
        else {
            fun_for_linux(getpid());
        }
    }
}
