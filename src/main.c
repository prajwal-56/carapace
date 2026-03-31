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
#include <string.h>

#define STACK_SIZE (1024 * 1024)

// for CLI args ( --mem )
long parse_mem(char *s){
    long val = atol(s);           // grab the number part
    char last = s[strlen(s)-1];   // grab last character
    if (last == 'M') val *= 1024 * 1024; // for MegaBytes
    else if (last == 'K') val *= 1024;  // for KiloBytes
    return val;
}


int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, "Usage: %s --mem {limit}M {command} [args...]\n", argv[0]);
        return 1;
    }

    // Check for --mem flag
    if(strcmp(argv[1], "--mem") != 0){
        fprintf(stderr, "Usage: %s --mem {limit}M {command} [args...]\n", argv[0]);
        return 1;
    }    

    //  
    long memory_limit = parse_mem(argv[2]);
    char **cmds = &argv[3]; // commands

    container_init(memory_limit, cmds);

    return 0;
}