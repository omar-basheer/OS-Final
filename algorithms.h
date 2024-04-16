#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "process.h"
#include "queue.h"

// void round_robin(struct Queue *ready_queue, int time_slice);

void round_robin(struct Process *processes, int num_processes, int time_slice);
void np_sjf(struct Queue* ready_queue);
void mlfq(struct Queue *queue, int q1_time_slice, int q2_time_slice);
void shortestJobFirstPreemptive(struct Process *processes, int num_processes);

#endif // ALGORITHMS_H
