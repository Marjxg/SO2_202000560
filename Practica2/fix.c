#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

pthread_mutex_t lock;

struct Args
{
    char *archivo;
    int numero;
    int lecturas;
    int final;
};

void *leerBloque(void *input){
    FILE *fp = fopen(((struct Args *)input)->archivo, "r");
    static int pos = 0;
    static int linea = 1;
    
    while (pos < ((struct Args *) input)->final){  
        pthread_mutex_lock(&lock);
        fseek(fp, pos, SEEK_SET);
        char row[1000];
        char *token;
        fgets(row, 1000, fp);
        
        printf("Hilo: %d, pos: %d, final: %d\n", ((struct Args *)input)->numero, pos, ((struct Args *) input)->final);
        printf("Hilo: %d , linea: %d, valor: %s\n",((struct Args *)input)->numero, linea, row);

        pos = ftell(fp);
        linea ++;
        pthread_mutex_unlock(&lock); 
        sleep(0.5);
    }
    fclose(fp);
}

int main(){  
    FILE *fp = fopen("usuarios.csv", "r");
    fseek(fp, 0, SEEK_END);
    int tamaño = ftell(fp);
    fclose(fp);

    int numHilos = 3;
    struct Args *args = malloc(sizeof(struct Args) * numHilos);

    pthread_mutex_init(&lock, NULL);

    pthread_t hilos[numHilos];

    for (int i = 0; i < numHilos; i++){
        args[i].final = tamaño;
        args[i].numero = i+1;
        args[i].archivo = "usuarios.csv";
        args[i].lecturas = 0;
        pthread_create(&hilos[i], NULL, leerBloque, (void *)&args[i]);
    }

    for (int i = 0; i < numHilos; i++){
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&lock); 
    return 0;
}
