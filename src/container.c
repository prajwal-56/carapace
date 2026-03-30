#define _GNU_SOURCE
#include "cgroups.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cgroups.h"
#include "namespaces.h"
#include "container.h"

#define STACK_SIZE (1024 * 1024)


void container_init(int argc, char *argv[]){
    char *stack = malloc(STACK_SIZE);

    if (argc < 3){
        fprintf(stderr, "Usage: %s <command> <memory_limit_in_bytes>\n", argv[0]);
        return;
    }

    long int memory_limit = atol(argv[1]);   
    if (memory_limit <= 0){
        fprintf(stderr, "Please provide a valid memory limit in bytes :(\n");
        return;
    
    }
    if (argc < 2){
        fprintf(stderr, "Please provide a command to execute :(\n");
        return;
    }

    child_args_t childArgs;
    childArgs.argv = argv;
    pipe(childArgs.pipefd); // create a pipe for synchronization
    
    pid_t pid = clone(childStuff, stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, &child_args_t);

    if (pid == -1){
        perror("something went wrong. Forking Failed :(");
        return;
    }

    printf("---- creating cgroup.memory.max -----\n" );
    cgroups_init(pid, memory_limit);

    // write(pipefd[1], "ready", 5); // signal the child process to continue after setting up cgroups

    wait(NULL);
    printf("\nVoila !!!\n");
    free(stack);

    cgroups_cleanup();
}