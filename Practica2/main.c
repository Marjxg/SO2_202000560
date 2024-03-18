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
};

struct data_struct {
    int cuenta;
    char nombre[100];
    float saldo;
};

struct data_struct usuarios[101];

struct Error_struct {
    int linea;
    char tipo[100];
};

struct Error_struct errores[100];
int contador_errores = 0;

bool estaEnArreglo(int valor) {
    for (int i = 0; i < 102; i++) {
        if (usuarios[i].cuenta == valor) {
            return true; 
        }
    }
    return false;
}

void crear_archivo_csv(struct Args *args) {
    time_t tiempo_actual;
    time(&tiempo_actual);
    struct tm *info_tiempo_local = localtime(&tiempo_actual);
    //char nombre_archivo[50];
    //strftime(nombre_archivo, sizeof(nombre_archivo), "%Y-%m-%d_%H-%M-%S.log", info_tiempo_local);

    FILE *archivo_csv = fopen("reporte_usuarios.log", "w");
    if (archivo_csv == NULL) {
        printf("Error al abrir el archivo csv\n");
        return;
    }
    fprintf(archivo_csv, "------ CARGA MASIVA USUARIOS -------\n");
    fprintf(archivo_csv, "%s\n", asctime(info_tiempo_local));

    fprintf(archivo_csv, "Usuarios Cargados\n");
    int total = 0;
    for (int i = 0; i < 3; i++){
        fprintf(archivo_csv, "\t -Hilo#");
        fprintf(archivo_csv, "%d", args[i].numero);
        fprintf(archivo_csv, ": ");
        fprintf(archivo_csv, "%d", args[i].lecturas);
        fprintf(archivo_csv, "\n");
        total += args[i].lecturas;
    }

    fprintf(archivo_csv, "Total: ");
    fprintf(archivo_csv, "%d", total);
    fprintf(archivo_csv, "\n\n");

    fprintf(archivo_csv, "Errores:\n");
    printf("%d", contador_errores);
    for (int i = 0; i < contador_errores; i++){
        fprintf(archivo_csv, "\t -Linea #");
        fprintf(archivo_csv, "%d", errores[i].linea);
        fprintf(archivo_csv, ": ");
        fprintf(archivo_csv, "%s", errores[i].tipo);
        fprintf(archivo_csv, "\n");
    }

    fclose(archivo_csv);
}


void *leerBloque(void *input){

    FILE *fp = fopen(((struct Args *)input)->archivo, "r");
    fseek(fp, 0, SEEK_END);
    int final = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    static int pos = 0;
    static int linea = 1;
    fclose(fp);

    //printf("--------- HILO %d -------------\n", ((struct Args *)input)->numero);
    while (pos < final){  
        pthread_mutex_lock(&lock);
        FILE *fp = fopen(((struct Args *)input)->archivo, "r");
        fseek(fp, pos, SEEK_SET);
        char row[1000];
        char *token;
        fgets(row, 1000, fp);
        
        int no_cuenta;
        float saldo;
        char *no_cuenta_str, *nombre, *saldo_str;

        if(row[0] != '\0'){
            //printf("Hilo: %d, pos: %d, final: %d\n", ((struct Args *)input)->numero, pos, final);
            //printf("Hilo: %d , linea: %d, valor: %s\n",((struct Args *)input)->numero, linea, row);
            no_cuenta_str = strtok(row, ",");
            nombre = strtok(NULL, ",");
            saldo_str = strtok(NULL, ",");
            
            no_cuenta = atoi(no_cuenta_str);
            saldo = atof(saldo_str);

            if (saldo < 0.0){
                errores[contador_errores].linea = linea;
                sprintf(errores[contador_errores].tipo, "%s", "El saldo no puede ser menor a 0");
                contador_errores++;
            }
            if (no_cuenta < 0){
                errores[contador_errores].linea = linea;
                sprintf(errores[contador_errores].tipo, "%s", "La cuenta no puede ser menor a 0");
                contador_errores++;
            }
            if (estaEnArreglo(no_cuenta)){
                errores[contador_errores].linea = linea;
                sprintf(errores[contador_errores].tipo, "%s", "La cuenta ya existe");
                contador_errores++;
            }
            
            if (saldo > 0.0 && no_cuenta > 0 && !estaEnArreglo(no_cuenta)){
                usuarios[linea].cuenta = no_cuenta;
                usuarios[linea].saldo = saldo;
                sprintf(usuarios[linea].nombre, "%s", nombre);

                if(((struct Args *)input)->numero == 1){
                    ((struct Args *)input)->lecturas=((struct Args *)input)->lecturas+1;
                    //printf("Hilo 1 %d",((struct Args *)input)->lecturas);
                } else if(((struct Args *)input)->numero == 2){
                    ((struct Args *)input)->lecturas=((struct Args *)input)->lecturas+1;
                    //printf("Hilo 2 %d",((struct Args *)input)->lecturas);
                } else if(((struct Args *)input)->numero == 3){
                    ((struct Args *)input)->lecturas=((struct Args *)input)->lecturas+1;
                    //printf("Hilo 3 %d",((struct Args *)input)->lecturas);
                }
            }
        } 

        pos = ftell(fp);
        linea ++;
        //sleep(0.5);
        fclose(fp);
        pthread_mutex_unlock(&lock); 
    }
}

int main(){   

    char filename[100];
    printf("Ingrese la ruta del archivo: ");
    scanf("%s", filename);

    int numHilos = 3;
    struct Args *args = malloc(sizeof(struct Args) * numHilos);

    pthread_mutex_init(&lock, NULL);

    pthread_t hilos[numHilos];

    for (int i = 0; i < numHilos; i++){
        args[i].numero = i+1;
        args[i].archivo = filename;
        args[i].lecturas = 0;
        pthread_create(&hilos[i], NULL, leerBloque, (void *)&args[i]);
    }

    for (int i = 0; i < numHilos; i++){
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&lock); 
    for (int i = 0; i < numHilos; i++){
        printf("Hilo %d, Lecturas: %d \n", args[i].numero, args[i].lecturas);
    }
    for (int i = 0; i < contador_errores; i++){
        printf("%d", i);
    }
    
    crear_archivo_csv(args);
    return 0;
}
