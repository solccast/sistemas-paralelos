# Sistemas Paralelos - Práctica 1: Optimización de algoritmos secuenciales

---
## Información útil 

|Acciones | Comandos|
| --- | --- |
|Para compilar en Linux con gcc| `gcc -o salidaEjecutable archivoFuente.c`|
|Para ejecutar| `./salidaEjecutable arg1 arg2 ... argN`| 

---

## Ejercicios 

### 1. Fibonacci
El algoritmo `fib.c` resuelve la serie de Fibonacci, para un número N dado, utilizando dos métodos: recursivo e iterativo. Analice los tiempos de ejecución de ambos métodos.  
- ¿Cuál es más rápido? ¿Por qué?  
**Nota**: Ejecute con N=1..50.

|N| ITERATIVO | RECURSIVO |
| --- | --- | --- |
|10|0.0000015020|0.0000066996|
|50|0.0000013113|537.5000690937|

Es mucho más rápido el iterativo porque no hace llamados constantes a nuevas funciones con sus respectivos espacios de direcciones y una función nueva depende de la anterior. 


---

### 2. Sumatoria
El algoritmo `funcion.c` resuelve, para un x dado, la siguiente sumatoria:

\[
\sum_{i=0}^{100000000} \left( 2 \cdot \frac{x^3 + 3x^2 + 3x + 2}{x^2 + 1} - i \right)
\]

El algoritmo compara dos alternativas de solución.  
- ¿Cuál de las dos formas es más rápida? ¿Por qué?

| $x$ |1era alternativa|2da alternativa|
| -- | -- | --|
|50 | <p>T. total: 1.5133619308</p> <p>T promedio: 0.0000000151</p>| <p>T. total: 8.8111691475</p> <p>T promedio: 0.0000000881</p>|
|100|<p>T. total: 1.5253829956</p> <p>T promedio: 0.0000000153</p>|<p>T. total: 8.8100621700</p> <p>T promedio: 0.0000000881</p>|
  
En el caso del primer algoritmo se almacena en una variable auxiliar el resultado y se lo va sumando, en cambio en el otro caso ante cada iteración va calculando. 

---

### 3. Optimización con GCC
Investigue en la documentación del compilador o a través de Internet qué opciones de optimización ofrece el compilador `gcc` (flag `-O`).  

- `-O0`: Este nivel (que consiste en la letra "O" seguida de un cero) desconecta por completo la optimización y es el predeterminado si no se especifica ningún nivel -O en CFLAGS o CXXFLAGS. El código no se optimizará.
- `-O1`: El nivel de optimización más básico. El compilador intentará producir un código rápido y pequeño sin tomar mucho tiempo de compilación. 
- `-O2`: Un paso delante de `-O1`. Es el nivel recomendado de optimización, a no ser que el sistema tenga necesidades especiales. `-O2` activará algunas opciones añadidas a las que se activan con `-O1`. Con `-O2`, el compilador intentará aumentar el rendimiento del código sin comprometer el tamaño y sin tomar mucho más tiempo de compilación. Se puede utilizar SSE o AVX en este nivel pero no se utilizarán registros YMM a menos que también se habilite ftree-vectorize.
- `-O3`: El nivel más alto de optimización posible. Activas optimizaciones que son caras en términos de tiempo de compilación y uso de memoria. El hecho de compilar con `-O3` no garantiza una forma de mejorar el rendimiento y, de hecho, en muchos casos puede ralentizar un sistema debido al uso de binarios de gran tamaño y mucho uso de la memoria. También se sabe que -O3 puede romper algunos paquetes. No se recomienda utilizar `-O3`. Sin embargo, también habilita - ftree-vectorize de modo que los bucles dentro del código se vectorizarán y se utilizarán los registros AVX YMM.

Compile y ejecute el algoritmo `matrices.c`, el cual resuelve una multiplicación de matrices de NxN. Explore los diferentes niveles de optimización para distintos tamaños de matrices.  

- ¿Qué optimizaciones aplica el compilador?  
- ¿Cuál es la ganancia respecto a la versión sin optimización del compilador?  
- ¿Cuál es la ganancia entre los distintos niveles?

| O | N | Tiempo |
| -- | -- | -- |
|0|<p>5</p> <p> 10 </p> | <p> 0.000037 </p> <p> 0.000257 </p>|
|1|<p>5</p> <p> 10 </p>|<p>0.000027</p> <p> 0.000187 </p>|
|2|<p>5</p> <p> 10 </p>|<p>0.000021</p> <p> 0.000145 </p>|
|3|<p>5</p> <p> 10 </p>|<p>0.000006</p> <p> 0.000019 </p>|

---

### 4. Precisión numérica
Dada la ecuación cuadrática:

\[
x^2 - 4.0000000x + 3.9999999 = 0
\]

Sus raíces son \( r_1 = 2.000316228 \) y \( r_2 = 1.999683772 \) (empleando 10 dígitos para la parte decimal).

#### a) `quadratic1.c`
El algoritmo `quadratic1.c` computa las raíces de esta ecuación empleando los tipos de datos `float` y `double`.  
- Compile y ejecute el código.  
- ¿Qué diferencia nota en el resultado?

#### b) `quadratic2.c`
El algoritmo `quadratic2.c` computa las raíces de esta ecuación, pero en forma repetida.  
- Compile y ejecute el código variando la constante `TIMES`.  
- ¿Qué diferencia nota en la ejecución?

#### c) `quadratic3.c`
El algoritmo `quadratic3.c` computa las raíces de esta ecuación, pero en forma repetida.  
- Compile y ejecute el código variando la constante `TIMES`.  
- ¿Qué diferencia nota en la ejecución?  
- ¿Qué diferencias puede observar en el código con respecto a `quadratic2.c`?  

**Nota**: Agregue el flag `-lm` al momento de compilar. Pruebe con el nivel de optimización que mejor resultado le haya dado en el ejercicio anterior.

---

### 5. Optimización de `matrices.c`
Analice el algoritmo `matrices.c`.  
- ¿Dónde cree que se producen demoras?  
- ¿Cómo podría optimizarse el código?  

Al menos, considere los siguientes aspectos:

- Explotación de localidad de datos a través de reorganización interna de matrices A, B o C (según corresponda).  
- El uso de *setters* y *getters* es una buena práctica en la programación orientada a objetos. ¿Tiene sentido usarlos en este caso? ¿Cuál es su impacto en el rendimiento?  
- ¿Hay expresiones en el cómputo que pueden refactorizarse para no ser computadas en forma repetida?  
- En lugar de ir acumulando directamente sobre la posición `C[i,j]` de la matriz resultado (línea 72), pruebe usar una variable local individual y, al finalizar el bucle más interno, asigne su valor a `C[i,j]`.  
  - ¿Esta modificación impacta en el rendimiento? ¿Por qué?  

Combine las mejoras que haya encontrado para obtener una solución optimizada y compare los tiempos con la solución original para diferentes tamaños de matrices.

---

### 6. Multiplicación de matrices por bloques
Analice y describa brevemente cómo funciona el algoritmo `mmblk.c`, que resuelve la multiplicación de matrices cuadradas de NxN utilizando una técnica de multiplicación por bloques.  

Luego, ejecute el algoritmo utilizando distintos tamaños de matrices y distintos tamaños de bloque (pruebe con valores que sean potencia de 2; p.e. N={512,1024,2048} y TB={16,32,64,128}).  

Finalmente, compare los tiempos con respecto a la multiplicación de matrices optimizada del ejercicio anterior.

- Según el tamaño de las matrices y de bloque elegido, ¿cuál es más rápido? ¿Por qué?  
- ¿Cuál sería el tamaño de bloque óptimo para un determinado tamaño de matriz?  
- ¿De qué depende el tamaño de bloque óptimo para un sistema?

---

### 7. Multiplicación de matriz por matriz triangular
Analice el algoritmo `triangular.c`, que resuelve la multiplicación de una matriz cuadrada por una matriz triangular inferior, ambas de NxN.

- ¿Cómo se podría optimizar el código?  
- ¿Se pueden evitar operaciones?  
- ¿Se puede reducir la memoria reservada?  

Implemente una solución optimizada y compare los tiempos probando con diferentes tamaños de matrices.

