#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "process.h"
#include "queue.h"

void round_robin(struct Queue* ready_queue);
void np_sjf(struct Queue* ready_queue);
void shortestTimeRemaining(struct Queue* queue);
void mlfq(struct Queue *queue, int q1_time_slice, int q2_time_slice);
void runPreemptiveSJF(struct CPU_Scheduler* scheduler);

#endif // ALGORITHMS_H
