#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

void main(int argc, char *argv[]){
    srand(time(NULL) + getpid());
    int randomFunc = rand() % 3 + 1;
    int randomTime = rand() % 3 + 1;

    if (randomFunc == 1){
        //Escribir
        printf("Escribir -> %s \n", argv[2]);
        int fileDescript = open("practica1.txt", O_RDONLY);
        write(fileDescript, "abcdefgh", strlen("abcdefgh"));
        sleep(randomTime);
    } else if (randomFunc == 2){
        //Leer
        printf("Leer -> %s \n", argv[2]);
        int fileDescript = open("practica1.txt", O_RDONLY);
        char buf[8];
        size_t nBytes;
        read(fileDescript, buf, nBytes); 
        sleep(randomTime);
        
    } else if (randomFunc == 3){
        //Seek
        printf("Seek -> %s \n", argv[2]);
        int fileDescript = open("practica1.txt", O_RDONLY);
        write(fileDescript, "12345678", strlen("12345678"));
        lseek(fileDescript, 0, SEEK_SET);
        sleep(randomTime);
    }
}