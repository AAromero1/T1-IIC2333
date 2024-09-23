#include "queue.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

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
        if(temp->start_time == time && temp->status == "READY"){
            if(prev == NULL){
                origin->head = temp->next;
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

void update_process(Queue* queue, int time){
    Process* temp = queue->head;
    while(temp != NULL){
        if(temp->status == "READY"){
            temp->waiting_time++;
        }
        else if(temp->status == "WAITING"){
            temp->t_io_waiting++;
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
        if(temp->status == "READY"){
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
        if(temp->status == "READY"){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool is_some_process_running(Queue* queue){
    Process* temp = queue->head;
    while(temp != NULL){
        if(temp->status == "RUNNING"){
            return true;
        }
        temp = temp->next;
    }
    return false;
}