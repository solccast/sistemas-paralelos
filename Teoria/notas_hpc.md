En el caso de GPU -> SIMD -> una sola unidad de control a múltiples cores (procesador vectorial). todas las unidades funcionales ejecutan el mismo código. La idea es poder usar cores más simples. 
Caso de CPU -> MIMD 

FPGA Y ASICS NO ENTRA AL PARCIAL KJJJJJ

--- 

## Sobre MPI 
Estándar que define el **pasaje de mensajes** 

> Lo que se crea son procesos 

Posee una estructura de programa particular
```c

int main(...){
    MPI_init(argc, ) //Esta debe ser la primera línea. Si se hacen cálculos pueden dar error
    //------- Todo lo que se haga se hace acá dentro

    //Lo más adecuando es un if. 
    if rank = 0
        function_slave(...)
    else 
        function_worker(...)

    //----------------------------------------------
    //Previo al return es necesario finalizar el MPI
    MPI_finalize()
    return 0
}
```

El ejecutar es `mpirun -np 4 ./programa.c` -> se  generan 4 copias del proceso
Cada proceso tiene su propio espacio de direccciones entonces las variables declaradas globales son copias para cada proceso. NO son variables compartidas. 

Sobre cómo se toma el tiempo: 
- La idea es que las matrices estén en un solo proceso y el master se encargue de distribuir (qué es lo que hay que darle a cada proceso) y luego recupera el resultado en el proceso master. Nadie va a poder comenzar hasta que el master no disrtribuya. Entonces, colocar uuna barrera antes del if y luego después del if entonces -> calculo del tiempo solo en el master.  

En el caso de MPI híbrido colocar `MPI_init_thread_safe(...)` para evitar el deadlock porque dentro del SO puede fallar mas no en el código. 

--

Con la estrategia de collapse -> eliminar. 