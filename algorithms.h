#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "process.h"
#include "queue.h"

// void round_robin(struct Queue *ready_queue, int time_slice);

void round_robin(struct Process *processes, int num_processes, int time_slice);
void np_sjf(struct Process *processes, int num_processes);
void mlfq (struct Process *processes, int num_processes, int q1_time_slice, int q2_time_slice);
void pp_sjf(struct Process *processes, int num_processes);

#endif // ALGORITHMS_H
