#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

double *A, *B, *C;
int N, k, l, T;
int block_size;

void * hello_word (void * ptr) ;

void * generar_suma (void * ptr){

    int id = *(int *)ptr; // Convertir el puntero a entero

    int inicio = id * block_size; // Calcular el inicio del bloque
    int fin = inicio + block_size; // Calcular el fin del bloque

    if (id == T - 1 ) { // Si es el último hilo
        fin = N; // Asegurarse de que cubra todo el rango
    }

    for (int i = inicio; i < fin; i++){
        C[i] = A[i] + B[i]; // Realizar la suma de matrices
    }
    
    //printf("\nHilo %d ha terminado de sumar el bloque [%d, %d)", id, inicio, fin);

    pthread_exit((void*)ptr);
    
}

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

int main(int argc,char*argv[]){
    double timetick;

    //Controla los argumentos al programa
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || (N % T != 0))
    {
        printf("\nUsar: %s n t\n  n: Dimension del vector)\n", argv[0]);
        printf(" t: Cantidad de hilos\n");
        printf(" n debe ser divisible por t\n");
        exit(1);
    }
    
    
    pthread_attr_t attr; // Atributos de los hilos
    int *status;
    int ids[T]; // Arreglo de ids de hilos
    pthread_t threads[T]; // Arreglo de hilos
    block_size = N / T; // Tamaño de bloque para cada hilo
    //Reservar espacio en memoria para las matrices 
    A=(double*)malloc(sizeof(double)*N);
    B=(double*)malloc(sizeof(double)*N);
    C=(double*)malloc(sizeof(double)*N);

    for (int i = 0; i < N; i++){
        A[i] = 1.0;
        B[i] = 1.0;
        C[i] = 0.0;
    }

    pthread_attr_init(&attr);
    
    
    /* Creación de los hilos */
    for (int i = 0; i < T; i++){
        ids[i] = i; 
        pthread_create(&threads[i], &attr, generar_suma, &ids[i]);
    }
    
    timetick = dwalltime();

    /*Espera a que los hilos terminen*/
    for (int i = 0; i<T; i++){
        pthread_join(threads[i], (void *) &status);
    }

    double time = dwalltime() - timetick;
    printf("Suma de vectores de dimensión %d. Tiempo en segundos %f\n", N, time);

    //Verificacion del resultado
    int check = 1;
    for (int i = 0; i < N; i++){
        check = check && (C[i] == 2.0);
    }

    if (check){
        printf("Suma de matrices resultado correcto\n");
    } else {
        printf("Suma de matrices resultado erroneo\n");
    }

    free(A);
    free(B);
    free(C);
    return (0);
}

void * hello_word (void * ptr) {
	
	int * p, id;
    p = (int *) ptr;
    id = *p;

	printf("\nHola mundo! Soy el hilo %d.",id);
	
    pthread_exit((void*)ptr);
}

