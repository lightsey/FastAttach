#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/ptrace.h>


void _attach_to_pid(pid_t target_pid) {
    if (ptrace(PTRACE_ATTACH, target_pid, 0, 0) == -1) {
        perror("Failed to attach to pid");
    }
    else {
        printf("Fast-attached to pid %lu\n", target_pid);
    }
}

void fastattach() {
    char *target_pid_env = NULL;
    pid_t target_pid = 0;

    target_pid_env = getenv("TARGET_PID");
    if (target_pid_env != NULL) {
        target_pid = strtoul(target_pid_env, NULL, 10);
        if (target_pid == 0 || target_pid == ULONG_MAX) {
            printf("Invalid value for target pid: %s\n", target_pid_env);
        }
        else {
            _attach_to_pid(target_pid);
        }
    }
}

