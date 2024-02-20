#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


int main(){
    //Creacion archivo para history
    int fd = open("syscalls.log", O_RDWR | O_CREAT, 0777);
    //Creacion archivo para child
    int fileDescript = open("practica1.txt", O_RDWR | O_CREAT, 0777);
    close(fileDescript);

    //Creacion primer proceso hijo
    pid_t child1 = fork();

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

            pid_t monitor = fork();
            
            if (monitor == -1){
                perror("fork");
                exit(1);
            }

            if (monitor == 0){
                char command[100];
                sprintf(command, "%s %d %d %s", "sudo stap trace.stp", child1, child2, " > syscalls.log\n");
                printf("%s", command);
                system(command);
            }

            int statLoc1, statLoc2, statLoc3;
            waitpid(child1, &statLoc1, 0);
            waitpid(child2, &statLoc2, 0);
            waitpid(monitor, &statLoc3, 0);
        }
    }
    return 0;
}