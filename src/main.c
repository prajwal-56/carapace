#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

int main(int argc, char *argv[] ){

    if(argc < 2){
        printf("huh !? pass some argunents (likely a bin file or smth like that )");
    } else {
        pid_t pid = fork();     

        if (pid == -1){
            perror("something went wrong. Forking Failed :(");
            return 1;
        }

        if (pid == 0){      // A fork has been succesfully created
            execvp(argv[1] , &argv[1]);

            perror("Something went wrong (most likely execvp)");
            exit(1);
        } else {
            wait(NULL);
            printf("\nVoila !!!\n");
        }
    }

    return 0;
}