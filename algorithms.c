#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
// #include "process.h"
// #include "scheduler.h"

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
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n", 
          current_process->process_id, 
          current_process->completion_time, 
          current_process->turnaround_time, 
          current_process->waiting_time
        );

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


void np_sjf(struct Queue* ready_queue){
  int current_time = 0;
  
  while (!isEmpty(ready_queue)) {
    // Find the process with the shortest remaining burst time
    struct Process* shortest_process = NULL;
    struct QueueNode* current_node = ready_queue->head;
    
    while (current_node != NULL) {
      struct Process* current_process = current_node->process;
      
      // Check if the process has arrived and its burst time is shorter than the current shortest burst time
      if (current_process->arrival_time <= current_time &&
          (shortest_process == NULL || current_process->burst_time < shortest_process->burst_time)) {
        shortest_process = current_process;
      }
      
      current_node = current_node->next;
    }
    
    if (shortest_process == NULL) {
      // No process is ready to execute, increment current time
      current_time++;
    } else {
      // Execute the shortest process
      dequeueProcess(ready_queue, shortest_process);
      shortest_process->completion_time = current_time + shortest_process->burst_time;
      calculateTurnaroundTime(shortest_process);
      calculateWaitingTime(shortest_process);
      
      printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n", 
             shortest_process->process_id, 
             shortest_process->completion_time, 
             shortest_process->turnaround_time, 
             shortest_process->waiting_time
            );
      
      current_time = shortest_process->completion_time;
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
                calculateWaitingTime(shortestRemainingProcess);
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


void mlfq(struct Queue *queues[], int num_queues) {
    int current_time = 0;
    int current_queue = 0;
    int time_quantum = 2;

    // Iterate until all queues are empty
    while (1) {
        struct Queue *current_queue_ptr = queues[current_queue];

        // Check if the current queue is not empty
        if (!isEmpty(current_queue_ptr)) {
            struct Process *current_process = dequeue(current_queue_ptr);

            if (current_process->burst_time - current_process->remaining_time == 0) {
                printf("Round Robin scheduling begins for process %d at time %d\n", current_process->process_id, current_time);
            }

            // Execute the process for the time quantum or until completion
            int execution_time = (current_process->remaining_time < time_quantum) ? current_process->remaining_time : time_quantum;
            current_time += execution_time;
            current_process->remaining_time -= execution_time;

            // Print message when process is moved to the next lower priority queue
            if (current_process->remaining_time > 0) {
                if (current_queue < num_queues - 1) {
                    printf("Process %d moved to lower priority queue at time %d\n", current_process->process_id, current_time);
                } else {
                    printf("Process %d remains in the lowest priority queue at time %d\n", current_process->process_id, current_time);
                }
            }

            // Process has completed execution
            if (current_process->remaining_time <= 0) {
                printf("Process %d completed at time %d\n", current_process->process_id, current_time);
                free(current_process);
            } else {
                // Move the process to the end of the current queue
                enqueue(current_queue_ptr, current_process);
            }
        }

        // Move to the next non-empty queue
        do {
            current_queue = (current_queue + 1) % num_queues;
        } while (isEmpty(queues[current_queue]));

        // Check if all queues are empty
        int all_empty = 1;
        for (int i = 0; i < num_queues; i++) {
            if (!isEmpty(queues[i])) {
                all_empty = 0;
                break;
            }
        }

        // Exit loop if all queues are empty
        if (all_empty) {
            break;
        }
    }
}






