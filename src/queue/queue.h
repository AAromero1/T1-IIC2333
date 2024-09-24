#pragma once
#include "../process/process.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue_t{
    Process* head;
    Process* last;
    int quantum;
} Queue;

Queue* create_queue(int quantum);
void insert_process(Queue* queue, Process* process);
void change_queue(Queue* origin, Queue* destiny, Process* process);
void free_queue(Queue* queue);
void change_process_status(Queue* queue, Process* process, char* status);
void change_queue_by_time(Queue* origin, int time, Queue* high_priority);
void update_priority_queue(Queue* high_priority, Queue* low_priority, int time);
void down_priority_queue(Queue* high_priority, Queue* low_priority, Process* process);
Process* get_process_by_priority(Queue* queue, int tick);
bool is_some_process_ready(Queue* queue);
bool is_empty(Queue* queue);
void print_queue(Queue* queue);
int process_in_queue(Queue* queue);
void update_process(Queue* queue, int time, Process* running_process);
void print_processes_to_file(FILE* file, Queue* queue);