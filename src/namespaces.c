#define _GNU_SOURCE
#include "cgroups.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include "namespaces.h"
#include "container.h"

int childStuff(void *arg){
    child_args_t *childArgs = (child_args_t *)arg; // casts the argument to the child_args_t struct
    // char **argv = childArgs->cmds; 

    // printf("\nthe PID of Child : %d\nParent ID : %d", getpid(), getppid());

    // printf("\nWaiting for the parent to finish the cgroups_init...\n"); 
    char buffer[6];

    read(childArgs->pipefd[0], buffer, 5); // when the parent finishes setup , this continues....

    // printf("\n---- parent setup complete ----\n");

    // changes the root directory to the new rootfs folder
    if (chroot("rootfs") != 0) {
        perror("chroot failed :(\n");
        return 1;
    }else{
        // printf("\n---- chroot successfull ----\n");
    }

    // changes the current working directory to the new root
    if (chdir("/") != 0) {
        printf("chdir failed :(\n");
        return 1;
    } else{
        // printf("\n---- changed root directory to rootfs ----\n");
    }
    // To execute that command (user's command)
    execvp( childArgs->cmds[0] , childArgs->cmds);
    perror("something went wrong (probably execvp failed):(\n");
    return 1;
}
