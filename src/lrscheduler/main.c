#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h> // bool, true, false
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"


int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	int quantum = atoi(argv[2]);

	Queue *high_priority = create_queue(quantum * 2);
	Queue *low_priority = create_queue(quantum);
	Queue *all_processes = create_queue(0);

	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	printf("Nombre archivo: %s\n", file_name);
	printf("Cantidad de procesos: %d\n", input_file->len);
	printf("Procesos:\n");
	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
			Process *process = create_process(input_file->lines[i][0], input_file->lines[i][1], 
			input_file->lines[i][2], input_file->lines[i][3], input_file->lines[i][4], input_file->lines[i][5], 
			input_file->lines[i][6]);
			insert_process(all_processes, process);
		}
		printf("\n");
	}

	int tick = 0;
	Process *process;
	while (true)
	{
		update_process(high_priority, tick);
		update_process(low_priority, tick);

		if(process!= NULL){
		 	int type = update_process_in_running(process, tick);
			if(type != 0){
				if(type == 3){
					down_priority_queue(high_priority, low_priority, process);
				}
				process =NULL;
			}
		}
		
		change_queue_by_time(all_processes, tick, high_priority);

		if (is_some_process_ready(high_priority)){
			process = get_process_by_priority(high_priority, tick);
			process->status = "RUNNING";
			process->quantum = high_priority->quantum;
			update_response_time(process, tick);
		}


		tick++;
	}
	
	free_queue(high_priority);
	free_queue(low_priority);
	free_queue(all_processes);	
	input_file_destroy(input_file);
}