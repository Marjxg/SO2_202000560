# Manual Técnico
Instalación de C
```
sudo apt-get build-essential
```
Compilar archivos
```
gcc nombreArchivo.c -o nombreArchivo
```
Ejecutar programa
```
./nombreArchivo
```
## Parent
Manejo de señal de interrupción
```
volatile sig_atomic_t interrupcion = 0; //Variable para almacenar el estado de la señal
void signalHandler() //Para establecer bandera
signal(SIGINT, signalHandler) //Para manejar la señal Ctrl C
```
Para la creación de procesos hijos
```
pid_t name = fork();
```
Para la ejecución de los ejecutables de los procesos hijos
```
execv()
```
Espera del proceso padre a finalización de procesos hijo
```
waitpid()
```
Ejecución de systemtap
```
sprintf() //Para almacenar en un Char Array el comando
system(commando) //Para ejecutar el comando
```

## Child
**Child es el proceso hijo, encargado de realizar operaciones de lectura, escritura y seek aleatorias con tiempo aleatorios.**
Tiempos aleatorios en 1-3
```
srand(time(NULL) + getpid());
int var = rang() % 3 + 1;
```
Funciones para el manejo de archivos
```
open();
read();
write();
lseek();
```
Manejo de señal de interrupción
```
volatile sig_atomic_t interrupcion = 0; //Variable para almacenar el estado de la señal
void signalHandler() //Para establecer bandera
signal(SIGINT, signalHandler) //Para manejar la señal Ctrl C
```

## System Tap
**Monitoreo del funcionamiento del Sistema Operativo. Interceptar llamadas al sistema realizadas por el proceso hijo.**

Instalacion

```
sudo apt install systemtap linux-headers-$(uname-r)
```

Comando para correr el archivo
```
sudo stap trace.stp
sudo stap trace.stp pid pid
```
Comando para redireccionar la salida hacia el syscalls.log
```
sudo stap trace.stp pid pid > syscalls.log
```
