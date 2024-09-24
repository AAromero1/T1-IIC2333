#include "queue.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Queue* create_queue(int quantum){
    Queue *queue = calloc(1, sizeof(Queue));
    queue->quantum = quantum;
    return queue;
}

void insert_process(Queue* queue, Process* process){
    if(queue->head == NULL){
        queue->head = process;
        queue->last = process;
    }else{
        queue->last->next = process;
        queue->last = process;
    }
    process->quantum = queue->quantum;
}

void pop_process(Queue* queue){
    if(queue->head != NULL){
        Process* temp = queue->head;
        queue->head = queue->head->next;
    }
}

void change_queue(Queue* origin, Queue* destiny, Process* process){
    Process* temp = origin->head;
    Process* prev = NULL;
    while(temp != NULL){
        if(temp == process){
            if(prev == NULL){
                origin->head = temp->next;
            }else{
                prev->next = temp->next;
            }
            temp->next = NULL;
            insert_process(destiny, temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

void free_queue(Queue* queue){
    Process* temp = queue->head;
    while(temp != NULL){
        Process* next = temp->next;
        free_process(temp);
        temp = next;
    }
    free(queue);
}

void change_process_status(Queue* queue, Process* process, char* status){
    Process* temp = queue->head;
    while(temp != NULL){
        if(temp == process){
            temp->status = status;
            break;
        }
        temp = temp->next;
    }
}

void change_queue_by_time(Queue* origin, int time, Queue* high_priority){
    Process* temp = origin->head;
    Process* prev = NULL;
    while(temp != NULL){
        if(temp->start_time == time && strcmp(temp->status, "READY") == 0){
            if(prev == NULL){
                origin->head = temp->next;
            }else{
                prev->next = temp->next;
            }
            Process* to_move = temp;
            temp = temp->next;
            to_move->next = NULL;
            insert_process(high_priority, to_move);
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
}

void update_priority_queue(Queue* high_priority, Queue* low_priority, int time){
    Process* temp = low_priority->head;
    Process* prev = NULL;
    while(temp != NULL){
        if(process_is_for_update_priority(temp, time)){
            if(prev == NULL){
                low_priority->head = temp->next;
            }else{
                prev->next = temp->next;
            }
            temp->next = NULL;
            insert_process(high_priority, temp);
        }
        prev = temp;
        temp = temp->next;
    }
}

void update_process(Queue* queue, int time, Process* running_process){
    Process* temp = queue->head;
    while(temp != NULL){
        if(strcmp(temp->status, "READY")== 0 && temp != running_process){
            temp->waiting_time++;
        }
        else if(strcmp(temp->status , "WAITING")== 0 && temp != running_process){
            update_io_waiting(temp);
        }
        temp = temp->next;
    }
}

void down_priority_queue(Queue* high_priority, Queue* low_priority, Process* process){
    Process* temp = high_priority->head;
    Process* prev = NULL;
    while(temp != NULL){
        if(temp == process){
            if(prev == NULL){
                high_priority->head = temp->next;
            }else{
                prev->next = temp->next;
            }
            temp->next = NULL;
            insert_process(low_priority, temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

Process* get_process_by_priority(Queue* queue, int tick){
    Process* temp = queue->head;
    Process* process = NULL;
    while(temp != NULL){
        if(strcmp(temp->status, "READY")== 0){
            if(process == NULL){
                process = temp;
            }else{

                if(get_priority(temp, tick) > get_priority(process, tick)){
                    process = temp;
                }

                else if(get_priority(temp, tick) == get_priority(process, tick)){
                    if(temp->pid < process->pid){
                        process = temp;
                    }
                }
            }
        }
        temp = temp->next;
    }
    return process;
}


bool is_some_process_ready(Queue* queue){
    Process* temp = queue->head;
    while(temp != NULL){
        if(strcmp(temp->status, "READY") == 0){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool is_some_process_running(Queue* queue){
    Process* temp = queue->head;
    while(temp != NULL){
        if(strcmp(temp->status, "RUNNING")== 0){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

int process_in_queue(Queue* queue){
    Process* temp = queue->head;
    int count = 0;
    while(temp != NULL){
        count++;
        temp = temp->next;
    }
    return count;
}

bool is_empty(Queue* queue){
    return queue->head == NULL;
}

void print_queue(Queue* queue){
    Process* temp = queue->head;
    while(temp != NULL){
        print_process(temp);
        temp = temp->next;
    }
}


int compare_by_pid(const void* a, const void* b) {
    Process* processA = *(Process**)a;
    Process* processB = *(Process**)b;
    return processA->pid - processB->pid;
}

void sort_queue_by_pid(Queue* queue) {
    int count = 0;
    Process* temp = queue->head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    if (count < 2) {
        return;
    }
    Process* processes[count];
    temp = queue->head;
    for (int i = 0; i < count; i++) {
        processes[i] = temp;
        temp = temp->next;
    }
    qsort(processes, count, sizeof(Process*), compare_by_pid);

    queue->head = processes[0];
    for (int i = 0; i < count - 1; i++) {
        processes[i]->next = processes[i + 1];
    }
    processes[count - 1]->next = NULL;
    queue->last = processes[count - 1];
}

void print_processes_to_file(FILE* file, Queue* queue) {
    Process* process = queue->head;
    while (process != NULL) {
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d\n",
                process->name,
                process->pid,
                process->interrupt,
                process->turnaround_time,
                process->response_time,
                process->waiting_time,
                process->time_after_deadline);
        process = process->next;
    }
}