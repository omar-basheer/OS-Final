#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
// #include "process.h"
// #include "scheduler.h"

#define TIME_SLICE 2

void round_robin(struct Queue* ready_queue){
  while(!isEmpty(ready_queue)){
    struct Process* current_process = dequeue(ready_queue);
    int remaining_time = current_process->burst_time;

    while (remaining_time > 0){
      if (remaining_time <= TIME_SLICE){
        //process completes within time slice
        updateProcess(current_process, 0, current_process->completion_time + remaining_time);
        printf("Process %d completed at time %d\n", current_process->process_id, current_process->completion_time);
        break;
      }
      else{
        // process needs more time, stop execution at time slice
        remaining_time -= TIME_SLICE;
        updateProcess(current_process, remaining_time, 0);
        printf("Process %d preemted at time %d\n", current_process->process_id, current_process->completion_time + TIME_SLICE);
        enqueue(ready_queue, current_process);
      }
    }
  }
};

void sjf(struct Queue* ready_queue){
  // Sort the ready queue based on burst time
  for (struct QueueNode* i = ready_queue->head; i != NULL; i = i->next){
    for (struct QueueNode* j = i->next; j != NULL; j = j->next){
      if (i->process->burst_time > j->process->burst_time){
        struct Process* temp = i->process;
        i->process = j->process;
        j->process = temp;
      }
    }
  }

  // Execute processes in the sorted order
  while(!isEmoty(ready_queue)){
    struct Process* current_process = dequeue(ready_queue);
    int remaining_time = current_process->burst_time;

    updateProcess(current_process, 0, current_process->burst_time);
    printf("Process %d completed at time %d\n", current_process->process_id, current_process->completion_time);
  }
};