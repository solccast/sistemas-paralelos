#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h> 

// Declaramos una constante para el tamaño óptimo de bloque
#define TAM_BLOQUE 128

double *A, *B, *C, *BT, *RES_MATRIZ, *CBT, *R; 
int N, T, cantidad_elementos_totales, blocksize; 
double maxA, maxB, minA, minB,promA, promB, sum; 
double escalar; 
pthread_mutex_t variableA;
pthread_mutex_t variableB;
pthread_barrier_t barrier;


double dwalltime();
void * multiplicacion_bloques (double *a, double *b, double *c, int id,int inicio, int fin);
void * calcular_formula (void * ptr);
void blkmul(double *ablk, double *bblk, double *cblk);
void verificarResultado(double *M, int N);

int main(int argc, char *argv[]){

  //Validación de argumentos
  if (argc != 3) {
    printf("Uso: %s <N> <T>\n", argv[0]);
    return 1;
  }
  
  N = atoi(argv[1]);
  T = atoi(argv[2]);
  
  if (N <= 0 || T <= 0) {
    printf("Los argumentos deben ser mayor a cero.\n");
    return 1;
  }
  
  if (N % T != 0) {
    printf("El tamaño de la matriz no es divisible por el número de hilos.\n");
        return 1;
    }
    
    cantidad_elementos_totales = N * N; // Cantidad total de elementos en la matriz
    blocksize = N / T; // Tamaño del bloque para cada hilo
    int i, j;
    double timetick; 
    pthread_attr_t attr; // Atributos de los hilos
    int *status;
    int ids[T]; // Identificadores de los hilos
    pthread_t threads[T]; // Identificadores de los hilos
    pthread_mutex_init(&variableA, NULL); // Inicializar el mutex
    pthread_mutex_init(&variableB, NULL); // Inicializar el mutex
    pthread_barrier_init(&barrier, NULL, T); // Inicializar la barrera
    
    // Reservar memoria para las matrices
    A = (double *)malloc(N * N * sizeof(double));
    B = (double *)malloc(N * N * sizeof(double));
    C = (double *)malloc(N * N * sizeof(double));
    BT = (double *)malloc(N * N * sizeof(double));
    RES_MATRIZ = (double *)malloc(N * N * sizeof(double));
    CBT = (double *)malloc(N * N * sizeof(double));
    R = (double *)malloc(N * N * sizeof(double)); 
    
    // Inicializar matrices A, B y C
    for (i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i * N + j] = 1.0; // Inicializar la matriz A por filas
        BT[i*N + j] = 0.0; // Inicialización para que no quede basura
        C[i * N + j] = 1.0; // Inicializar la matriz C por filas
        RES_MATRIZ[i*N + j] = 0.0; // Inicializar la matriz resultante para que no quede basura
        CBT[i*N + j] = 0.0; // Inicializar la matriz resultado de C*B^T para que no quede basura
        R[i * N + j] = 0.0; // Inicializar la matriz resultante para que no quede basura
      }
    }

    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        B[j*N + i] = 1.0; // Inicializar la matriz B por columnas
      }
    }
    
    // Inicializar variables 
    minA = A[0];
    maxA = A[0];
    promA = 0.0;
    minB = B[0];
    maxB = B[0];
    promB = 0.0;
    
    pthread_attr_init(&attr);
    

    timetick = dwalltime();
    /*Creación de los hilos*/
    for (i = 0; i < T; i++){
      ids[i] = i;
      pthread_create(&threads[i], &attr, calcular_formula, &ids[i]);
    }
    
    for (i = 0; i < T; i++){
      pthread_join(threads[i], (void *) &status);
    }
    
    double workTime = dwalltime() - timetick;
    
    printf("\nMultiplicacion de matrices de %dx%d con %d hilos. Tiempo en segundos %f\n", N, N, T,workTime);
    
    verificarResultado(R, N); // Verificar el resultado de la multiplicación de matrices
    
    free(A);
    free(B);
    free(C);
    free(BT);
    free(RES_MATRIZ);
    free(R);
    free(CBT);
    pthread_mutex_destroy(&variableA); 
    pthread_mutex_destroy(&variableB);
    pthread_barrier_destroy(&barrier);
    return 0;
  }
  
void * calcular_formula (void * ptr) {
    int * p, id;
    p = (int *) ptr;
    id = *p;
    // Calculo del inicio y fin del bloque que le corresponde al hilo
    int inicio = id * blocksize;
    int fin = inicio + blocksize; // Se define el bloque de filas que le corresponde al hilo
    int i, j, k;
    double valorActual; 
    
    //Búsqueda de mínimo, máximo y promedio de A
    double local_minA = A[inicio];
    double local_maxA = A[inicio];
    double local_sumA = 0.0;
        
    for (i = inicio; i < fin; i++) {
      for (j = 0; j < N; j++) {
        valorActual = A[i * N + j];
        local_sumA += valorActual;
        if (valorActual < local_minA) {
          local_minA = valorActual;
        }
        if (valorActual > local_maxA) {
          local_maxA = valorActual;
        }
      }
    } 

    // Actualizar los valores globales de A usando mutex
    pthread_mutex_lock(&variableA);
    if (local_minA < minA) {
      minA = local_minA;
    }
    if (local_maxA > maxA) {
      maxA = local_maxA;
    }
    promA += local_sumA; // Sumar el promedio local al global
    pthread_mutex_unlock(&variableA);
    
    
    //Búsqueda de mínimo, máximo y promedio de B    
    double local_minB = B[inicio];
    double local_maxB = B[inicio];
    double local_sumB = 0.0;
    
    for (i = inicio; i < fin; i++) {
      for (j = 0; j < N; j++) {    
        valorActual = B[i * N + j];
        local_sumB += valorActual;
        if (valorActual < local_minB) {
          local_minB = valorActual;
        }
        if (valorActual > local_maxB) {
          local_maxB = valorActual;    
        }
      }
    } 
      
    
    // Actualizar los valores globales de B usando mutex
    pthread_mutex_lock(&variableB);
    if (local_minB < minB) {
      minB = local_minB;
    }
    if (local_maxB > maxB) {
      maxB = local_maxB;
    }
    promB += local_sumB; // Sumar el promedio local al global
    pthread_mutex_unlock(&variableB);
    
    // Multiplicación de matrices
    multiplicacion_bloques(A, B, RES_MATRIZ, id, inicio, fin); // Se llama a la función de multiplicación de matrices
    
    // Trasposición de la matriz B
    for (i = inicio; i < fin; i++)
    {
      int desplazamiento_i = i*N;
      for (j = 0; j < N; j++)
      {
        BT[j * N + i] = B[desplazamiento_i + j]; // Trasponemos la matriz B
      }
    }
    
    pthread_barrier_wait(&barrier); // Para que los hilos calculen la multiplicación con BT ya traspuesta 
    
    if (id == 0) //Solo un hilo se ocupa de hacer el cálculo del escalar
    {
      promA = promA / (cantidad_elementos_totales);
      promB = promB / (cantidad_elementos_totales);
      escalar = (maxA * maxB - minA * minB) / (promA * promB);
    } 
    
    multiplicacion_bloques(C, BT, CBT, id, inicio, fin); // Se llama a la función de multiplicación de matrices 
    
    //Realizamos la suma de matrices y multiplicamos por el escalar
    for (i = inicio; i < fin; i++)
    {
      int desplazamiento_i = i*N;
      for (j = 0; j < N; j++)
      {
        R[desplazamiento_i + j] = (escalar * RES_MATRIZ[desplazamiento_i + j]) + CBT[desplazamiento_i + j];
      }
    }
    
    pthread_exit((void*)ptr);
  }    
  
  void * multiplicacion_bloques (double *a, double *b, double *c, int id, int inicio, int fin) {
    int i, j, k, desplazamiento_i, desplazamiento_j;

    for (i = inicio; i < fin; i += TAM_BLOQUE)
    {
      desplazamiento_i = i * N;
      for (j = 0; j < N; j += TAM_BLOQUE)
      {
        desplazamiento_j = j * N;
        for (k = 0; k < N; k += TAM_BLOQUE)
        {
          blkmul(&a[desplazamiento_i + k], &b[desplazamiento_j + k], &c[desplazamiento_i + j]);
        }
      }
    }
  }   
  
  /* Multiply (block)submatrices */
  void blkmul(double *ablk, double *bblk, double *cblk) // Variables:
  {
    int i, j, k, desplazamiento_i, desplazamiento_j;    /* Guess what... again... */
    double suma;
    for (i = 0; i < TAM_BLOQUE; i++) // Recorre la fila i
    {
      desplazamiento_i = i * N; 
      for (j = 0; j < TAM_BLOQUE; j++) //Recorre la columna j
      {
        desplazamiento_j = j * N;
        suma = 0.0; // Inicializa la suma en cero
        for  (k = 0; k < TAM_BLOQUE; k++) //Recorre cada uno de los cosos 
        {
          suma += ablk[desplazamiento_i + k] * bblk[desplazamiento_j + k]; // Setea el resultado de la fila A[i] por la columna B[j]
        }
        cblk[desplazamiento_i + j] += suma; // Asigna el resultado de la multiplicación a la matriz resultante  
      }
    }
  }
  
  void verificarResultado(double *R, int N) {
    int check=1;
    int i, j;
    
    /* Check results (just in case...) */
    for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
      {
        if (R[i*N + j] != N)
        {
          //printf("Error at %d, %d, value: %f\n", i, j, R[i*N + j]);
          check = 0; // Error encontrado
        }
      }
    }
    
    if (check){
      printf("Multiplicacion de matrices resultado correcto\n");
    } else {
      printf("Multiplicacion de matrices resultado incorrecto\n");
    }
  }
  
  //Para calcular tiempo
  double dwalltime(){
      double sec;
      struct timeval tv;
  
      gettimeofday(&tv,NULL);
      sec = tv.tv_sec + tv.tv_usec/1000000.0;
      return sec;
  }