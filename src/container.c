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


void container_init( long memory_limit, char **cmds){
    char *stack = malloc(STACK_SIZE);

    // if (argc < 3){
    //     fprintf(stderr, "Usage: %s <command> <memory_limit_in_bytes>\n", argv[0]);
    //     return;
    // }

    // long int memory_limit = atol(argv[1]);   
    if (memory_limit <= 0){
        fprintf(stderr, "Please provide a valid memory limit in bytes :(\n");
        return;
    
    }
    // if (argc < 2){
    //     fprintf(stderr, "Please provide a command to execute :(\n");
    //     return;
    // }

    child_args_t childArgs;
    childArgs.cmds = cmds; // set the commands to execute in the child process
    pipe(childArgs.pipefd); // create a pipe for synchronization
    
    pid_t pid = clone(childStuff, stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, &childArgs);

    if (pid == -1){
        perror("something went wrong. Forking Failed :(");
        return;
    }

    // printf("---- creating cgroup.memory.max -----\n" );
    cgroups_init(pid, memory_limit);

    write(childArgs.pipefd[1], "ready", 5); // signal the child process to continue after setting up cgroups


    int status; // for : How the child process terminated
    waitpid(pid , &status , 0);

    if (WIFSIGNALED(status)){  // WIFSIGNALED(status) - whether it got terminated by signal  
        if (WTERMSIG(status) == SIGKILL ){
            fprintf(stderr, "[carapace] the container likely got (OOM)killed \n");
        } else {
            fprintf(stderr , "uhh...")
        }
    }

    // TO confirm whether the child really got OOM killed - check the memory.events file for oom_kill count 
    // FILE *f = fopen("/sys/fs/cgroup/carapace/memory.events", "r");
    // char line[256];
    // while (fgets(line, sizeof(line), f)) {
    //     if (strncmp(line, "oom_kill", 8) == 0) {
    //         // parse the count after "oom_kill <N>"
    //         int count;
    //         sscanf(line, "oom_kill %d", &count);
    //         if (count > 0)
    //             fprintf(stderr, "[carapace] OOM confirmed: %d process(es) killed\n", count);
    //     }
    // }
    // fclose(f);
    wait(NULL);
    // printf("\nVoila !!!\n");
    free(stack);


    cgroups_cleanup();
}