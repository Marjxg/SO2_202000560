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

        char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char destino[8] = "";
    srand(time(NULL));

    for (size_t i = 0; i < 8; i++) {
        int randomPos = rand() % 62;
        destino[i] = characters[randomPos];
    }
    printf("%.*s\n", 8, destino);

        const void *ptr = (const void *)destino;
        int fileDescript = open("practica1.txt", O_WRONLY);
        write(fileDescript, ptr, strlen("abcdefgh"));
        close(fileDescript);
        sleep(randomTime);

    } else if (randomFunc == 2){
        //Leer
        printf("Leer -> %s \n", argv[2]);
        int fileDescript = open("practica1.txt", O_RDONLY);
        char buf[8];
        size_t nBytes;
        read(fileDescript, buf, nBytes);
        close(fileDescript);
        sleep(randomTime);
        
    } else if (randomFunc == 3){
        //Seek
        printf("Seek -> %s \n", argv[2]);
        int fileDescript = open("practica1.txt", O_RDONLY);
        write(fileDescript, "12345678", strlen("12345678"));
        lseek(fileDescript, 0, SEEK_SET);
        close(fileDescript);
        sleep(randomTime);
    }
}