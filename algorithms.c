#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"
#include "scheduler.h"

#define TIME_SLICE 2

void round_robin(struct Queue* ready_queue){
  int current_time = 0;
  while(!isEmpty(ready_queue)){
    struct Process* current_process = dequeue(ready_queue);
    int remaining_time = current_process->remaining_time;
      if (remaining_time <= 0){
        //process completes within time slice
        current_time = current_time + remaining_time;
        current_process->completion_time = current_process->completion_time + remaining_time;
        updateProcess(current_process, 0, current_process->completion_time);
        printf("Process %d completed at time %d\n", current_process->process_id, current_process->completion_time);
      }
      else{
        // process needs more time, stop execution at time slice
        if (remaining_time < TIME_SLICE){
          current_time = current_time + remaining_time;
        }
        else {
          current_time = current_time + TIME_SLICE;
        }
        current_process->completion_time = current_time;
        remaining_time =  remaining_time - TIME_SLICE;
        if (remaining_time < 0){
          remaining_time = 0;
        };
        updateProcess(current_process, remaining_time, current_process->completion_time);
        printf("Process %d preemted at time %d\n", current_process->process_id, current_process->completion_time);
        enqueue(ready_queue, current_process);
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
  while(!isEmpty(ready_queue)){
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
                printf("Process %d completed at time %d.\n", shortestRemainingProcess->process_id, currentTime);
            }
            else{
                shortestRemainingProcess->remaining_time = remainingTime;
                enqueue(tempQueue, shortestRemainingProcess);
                printf("Process %d preempted at time %d.\n", shortestRemainingProcess->process_id, currentTime);
            }
        }

        else{
            currentTime++;
        }
    }
}


void sjn(struct Queue* ready_queue) {
    while (!isEmpty(ready_queue)) {
        // Sort the ready queue based on burst time
        for (struct QueueNode* i = ready_queue->head; i != NULL; i = i->next) {
            for (struct QueueNode* j = i->next; j != NULL; j = j->next) {
                if (i->process->burst_time > j->process->burst_time) {
                    struct Process* temp = i->process;
                    i->process = j->process;
                    j->process = temp;
                }
            }
        }

        // Dequeue the process with the shortest burst time
        struct Process* current_process = dequeue(ready_queue);
        int remaining_time = current_process->remaining_time;

        // Simulate execution of the process for a time quantum (e.g., 1 unit of time)
        if (remaining_time > 0) {
            updateProcess(current_process, remaining_time - 1, current_process->completion_time);
            printf("Process %d preempted at time %d\n", current_process->process_id, current_process->completion_time);
            // Re-enqueue the process back into the ready queue for future execution
            enqueue(ready_queue, current_process);
        } else {
            printf("Process %d completed at time %d\n", current_process->process_id, current_process->completion_time);
        }
    }
}

void runPreemptiveSJF(struct CPU_Scheduler* scheduler) {
    int currentTime = 0;
    struct Process* currentProcess = NULL;
    int isCPUIdle = 1;

    printf("Starting Preemptive SJF Simulation\n");

    while (!isEmpty(scheduler->ready_queue)) {
        // Check and enqueue newly arrived processes
        for (int i = 0; i < scheduler->num_processes; i++) {
            if (scheduler->processes[i].arrival_time == currentTime) {
                printf("Time %d: Process %d arrives\n", currentTime, scheduler->processes[i].process_id);
                enqueue(scheduler->ready_queue, &scheduler->processes[i]);
            }
        }

        // Decide on preemption or continuation of the current process
        if (!isCPUIdle && !isEmpty(scheduler->ready_queue) &&
            peek(scheduler->ready_queue)->remaining_time < currentProcess->remaining_time) {
            printf("Time %d: Process %d preempted by Process %d\n", currentTime, currentProcess->process_id, peek(scheduler->ready_queue)->process_id);
            currentProcess->remaining_time -= (currentTime - currentProcess->last_start_time);
            enqueue(scheduler->ready_queue, currentProcess);
            isCPUIdle = 1;
        }

        if (isCPUIdle && !isEmpty(scheduler->ready_queue)) {
            currentProcess = dequeue(scheduler->ready_queue);
            printf("Time %d: Process %d starts execution\n", currentTime, currentProcess->process_id);
            currentProcess->last_start_time = currentTime;
            isCPUIdle = 0;
        }

        // Process execution for a time unit
        if (!isCPUIdle) {
            currentProcess->remaining_time--;
            if (currentProcess->remaining_time <= 0) {
                printf("Time %d: Process %d completes execution\n", currentTime, currentProcess->process_id);
                isCPUIdle = 1;
                currentProcess = NULL;
            }
        }

        currentTime++;
    }

    printf("Preemptive SJF Simulation Complete\n");
}
