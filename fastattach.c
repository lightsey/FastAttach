#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define __USE_GNU
#include <dlfcn.h>

#define PTRACE_ATTACH  16

static int skip_next_attach = 0;

long ptrace(int request, pid_t pid, void *addr, void *data)
{
    static long (*ptrace_real) (int request, pid_t pid, void *addr, void *data) = NULL;
    if (ptrace_real == NULL)
        ptrace_real = dlsym(RTLD_NEXT, "ptrace");

    if (skip_next_attach && request == PTRACE_ATTACH) {
        skip_next_attach = 0;
        return 0;
    }
    return ptrace_real(request, pid, addr, data);
}

void fastattach(int argc, char **argv, char **env)
{
    char **target_arg;
    pid_t target_pid = 0;

    for (target_arg = argv; *target_arg != NULL; target_arg++) {
        if (strcmp(*target_arg, "-p") == 0) {
            target_arg++;
            break;
        }
    }
    if (*target_arg != NULL) {
        target_pid = strtoul(*target_arg, NULL, 10);
        if (target_pid != 0 && target_pid != ULONG_MAX && ptrace(PTRACE_ATTACH, target_pid, 0, 0) == 0) {
            printf("Fast-attached to pid %lu\n", target_pid);
            skip_next_attach = 1;
        }
    }
}
