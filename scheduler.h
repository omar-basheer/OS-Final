#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.h"
#include "queue.h"

struct ReadyQueue {
  int num_processes;
  struct Process *processes;
};

struct CPU_Scheduler {
  int num_processes;
  struct Process *processes;
  struct Queue *ready_queue;
};

struct CPU_Scheduler* initScheduler(int num_processes, int *arrival_times, int *burst_times, int *priorities);
void addToReadyQueue(struct CPU_Scheduler* scheduler, struct Process* process);
struct Process* getNextProcess(struct CPU_Scheduler* scheduler);
void printProcesses(struct CPU_Scheduler* scheduler);
#endif
