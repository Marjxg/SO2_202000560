#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void main(int argc, char *argv[]){
    printf("\nSoy el proceso hijo %s\n", argv[2]);
    sleep(3);
}