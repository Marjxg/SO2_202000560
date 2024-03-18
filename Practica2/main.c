#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

pthread_mutex_t lock;

struct Args
{
    char *archivo;
    int numero;
    int lecturas;
};

typedef struct nodo{ 
    int cuenta;
    char nombre[100];
    float saldo;
    struct nodo *sig; 
}usuario; 
 
typedef usuario *puntero;
puntero inicio = NULL; 

struct transacciones{ 
    char *op;
    char *c1;
    char *c2;
    char *sd;
    int linea;
};
struct transacciones transacs[300];
int contador_transac = 0;

struct Error_struct {
    int linea;
    char tipo[100];
};

struct Error_struct errores[100];
int contador_errores = 0;
struct Error_struct errores2[100];
int contador_errores2 = 0;

int contador_retiros = 0;
int contador_depositos = 0;
int contador_transferencia = 0;

void insertarEnLista (puntero *inicio, int cuenta, char nombre[100], float saldo){
    puntero nuevo;
    nuevo = malloc(sizeof(usuario)); 
    nuevo->cuenta = cuenta;
    nuevo-> saldo = saldo;
    nuevo->sig = *inicio; 
    sprintf(nuevo->nombre, "%s", nombre);
    *inicio = nuevo;
}

void imprimirLista(puntero cabeza){
    while(cabeza != NULL){
        printf("%d,%s,%f\n",cabeza->cuenta,cabeza->nombre,cabeza->saldo);
        cabeza = cabeza->sig;
    }
}

bool esNumerico(char * str){
    bool entero = true;
    for (int i = 0; i < strlen(str); i++){
        if(!isdigit(str[i])){
            if(str[i] != '.'){
                if(str[i] != '\n'){
                    if(str[i] != '-'){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool estaEnArreglo(puntero cabeza, int valor) {
    while(cabeza != NULL){
        if(cabeza->cuenta == valor){
            return true;
        }
        cabeza = cabeza->sig;
    }
    return false;
}

void depositar2(puntero *cabeza, int cuenta, float monto, int linea) {
    puntero actual = *cabeza;
    while (actual != NULL && actual->cuenta !=cuenta) {
        actual = actual->sig;
    }

    if (actual != NULL) {
        actual->saldo += monto;
        contador_depositos++;
    } else {
        errores2[contador_errores2].linea = linea;
        sprintf(errores2[contador_errores2].tipo, "%s %d", "No existe la cuenta destino: ", cuenta);
        contador_errores2++;
    }
}

void retirar2(puntero *cabeza, int cuenta, float monto, int linea) {

    puntero actual = *cabeza;
    while (actual != NULL && actual->cuenta !=cuenta) {
        actual = actual->sig;
    }

    if (actual != NULL) {
        if (actual->saldo >= monto){
            actual->saldo -= monto;
            contador_retiros++;
        } else {
            errores2[contador_errores2].linea = linea;
            sprintf(errores2[contador_errores2].tipo, "%s %d", "No hay fondos suficientes en", cuenta);
            contador_errores2++;
            }
    } else {
        errores2[contador_errores2].linea = linea;
        sprintf(errores2[contador_errores2].tipo, "%s %d", "No existe la cuenta origen: ", cuenta);
        contador_errores2++;
    }
}

void transaccion2(puntero *cabeza, int cuenta1, int cuenta2, float monto, int linea) {

    puntero origen = *cabeza;
    while (origen != NULL && origen->cuenta !=cuenta1) {
        origen = origen->sig;
    }

    puntero destino = *cabeza;
    while (destino != NULL && destino->cuenta !=cuenta2) {
        destino = destino->sig;
    }

    if (origen != NULL) {
        if (destino != NULL){
            if (origen->saldo >= monto){
                origen->saldo -= monto;
                destino->saldo += monto;
                contador_transferencia++;
            } else {
                errores2[contador_errores2].linea = linea;
                sprintf(errores2[contador_errores2].tipo, "%s %d", "No hay fondos suficientes en: ", cuenta1);
                contador_errores2++;
            }

        } else {
            errores2[contador_errores2].linea = linea;
            sprintf(errores2[contador_errores2].tipo, "%s %d", "No existe la cuenta origen: ", cuenta1);
            contador_errores2++;
        }
    } else {
        errores2[contador_errores2].linea = linea;
        sprintf(errores2[contador_errores2].tipo, "%s %d", "No existe la cuenta destino: ", cuenta2);
        contador_errores2++;
    }
}

void analizarOp(){
    for(int i=0; i<contador_transac; i++){
        if(esNumerico(transacs[i].op)){
            if(esNumerico(transacs[i].c1)){
                if(esNumerico(transacs[i].c2)){
                    int operacion, cuenta1, cuenta2;
                    float saldo;
                    operacion = atoi(transacs[i].op);
                    cuenta1 = atoi(transacs[i].c1);
                    cuenta2 = atoi(transacs[i].c2);
                    saldo = atof(transacs[i].sd);
                    //printf("%s, %d\n", cabeza->op, operacion);
                    if (operacion > 3 || operacion < 1){
                        errores2[contador_errores2].linea = transacs[i].linea;
                        sprintf(errores2[contador_errores2].tipo, "%s %d", "Operacion inválida: ", operacion);
                        contador_errores2++;
                    } else {
                        if(cuenta1 >= 0){
                            if(cuenta2 >= 0){
                                if(saldo >= 0){
                                    if (operacion == 1){
                                        depositar2(&inicio, cuenta1, saldo, transacs[i].linea);
                                    } else if (operacion == 2){
                                        retirar2(&inicio, cuenta1, saldo, transacs[i].linea);
                                    } else if (operacion == 3){
                                        transaccion2(&inicio, cuenta1, cuenta2, saldo, transacs[i].linea);
                                    }
                                } else {
                                    errores2[contador_errores2].linea = transacs[i].linea;
                                    sprintf(errores2[contador_errores2].tipo, "%s %f", "Valor inválido: ", saldo);
                                    contador_errores2++;
                                }
                            } else {
                                errores2[contador_errores2].linea = transacs[i].linea;
                                sprintf(errores2[contador_errores2].tipo, "%s %d", "Valor inválido: ", cuenta2);
                                contador_errores2++;
                            }
                        } else {
                            errores2[contador_errores2].linea = transacs[i].linea;
                            sprintf(errores2[contador_errores2].tipo, "%s %d", "Valor inválido: ", cuenta1);
                            contador_errores2++;
                        }
                    }
                } else {
                    errores2[contador_errores2].linea = transacs[i].linea;
                    sprintf(errores2[contador_errores2].tipo, "%s %s", "Cuenta 2 no es numérico", transacs[i].c2);
                    contador_errores2++;
                }
            } else {
                errores2[contador_errores2].linea = transacs[i].linea;
                sprintf(errores2[contador_errores2].tipo, "%s %s", "Cuenta 1 no es numérico", transacs[i].c1);
                contador_errores2++;
            }
        } else {
            errores2[contador_errores2].linea = transacs[i].linea;
            sprintf(errores2[contador_errores2].tipo, "%s %s", "Cuenta 1 no es numérico", transacs[i].op);
            contador_errores2++;
        }
    }
}

void reporte_estado_cuenta(puntero cabeza) {
    FILE *archivo_csv = fopen("estado_cuenta.csv", "w");
    if (archivo_csv == NULL) {
        printf("Error al abrir el archivo csv\n");
        return;
    }
    fprintf(archivo_csv, "no_cuenta,nombre,saldo\n");
    while(cabeza != NULL){
        fprintf(archivo_csv, "%d,%s,%f\n",cabeza->cuenta,cabeza->nombre,cabeza->saldo);
        cabeza = cabeza->sig;
    }
    fclose(archivo_csv);
}

void reporte_carga_usuarios(struct Args *args) {
    time_t tiempo_actual;
    time(&tiempo_actual);
    struct tm *info_tiempo_local = localtime(&tiempo_actual);
    char filename[100];
    strftime(filename, sizeof(filename), "Carga_%Y_%m_%d-%H_%M_%S.log", info_tiempo_local);

    FILE *archivo_csv = fopen(filename, "w");
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
    for (int i = 0; i < contador_errores; i++){
        fprintf(archivo_csv, "\t -Linea #");
        fprintf(archivo_csv, "%d", errores[i].linea);
        fprintf(archivo_csv, ": ");
        fprintf(archivo_csv, "%s", errores[i].tipo);
        fprintf(archivo_csv, "\n");
    }

    fclose(archivo_csv);
}

void reporte_carga_operaciones(struct Args *args) {
    time_t tiempo_actual;
    time(&tiempo_actual);
    struct tm *info_tiempo_local = localtime(&tiempo_actual);
    char filename[100];
    strftime(filename, sizeof(filename), "Operaciones_%Y_%m_%d-%H_%M_%S.log", info_tiempo_local);

    FILE *archivo_csv = fopen(filename, "w");
    if (archivo_csv == NULL) {
        printf("Error al abrir el archivo csv\n");
        return;
    }
    fprintf(archivo_csv, "------ RESUMEN DE OPERACIONES -------\n");
    fprintf(archivo_csv, "Fecha: %s\n", asctime(info_tiempo_local));

    fprintf(archivo_csv, "Operaciones realizadas\n");
    fprintf(archivo_csv, "\t -Retiros: %d\n", contador_retiros);
    fprintf(archivo_csv, "\t -Depósitos: %d\n", contador_depositos);
    fprintf(archivo_csv, "\t -Transferencias: %d\n", contador_transferencia);
    fprintf(archivo_csv, "Total: %d\n", (contador_transferencia+contador_depositos+contador_retiros));
    fprintf(archivo_csv, "\n");

    fprintf(archivo_csv, "Operaciones por hilo\n");
    int total = 0;
    for (int i = 0; i < 4; i++){
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
    for (int i = 0; i < contador_errores2; i++){
        fprintf(archivo_csv, "\t -Linea #");
        fprintf(archivo_csv, "%d", errores2[i].linea);
        fprintf(archivo_csv, ": ");
        fprintf(archivo_csv, "%s", errores2[i].tipo);
        fprintf(archivo_csv, "\n");
    }
    fprintf(archivo_csv, "Total errores: %d\n", contador_errores2);
    fclose(archivo_csv);
}

void consultar(puntero *cabeza) {
    int numeroCuenta;

    printf("Ingrese el número de cuenta: ");
    scanf("%d", &numeroCuenta);

    puntero actual = *cabeza;
    while (actual != NULL && actual->cuenta !=numeroCuenta) {
        actual = actual->sig;
    }

    if (actual != NULL) {
        printf("No. de cuenta: %d a nombre de %s con un saldo de %.2f\n", actual->cuenta, actual->nombre, actual->saldo);
    } else {
        printf("No se encontró la cuenta %d\n", numeroCuenta);
    }
}

void transaccion(puntero *cabeza) {
    int numeroCuenta1, numeroCuenta2;
    float monto;

    printf("Ingrese el número de cuenta origen: ");
    scanf("%d", &numeroCuenta1);

    printf("Ingrese el número de cuenta destino: ");
    scanf("%d", &numeroCuenta2);

    printf("Ingrese el monto a depositar: ");
    scanf("%f", &monto);

    puntero origen = *cabeza;
    while (origen != NULL && origen->cuenta !=numeroCuenta1) {
        origen = origen->sig;
    }

    puntero destino = *cabeza;
    while (destino != NULL && destino->cuenta !=numeroCuenta2) {
        destino = destino->sig;
    }

    if (origen != NULL) {
        if (destino != NULL){
            if (origen->saldo >= monto){
                origen->saldo -= monto;
                destino->saldo += monto;
                printf("Transacción realizada exitosamente. Saldo origen: %.2f Saldo destino: %.2f\n", origen->saldo, destino->saldo);
            } else {
                printf("No cuenta con los fondos suficientes");
            }

        } else {
            printf("No se encontró la cuenta destino %d\n", numeroCuenta2);
        }
    } else {
        printf("No se encontró la cuenta origen %d\n", numeroCuenta1);
    }
}

void retirar(puntero *cabeza) {
    int numeroCuenta;
    float monto;

    printf("Ingrese el número de cuenta: ");
    scanf("%d", &numeroCuenta);

    printf("Ingrese el monto a depositar: ");
    scanf("%f", &monto);

    puntero actual = *cabeza;
    while (actual != NULL && actual->cuenta !=numeroCuenta) {
        actual = actual->sig;
    }

    if (actual != NULL) {
        if (actual->saldo >= monto){
            actual->saldo -= monto;
            printf("Retiro realizado exitosamente. Saldo actual: %.2f\n", actual->saldo);
        } else {
            printf("No cuenta con los fondos suficientes");
        }
    } else {
        printf("No se encontró la cuenta %d\n", numeroCuenta);
    }
}

void depositar(puntero *cabeza) {
    int numeroCuenta;
    float monto;

    printf("Ingrese el número de cuenta: ");
    scanf("%d", &numeroCuenta);

    printf("Ingrese el monto a depositar: ");
    scanf("%f", &monto);

    puntero actual = *cabeza;
    while (actual != NULL && actual->cuenta !=numeroCuenta) {
        actual = actual->sig;
    }

    if (actual != NULL) {
        actual->saldo += monto;
        printf("Depósito realizado exitosamente. Saldo actual: %.2f\n", actual->saldo);
    } else {
        printf("No se encontró la cuenta %d\n", numeroCuenta);
    }
}

void *leerOps(void *input){

    FILE *fp = fopen(((struct Args *)input)->archivo, "r");
    fseek(fp, 0, SEEK_END);
    int final = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    static int pos = 0;
    static int linea = 1;
    fclose(fp);

    while (pos < final){  
        pthread_mutex_lock(&lock);
        FILE *fp = fopen(((struct Args *)input)->archivo, "r");
        fseek(fp, pos, SEEK_SET);
        char row[1000];
        char *token;
        fgets(row, 1000, fp);
        
        char *operacion_str, *cuenta1_str, *cuenta2_str, *saldo_str;

        operacion_str = strtok(row, ",");
        cuenta1_str = strtok(NULL, ",");
        cuenta2_str = strtok(NULL, ",");
        saldo_str = strtok(NULL, ",");

        if(row[0] != '\0'){
            if(!(strcmp(operacion_str, "operacion") == 0)){
                struct transacciones nueva_transaccion;
                nueva_transaccion.op = strdup(operacion_str); 
                nueva_transaccion.c1 = strdup(cuenta1_str); 
                nueva_transaccion.c2 = strdup(cuenta2_str); 
                nueva_transaccion.sd = strdup(saldo_str);
                nueva_transaccion.linea = linea;
                transacs[contador_transac] = nueva_transaccion;
                contador_transac++;
                //printf("%d", contador_transac);
                //printf("%s",transacs[contador_transac].op);
                ((struct Args *)input)->lecturas=((struct Args *)input)->lecturas+1;
            }
        }

        pos = ftell(fp);
        linea ++;
        fclose(fp);
        pthread_mutex_unlock(&lock); 
        sleep(0.5);
    }
}

int carga_ops(){
    char filename[100];
    printf("Ingrese la ruta del archivo: ");
    scanf("%s", filename);

    int numHilos = 4;
    struct Args *args = malloc(sizeof(struct Args) * numHilos);

    pthread_mutex_init(&lock, NULL);

    pthread_t hilos[numHilos];

    for (int i = 0; i < numHilos; i++){
        args[i].numero = i+1;
        args[i].archivo = filename;
        args[i].lecturas = 0;
        pthread_create(&hilos[i], NULL, leerOps, (void *)&args[i]);
    }

    for (int i = 0; i < numHilos; i++){
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&lock); 
    //imprimirOp(cabeza);
    analizarOp();
    reporte_carga_operaciones(args);
    return 0;
}

int menu() {
    int opcion;
    do{
        printf("\n**Menú de operaciones**\n");
        printf("1. Depositar\n");
        printf("2. Retirar\n");
        printf("3. Transacción\n");
        printf("4. Consultar saldo\n");
        printf("5. Reporte Estado Cuenta\n");
        printf("6. Cargar operaciones\n");
        printf("7. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion){
            case 1:
                depositar(&inicio);
                break;
            case 2:
                retirar (&inicio);
                break;
            case 3:
                transaccion (&inicio);
                break;
            case 4:
                consultar (&inicio);
                break;
            case 5:
                reporte_estado_cuenta(inicio);
                break;
            case 6:
                carga_ops ();
                break;
        } 
    } while (opcion != 7);
    return 0;
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
            if(!(strcmp(no_cuenta_str, "no_cuenta") == 0)){
                if(esNumerico(no_cuenta_str) && esNumerico(saldo_str)){
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
                    if (estaEnArreglo(inicio, no_cuenta)){
                        errores[contador_errores].linea = linea;
                        sprintf(errores[contador_errores].tipo, "%s", "La cuenta ya existe");
                        contador_errores++;
                    }
                    
                    if (saldo >= 0.0 && no_cuenta >= 0 && !estaEnArreglo(inicio, no_cuenta)){
                        insertarEnLista(&inicio, no_cuenta, nombre, saldo);

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
                } else {
                    errores[contador_errores].linea = linea;
                    sprintf(errores[contador_errores].tipo, "%s", "El valor no es numérico");
                    contador_errores++;
                }
            }
        } 

        pos = ftell(fp);
        linea ++;
        fclose(fp);
        pthread_mutex_unlock(&lock); 
        sleep(0.5);
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
    reporte_carga_usuarios(args);
    //reporte_estado_cuenta(inicio);
    menu();
    return 0;
}
