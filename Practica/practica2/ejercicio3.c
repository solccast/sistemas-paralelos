#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

double *A; 
int ocurrencias; 
int N, T, block_size;
double valor = 1.0; // Valor a buscar en el vector
pthread_mutex_t mutex; // Mutex para proteger la variable compartida

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

    //Controla los argumentos al programa
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || (N % T != 0))
    {
        printf("\nUsar: %s n t\n  n: Dimension del vector)\n", argv[0]);
        printf(" t: Cantidad de hilos\n");
        printf(" n debe ser divisible por t\n");
        exit(1);
    }

    ocurrencias = 0; 
    //Reserva de espacio
    A=(double*)malloc(sizeof(double)*N);

    //Inicializar vector 
    for (int i = 0; i < N; i++){
        A[i] = 1.0;
    }

    //Atributos de los hilos
    pthread_mutex_init(&mutex, NULL); // Inicializar el mutex
    pthread_attr_t attr;
    int *status;
    int ids[T]; // Arreglo de ids de hilos
    pthread_t threads[T]; // Arreglo de hilos
    block_size = N / T; // Tamaño de bloque para cada hilo
    pthread_attr_init(&attr);

    //Creación de los hilos
    for (int i = 0; i < T; i++){
        ids[i] = i; 
        pthread_create(&threads[i], &attr, ocurrencias_vector, &ids[i]);
    }

    timetick = dwalltime();

    /* Espera a que los hilos terminen */
    for (int i = 0; i<T; i++){
        pthread_join(threads[i], (void *) &status);
    }

    timetick = dwalltime() - timetick;
    printf("Tiempo de ejecución: %f segundos\n", timetick);

    /* Verificacion del resultado */
    if (ocurrencias == N){
        printf("El resultado es correcto. Ocurrencias vale: %i\n", ocurrencias);
        printf("El vector es de tamaño %d y tiene %d ocurrencias de %f\n", N, (int)ocurrencias, valor);
    } else {
        printf("El resultado es incorrecto\n");
    }
    
    return 0;
}

void * ocurrencias_vector (void *ptr){
    int id = *(int *)ptr; // Convertir el puntero a entero
    int i;
    int inicio = id * block_size; // Calcular el inicio del bloque
    int fin = inicio + block_size; // Calcular el fin del bloque
    int contador = 0;

    for (i = inicio; i < fin; i++){
        if (A[i] == valor){ // Contar ocurrencias de 1.0
            contador++;
        }
    }
    
    if (contador > 0){
        pthread_mutex_lock(&mutex); // Bloquear el mutex antes de modificar la variable compartida
        ocurrencias += contador;
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex después de modificar la variable compartida
    }


    pthread_exit((void*)ptr); // Se termina el hilo
}