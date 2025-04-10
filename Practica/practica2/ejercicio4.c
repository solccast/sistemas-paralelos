#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h> 

double *A; 
int N, T;
int blocksize; 
double min, max; 


void * analizar_vector (void * ptr); 

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

int main(int argc, char*argv[]){

    //Controla los argumentos al programa
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || (N % T != 0))
    {
        printf("\nUsar: %s n t\n  n: Dimension del vector)\n", argv[0]);
        printf(" t: Cantidad de hilos\n");
        printf(" n debe ser divisible por t\n");
        exit(1);
    }

    double timetick; 

    //Reserva de espacio para A 
    A = (double*)malloc(sizeof(double)*N);

    //Inicializar el vector A 
    for (int i = 0; i < N; i++){
        A[i] = 1.0;
    }

    pthread_attr_t attr; // Atributos de los hilos
    int *status;
    int ids[T]; // Arreglo de ids de hilos
    pthread_t threads[T]; // Arreglo de hilos
    blocksize = N / T; // Tamaño de bloque para cada hilo

    pthread_attr_init(&attr);

    //Creación de los hilos
    for (int i = 0; i < T; i++){
        ids[i] = i; 
        pthread_create(&threads[i], &attr, analizar_vector, &ids[i]);
    }

    timetick = dwalltime();

    //Espera a que los hilos terminen
    for (int i = 0; i<T; i++){
        pthread_join(threads[i], (void *) &status);
    }

    timetick = dwalltime() - timetick;
    printf("Tiempo de ejecución: %f segundos\n", timetick);

    
    free(A);
    return 0;
}

void * analizar_vector (void * ptr){
    int id = *(int *)ptr; // ID del hilo

    printf("Hilo %d: ", id);

    pthread_exit((void*)ptr);
}