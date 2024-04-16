#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct Queue *initQueue() {
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
  if (queue == NULL) {
    printf("Memory allocation failed for queue.\n");
    exit(EXIT_FAILURE);
  }
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;

  return queue;
};

int isEmpty(struct Queue* queue) {
    return (queue->head == NULL);
};

void enqueue (struct Queue* queue, struct Process* process){
  struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
  if (newNode == NULL){
    printf("Memory allocation failed for queuenode.\n");
    exit(EXIT_FAILURE);
  }

  newNode->process = process;
  newNode->next = NULL;

  if (isEmpty(queue)){
    queue->head = newNode;
    queue->tail = newNode;
  }
  else{
    queue->tail->next = newNode;
    queue->tail = newNode;
  }
  queue->size++;

};

struct Process* dequeue (struct Queue* queue){
  struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
  if (isEmpty(queue)){
    printf("Queue is empty. Cannot dequeue.\n");
    exit(EXIT_FAILURE);
  }
  struct QueueNode* temp = queue->head;
  struct Process* dequeuedProcess = temp->process;
  queue->head = temp->next;
  if (queue->head == NULL) {
      queue->tail = NULL;
  }
  free(temp);
  queue->size--;
  return dequeuedProcess;

};

void printQueue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }

    struct QueueNode* current = queue->head;
    printf("Queue: ");
    while (current != NULL) {
        printf("%d ", current->process->process_id);
        current = current->next;
    }
    printf("\n");
}


struct Process* dequeueProcess(struct Queue* queue, struct Process* process_to_remove) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        exit(EXIT_FAILURE);
    }

    struct QueueNode* current_node = queue->head;
    struct QueueNode* prev_node = NULL;

    // Search for the process to remove
    while (current_node != NULL) {
        if (current_node->process == process_to_remove) {
            if (prev_node == NULL) { // If the process to remove is the head of the queue
                queue->head = current_node->next;
                if (queue->head == NULL) {
                    queue->tail = NULL;
                }
            } else {
                prev_node->next = current_node->next;
                if (current_node == queue->tail) {
                    queue->tail = prev_node;
                }
            }
            struct Process* dequeued_process = current_node->process;
            free(current_node);
            queue->size--;
            return dequeued_process;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    // If the process to remove is not found in the queue
    printf("Process not found in the queue.\n");
    exit(EXIT_FAILURE);
}


struct Process* peek(struct Queue* queue) {
    if (isEmpty(queue)) {
        return NULL; // Queue is empty, nothing to peek at
    }
    return queue->head->process; // Return the process at the front of the queue without dequeuing it
}