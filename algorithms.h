#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "process.h"
#include "queue.h"

void round_robin(struct Queue* ready_queue);
void sjf(struct Queue* ready_queue);
void priority_scheduler(struct Queue* queue);
void shortestTimeRemaining(struct Queue* queue);
void sjn(struct Queue* ready_queue);
void runPreemptiveSJF(struct CPU_Scheduler* scheduler);

#endif // ALGORITHMS_H
