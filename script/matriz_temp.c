#include<stdio.h>
#include<stdlib.h>

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

void initvalmatfila(double *mat, int n, double val); // Inicializa la matriz mat de nxn con el valor val por fila
void initvalmatcolumna(double *mat, int n, double val); // Inicializa la matriz mat de nxn con el valor val por columna
void printMatrixByRows(double *mat, int n); // Imprime la matriz mat de nxn por filas
void printMatrixByColumns(double *mat, int n); // Imprime la matriz mat de nxn por columnas

void multiplicacion_matrices(double *mat_a, double *mat_b, double *mat_res, int n, int transpose); // Multiplica las matrices mat_a y mat_b, si transpose es 1, transponemos la matriz B
void analizar_matriz(double *mat_a, double *min_a, double *max_a, double *promedio, int n); // Busca los valores minimo y maximo de la matriz mat_a de nxn
void multiplicar_matriz_por_escalar(double *mat, double escalar, double *mat_res, int n); // Multiplica la matriz mat de nxn por el escalar escalar
void suma_matrices(double *mat_a, double *mat_b, double *mat_res, int n); // Suma las matrices mat_a y mat_b de nxn, el resultado se almacena en mat_res

int main (int argc,char*argv[]){

    double *A, *B, *C; //Matrices cuadradas de NxN con elementos de tipo double
    double *R; //Matriz resultado de la multiplicacion de todas las matrices 
    double *RES_PARCIAL; //Matriz resultado de la multiplicacion de A y B
    double *RES_PRIMERA_PARTE_FORM;
    double *RES_PARCIAL_T; //Matriz resultado de la multiplicacion de C y B transpuesta
    int n; //Tamanio de las matrices
    int bs; //Tamanio del bloque
    int i; //Contador 
    double timetick;
    double minA, minB, maxA, maxB, promA, promB;  
    double escalar_parcial; 

    /* Check command line parameters */
     //Controla los argumentos al programa
    if ((argc != 2) || ((n = atoi(argv[1])) <= 0) )
    {
        printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
        exit(1);
    }

    // Reservamos memoria para las matrices
    A = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para A
    B = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para B
    C = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para C
    R = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para R
    RES_PARCIAL = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para RES_PARCIAL
    RES_PARCIAL_T = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para RES_PARCIAL_T
    RES_PRIMERA_PARTE_FORM = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para RES_PRIMERA_PARTE_FORM

    // Inicializamos las matrices A y C por fila
    initvalmatfila(A, n, 1.0); //Inicializamos A por fila
    initvalmatcolumna(B, n, 1.0); //Inicializamos B por columna
    initvalmatfila(C, n, 1.0); //Inicializamos C por fila
    initvalmatfila(RES_PRIMERA_PARTE_FORM, n, 0.0);

    //printMatrixByRows(A, n); //Imprimimos la matriz A por filas
    //printMatrixByColumns(B, n); //Imprimimos la matriz B por columnas

    timetick = dwalltime(); //Guardamos el tiempo de inicio 

    analizar_matriz(A, &minA, &maxA, &promA, n);
    analizar_matriz(B, &minB, &maxB, &promB, n);
    multiplicacion_matrices(A, B, RES_PARCIAL, n, 0); //Multiplicamos A y B, el resultado se almacena en C
    // printf("Estos son los extremos y promedios de las matrices: \n"
    //        "A: min=%.1f, max=%.1f, prom=%.1f; "
    //        "B: min=%.1f, max=%.1f, prom=%.1f\n",
    //        minA, maxA, promA, minB, maxB, promB);
    //printMatrixByRows(RES_PARCIAL, n); //Imprimimos la matriz resultado de la multiplicacion de A y B por filas
    
    //Primera parte de la fórmula:
    //printf("maxA: %.2f, maxB: %.2f, minA: %.2f, minB: %.2f\n", maxA, maxB, minA, minB);

    double num = (maxA * maxB) - (minA * minB);
    double den = promA * promB;
    escalar_parcial = num / den; //Calculamos el escalar parcial

    multiplicar_matriz_por_escalar(RES_PARCIAL, escalar_parcial, RES_PRIMERA_PARTE_FORM, n); //Multiplicamos la matriz RES_PARCIAL por el escalar parcial
    //printMatrixByRows(RES_PRIMERA_PARTE_FORM, n); //Imprimimos la matriz resultado de la multiplicacion de A y B por filas

    //Segunda parte de la fórmula
    // printf("\nMatriz C:\n");
    // printMatrixByRows(C, n); //Imprimimos la matriz C por filas
    multiplicacion_matrices(C, B, RES_PARCIAL_T, n, 1); //Multiplicamos C y B transpuesta, el resultado se almacena en RES_PARCIAL_T
    // printf("Matriz RESULTADO PARCIAL SEGUNDA PARTE:\n");  
    // printMatrixByRows(RES_PARCIAL_T, n); 

    // //última parte de la fórmula
    suma_matrices(RES_PRIMERA_PARTE_FORM, RES_PARCIAL_T, R, n); //Sumamos la matriz RES_PRIMERA_PARTE_FORM y RES_PARCIAL_T, el resultado se almacena en R
    timetick = dwalltime() - timetick; //Guardamos el tiempo de fin 

    //printf("Matriz resultado:\n");
    printMatrixByRows(R, n); //Imprimimos la matriz resultado por filas

    printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",n,n, timetick); //Imprimimos el tiempo de la multiplicacion de matrices

    //Liberar memoria
    free(A);
    free(B);
    free(C);
    free(R);
    free(RES_PARCIAL);
    free(RES_PARCIAL_T);
    free(RES_PRIMERA_PARTE_FORM);
    return (0);
}

void initvalmatfila(double *mat, int n, double val){
    int i, j;
    int count = 0; 
    for (i = 0; i<n; i++){
        for (j = 0; j<n; j++){
            mat[i*n + j] = count++;
        }
    }
}

void initvalmatcolumna(double *mat, int n, double val){
    int i, j; 
    int count = 0; 
    for (i = 0; i<n; i++){
        for (j = 0; j<n; j++){
            mat[j+i*n] = count++;
        }
    }
}

void printMatrixByRows(double *mat, int n){
    printf("\nMatriz en memoria (orden fila):\n");
    for (int k = 0; k < n * n; k++) {
        printf("%.1f ", mat[k]);
    }
    printf("\n\nMatriz visual:\n");
    for (int l = 0; l < n; l++) {
        for (int z = 0; z < n; z++) {
            printf("%.1f ", mat[l * n + z]);
        }
        printf("\n");
    }
}

void printMatrixByColumns(double *mat, int n){
    printf("\nMatriz en memoria (orden columna):\n");
    for (int k = 0; k < n * n; k++) {
        printf("%.1f ", mat[k]);
    }
    printf("\n\nMatriz visual:\n");
    for (int l = 0; l < n; l++) {
        for (int z = 0; z < n; z++) {
            printf("%.1f ", mat[l + z * n]);
        }
        printf("\n");
    }
}

void multiplicacion_matrices(double *mat_a, double *mat_b, double *mat_res, int n, int transpose){
    for (int l = 0; l < n; l++){
        for (int z = 0; z < n; z++){
            mat_res[l + z * n] = 0.0; // Inicializamos la matriz resultado en 0.0
        }
    }
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            double sum = 0.0;
            for (int k = 0; k < n; k++){
                if (transpose == 1){
                    //printf("Multiplicando: %.1f * %.1f\n", mat_a[i*n + k], mat_b[k + j*n]);
                    sum += mat_a[i*n + k] * mat_b[k*n + j]; // Multiplicamos la fila i de A por la columna j de B transpuesta
                } else {
                    //printf("Multiplicando: %.1f * %.1f\n", mat_a[i*n + k], mat_b[k*n + j]);
                    sum += mat_a[i*n + k] * mat_b[j*n + k]; 
                }
            }
            printf("Suma parcial: %.1f\n", sum);
            mat_res[i*n + j] = sum; // mat_res[i][j] = sum, mat_res se encuentra ordenada por filas
        }
    }
}

void analizar_matriz(double *mat_a, double *min_a, double *max_a, double *promedio, int n){
    double aux_min_a = mat_a[0]; // Inicializamos el minimo en el primer elemento de la matriz
    double aux_max_a = mat_a[0]; // Inicializamos el maximo en el primer elemento de la matriz
    double aux_promedio = 0; 

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            aux_promedio += mat_a[i*n+j]; 
            if (mat_a[i*n + j] < aux_min_a){
                aux_min_a = mat_a[i*n + j]; // Si encontramos un nuevo minimo, lo actualizamos
            }
            if (mat_a[i*n + j] > aux_max_a){
                aux_max_a = mat_a[i*n + j]; // Si encontramos un nuevo maximo, lo actualizamos
            }
        }
    }
    
    // Calculamos el promedio y lo guardamos en el puntero promedio
    *promedio = aux_promedio / (n*n); // Promedio de la matriz
    *min_a = aux_min_a; // Guardamos el minimo encontrado
    *max_a = aux_max_a; // Guardamos el maximo encontrado
    printf("Minimo: %.1f\n", aux_min_a); // Imprimimos el minimo encontrado
    printf("Maximo: %.1f\n", aux_max_a); // Imprimimos el maximo encontrado
    printf("Promedio: %.1f\n", *promedio);
}

void multiplicar_matriz_por_escalar(double *mat, double escalar, double *mat_res, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            mat_res[i*n + j] = mat[i*n+j] * escalar; // Multiplicamos cada elemento de la matriz por el escalar
        }
    }
}

void suma_matrices(double *mat_a, double *mat_b, double *mat_res, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            mat_res[i*n + j] = mat_a[i*n+j] + mat_b[i*n+j]; // Sumamos cada elemento de las matrices
        }
    }
  
}

// Fin del programa