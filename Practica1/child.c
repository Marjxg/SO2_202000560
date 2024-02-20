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
    int fileDescript = open("practica1.txt", O_WRONLY);

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
        write(fileDescript, ptr, strlen("abcdefgh"));
        sleep(randomTime);

    } else if (randomFunc == 2){
        //Leer
        printf("Leer -> %s \n", argv[2]);
        char buf[8];
        ssize_t nBytes; // Cambio a ssize_t para manejar valores negativos de retorno
        nBytes = read(fileDescript, buf, sizeof(buf) - 1); // Leer hasta sizeof(buf) - 1 bytes
        if (nBytes >= 0) {
            buf[nBytes] = '\0'; // Agregar terminador nulo al final del buffer
            printf("Bytes leídos: %ld, Contenido: %s\n", nBytes, buf);
        } else {
            perror("read"); // Mostrar error si la lectura falla
        }
        sleep(randomTime);
        
    } else if (randomFunc == 3){
        //Seek
        printf("Seek -> %s \n", argv[2]);
        write(fileDescript, "12345678", strlen("12345678"));
        lseek(fileDescript, 0, SEEK_SET);
        sleep(randomTime);
    }
}