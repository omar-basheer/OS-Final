#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "process.h"
#include "queue.h"

void round_robin(struct Queue* ready_queue);
void np_sjf(struct Queue* ready_queue);
void shortestTimeRemaining(struct Queue* queue);
void mlfq(struct Queue *queues[], int num_queues);
// void sjn(struct Queue* ready_queue);

#endif // ALGORITHMS_H
