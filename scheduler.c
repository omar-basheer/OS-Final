#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

struct CPU_Scheduler* initScheduler(int num_processes, int *arrival_times, int *burst_times, int *priorities) {
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
    scheduler->ready_queue = initQueue(); // Initialize the ready queue

    for (int i = 0; i < num_processes; i++) {
        scheduler->processes[i] = *createProcess(i + 1, arrival_times[i], burst_times[i], priorities[i]);
        addToReadyQueue(scheduler, &(scheduler->processes[i]));
    }
    return scheduler;
};

void addToReadyQueue(struct CPU_Scheduler* scheduler, struct Process* process) {
    enqueue(scheduler->ready_queue, process);
};

struct Process* getNextProcess(struct CPU_Scheduler* scheduler) {
    return dequeue(scheduler->ready_queue);
};

void printProcesses(struct CPU_Scheduler* scheduler) {
    printf("Processes:\n");
    printf("-----------------------------------------------------------------\n");
    printf("| Process ID | Arrival Time | Burst Time | Priority | Remaining Time |\n");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < scheduler->num_processes; i++) {
        struct Process* process = &(scheduler->processes[i]);
        printf("| %-11d| %-13d| %-11d| %-9d| %-15d|\n", process->process_id, process->arrival_time, process->burst_time, process->priority, process->remaining_time);
    }
    printf("-----------------------------------------------------------------\n");
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
