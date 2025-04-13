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

void verificarResultado(double *M, int N);

int main (int argc,char*argv[]){

    double *A, *B, *C, *BT; //Matrices cuadradas de NxN con elementos de tipo double
    double *R; //Matriz resultado 
    double *RES_PARCIAL; //Matriz resultado de la multiplicacion de A y B
    double *RES_PARCIAL_T; //Matriz resultado de la multiplicacion de C y B transpuesta
    int i, j, k, sub_i, sub_j, sub_k; //Índices auxiliares que nos ayudarán a recorrer las matrices 
    double timetick;
    double minA, minB, maxA, maxB, promA, promB;  
    double den, num;
    double escalar = 0.0; 
    double valorActual; 
    int cantidad_elementos_totales;
    int desplazamiento_i, desplazamiento_j;


    if (argc < 3) {
        printf("Uso: %s <n> <tam_bloque>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int tam_bloque = atoi(argv[2]);

    if (n <= 0 || tam_bloque <= 0) {
        printf("Error: Los valores de n y bs deben ser mayores que 0\n");
        return 1;
    }

    // Verifica que bs sea divisor de n
    if (n % tam_bloque != 0) {
        printf("Error: bs debe ser un divisor exacto de n\n");
        return 1;
    }

    printf("Parámetros recibidos correctamente: n=%d, bs=%d\n", n, tam_bloque);

    // Reservamos memoria para las matrices
    A = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para A
    B = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para B
    C = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para C
    R = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para R
    RES_PARCIAL = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para RES_PARCIAL
    RES_PARCIAL_T = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para RES_PARCIAL_T
    BT = (double *) malloc(n*n*sizeof(double)); //Reservamos memoria para BT

    cantidad_elementos_totales = n * n; 

    double count = 0.0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            A[i*n + j] = count; // Inicializamos A por fila
            B[j*n + i] = count; // Inicializamos B por columna
            C[i*n + j] = count; // Inicializamos C por fila
            RES_PARCIAL[i*n + j]= 0.0; //Matriz donde se almacenará la primera parte de la fórmula 
            RES_PARCIAL_T[i*n + j]= 0.0; //Matriz donde se almacenará la segunda parte de la fórmula
            R[i*n + j] = 0.0; //Matriz resultado
            count+= 1.0; //Incrementamos el valor de count para que cada elemento de la matriz sea diferente
        }
    }

    timetick = dwalltime(); //Inicio del conteo 

    //---- Calculo del conteo del tiempo de ejecución ----- //

    // Recorrido de la matriz A
    minA = A[0]; // Inicializamos el minimo en el primer elemento de la matriz
    maxA = B[0]; // Inicializamos el maximo en el primer elemento de la matriz
    promA = 0; 

    //Posteriormente usaremos valorActual para evaluar la eficiencia 

    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            valorActual = A[i*n+j]; 
            promA += valorActual; 
            if (valorActual < minA){
                minA = valorActual; // Si encontramos un nuevo minimo, lo actualizamos
            }
            if (valorActual > maxA){
                maxA = valorActual; // Si encontramos un nuevo maximo, lo actualizamos
            }
        }
    }

    // Calculo del promedio final de la matriz A
    promA = promA / cantidad_elementos_totales; 


    // Recorrido de la matriz B
    minB = B[0]; // Inicializamos el minimo en el primer elemento de la matriz
    maxB = B[0]; // Inicializamos el maximo en el primer elemento de la matriz
    promB = 0.0;

    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            valorActual = B[j*n+i];
            promB += valorActual; // Acumulamos el promedio de la matriz B
            if (valorActual < minB){
                minB = valorActual; // Si encontramos un nuevo minimo, lo actualizamos
            }
            if (valorActual > maxB){
                maxB = valorActual; // Si encontramos un nuevo maximo, lo actualizamos
            }
        }
    }

    promB = promB / cantidad_elementos_totales; // Calculo del promedio final de la matriz B

    // Obtenemos el escalar parcial
    num = (maxA * maxB) - (minA * minB);
    den = promA * promB; 
    escalar = num / den; //Calculamos el escalar 

    // Multiplicación de las matrices A y B 
    for (i = 0; i < n; i+= tam_bloque){
        for (j = 0; j < n; j+= tam_bloque){
            for (k = 0; k < n; k+= tam_bloque){
                //Recorrido submatrices 
                for (sub_i = i; sub_i < i + tam_bloque; sub_i++){
                    desplazamiento_i = sub_i*n; 
                    for (sub_j = j; sub_j < j + tam_bloque; sub_j++){
                        double sumaParcial = 0.0; // Inicializamos el valor actual en 0.0
                        for (sub_k = k; sub_k < k + tam_bloque; sub_k++){
                            sumaParcial += A[desplazamiento_i + sub_k] * B[sub_k*n + sub_j]; 
                        }
                        RES_PARCIAL[desplazamiento_i + sub_j] += sumaParcial; // Guardamos el resultado en la matriz RES_PARCIAL 
                    }
                }
            } 
        }
    }    

    // Trasponemos B 
    for (i = 0; i < n; i++){
          for (j = 0; j < n; j++){
                BT[j*n + i] = B[i*n + j]; 
          }
     }

    // Multiplicación de matrices: C * B transpuesta
    for (i = 0; i < n; i+= tam_bloque){
        for (j = 0; j < n; j+= tam_bloque){
            for (k = 0; k < n; k+= tam_bloque){
                //Recorrido submatrices 
                for (sub_i = i; sub_i < i + tam_bloque; sub_i++){
                    desplazamiento_i = sub_i*n; 
                    for (sub_j = j; sub_j < j + tam_bloque; sub_j++){
                        desplazamiento_j = sub_j*n;
                        double sumaParcial = 0.0; // Inicializamos el valor actual en 0.0
                        for (sub_k = k; sub_k < k + tam_bloque; sub_k++){
                            //sumaParcial += C[desplazamiento_i + sub_k] * B[desplazamiento_j + sub_k]; //Se multiplica por B transpuesta 
                            sumaParcial+= C[desplazamiento_i + sub_k] * BT[sub_k*n + sub_j]; //Se multiplica por B transpuesta
                        }
                        RES_PARCIAL_T[desplazamiento_i + sub_j] += sumaParcial; // Almacenamos el resultado más la suma de la primera parte 
                    }
                }
            } 
        }
    }
    for (int m = 0; m < cantidad_elementos_totales; m++){
        R[m] = (escalar * RES_PARCIAL[m]) + RES_PARCIAL_T[m] ; //Sumamos la matriz resultado de la primera parte con la segunda parte
    }
    timetick = dwalltime() - timetick; //Guardamos el tiempo de fin 

    printf("\nMultiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",n,n, timetick); //Imprimimos el tiempo de la multiplicacion de matrices

    verificarResultado(R, n); //Verificamos el resultado de la multiplicacion de matrices

    printf("\n----------------------\n");

    printf("\nVector A");
    for(i = 0; i < cantidad_elementos_totales; i++){
        printf("%f ", A[i]);
    }
    printf("\n----------------------\n");

    printf("\nVector B");
    for(i = 0; i < cantidad_elementos_totales; i++){
        printf("%f ", B[i]);
    }    

    printf("\n----------------------\n");

    printf("\nResultados de la multiplicacion de matrices:\n");
    for (i = 0; i < cantidad_elementos_totales; i++){
        printf("%f ", R[i]); //Imprimimos el resultado de la multiplicacion de matrices
    }
    printf("\n");

    printf("\n----------------------\n");

    printf("\nResultados de la multiplicacion de matrices con B transpuesta:\n");
    for (i = 0; i < cantidad_elementos_totales; i++){
        printf("%f ", RES_PARCIAL_T[i]); //Imprimimos el resultado de la multiplicacion de matrices
    }

    //Liberar memoria
    free(A);
    free(B);
    free(C);
    free(R);
    free(BT);
    free(RES_PARCIAL);
    free(RES_PARCIAL_T);
    return (0);
}

void verificarResultado(double *R, int N) {
    int check=1;
    int i, j;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            check=check&&(R[j*N+i]==N);
         }
    }

    if(check){
         printf("Multiplicacion de matrices resultado correcto\n");
    }else{
         printf("Multiplicacion de matrices resultado erroneo\n");
    }
}
// Fin del programa