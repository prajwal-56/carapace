#ifndef CONTAINER_H
#define CONTAINER_H

typedef struct {
    char **cmds;
    int pipefd[2];      // the pipe fd - for syncing - (prevent racing conditoin) 
    char *root;         // root directory for chroot
} child_args_t;


void container_init(long memory_limit, char **cmd , char *root);

#endif