# Sistemas Paralelos - Clase 3 - 2025

## Estándar Pthreads 

### Modelos de memoria compartida
- Basado en procesos
- Basado en Pthreads -> se supone toda la memoria global .
- Basados en directivas (extienden el modelo de threads)

> Un thread es un único hilo de control en el flujo de un programa 
Ventajas de lo basado en hilos ante el de procesos. 
•
"Liviandad" → Rendimiento: Los hilos son más ligeros que los procesos. Esto conlleva a una intercomunicación más rápida debido a que comparten memoria y a un cambio de contexto menos costoso.
•
Ocultamiento de latencia → Multi-tasking: La ejecución de múltiples hilos contribuye a reducir la latencia ocasionada por los accesos a memoria, la E/S y la comunicación.
•
Planificación y balance de carga: Las APIs de hilos suelen permitir la creación de una gran cantidad de tareas concurrentes que pueden ser mapeadas dinámicamente a través de primitivas a nivel de sistema. Esto minimiza el overhead por ociosidad y facilita la distribución del trabajo ante cargas irregulares.
•
Facilidad de programación y uso extendido: El modelo de hilos es generalmente más fácil de programar que el modelo de pasaje de mensajes, ya que no requiere el manejo explícito de la comunicación de datos.
•
Portabilidad: Permite migrar aplicaciones entre diversas arquitecturas, lo cual es una ventaja significativa.

#### POSIX Threads 
POSIX se ha establecido como una API estándar para manejo de Threads, provista por la mayoría de los desarrolladores de sistemas operativos.
Las rutinas más usadas de Pthreads: 
- Manejo de threads -> creación, terminación, etc. 
- Mutexes -> exclusión mutua
- Variables condición -> sincronización por condición

> Hay que hacer tantos join como create. 

## Fundamentos de programación en memoria compartida

> buscar diferencia entre amperson y el otro dato 
