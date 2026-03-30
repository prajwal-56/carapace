#ifndef CONTAINER_H
#define CONTAINER_H

typedef struct {
    char **argv;
    int pipefd[2];      // the pipe fd - for syncing - (prevent racing conditoin) 
} child_args_t;


void container_init();

#endif