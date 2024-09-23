#pragma once
#include <stdbool.h>
typedef struct process_t{
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


Process* create_process(int pid, char *name, int start_time, int t_cpu_burst, int n_burst, int io_wait, int t_deadline);
void free_process(Process* process);
void print_process(Process* process);
bool process_is_finished(Process* process);
bool process_is_for_update_priority(Process* process, int tick);
void process_interrupted(Process* process, int tick);
void process_leave_CPU(Process* process, int tick);
int get_priority(Process* process, int tick);
void update_io_waiting(Process* process);
void update_response_time(Process* process, int tick);
void process_finish(Process* process, int tick);
int update_process_in_running(Process* process, int tick);