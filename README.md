# T1-IIC2333

## Descripción

Este proyecto implementa un scheduler utilizando el algoritmo Round Robin (RR). El objetivo es modelar y simular la planificación de procesos en un sistema operativo, distribuyendo el tiempo de CPU de manera equitativa entre todos los procesos.

## Funcionalidades Principales
- **Estructura del Proceso**: Definición de la estructura [`Process`]("Go to definition") utilizada para representar cada proceso en el scheduler.

```c
typedef struct process_t {
    int pid;
    char* name;
    char* status;
    int start_time;
    int t_cpu_burst;
    int n_burst;
    int io_wait;
    int t_deadline;
    struct process_t* next;
    int interrupt;
    int quantum_tick;
    int quantum;
    int cpu_burst_tick;
    int waiting_time;
    int time_after_deadline;
    int response_time;
    int t_lcpu;
    int t_io_waiting;
    int turnaround_time;
} Process;
```
- **Inicialización de Procesos**: Carga y configuración de los procesos que serán gestionados por el scheduler.
- **Estructura de la Cola**: Definición de la estructura [`Queue`]("Go to definition") utilizada para gestionar la cola de procesos en el scheduler.

```c
typedef struct queue_t {
    Process* head;
    Process* last;
    int quantum;
} Queue;
```
- **Algoritmo Round Robin**: Implementación del algoritmo RR para la planificación de procesos.
- **Gestión de Tiempos**: Control y seguimiento del tiempo de ejecución de cada proceso.
- **Interrupciones y Context Switching**: Manejo de interrupciones y cambio de contexto entre procesos.

## Estructura del Proyecto

- [`src/`]: Contiene el código fuente del proyecto.

## Compilación y Ejecución

Para compilar el proyecto, ejecute el siguiente comando en la raíz del proyecto:

```sh
make
```

Para ejecutar el scheduler, utilice el siguiente comando:

```sh
./lrscheduler <input.txt> <outputfile> <quantum>
```

Se recomienda que el outputfile sea en .csv

## Explicación de la Función `main`

La función `main` es el punto de entrada del programa y se encarga de coordinar la ejecución del scheduler. A continuación se describe su funcionamiento paso a paso:

1. **Lectura del Input**:
   - Se obtienen los nombres de los archivos de entrada y salida, así como el valor del quantum desde los argumentos de la línea de comandos.
   - Se crean las colas de alta prioridad, baja prioridad, todos los procesos y procesos finalizados.

2. **Carga de Procesos**:
   - Se lee el archivo de entrada y se crean los procesos a partir de los datos leídos.
   - Cada proceso se inserta en la cola de todos los procesos.

3. **Simulación del Scheduler**:
   - Se inicializa el tick (unidad de tiempo).
   - En cada tick, se actualizan los procesos en las colas de alta y baja prioridad.
   - Si hay un proceso en ejecución, se actualiza su estado y se maneja su cambio de cola o finalización.
   - Se mueven los procesos a las colas de alta prioridad según su tiempo de llegada.
   - Se actualizan las prioridades de las colas.
   - Si no hay un proceso en ejecución, se selecciona uno de las colas de alta o baja prioridad para ejecutarlo.
   - Se imprimen las colas de alta y baja prioridad.

4. **Finalización de la Simulación**:
   - La simulación termina cuando todas las colas están vacías.
   - Se ordenan los procesos finalizados por su PID y se escriben en el archivo de salida.
   - Se liberan los recursos utilizados (colas y archivo de entrada).
Se recomienda que el outputfile sea en .csv

