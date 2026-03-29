#define _GNU_SOURCE
#include "cgroups.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include "namespaces.h"

int childStuff(void *arg){
    char **argv = (char **)arg; // casts the array back into string array

    printf("\nthe PID of Child : %d\nParent ID : %d", getpid(), getppid());

    // changes the root directory to the new rootfs folder
    if (chroot("rootfs") != 0) {
        printf("chroot failed :(\n");
        return 1;
    }else{
        printf("\n---- chroot successfull ----\n");
    }

    // changes the current working directory to the new root
    if (chdir("/") != 0) {
        printf("chdir failed :(\n");
        return 1;
    } else{
        printf("\n---- changed root directory to rootfs ----\n");
    }
    // To execute that command (user's command)
    execvp(argv[2], &argv[2]);

    perror("something went wrong :(\n");
    return 1;
}
