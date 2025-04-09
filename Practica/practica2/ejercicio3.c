#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

double *A; 

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

void * ocurrencias_vector (void *ptr);

int main (int argc,char*argv[]){
    double timetick;

    //Reserva de espacio
    A=(double*)malloc(sizeof(double)*N);

    //Inicializar vector 
    for (int i = 0; i < N; i++){
        A[i] = 1.0;
    }

    
    return 0;
}

void * ocurrencias_vector (void *ptr){
    int id = *(int *)ptr; // Convertir el puntero a entero
 
    pthread_exit((void*)ptr); // Se termina el hilo
}