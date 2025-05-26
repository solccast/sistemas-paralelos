#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <omp.h>

#define tam_bloque 128 // Tamaño del bloque óptimo

double dwalltime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void verificarResultado(double *R, int N);

int main(int argc, char *argv[]) {
    double *A, *B, *C, *BT; //Matrices cuadradas de NxN con elementos de tipo double
    double *R; //Matriz resultado
    double *RES_PARCIAL; //Matriz resultado de la multiplicacion de A y B
    double *RES_PARCIAL_T; //Matriz resultado de la multiplicacion de C y B transpuesta
    int i, j, k, sub_i, sub_j, sub_k, desplazamiento_i, desplazamiento_j; //Índices auxiliares para recorrer las matrices 
    double minA, minB, maxA, maxB, promA, promB;
    double escalar = 0.0; 
    int cantidad_elementos_totales;
    double valorA, valorB; // Variables para almacenar los valores actuales de A y B

    // Verificar argumentos de entrada
    if (argc != 3) {
        printf("Uso: %s <Tamaño N> <Cantidad de hilos T>\n", argv[0]);
        return 1;
    }
    //extraer argumentos de entrada
    int N = atoi(argv[1]);
    int T = atoi(argv[2]);
    if (N <= 0 || T <= 0 || N % tam_bloque != 0) {
        printf("Error: N debe ser positivo y divisible por el tamaño de bloque (%d)\n", tam_bloque);
        return 1;
    }

    // Alocación de memoria
    A = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para A
    B = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para B
    C = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para C
    R = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para R
    BT = (double *) malloc(N*N*sizeof(double)); 
    RES_PARCIAL = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para RES_PARCIAL
    RES_PARCIAL_T = (double *) malloc(N*N*sizeof(double)); //Reservamos memoria para RES_PARCIAL_T

    cantidad_elementos_totales = N * N; 

    // Inicializamos las matrices A, B y C
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i*N + j] = 1.0; // Inicializamos A por fila
            B[j*N + i] = 1.0; // Inicializamos B por columna
            C[i*N + j] = 1.0; // Inicializamos C por fila
            RES_PARCIAL[i*N + j] = 0.0; // Matriz donde se almacenará la primera parte de la fórmula 
            RES_PARCIAL_T[i*N + j] = 0.0; // Matriz donde se almacenará la segunda parte de la fórmula
            R[i*N + j] = 0.0; // Matriz resultado
        }
    
    minA = A[0]; // Inicializamos el minimo en el primer elemento de la matriz
    maxA = A[0]; // Inicializamos el maximo en el primer elemento de la matriz
    promA = 0; 
    minB = B[0]; // Inicializamos el minimo en el primer elemento de la matriz
    maxB = B[0]; // Inicializamos el maximo en el primer elemento de la matriz
    promB = 0.0;

    double tiempo = dwalltime();

    omp_set_num_threads(T); // Establecemos la cantidad de hilos a utilizar

    #pragma omp parallel private(i, j, k, sub_i, sub_j, sub_k, desplazamiento_i, desplazamiento_j, valorA, valorB)
    {
        // Cálculo de min, max y  prom de A 
        #pragma omp for nowait schedule(static) reduction(min:minA) reduction(max:maxA) reduction(+:promA) //nowait para que sigan ejecutando los hilos las estadisticas de B sin esperar a que terminen todos
        for (i = 0; i < N; i++) {
            desplazamiento_i = i * N;
            for (j = 0; j < N; j++) {
                valorA = A[desplazamiento_i + j];

                if (valorA < minA) 
                    minA = valorA;
                if (valorA > maxA) 
                    maxA = valorA;
                promA += valorA;
           }
        } 
        //Calculo de min max y prom de B
        #pragma omp for schedule(static) reduction(min:minB) reduction(max:maxB) reduction(+:promB)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                valorB = B[j * N + i];

                if (valorB < minB) 
                    minB = valorB;
                if (valorB > maxB) 
                    maxB = valorB;
                promB += valorB;
            }
       }

        // Escalar
        #pragma omp single
        {
            promA /= (cantidad_elementos_totales);
            promB /= (cantidad_elementos_totales);
            escalar = ((maxA * maxB) - (minA * minB)) / (promA * promB);
        }

        // Trasposición de la matriz B a la matriz BT
        #pragma omp for schedule(static) //collapse(2)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                BT[j * N + i] = B[i * N + j];
            }
        }

        // Multiplicación A * B -> RES_PARCIAL
        #pragma omp for nowait schedule(static) //collapse(3)//nowait para que sigan ejecutando los hilos la sigiente multiplicación sin esperar a que terminen todos
        for (i = 0; i < N; i+= tam_bloque){
            for (j = 0; j < N; j+= tam_bloque){
                for (k = 0; k < N; k+= tam_bloque){
                    //Recorrido submatrices 
                    for (sub_i = i; sub_i < i + tam_bloque; sub_i++){
                        desplazamiento_i = sub_i*N; 
                        for (sub_j = j; sub_j < j + tam_bloque; sub_j++){
                            desplazamiento_j = sub_j * N;
                            double sumaParcial = 0.0; 
                            for (sub_k = k; sub_k < k + tam_bloque; sub_k++){
                                sumaParcial += A[desplazamiento_i + sub_k] * B[desplazamiento_j + sub_k];
                            }
                            RES_PARCIAL[desplazamiento_i + sub_j] += sumaParcial; 
                        }
                    }
                } 
            }
       }    

        // Multiplicación C * BT -> RES_PARCIAL_T
        #pragma omp for schedule(static) //collapse(3)//Sin nowait porque se necesita que todos los hilos terminen para sumar los resultados
        for (i = 0; i < N; i+= tam_bloque){
            for (j = 0; j < N; j+= tam_bloque){
                for (k = 0; k < N; k+= tam_bloque){
                    //Recorrido submatrices 
                    for (sub_i = i; sub_i < i + tam_bloque; sub_i++){
                        desplazamiento_i = sub_i*N; 
                        for (sub_j = j; sub_j < j + tam_bloque; sub_j++){
                            double sumaParcial = 0.0; 
                            desplazamiento_j = sub_j * N;
                            for (sub_k = k; sub_k < k + tam_bloque; sub_k++){
                                sumaParcial += C[desplazamiento_i + sub_k] * BT[desplazamiento_j + sub_k];
                            }
                            RES_PARCIAL_T[desplazamiento_i + sub_j] += sumaParcial;  
                        }
                    }
                } 
            }
        }

        // Cálculo final R = escalar * RES_PARCIAL + RES_PARCIAL_T
        #pragma omp for schedule(static)
        for (int i = 0; i < cantidad_elementos_totales; i++) {
            R[i] =(escalar * RES_PARCIAL[i]) + RES_PARCIAL_T[i];
        }
    }

    tiempo = dwalltime() - tiempo;

    printf("Multiplicación %dx%d con %d hilos. Tiempo de procesamiento: %.6f segundos\n", N, N, T, tiempo);

    verificarResultado(R, N);

    free(A); 
    free(B); 
    free(BT); 
    free(C); 
    free(R); 
    free(RES_PARCIAL); 
    free(RES_PARCIAL_T);
    return 0;
}

void verificarResultado(double *R, int N) {
    int check = 1;
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (R[i*N + j] != N)
            {
                printf("Error at %d, %d, value: %f\n", i, j, R[i*N + j]);                check = 0; // Error encontrado
            }
        }
    }
    
    if (check){
        printf("Multiplicacion de matrices resultado correcto\n");
    }
}
// Fin del programa