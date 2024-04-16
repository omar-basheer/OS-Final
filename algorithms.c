#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"
#include "scheduler.h"


void round_robin(struct Process *processes, int num_processes, int time_slice){
  struct Queue *ready_queue = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  for(int i = 0; i < num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  while (total_burst_time > 0){
    
    if (!isEmpty(ready_queue)){
      printQueue(ready_queue);
      struct Process *current_process = dequeue(ready_queue);
      printf("process %d dequeued\n", current_process->process_id);
      printQueue(ready_queue);

      // execute process to completion
      if (current_process->remaining_time <= time_slice){
        current_time += current_process->remaining_time;
        printf("current time: %d, updated in execute to completion\n", current_time);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
      }
      else{
        // execute process for time slice
        current_time += time_slice;
        printf("current time: %d, updated in execute for time slice\n", current_time);
        current_process->remaining_time -= time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= time_slice;
        printf("Process %d preempted at time %d\n", current_process->process_id, current_time);
      }

      // Check if any new processes have arrived and add them to the ready queue
      printf("checking for new processes after execution\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      // Add the current process back to the ready queue if it has remaining time
      if (current_process->remaining_time > 0){
        enqueue(ready_queue, current_process);
      }
      printf("\n");

    }else{
      printf("current time: %d\n", current_time);
      printf("\n");

      printf("checking for new processes\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("no new processes have arrived, current time: %d\n", current_time);
      }

      printQueue(ready_queue);
      printf("\n");

      if (total_burst_time <= 0){
        break;
      }
    }
  }
  printf("\n");
  printf("Round Robin Scheduling Complete\n");

}

void np_sjf(struct Process *processes, int num_processes){
  struct Queue *ready_queue = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  for(int i = 0; i < num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  while(total_burst_time > 0){

    if (!isEmpty(ready_queue)){
      printQueue(ready_queue);

      // find process with shortest remaining time
      struct Process *shortest_process = NULL;
      struct QueueNode *current_node = ready_queue->head;

      while (current_node != NULL){
        struct Process *current_process = current_node->process;

        if (shortest_process == NULL || current_process->burst_time < shortest_process->burst_time){
          shortest_process = current_process;
        }
        current_node = current_node->next;
      }
      printf("shortest process in ready queue: %d\n", shortest_process->process_id);

      //execute process to completion
      dequeueProcess(ready_queue, shortest_process);
      current_time += shortest_process->burst_time;
      shortest_process->completion_time = current_time;
      total_burst_time -= shortest_process->burst_time;
      shortest_process->remaining_time = 0;
      calculateTurnaroundTime(shortest_process);
      calculateWaitingTime(shortest_process);

      printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
             shortest_process->process_id,
             shortest_process->completion_time,
             shortest_process->turnaround_time,
             shortest_process->waiting_time);

      // check if any new processes have arrived and add them to the ready queue
      printf("checking for new processes after execution\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }
      printf("\n");

    }else{
      printf("current time: %d\n", current_time);
      printf("\n");

      printf("checking for new processes\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("no new processes have arrived, current time: %d\n", current_time);
      }

      printQueue(ready_queue);
      printf("\n");

      if (total_burst_time <= 0){
        break;
      }

    }
  }
  printf("\n");
  printf("Non-Preemptive Shortest Job First Scheduling Complete\n");

}


void pp_sjf(struct Process *processes, int num_processes){
  struct Queue *ready_queue = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  for (int i = 0; i<num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  while (total_burst_time > 0){

    if (!isEmpty(ready_queue)){
      printQueue(ready_queue);

      // find process with shortest remaining time
      struct Process *shortest_process = NULL;
      struct QueueNode *current_node = ready_queue->head;

      while (current_node != NULL){
        struct Process *current_process = current_node->process;

        if (shortest_process == NULL || current_process->remaining_time < shortest_process->remaining_time){
          shortest_process = current_process;
        }
        current_node = current_node->next;
      }
      printf("shortest process in ready queue: %d\n", shortest_process->process_id);

      // execute the process for one time slice
      dequeueProcess(ready_queue, shortest_process);
      current_time += 1;
      printf("current time: %d, updated in execute for time slice\n", current_time);
      shortest_process->remaining_time -= 1;
      shortest_process->completion_time = current_time;
      total_burst_time -= 1;
      printf("Process %d preempted at time %d\n", shortest_process->process_id, current_time);

      // check if any new processes have arrived and then to the ready queue
      printf("checking for new processes after execution\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      // add the current process back to the queue if it has remaining time
      if (shortest_process->remaining_time > 0){
        enqueue(ready_queue, shortest_process);
      }
      else{
        shortest_process->completion_time = current_time;
        calculateTurnaroundTime(shortest_process);
        calculateWaitingTime(shortest_process);

        printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
               shortest_process->process_id,
               shortest_process->completion_time,
               shortest_process->turnaround_time,
               shortest_process->waiting_time);
      }

      printf("\n");

    }else{
      printf("current time: %d\n", current_time);
      printf("\n");

      printf("checking for new processes\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("no new processes have arrived, current time: %d\n", current_time);
      }

      printQueue(ready_queue);
      printf("\n");

      if (total_burst_time <= 0){
        break;
      }
    }
  }
}

void mlfq (struct Process *processes, int num_processes, int q1_time_slice, int q2_time_slice){
  struct Queue *ready_queue = initQueue();
  struct Queue *queue2 = initQueue();
  struct Queue *queue3 = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  for (int i = 0; i<num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  while(total_burst_time > 0){
    if (!isEmpty(ready_queue)){
      printf("highest priority queue\n");
      printQueue(ready_queue);
      struct Process *current_process = dequeue(ready_queue);
      printf("process %d dequeued\n", current_process->process_id);
      printQueue(ready_queue);

      // execute process to completion
      if (current_process->remaining_time <= q1_time_slice){
        current_time += current_process->remaining_time;
        printf("current time: %d, updated in execute to completion\n", current_time);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d in queue 1, turnaround time: %d, waiting time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
      }
      else{
        // execute process for time slice
        current_time += q1_time_slice;
        printf("current time: %d, updated in execute for time slice\n", current_time);
        current_process->remaining_time -= q1_time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= q1_time_slice;
        printf("Process %d preempted at time %d\n", current_process->process_id, current_time);
      }

      // Check if any new processes have arrived and add them to the ready queue
      printf("checking for new processes after execution\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      // Add the current process to a lower priority queue if it has remaining time
      if (current_process->remaining_time > 0){
        enqueue(queue2, current_process);
        printf("process %d moved to queue 2\n", current_process->process_id);
      }
      printf("\n");
    }
    if(!isEmpty(queue2)){
      printf("middle priority queue\n");
      printQueue(queue2);
      struct Process *current_process = dequeue(queue2);
      printf("process %d dequeued\n", current_process->process_id);
      printQueue(queue2);

      // execute process to completion
      if (current_process->remaining_time <= q2_time_slice){
        current_time += current_process->remaining_time;
        printf("current time: %d, updated in execute to completion\n", current_time);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d in queue 2, turnaround time: %d, waiting time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
      }
      else{
        // execute process for time slice
        current_time += q1_time_slice;
        printf("current time: %d, updated in execute for time slice\n", current_time);
        current_process->remaining_time -= q1_time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= q1_time_slice;
        printf("Process %d preempted at time %d\n", current_process->process_id, current_time);
      }

      // Check if any new processes have arrived and add them to the ready queue
      printf("checking for new processes after execution\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      // Add the current process to a lower priority queue if it has remaining time
      if (current_process->remaining_time > 0){
        int process_age = current_process->arrival_time + current_time;
        if(process_age > 10){
          enqueue(ready_queue, current_process);
          printf("process %d moved to queue 1 (ready queue)\n", current_process->process_id);
        }
        enqueue(queue3, current_process);
        printf("process %d moved to queue 3\n", current_process->process_id);
      }
      printf("\n");
    }
    if(!isEmpty(queue3)){
      printf("lowest priority queue\n");
      printQueue(queue3);
      struct Process *current_process = dequeue(queue3);
      printf("process %d dequeued\n", current_process->process_id);
      printQueue(queue3);

      current_time += current_process->remaining_time;
      printf("current time: %d, updated in execute to completion\n", current_time);
      current_process->completion_time = current_time;
      total_burst_time -= current_process->remaining_time;
      current_process->remaining_time = 0;
      calculateTurnaroundTime(current_process);
      calculateWaitingTime(current_process);

      printf("Process %d completed at time %d in queue 3, turnaround time: %d, waiting time: %d\n",
              current_process->process_id,
              current_process->completion_time,
              current_process->turnaround_time,
              current_process->waiting_time);
    }

    if(isEmpty(ready_queue) && isEmpty(queue2) && isEmpty(queue3)){
      printf("current time: %d\n", current_time);
      printf("\n");

      printf("checking for new processes\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d, current time: %d\n", processes[j].process_id, processes[j].arrival_time, current_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("no new processes have arrived, current time: %d\n", current_time);
      }

      printQueue(ready_queue);
      printf("\n");

      if (total_burst_time <= 0){
        break;
      }
    }
  }
  printf("\n");
  printf("Multi-Level Feedback Queue Scheduling Complete\n");
}
