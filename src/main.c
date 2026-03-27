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
#include "namespaces.h"
#include "container.h"

#define STACK_SIZE (1024 * 1024)

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    container_init(argc, argv);

    return 0;
}