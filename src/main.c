#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cgroups.h"

#define STACK_SIZE (1024 * 1024)

int childStuff(void *arg)
{
    char **argv = (char **)arg; // casts the array back into string array

    printf("\nthe PID of Child : %d\nParent ID : %d", getpid(), getppid());

    // To execute that command (user's command)
    execvp(argv[1], &argv[1]);

    perror("something went wrong :(");
    return 1;
}

int main(int argc, char *argv[]){

    char *stack = malloc(STACK_SIZE);

    if (argc < 2){
        printf("huh !? pass some argunents (likely a bin file or smth like that )");
    } else {
        pid_t pid = clone(childStuff, stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, argv);

        if (pid == -1){
            perror("something went wrong. Forking Failed :(");
            return 1;
        }

        printf("---- cgroup thing working -----\n" );
        printf("---- creating cgroup.memory.max - allocating 56MB -----\n" );
        cgroups_init(pid, 56000000); // 56 MB)
        wait(NULL);
        printf("\nVoila !!!\n");
        free(stack);
    }

    return 0;
}