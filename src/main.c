#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[] ){

    int i = 0;
    while (i < argc){
        printf("\narg %d  : %s" , i , argv[i]);
        i++;
    }

    return 0;
}