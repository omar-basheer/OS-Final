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

#endif
