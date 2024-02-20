#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

volatile __sig_atomic_t interruption = 0;

void signalHandler(int sigReceived)
{
    printf("\nINTERRUPCION CON CTRL C\n");
    interruption = 1;
}

int main(int argc, char *argv[])
{
    // Abrir archivo
    int fileDescript = open("practica1.txt", O_WRONLY);
    srand(time(NULL) + getpid());
    // Senal
    signal(SIGINT, signalHandler);

    while (!interruption)
    {
        int randomFunc = rand() % 3 + 1;
        int randomTime = rand() % 3 + 1;

        if (randomFunc == 1)
        {
            // Escribir
            //printf("Escribir -> %s \n", argv[2]);

            char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            char destino[8] = "";
            srand(time(NULL));

            for (size_t i = 0; i < 8; i++)
            {
                int randomPos = rand() % 62;
                destino[i] = characters[randomPos];
            }
            write(fileDescript, destino, 8);
        }
        else if (randomFunc == 2)
        {
            // Leer
            //printf("Leer -> %s \n", argv[2]);
            char buf[8];
            read(fileDescript, buf, 8);
        }
        else if (randomFunc == 3)
        {
            // Seek
            //printf("Seek -> %s \n", argv[2]);
            lseek(fileDescript, 0, SEEK_SET);
        }
        sleep(randomTime);
    }

    return 0;
}