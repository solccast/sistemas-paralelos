# Sistemas Paralelos - Clase 3 - 2025: Programación en pasaje de mensajes - Estándar MPI

## Fundamentos de programación en pasaje de mensajes

### Plataformas de memoria compartida
Consisten de un conjunto de nodos de procesamiento, cada uno con su propio espacio de datos, entonces hay múltiples copias de un mismo data -> comunicación mediante **intercambio de mensajes** para sincronización y consistencia. 

Consiste de un conjunto de procesos, donde cada uno cuenta con su propio espacio de direcciones. Cada dato pertenece a una partición y toda interacción requiere la cooperación de dos procesos. 

La mayoría de los programas se escriben siguiendo el modelo **Single Program Multiple Data (SPMD)**: todos los procesos ejecutan el mismo programa pero no necesariamente las mismas instrucciones (sentencias de selección).  

> No confundir con un programa SIMD 

El ajuste (_tuning_) para mejorar el rendimiento puede ser óptimo: puede manejar el balance de carga, la redistribución de datos y procesos, etc. 
Existen librerías para facilitar la ejecución de código sobre diferentes arquitecturas: _Buena portabilidad de código, no necesariamente portabilidad de rendimiento_

Ventajas del modelo de pasaje de mensajes: 
- Mayor control dele programador para lograr sistemas eficientes y escalables.
- Más fácil de predecir el rendimiento 
- Puede implementarse en muchas arquitecturas paralelas. 

Desventajas del modelo de pasaje de mensajes: 
- Mayor complejidad al implementar estos algoritmos para lograr alto rendimiento. 

### Operaciones 
- **Send**: `Send (void *sendbuf, int nelems, int dest)`
- **Receive**: `Receive (void *recvbuf, int nelems, int source)`

Existen diferentes protocolos para Sned y Receive: bloqueante y no bloqueante. 
- **Bloqueante**: el control al proceso llamador se devuelve una vez que todos los recursos involucrados pueden ser reutilizados -> aplica al emisor como al receptor. Se aplica tanto al emisor como al receptor. Genera ociosidad en los procesos. Variantes: 
    - Sin _buffering_: el _send_ se bloquea hasta que el receptor no termina el _receive_ del mensaje. Impacto: ocio del proceso emisor o del receptor. En este protocolo hay riesgo de **deadlock**.  
    - Con _buffering_: el _send_ se bloquea hasta que el mensaje llega a un buffer prealocado del sistema (diferente al del receptor). La transmisión del mensaje puede realizarse con hardware especial o no. En el primer caso no intervierne la CPU y comienza la transmisión al buffer del receptor; en el otro caso es el proceso emisor el que se encarga de transmitir el mensaje al receptor y recién ahí se desbloquea. Impacto: reduce el ocio pero aumenta el costo por el manejo de buffers y su restricción de tamaño, reducción de deadlocks.

> Overhead se genera a partir del ocio de los protocolos sin buffering o del manejo de buffer en los protocolos con buffering. 

- **No bloqueante**: Para evitar _overhead_ se devuelve el control de la operación inmediatamente (priorizan el rendimiento). Requiere un posterior chequeo para asegurarse la finalización de la comunicación -> deja en manos del programador asegurar la semántica del Send. Hay dos variantes: 
    - Sin _buffering_: inicia comunicación al llegar al receive. 
    - Con _buffering_: el emisor utiliza acceso directo a memoria para copiar los datos a un buffer prealocado mientras el proceso continúa su cómputo (reduce el tiempo en que el dato no está seguro).

|                               | **Operaciones bloqueantes**                                                                 | **Operaciones no bloqueantes**                                                              |
|-------------------------------|---------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|
| **Con buffering**             | El emisor retoma el control una vez que los datos han sido copiados al buffer              | El emisor retoma el control una vez que ha iniciado la transferencia DMA al buffer aún cuando podría no haberse completado.         |
| **Sin buffering**             | El emisor se bloquea hasta que el receptor alcance el receive correspondiente             | El emisor envía los datos cuando el receptor alcanza el receive                            |
| **Semántica del send/receive**| La semántica del send y receive está asegurada por la operación correspondiente            | El programador debe asegurar explícitamente el cumplimiento de la semántica de las operaciones                |

  
## Estándar Message Passing Interface MPI
MPI sigue el modelo SPMD: todos los procesos siguen el mismo programa pero no significa que todos haran lo mismo. 

### Inicio y finalización de entorno
- `MPI_init (int *argc, char **argv)` inicializa el entorno MPI, debe ser invocada por todos los procesos antes que cualquier otro llamado a rutinas MPI. 
- `MPI_Finalize()` cierra el entorno MPI. 

### Comunicadores
Un comunicador define el dominio de comunicación: qué procesos pueden comunicarse entre sí. Son variables del tipo `MPI_Comm`. Un proceso puede pertenecer a muchos comunicadores. Existe un comunicador que incluye a todos los procesos de la aplicación: `MPI_COMM_WORLD` (broadcast). 

#### Adquisición de información
- `MPI_COMM_SIZE`: indica la cantidad de procesos en el comunicador (` MPI_COMM_size(int *argc, char **argv)`)
- `MPI_COMM_RANK`: indica el identificador (rank) del proceso dentro de ese comunicador. Cada proceso puede tener un rank diferente en cada comunicador. 

#### Operaciones de comunicación
MPI soporta:
- **Comunicaciones punto a punto**: operaciones de comunicación que involucran a dos procesos → bloqueantes y no bloqueantes
- **Comunicaciones colectivas**: operaciones de comunicación que pueden involucrar a dos o más procesos → bloqueantes y no bloqueantes

##### Comunicaciones punto a punto
- `MPI_Send`: rutina básica para enviar datos a otro proceso. 
```c
MPI_Send (void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador)
// *buf: dirección del búffer de envío
// cantidad: cantidad de elementos que se leerán desde el búffer
// tipoDato: tipo de dato de los elementos que se tomarán del búffer
// destino: rank del proceso destino (el que ejecutará el receive)
// tag: identifica la operación de comunicación 
// comunicador: comunicador empleado. 
```
