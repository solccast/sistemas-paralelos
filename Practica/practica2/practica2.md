# Sistemas Paralelos - Práctica 2: Programación con Pthreads 

Información útil para compilar y ejecutar:
- Para compilar en Linux con gcc+Pthreads: `gcc -pthread –o salidaEjecutable archivoFuente`
- Para ejecutar: `./salidaEjecutable arg1 arg2 … argN`

En todos los casos debe calcularse la eficiencia de la implementación paralela por sobre la secuencial:
$$ Speedup(T) = \frac{T_{secuencial}}{T_{paralelo}} $$
$$ Eficiencia(T) = \frac{Speedup(T)}{T} $$


1. Desarrolle un algoritmo paralelo que compute una suma de vectores: 𝐴𝑖 = 𝐵𝑖 + 𝐶𝑖. Para ello, considere lo siguiente:
- Identifique las regiones de código que pueden ejecutarse en paralelo y las que no. ¿Existen dependencias?
- Analice cómo distribuir el trabajo entre los hilos. ¿El cómputo es regular? ¿La arquitectura es homogénea?
Mida el tiempo de ejecución para diferentes valores de N y T={2,4,8}. Analice el rendimiento.

Sin pthreads:
| N | Tiempo en segundos |
| -- | -- |
|10000 | 0.000087 |
| 100000 | 0.000795 |
| 1000000 | 0.008132|

Usando pthreads: 
| N | T | Tiempo en segundos |
| -- | -- | -- |
| 10000 | 2 | 0.001791| 
| 10000 | 4 | 0.010302|
| 10000 | 8 | 0.005303|
| 100000 | 2 | 0.001961|
| 100000 | 4 | 0.001444|
| 100000 | 8 | 0.005179 | 
| 1000000 | 2 | 0.004628 |
| 1000000 | 4 | 0.006209 |
| 1000000 | 8 | 0.005168 | 

Comparación 
| N       | T  | SpeedUp     | Eficiencia   |
|---------|----|-------------|--------------|
| 10000   | 2  | 0.048576214 | 0.024288107  |
| 10000   | 4  | 0.008444962 | 0.002111241  |
| 10000   | 8  | 0.016405808 | 0.002050726  |
| 100000  | 2  | 0.405405405 | 0.202702703  |
| 100000  | 4  | 0.550554017 | 0.137638504  |
| 100000  | 8  | 0.153504538 | 0.019188067  |
| 1000000 | 2  | 1.757130510 | 0.878565255  |
| 1000000 | 4  | 1.309711709 | 0.327427927  |
| 1000000 | 8  | 1.573529412 | 0.196691176  |


2. Desarrolle un algoritmo paralelo que compute la multiplicación de matrices cuadradas de NxN. Primero, considere a la versión optimizada del ejercicio 6 de la práctica anterior como algoritmo base. Luego, paralelice la versión que computa por bloques. Mida el tiempo de ejecución para N={512, 1024, 2048, 4096} y T={2,4,8}. Analice el rendimiento.

🔹 Tiempo (s) - Secuencial

| N    | Bs | Tiempo (s) |
|------|----|------------|
| 512  | 2  | 0.491159    |
| 512  | 4  | 0.524724    |
| 512  | 8  | 0.509878    |
| 1024 | 2  | 4.415312    |
| 1024 | 4  | 4.065369    |
| 1024 | 8  | 4.076478    |
| 2048 | 2  | 32.917257   |
| 2048 | 4  | 35.733807   |
| 2048 | 8  | 32.961463   |
| 4096 | 2  | 271.716235  |
| 4096 | 4  | 285.094543  |
| 4096 | 8  | 306.068730  |


🔹 Tiempo (s) - Pthreads

| N    | T  | Tiempo (s) |
|------|----|------------|
| 512  | 2  | 0.328137    |
| 512  | 4  | 0.203509    |
| 512  | 8  | 0.209814    |
| 1024 | 2  | 2.370461    |
| 1024 | 4  | 1.388153    |
| 1024 | 8  | 1.320993    |
| 2048 | 2  | 19.845548   |
| 2048 | 4  | 11.712600   |
| 2048 | 8  | 10.508726   |
| 4096 | 2  | 168.332116  |
| 4096 | 4  | 103.017874  |
| 4096 | 8  | 92.089667   |


🔹 SpeedUp y Eficiencia

| N    | T  | SpeedUp     | Eficiencia   |
|------|----|-------------|--------------|
| 512  | 2  | 1.496811    | 0.748405     |
| 512  | 4  | 2.578382    | 0.644596     |
| 512  | 8  | 2.430143    | 0.303768     |
| 1024 | 2  | 1.862639    | 0.931319     |
| 1024 | 4  | 2.928617    | 0.732154     |
| 1024 | 8  | 3.085919    | 0.385740     |
| 2048 | 2  | 1.658672    | 0.829336     |
| 2048 | 4  | 3.050886    | 0.762721     |
| 2048 | 8  | 3.136580    | 0.392073     |
| 4096 | 2  | 1.614168    | 0.807084     |
| 4096 | 4  | 2.767428    | 0.691857     |
| 4096 | 8  | 3.323595    | 0.415449     |