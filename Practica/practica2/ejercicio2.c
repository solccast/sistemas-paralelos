/* Blocked matrix multiplication example */
/* Fernando G. Tinetti                   */

#include<stdio.h>
#include<stdlib.h>   /* malloc() */
#include<time.h>
#include<pthread.h>

double *a, *b, *c; 
int n, bs, T; 

void * multiplicacion_bloques (void * ptr) ;

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose); 
 
/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs);

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs);

/* Time in seconds from some point in the past */
double dwalltime();



/************** MAIN *************/
int main(int argc, char *argv[])
{
  int i, j;
  pthread_attr_t attr; // Atributos de los hilos
  int *status;
  double timetick;
  
  /* Check command line parameters */
  if ( (argc != 3) || ((n = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || ((n % T) != 0) )
  {
    printf("\nEstamos recibiendo: %d argumentos\n", argc);
      printf("\nError en los par�metros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
      exit(1);
  }
    
  int ids[T]; // Arreglo de ids de hilos
  pthread_t threads[T]; // Arreglo de hilos
  bs = n / T; // Tamaño de bloque para cada hilo

  /* Getting memory */  
  a = (double *) malloc(n*n*sizeof(double));
  b = (double *) malloc(n*n*sizeof(double));
  c = (double *) malloc(n*n*sizeof(double));

  /* Init matrix operands */
  initvalmat(a, n, 1.0, 0);
  initvalmat(b, n, 1.0, 1);
  
  // printf("Multiplying %d x %d matrices\n", n, n);
  
  
  pthread_attr_init(&attr); // Se inicializan los atributos de los hilos
  /* Creación de los hilos */
  for (int i = 0; i < T; i++){
    ids[i] = i; 
    pthread_create(&threads[i], &attr, multiplicacion_bloques, &ids[i]);
  }
    
  timetick = dwalltime();
    
    //matmulblks(a, b, c, n, bs);
    
  for (int i = 0; i < T; i++){
    pthread_join(threads[i], (void *) &status);
  }
  
  double workTime = dwalltime() - timetick;

  /* Check results (just in case...) */
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      if (c[i*n + j] != n)
      {
        printf("Error at %d, %d, value: %f\n", i, j, c[i*n + j]);
      }
    }
  }

  printf("\nMMBLK-SEC; %d; %d; %lf; %lf\n",n,bs,workTime,((double)2*n*n*n)/(workTime*1000000000));
printf("\nMultiplicacion de matrices de %dx%d. Tiempo en segundos %f\n", n, n, workTime);
  return 0;
}

/*****************************************************************/

/* Init square matrix with a specific value */
void initvalmat(double *mat, int n, double val, int transpose)
{
  int i, j;      /* Indexes */

	if (transpose == 0) {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
	} else {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
            mat[j*n + i] = val;
		}
    }
}
}

/*****************************************************************/
/* Hilos */
void * multiplicacion_bloques (void * ptr) {

    int * p, id;
    p = (int *) ptr;
    id = *p;
    
    int fila_inicio = id * bs;
    int fila_fin = fila_inicio + bs;

    int i, j, k; 

    for (i = fila_inicio; i < fila_fin; i += bs) {
      for (j = 0; j < n; j += bs) {
          for (k = 0; k < n; k += bs) {
              blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n, bs);
          }
      }
    }

    pthread_exit((void*)ptr); // Se termina el hilo
}    

/**************************************************************** */
/* Multiply square matrices, blocked version */
void matmulblks(double *a, double *b, double *c, int n, int bs)
{
  int i, j, k;    /* Guess what... */

  /* Init matrix c, just in case */  
  initvalmat(c, n, 0.0, 0);

  //Se divide en submatrices de tamaño bsxbs 
  
  for (i = 0; i < n; i += bs) //Bs es lo que se recibe por parámetro, se recorren primero las filas, los saltos que se dan son por cada bs, mientras sea menor a n 
  {
    for (j = 0; j < n; j += bs) //Se recorren las columnas, también cada salto que se da es por bs, mientras sea menor a n 
    {
      for  (k = 0; k < n; k += bs) //k también se encarga de saltar dale el tamaño de bs
      {
        blkmul(&a[i*n + k], &b[j*n + k], &c[i*n + j], n, bs); //Lo que envía al otro método es la fila de la matriz A, la coluna de la matriz B, la fila de la matriz A, el tamño de la matriz y el bloque bs

    }
    }
  }
}

/*****************************************************************/

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int n, int bs) // Variables:
{
  int i, j, k;    /* Guess what... again... */

  for (i = 0; i < bs; i++) // Recorre la fila i
  {
    for (j = 0; j < bs; j++) //Recorre la columna j
    {
      for  (k = 0; k < bs; k++) //Recorre cada uno de los cosos 
      {
        cblk[i*n + j] += ablk[i*n + k] * bblk[j*n + k]; //Setea el resultado de la fila A[i] por la columna B[j]
      }
    }
  }
}
    
/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
