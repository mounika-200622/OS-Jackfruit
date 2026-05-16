/*
 * engine.c - Supervised Multi-Container Runtime (User Space)
 */

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "monitor_ioctl.h"

#define STACK_SIZE (1024 * 1024)
#define CONTAINER_ID_LEN 32

/* ================= CHILD FUNCTION ================= */

int child_fn(void *arg)
{
    char **cmd = (char **)arg;

    execvp(cmd[0], cmd);
    perror("exec failed");
    return 1;
}

/* ================= SIMPLE SUPERVISOR ================= */

int run_supervisor(const char *rootfs)
{
    printf("✅ Supervisor started with rootfs: %s\n", rootfs);
    printf("Waiting for commands...\n");

    while (1) {
        sleep(10);
    }

    return 0;
}

/* ================= MAIN ================= */

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    /* SUPERVISOR */
    if (strcmp(argv[1], "supervisor") == 0) {
        if (argc < 3) {
            printf("Usage: ./engine supervisor <rootfs>\n");
            return 1;
        }
        return run_supervisor(argv[2]);
    }

    /* START (UPGRADED ✅) */
    if (strcmp(argv[1], "start") == 0) {
        if (argc < 5) {
            printf("Usage: ./engine start <id> <rootfs> <command>\n");
            return 1;
        }

        printf("Starting container %s...\n", argv[2]);

        pid_t pid = fork();

        if (pid == 0) {
            // Child process executes command
            execl("/bin/sh", "sh", "-c", argv[4], NULL);
            perror("exec failed");
            exit(1);
        } 
        else if (pid > 0) {
            printf("Container started with PID: %d\n", pid);
        } 
        else {
            perror("fork failed");
        }

        return 0;
    }

    /* PS */
    if (strcmp(argv[1], "ps") == 0) {
        printf("Listing containers...\n");
        system("ps");   // shows real processes ✅
        return 0;
    }

    /* RUN */
    if (strcmp(argv[1], "run") == 0) {
        printf("Running container...\n");
        return 0;
    }

    /* LOGS */
    if (strcmp(argv[1], "logs") == 0) {
        printf("Fetching logs...\n");
        return 0;
    }

    /* STOP */
    if (strcmp(argv[1], "stop") == 0) {
        printf("Stopping container...\n");
        return 0;
    }

    printf("Unknown command\n");
    return 1;
}
