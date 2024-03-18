#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  
#include <sys/wait.h>

void* myFunction(void *arg)
{
    int n = *(int*)arg; 

    static int x = 10; 

    int y = x; 

    printf("Hilo %d: x = %d, y = %d\n", n, x, y);


    if (x != y) {
        printf("Hilo %d: Error! x != y! %d != %d\n", n, x, y);
    }

    x++;  

    return 0;
}

#define THREAD_COUNT 5

int main(void)
{
    pthread_t thread[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {

        int *a = malloc(sizeof (int)); 
        *a = i;
        pthread_create(&thread[i], NULL, myFunction, (void *) a);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(thread[i], NULL);
    }
}