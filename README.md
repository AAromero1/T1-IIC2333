# T1-IIC2333 :computer:

## Descripción :blue_book:

Este proyecto implementa un scheduler utilizando el algoritmo Round Robin (RR). El objetivo es modelar y simular la planificación de procesos en un sistema operativo, distribuyendo el tiempo de CPU de manera equitativa entre todos los procesos.

## Funcionalidades Principales :bookmark_tabs:
- **Estructura del Proceso**: Definición de la estructura utilizada para representar cada proceso en el scheduler.

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
- **Modelacion del Proceso**: El struct de proceso cuenta con una serie de funciones que permiten manejarlo facilmente, por ejemplo, las funciones ```process_interrupted``` y ```process_leave_CPU``` manejan los casos donde el proceso se interrumpe, pasando a "READY" y donde termina de usar CPU, pasando a "WAITING".

- **Inicialización de Procesos**: Carga y configuración de los procesos que serán gestionados por el scheduler.

- **Estructura de la Cola**: Definición de la estructura utilizada para gestionar la cola de procesos en el scheduler.

```c
typedef struct queue_t {
    Process* head;
    Process* last;
    int quantum;
} Queue;
```

- **Modelacion de la Cola**: Tambien se definieron funciones que permiten trabajar con las colas de baja y alta prioridad facilmente, por ejemplo, la funcion ```update_priority_queue``` revisa si es que hay procesos que se deben mover a la fila de alta prioridad y los cambia en caso de ser asi. 

* Se instancia la cola ```high``` con quantum * 2 y la cola ```low``` con quantum q. Ademas, existen las colas de ```all_processes``` y ```finish``` para auxiliar el inicio y el fin del programa.

```c
Queue *high_priority = create_queue(quantum * 2);
Queue *low_priority = create_queue(quantum);

Queue *all_processes = create_queue(0);
Queue *finish = create_queue(0);
```

## Estructura del Proyecto :open_file_folder:

- [`src/`]: Contiene el código fuente del proyecto.
- [`/lrscheduler`]: Contiene el código principal ```main.c``` donde se ejecuta el flujo del scheduler.
- [`/process`]: Contiene el código del struct y funciones de los processos.
- [`/queue`]: Contiene el código del struct y funciones de las colas.

## Compilación y Ejecución :pencil:

Para compilar el proyecto, ejecute el siguiente comando en la raíz del proyecto:

```sh
make
```

Para ejecutar el scheduler, utilice el siguiente comando:

```sh
./lrscheduler <input.txt> <outputfile> <quantum>
```

Se recomienda que el outputfile sea en .csv

## Explicación de la Función `main` :pencil2:

La función `main` se encarga de coordinar la ejecución del scheduler. A continuación se describe su funcionamiento paso a paso:

1. **Lectura del Input**:
   - Se obtienen los nombres de los archivos de entrada y salida, así como el valor del quantum desde los argumentos de la línea de comandos.
   - Se crean las colas de alta prioridad, baja prioridad, todos los procesos y procesos finalizados.

2. **Carga de Procesos**:
   - Se lee el archivo de entrada y se crean los procesos a partir de los datos leídos.
   - Cada proceso se inserta en la cola de todos los procesos.

   ```c
   for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
		}
		Process *process = create_process(input_file->lines[i][0],
       atoi(input_file->lines[i][1]), atoi(input_file->lines[i][2]), 
       atoi(input_file->lines[i][3]), atoi(input_file->lines[i][4]), 
       atoi(input_file->lines[i][5]), atoi(input_file->lines[i][6]));
			insert_process(all_processes, process);
	}
   ```

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

   ```c
   if(is_empty(high_priority ) && is_empty(low_priority) && is_empty(all_processes)){
			break;
		}

   (...)

   sort_queue_by_pid(finish);
   FILE *file = fopen(output_file, "w");
   if (!file) {
      fprintf(stderr, "Error al abrir el archivo %s para escritura.\n", output_file);
      return 1;
   }
   ```