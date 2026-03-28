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

    chroot("/rootfs"); // changes the root directory to the new rootfs folder
    chdir("/"); // changes the current working directory to the new root
    
    // To execute that command (user's command)
    execvp(argv[1], &argv[1]);

    perror("something went wrong :(\n");
    return 1;
}
