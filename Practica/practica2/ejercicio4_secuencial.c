#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 double *A;
 int i,j,k,N;
 int check=1;
 double timetick;


 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }


    //Aloca memoria para los vectores
    A=(double*)malloc(sizeof(double)*N);

    //Inicializa los vectores en 1, el resultado será una vector con todos sus valores en 2
    for(i=0;i<N;i++)
	    A[i] = i;
  
    double min = A[i];
    double max = A[i];
    double promedio = 0.0; 

    timetick = dwalltime();

    for(i = 0; i<N; i++){
        promedio += A[i];
        if (A[i] < min){
            min = A[i];
        }
        if (A[i] >  max){
            max = A[i];
        }
    }
  
    promedio = promedio / N; 

	double time = dwalltime() - timetick;

    printf("Analisis de vector de dimensión %d. Tiempo en segundos %f\n",N,time);

    printf("Valor mínimo: %f\n", min);
    printf("Valor máximo: %f\n", max);
    printf("Promedio: %f\n", promedio);
    printf("--------------------------\n");

    for (i = 0; i < N; i++)
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
    return(0);
}
