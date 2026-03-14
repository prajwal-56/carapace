#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <signal.h>

#define STACK_SIZE (1024 * 1024)

int childStuff(void *arg)
{
    char **argv = (char **)arg; // casts the array back into string array

    printf("\nthe PID of Child : %d\n", getpid());

    // To execute that command (user's command)
    execvp(argv[1], &argv[1]);

    perror("something went wrong :(");
    return 1;
}

int main(int argc, char *argv[])
{

    char *stack = malloc(STACK_SIZE);

    if (argc < 2)
    {
        printf("huh !? pass some argunents (likely a bin file or smth like that )");
    }
    else
    {
        pid_t pid = clone(childStuff, stack + STACK_SIZE, SIGCHLD, argv);

        if (pid == -1)
        {
            perror("something went wrong. Forking Failed :(");
            return 1;
        }

        wait(NULL);
        printf("\nVoila !!!\n");
        free(stack);
    }

    return 0;
}