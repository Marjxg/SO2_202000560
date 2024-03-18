#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include<string.h>

pthread_mutex_t lock;

struct Usuarios
{
    int cuenta;
    char nombre[100];
    float saldo;
};

struct Usuarios usuarios[200];

struct Args
{
    int inicio;
    int fin;
    char *archivo;
    int numero;
};

struct Args *dividirArchivo(char *nombreArchivo, int numHilos)
{
    // Tamaño del archivo
    FILE *fp = fopen(nombreArchivo, "r");
    fseek(fp, 0, SEEK_END);
    int tamanioArchivo = ftell(fp);
    fclose(fp);

    // Tamaño del bloque
    int numLineas = tamanioArchivo / sizeof(char);
    int tamanioBloque = numLineas / numHilos;

    // Reservar en memoria el espacio para los argumentos de los hilso
    struct Args *args = malloc(sizeof(struct Args) * numHilos);

    // Almacenar las posiciones de inicio y fin
    for (int i = 0; i < numHilos; i++)
    {
        args[i].inicio = i * tamanioBloque;
        args[i].fin = (i + 1) * tamanioBloque - 1;
        args[i].archivo = nombreArchivo;
    }
    args[numHilos - 1].fin = tamanioArchivo - 1;

    return args;
}

// Función para leer un bloque de archivo
void *leerBloque(void *input)
{
    pthread_mutex_lock(&lock); 
    FILE *fp = fopen(((struct Args *)input)->archivo, "r");
    fseek(fp, 0, SEEK_END);
    int final = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    static int pos = 0;
    static int linea = 0;
    char *token;

    while (pos != final)
    {  
        fseek(fp, pos, SEEK_SET);
        char row[1000];
        fgets(row, 1000, fp);

        //printf("Hilo: %d, pos: %d, final: %d\n", ((struct Args *)input)->numero, pos, final);
        printf("Hilo: %d , linea: %d, valor: %s",((struct Args *)input)->numero, linea, row);

        token = strtok(row, ",");
        usuarios[linea].cuenta = atoi(token);

        token = strtok(NULL, ",");
        sprintf(usuarios[linea].nombre, "%s", token);

        token = strtok(NULL, ",");
        usuarios[linea].saldo = atoi(token);

        pos = ftell(fp);
        linea ++;
        sleep(1);
    }
    fclose(fp);
    pthread_mutex_unlock(&lock); 
}

int main()
{   
    char* filename;
    printf("Ingrese la ruta del archivo CSV: ");
    scanf("%s", filename);

    int numHilos = 3;
    struct Args *args = malloc(sizeof(struct Args) * numHilos);
    for (int i = 0; i < numHilos; i++)
    {
        args[i].archivo = "usuarios.csv";
    }

    pthread_mutex_init(&lock, NULL);

    pthread_t hilos[numHilos];
    for (int i = 0; i < numHilos; i++)
    {
        args[i].numero = i+1;
        pthread_create(&hilos[i], NULL, leerBloque, (void *)&args[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&lock); 
    /*printf("\n\n");
    for (int i = 0; i < sizeof(usuarios); i++) {
        printf("Cuenta: %d, Nombre: %s, Saldo: %.2f\n", usuarios[i].cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }*/
    return 0;
}
