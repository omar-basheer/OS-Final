#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
# include <unistd.h>
#include <sys/wait.h>

struct CPU_Scheduler* initScheduler(int num_processes, int *arrival_times, int *burst_times) {
    struct CPU_Scheduler* scheduler = (struct CPU_Scheduler*)malloc(sizeof(struct CPU_Scheduler));
    if (scheduler == NULL) {
        printf("Memory allocation failed for scheduler.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the array of processes
    scheduler->processes = (struct Process*)malloc(num_processes * sizeof(struct Process));
    if (scheduler->processes == NULL) {
        printf("Memory allocation failed for processes.\n");
        exit(EXIT_FAILURE);
    }

    scheduler->num_processes = num_processes;

    for (int i = 0; i < num_processes; i++) {
        scheduler->processes[i] = *createProcess(i + 1, arrival_times[i], burst_times[i]);
    }
    return scheduler;
};

void printProcesses(struct CPU_Scheduler* scheduler) {
    printf("Processes:\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("| Process ID | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Remaining Time |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < scheduler->num_processes; i++) {
        struct Process* process = &(scheduler->processes[i]);
        printf("| %-11d| %-13d| %-11d| %-16d| %-16d| %-13d| %-15d|\n", 
               process->process_id, process->arrival_time, process->burst_time, 
               process->completion_time, process->turnaround_time, process->waiting_time, process->remaining_time);
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("\n");
}


int areAllProcessesCompleted(struct CPU_Scheduler* scheduler) {
    for (int i = 0; i < scheduler->num_processes; i++) {
        if (scheduler->processes[i].remaining_time > 0) {
            return -1; // Found a process that is not yet completed
        }
    }
    return 0; // All processes have completed
}
