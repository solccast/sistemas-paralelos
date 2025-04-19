#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h> 
#include<semaphore.h>

double *A; 
int N, T;
int blocksize; 
double min, max, promedio; 
sem_t sem_min;
sem_t sem_max;
sem_t sem_promedio; 

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
        //A[i] = 1.0;
        A[i] = (double)i;
    }

    pthread_attr_t attr; // Atributos de los hilos
    int *status;
    int ids[T]; // Arreglo de ids de hilos
    pthread_t threads[T]; // Arreglo de hilos
    blocksize = N / T; // Tamaño de bloque para cada hilo
    min = A[0]; // Inicializa min con el primer elemento del vector
    max = A[0]; // Inicializa max con el primer elemento del vector
    promedio = 0.0; // Inicializa promedio en 0

    sem_init(&sem_min, 0, 1); // Se inicializa el semáforo en 1 (disponible)
    sem_init(&sem_max, 0, 1); // Se inicializa el semáforo en 1 (disponible)
    sem_init(&sem_promedio, 0, 1); // Se inicializa el semáforo en 1 (disponible)


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

    promedio = promedio / N; // Calcula el promedio total
    
    timetick = dwalltime() - timetick;
    
    printf("--------------------------\n");
    printf("Tiempo de ejecución: %f segundos\n", timetick);
    printf("Valor mínimo: %f\n", min);
    printf("Valor máximo: %f\n", max);
    printf("Promedio: %f\n", promedio);
    printf("--------------------------\n");

    // Verifica el resultado
    int check = 1; 
    for (int i = 0; i < N; i++)
        check = check && (max == (double)(N - 1)) && (min == 0.0) && (promedio == (double)(N - 1) / 2);

    if (check)
    {
        printf("Calculo correcto\n");
    }
    else
    {
        printf("Calculo erroneo\n");
    }
    
    free(A);
    sem_destroy(&sem_min); 
    sem_destroy(&sem_max); 
    sem_destroy(&sem_promedio); 


    return 0;
}

void * analizar_vector (void * ptr){
    int id = *(int *)ptr; // ID del hilo
    int i; 
    int inicio = id * blocksize; // Índice de inicio del bloque
    int fin = inicio + blocksize; // Índice de fin del bloque
    double min_local = A[inicio]; // Inicializa min_local con el primer elemento del bloque
    double max_local = A[inicio]; // Inicializa max_local con el primer elemento del bloque
    double suma_local = 0.0; // Inicializa suma_local en 0

    for (i = inicio; i < fin; i++){
        if (A[i] < min_local) {
            min_local = A[i]; // Actualiza min_local si es menor
        }
        if (A[i] > max_local) {
            max_local = A[i]; // Actualiza max_local si es mayor
        }
        suma_local += A[i]; // Suma los elementos del bloque
    }

    // Sección crítica para actualizar min, max y promedio  
    sem_wait(&sem_min); 
    if (min_local < min) {
        min = min_local; // Actualiza min si es menor
    }
    sem_post(&sem_min);

    sem_wait(&sem_max);
    if (max_local > max) {
        max = max_local; // Actualiza max si es mayor
    }
    sem_post(&sem_max);

    sem_wait(&sem_promedio);
    promedio += suma_local; // Suma los resultados locales al promedio
    sem_post(&sem_promedio);
     
    pthread_exit((void*)ptr);
}