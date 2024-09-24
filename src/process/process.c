#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

Process* create_process(char *name ,int pid, int start_time, int t_cpu_burst, int n_burst, int io_wait, int t_deadline){
    Process *process = calloc(1, sizeof(Process));
    process->pid = pid;
    process->name = name;
    process->status = "READY";
    process->start_time = start_time;
    process->t_cpu_burst = t_cpu_burst;
    process->cpu_burst_tick = t_cpu_burst;
    process->n_burst = n_burst;
    process->io_wait = io_wait;
    process->t_deadline = t_deadline;
    process->t_lcpu = 0;
    process-> response_time = -1;
    process->quantum_tick = 0;
    return process;
}

void free_process(Process* process){
    free(process);
}

void print_process(Process* process){
    printf("PID: %d\n", process->pid);
    printf("Status: %s\n", process->status);
   // printf("Start time: %d\n", process->start_time);
   printf("CPU burst: %d\n", process->t_cpu_burst);
   // printf("Number of bursts: %d\n", process->n_burst);
  //  printf("IO wait: %d\n", process->io_wait);PO
    //printf("Deadline: %d\n", process->t_deadline);
    printf("waiting time: %d\n", process->waiting_time);
    printf("Interrupt: %d\n", process->interrupt);
    printf("Quantum used: %d\n", process->quantum_tick);
    printf("Change\n\n\n");
}

void print_for_enum(Process* process){
    printf("\nname: %s\n", process->name);
    printf("pid: %d\n", process->pid);
    printf("Interrupt: %d\n", process->interrupt);
    printf("Turnaround time: %d\n", process->turnaround_time);
    printf("response time: %d\n", process->response_time);
    printf("waiting time: %d\n", process->waiting_time);
    printf("time after deadline: %d\n\n", process->time_after_deadline);
}


bool process_is_finished(Process* process){
    return process->n_burst == 0;
}

bool process_is_for_update_priority(Process* process, int tick){
    return 2 * process->t_deadline < tick - process->t_lcpu;
}

int update_process_in_running(Process* process, int tick){
    printf("\nUpdating process in running state:\n");
    printf("PID: %d, Tick: %d\n", process->pid, tick);
    printf("Quantum tick before update: %d\n", process->quantum_tick);
    printf("CPU burst tick before update: %d\n", process->cpu_burst_tick);

    if((tick - process->t_lcpu) - process->t_deadline > 0 ){
        printf("Process has missed deadline\n", process->pid);
        printf("Time LCPU: %d\n", process->t_lcpu);
        printf("Deadline: %d\n", process->t_deadline);
        process->time_after_deadline = (tick - process->t_lcpu) - process->t_deadline;
    }

    if(process->cpu_burst_tick == 0){
        process_leave_CPU(process, tick);

        if(process->n_burst == 0){
            process_finish(process, tick);
            return 1;
        }

        if(process->quantum_tick>= process->quantum){
            printf("Leave CPU and change to LOW %d\n", process->pid);
            process->t_lcpu = tick;
            printf("Time LCPU: %d\n", process->t_lcpu);
            process->status = "READY";
            process->quantum_tick = 0;
            process->quantum = 0;
            return 3;
        }

        update_io_waiting(process);
        return 2;
        
    }
    else if(process->quantum_tick >= process->quantum){

        if(process->cpu_burst_tick == 1 && process->n_burst == 1){
            printf("Process PID: %d has no more bursts left\n", process->pid);
            process_finish(process, tick);
            return 1;
        }
        printf("Quantum expired for PID: %d\n", process->pid);
        process_interrupted(process, tick);
        return 3;
    }
    process->quantum_tick++;
    process->cpu_burst_tick--;
    process->t_lcpu = tick;
    return 0;
}

void process_interrupted(Process* process, int tick){
    process->interrupt++;
    process->t_lcpu = tick;
    printf("Time LCPU: %d\n", process->t_lcpu);
    process->status = "READY";
    process->quantum_tick = 0;
    process->quantum = 0;
}

void process_leave_CPU(Process* process, int tick){
    process->n_burst--;
    process->cpu_burst_tick = process->t_cpu_burst;
    process->t_lcpu = tick;
    printf("Time LCPU: %d\n", process->t_lcpu);
    process->status = "WAITING";
}

int get_priority(Process* process, int tick){
    return (tick - process->t_lcpu) - process->t_deadline;
}

void update_io_waiting(Process* process){
    printf("\nUpdating IO waiting for PID: %d\n", process->pid);
    printf("Current IO waiting: %d\n", process->t_io_waiting);
    printf("IO wait: %d\n\n", process->io_wait);
    if(process->t_io_waiting == process->io_wait){
        process->status = "READY";
        process->t_io_waiting = 0;
    }
    else if(process->status != "FINISHED"){
        printf("Incrementing waiting_time for PID: %d\n", process->pid);
        printf("actual waiting time: %d\n", process->waiting_time); 
        process->t_io_waiting++;
        process->waiting_time++;
    }
}

void update_response_time(Process* process, int tick){
    if(process->response_time == -1 && strcmp(process->status, "RUNNING") == 0){
        process->response_time = tick - process->start_time;
    }
}

void process_finish(Process* process, int tick){
    process->status = "FINISHED";
    process->turnaround_time = tick - process->start_time;
    process->t_lcpu= tick;
    printf("Time LCPU: %d\n", process->t_lcpu);
}



void update_quantum(Process* process, int quantum){
    if(process->quantum_tick == 0){
        process->quantum = quantum;
    }
}