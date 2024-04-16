#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

struct QueueNode {
  struct Process *process;
  struct QueueNode *next;
};

struct Queue {
  struct QueueNode *head;
  struct QueueNode *tail;
  int size;
};

struct Queue *initQueue();
void enqueue(struct Queue *queue, struct Process *process);
struct Process *dequeue(struct Queue *queue);
struct Process* dequeueProcess(struct Queue* queue, struct Process* process_to_remove);
void printQueue(struct Queue* queue);
struct Process *peek(struct Queue *queue);
int isEmpty(struct Queue *queue);


#endif /* QUEUE_H */
