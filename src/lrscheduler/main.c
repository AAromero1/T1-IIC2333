#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <stdbool.h> // bool, true, false
#include "../process/process.h"
#include "../queue/queue.h"
#include "../file_manager/manager.h"

Process *process = NULL;

void update_ready_to_running_process(Queue* high_priority, Queue* low_priority, Queue* finish , Process* process,
int tick){
	update_response_time(process, tick);
	int type = update_process_in_running(process, tick);
	if(type != 0){
		if(type == 3){
			down_priority_queue(high_priority, low_priority, process);
		}
		if(type == 1){
		change_queue(high_priority, finish, process);
		change_queue(low_priority, finish, process);	
		}
		process =NULL;
	}
}

int main(int argc, char const *argv[])
{
	/*Lectura del input*/
	char *file_name = (char *)argv[1];
	char *output_file = (char *)argv[2];
	int quantum = atoi(argv[3]);
	Queue *high_priority = create_queue(quantum * 2);
	Queue *low_priority = create_queue(quantum);


	Queue *all_processes = create_queue(0);
	Queue *finish = create_queue(0);


	InputFile *input_file = read_file(file_name);

	/*Mostramos el archivo de input en consola*/
	for (int i = 0; i < input_file->len; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			printf("%s ", input_file->lines[i][j]);
		}
		Process *process = create_process(input_file->lines[i][0], atoi(input_file->lines[i][1]), 
			atoi(input_file->lines[i][2]), atoi(input_file->lines[i][3]), atoi(input_file->lines[i][4])
			, atoi(input_file->lines[i][5]), atoi(input_file->lines[i][6]));
			insert_process(all_processes, process);
	}

	int tick = 0;
	
	while (true)
	{
		update_process(high_priority, tick, process);
		update_process(low_priority, tick, process);

		if(process!= NULL){
		 	int type = update_process_in_running(process, tick);
			print_process(process);
			if(type != 0){
				if(type == 3){
					down_priority_queue(high_priority, low_priority, process);
				}
				if(type == 1){
					change_queue(high_priority, finish, process);
					change_queue(low_priority, finish, process);	
				}
				process =NULL;
			}
		}
		
		change_queue_by_time(all_processes, tick, high_priority);
		update_priority_queue(high_priority, low_priority, tick);

		if (process == NULL){
			if (is_some_process_ready(high_priority)){
				process = get_process_by_priority(high_priority, tick);
				process->status = "RUNNING";
				update_quantum(process, high_priority->quantum);
				update_ready_to_running_process(high_priority, low_priority, finish, 
				process, tick);

			}
			if(is_some_process_ready(low_priority) && is_some_process_ready(high_priority) == false && process == NULL){
				process = get_process_by_priority(low_priority, tick);
				process->status = "RUNNING";
				update_quantum(process, low_priority->quantum);
				update_ready_to_running_process(high_priority, low_priority, finish, 
				process, tick);
		}
		}
		

		print_queue(high_priority);
		print_queue(low_priority);
		tick++;

		if(is_empty(high_priority ) && is_empty(low_priority) && is_empty(all_processes)){
			break;
		}
	}

	sort_queue_by_pid(finish);
    FILE *file = fopen(output_file, "w");
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo %s para escritura.\n", output_file);
        return 1;
    }
	

    print_processes_to_file(file, finish);
    fclose(file);
	free_queue(high_priority);
	free_queue(low_priority);
	free_queue(all_processes);
	free_queue(finish);	
	input_file_destroy(input_file);

}



