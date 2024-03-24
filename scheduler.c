#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>

struct CPU_Scheduler* initScheduler(int num_processes, struct Process *processes) {
    struct CPU_Scheduler* scheduler = (struct CPU_Scheduler*)malloc(sizeof(struct CPU_Scheduler));
    if (scheduler == NULL) {
        printf("Memory allocation failed for scheduler.\n");
        exit(EXIT_FAILURE);
    }
    scheduler->num_processes = num_processes;
    scheduler->processes = processes;
    scheduler->ready_queue = initQueue(); // Initialize the ready queue
    return scheduler;
};

void addToReadyQueue(struct CPU_Scheduler* scheduler, struct Process* process) {
    enqueue(scheduler->ready_queue, process);
};

struct Process* getNextProcess(struct CPU_Scheduler* scheduler) {
    return dequeue(scheduler->ready_queue);
};