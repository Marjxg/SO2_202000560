# Manual Técnico
Marjorie Gissell Reyes Franco - 202000560

Instalación de C
```
sudo apt-get build-essential
```
Compilar archivos
```
gcc main.c -o main
```
Ejecutar programa
```
./main
```

### Archivos de carga
Carga masiva "TRANSACCIONES"
* Formato:
operacion,cuenta1,cuenta2,monto
* Tipos de dato:
int,int,int,float
* Ejemplo:

![alt text](image.png)

Carga masiva "USUARIOS"
* Formato:
no_cuenta,nombre,saldo
* Tipos de dato:
int,char*,float
* Ejemplo:

![alt text](image-1.png)

### Estructuras
Estructura para manejar los argumentos de los hilos

![alt text](image-2.png)

Estructura para manejar los nodos de usuarios en la lista enlazada

![alt text](image-3.png)

Estructura para el manejo de las transacciones ingresadas en la carga masiva

![alt text](image-4.png)

Estructura para el manejo de usuarios

![alt text](image-5.png)

### Variables globales
* Manejo de puntero inicial de la lista enlazada de usuarios
```
typedef usuario *puntero;
puntero inicio = NULL; 
```
* Manejo de la lista de transacciones ingresadas en carga masiva
```
struct transacciones transacs[300];
int contador_transac = 0;
```
* Manejo de la lista de errores de la carga masiva de usuarios
```
struct Error_struct errores[100];
int contador_errores = 0;
```
* Manejo de la lista de errores de la carga masiva de transacciones
```
struct Error_struct errores2[100];
int contador_errores2 = 0;
```
* Manejo de contadores para saber la cantidad de operaciones realizadas
```
int contador_retiros = 0;
int contador_depositos = 0;
int contador_transferencia = 0;
```

### Funciones
Maneja el flujo de inicio, es decir, la carga masiva de usuarios por medio de la creación de 3 hilos.

![alt text](image-6.png)

Función propia de los hilos, se encarga de acceder a la línea actual del archivo para realizar lectura por filas, análisis y almacenamiento de datos.

![alt text](image-7.png)

Función para mostrar en consola el menu de opciones y su redirección a su respectiva funcionalidad.

![alt text](image-8.png)

Función que se encarga de la creación de 4 hilos y su asociación con sus respectivos argumentos y función.

![alt text](image-9.png)

Función propia de los hilos encargados de la carga masiva de transacciones, accede a la línea actual del archivo para realizar la lectura por filas y su almacenamiento.

![alt text](image-10.png)

Función encargada de analizar el formato de las transacciones, validar errores y funcionalidades específicas

![alt text](image-18.png)

Función para la operación individual de depositar, valida la existencia de la cuenta en la lista enlazada

![alt text](image-11.png)

Función para la operación individual de retirar, valida la existencia de la cuenta en la lista enlazada y que el saldo sea suficiente

![alt text](image-12.png)

Función para la operación individual de transferir, valida la existencia de las cuentas en la lista enlazada y que el saldo sea suficiente

![alt text](image-13.png)

Función para la operación individual de consultar, valida la existencia de la cuenta en la lista enlazada

![alt text](image-14.png)


Función para crear el reporte de estados de cuenta

![alt text](image-17.png)

Función para crear el reporte de carga masiva de usuarios

![alt text](image-16.png)

Función para crear el reporte de carga masiva de operaciones

![alt text](image-15.png)

Función para validar si una cuenta ya existe en el sistema

![alt text](image-19.png)

Función para validar si es un valor numérico o no

![alt text](image-20.png)

Función para insertar en la lista enlazada los registros de nuevos usuarios

![alt text](image-21.png)
