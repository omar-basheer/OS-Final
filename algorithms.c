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


void priority_scheduler(struct Queue* queue){

    if(isEmpty(queue)){
        printf("Queue is empty. Cannot schedule.\n");
        return;
    };

    struct Queue* tempQueue = initQueue();
    struct Process* currentProcess = NULL;
    struct Process* highestPriorityProcess = NULL;
    int totalProcessses = queue ->size;
    int currentTime = 0;

    while (totalProcessses > 0){
        while(!isEmpty(queue) && queue->head->process->arrival_time <= currentTime){
            currentProcess = dequeue(queue);
            enqueue(tempQueue, currentProcess);
        }

        highestPriorityProcess = NULL;
        struct QueueNode* tempNode = tempQueue->head;

        while (tempNode != NULL){
            if(highestPriorityProcess == NULL || tempNode->process->priority < highestPriorityProcess->priority){
                highestPriorityProcess = tempNode ->process;
            }
            tempNode = tempNode->next;
        }

        if(highestPriorityProcess != NULL){
            int remainingTime = highestPriorityProcess->remaining_time - 1;
            currentTime++;
            if(remainingTime == 0){
                highestPriorityProcess->completion_time = currentTime;
                calculateTurnaroundTime(highestPriorityProcess);
                calculateWaitingTime(highestPriorityProcess, currentTime);
                totalProcessses--;
            }
            else{
                highestPriorityProcess->remaining_time = remainingTime;
                enqueue(tempQueue, highestPriorityProcess);
            }
        }

        else{
            currentTime++;
        }

    }


}



void shortestTimeRemaining(struct Queue* queue){
    if(isEmpty(queue)){
        printf("Queue is empty. Cannot schedule.\n");
        return;
    };

    struct Queue* tempQueue = initQueue();
    struct Process* currentProcess = NULL;
    int totalProcessses = queue ->size;
    int currentTime = 0;

    while(totalProcessses > 0){
        while(!isEmpty(queue) && queue->head->process->arrival_time <= currentTime){
            currentProcess = dequeue(queue);
            enqueue(tempQueue, currentProcess);
        }

        struct QueueNode* tempNode = tempQueue->head;
        struct Process* shortestRemainingProcess = tempNode->process;
        while (tempNode != NULL){
            if(tempNode->process->remaining_time < shortestRemainingProcess->remaining_time){
                shortestRemainingProcess = tempNode->process;
            }
            tempNode = tempNode->next;
        }

        if(shortestRemainingProcess != NULL){
            int remainingTime = shortestRemainingProcess->remaining_time - 1;
            currentTime++;
            if(remainingTime == 0){
                shortestRemainingProcess->completion_time = currentTime;
                calculateTurnaroundTime(shortestRemainingProcess);
                calculateWaitingTime(shortestRemainingProcess, currentTime);
                totalProcessses--;
            }
            else{
                shortestRemainingProcess->remaining_time = remainingTime;
                enqueue(tempQueue, shortestRemainingProcess);
            }
        }

        else{
            currentTime++;
        }
    }


}




