#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

void main(){
    //Creacion archivo
    int fileDescript = open("practica1.txt", O_RDWR | O_CREAT, 0777);
    close(fileDescript);

    //Creacion primer proceso hijo
    pid_t child1 = fork();
    int pid1 = child1;

    if (child1 == -1){
        perror("fork");
        exit(1);
    }

    if (child1 == 0){
        char *argPtr[4];
        argPtr[0] = "child.c";
        argPtr[1] = "Hola";
        argPtr[2] = "Soy el proceso hijo 1";
        argPtr[3] = NULL;

        execv("/home/mint/Documents/SO2_202000560/Practica1/child.bin", argPtr);
    } else {
        pid_t child2 = fork();
        int pid2 = child2;

        printf("PIDS %d %d\n", pid1, pid2);
            char command[100];
            sprintf(command, "%s %d %d %s", "sudo stap trace.stp", pid1, pid2, " > syscalls.log");
            printf("%s \n", command);
            system(command);

        if (child2 == -1){
            perror("fork");
            exit(1);
        }

        if (child2 == 0){

            char *argPtr[4];
            argPtr[0] = "child.c";
            argPtr[1] = "hola";
            argPtr[2] = "Soy el proceso hijo 2";
            argPtr[3] = NULL;

            execv("/home/mint/Documents/SO2_202000560/Practica1/child.bin", argPtr);
        } else {

            int status;
            wait(&status);

            if(WIFEXITED(status)){
                printf("\nLos procesos hijos terminaron con estado = %d\n", WIFEXITED(status));
            } else {
                printf("Ocurrio un error al terminar el proceso hijo 1\n");
            }
        }

        printf("Soy el proceso Padre\n");

            int status;
            wait(&status);

            if(WIFEXITED(status)){
                printf("\nLos procesos hijos terminaron con estado = %d\n", WIFEXITED(status));
            } else {
                printf("Ocurrio un error al terminar el proceso hijo 1\n");
            }

            printf("Terminando proceso padre \n");
    }
}