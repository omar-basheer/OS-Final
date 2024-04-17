#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"
#include "scheduler.h"


/**
 * Implements the Round Robin scheduling algorithm.
 * 
 * @param processes An array of Process structures representing the processes to be scheduled.
 * @param num_processes The number of processes in the processes array.
 * @param time_slice The time slice for each process.
 */
void round_robin(struct Process *processes, int num_processes, int time_slice){
  struct Queue *ready_queue = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  // Calculate the total burst time of all processes
  for(int i = 0; i < num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  printf("Starting Round Robin Scheduling\n");
  printf("================================\n");

  // Continue scheduling until all processes have completed
  while (total_burst_time > 0){
    
    if (!isEmpty(ready_queue)){
      printf("\nCurrent Queue: ");
      printQueue(ready_queue);
     

      struct Process *current_process = dequeue(ready_queue);
      printf("\nProcess %d dequeued\n", current_process->process_id);

      // Execute process to completion or for time slice
      if (current_process->remaining_time <= time_slice){
        current_time += current_process->remaining_time;
        printf("Current Time: %d (Process %d executed to completion)\n", current_time, current_process->process_id);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d, Turnaround Time: %d, Waiting Time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
        printf("Process %d work, sum = %lld\n", current_process->process_id, current_process->work);
      }
      else{
        current_time += time_slice;
        printf("Current Time: %d (Process %d preempted after time slice)\n", current_time, current_process->process_id);
        current_process->remaining_time -= time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= time_slice;
      }

      // Check for new processes and add them to the ready queue
      printf("\nChecking for new processes after execution\n");
      for (int j = 0; j < num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      // Add current process back to the ready queue if it has remaining time
      if (current_process->remaining_time > 0){
        enqueue(ready_queue, current_process);
      }

    }
    else{
      printf("\nCurrent Time: %d\n", current_time);
      printf("Checking for new processes\n");
      for (int j = 0; j < num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("No new processes arrived, Current Time: %d\n", current_time);
      }

      if (total_burst_time <= 0){
        break;
      }
    }
  }
  
  printf("\nRound Robin Scheduling Complete\n");
  printf("===============================\n");
}

/**
 * Non-Preemptive Shortest Job First Scheduling algorithm.
 * 
 * @param processes An array of Process structures representing the processes to be scheduled.
 * @param num_processes The number of processes in the processes array.
 */
void np_sjf(struct Process *processes, int num_processes){
  struct Queue *ready_queue = initQueue();
  int current_time = 0;
  int total_burst_time = 0;

  // Calculate the total burst time of all processes
  for(int i = 0; i < num_processes; i++){
    total_burst_time += processes[i].burst_time;
  }

  printf("Starting Non-Preemptive Shortest Job First Scheduling\n");
  printf("=====================================================\n");

  // Continue scheduling until all processes have completed
  while(total_burst_time > 0){

    if (!isEmpty(ready_queue)){
      printf("\nCurrent Queue: ");
      printQueue(ready_queue);

      // Find process with shortest remaining time
      struct Process *shortest_process = NULL;
      struct QueueNode *current_node = ready_queue->head;

      // Iterate through the ready queue to find the process with the shortest burst time
      while (current_node != NULL){
        struct Process *current_process = current_node->process;

        if (shortest_process == NULL || current_process->burst_time < shortest_process->burst_time){
          shortest_process = current_process;
        }
        current_node = current_node->next;
      }
      printf("\nShortest Process in Ready Queue: %d\n", shortest_process->process_id);

      // Execute process to completion
      dequeueProcess(ready_queue, shortest_process);
      current_time += shortest_process->burst_time;
      shortest_process->completion_time = current_time;
      total_burst_time -= shortest_process->burst_time;
      shortest_process->remaining_time = 0;
      calculateTurnaroundTime(shortest_process);
      calculateWaitingTime(shortest_process);

      printf("Process %d completed at time %d, Turnaround Time: %d, Waiting Time: %d\n",
             shortest_process->process_id,
             shortest_process->completion_time,
             shortest_process->turnaround_time,
             shortest_process->waiting_time);
       printf("Process %d work, sum = %lld\n", shortest_process->process_id, shortest_process->work);

      // Check for new processes and add them to the ready queue
      printf("\nChecking for new processes after execution\n");
      for (int j = 0; j < num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

    }else{
      printf("\nCurrent Time: %d\n", current_time);
      printf("Checking for new processes\n");
      for (int j = 0; j < num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("No new processes arrived, Current Time: %d\n", current_time);
      }

      if (total_burst_time <= 0){
        break;
      }

      printf("\nCurrent Queue: ");
      printQueue(ready_queue);
    }
  }
  
  printf("\nNon-Preemptive Shortest Job First Scheduling Complete\n");
  printf("=====================================================\n");
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
      printf("\nChecking for new processes after execution\n");
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
       printf("Process %d work, sum = %lld\n", shortest_process->process_id, shortest_process->work);

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

  printf("Starting Multi-Level Feedback Queue Scheduling\n");
  printf("===============================================\n");

  while(total_burst_time > 0){

    if (!isEmpty(ready_queue)){
      printf("\nHighest Priority Queue:\n");
      printQueue(ready_queue);
      struct Process *current_process = dequeue(ready_queue);
      printf("Process %d dequeued\n", current_process->process_id);

      // Execute process to completion or for time slice
      if (current_process->remaining_time <= q1_time_slice){
        current_time += current_process->remaining_time;
        printf("Current Time: %d (Process %d executed to completion in Queue 1)\n", current_time, current_process->process_id);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d, Turnaround Time: %d, Waiting Time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
        printf("Process %d work, sum = %lld\n", current_process->process_id, current_process->work);
      }
      
      else{
        current_time += q1_time_slice;
        printf("Current Time: %d (Process %d preempted after time slice in Queue 1)\n", current_time, current_process->process_id);
        current_process->remaining_time -= q1_time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= q1_time_slice;
      }

      // Check for new processes and add them to the ready queue or lower priority queues
      printf("\nChecking for new processes after execution in Queue 1\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (current_process->remaining_time > 0){
        enqueue(queue2, current_process);
        printf("Process %d moved to Queue 2\n", current_process->process_id);
      }
    }

    if(!isEmpty(queue2)){
      printf("\nMiddle Priority Queue:\n");
      printQueue(queue2);
      struct Process *current_process = dequeue(queue2);
      printf("Process %d dequeued\n", current_process->process_id);

      // Execute process to completion or for time slice
      if (current_process->remaining_time <= q2_time_slice){
        current_time += current_process->remaining_time;
        printf("Current Time: %d (Process %d executed to completion in Queue 2)\n", current_time, current_process->process_id);
        current_process->completion_time = current_time;
        total_burst_time -= current_process->remaining_time;
        current_process->remaining_time = 0;
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d, Turnaround Time: %d, Waiting Time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);

      printf("Process %d work, sum = %lld\n", current_process->process_id, current_process->work);
      }
      else{
        current_time += q2_time_slice;
        printf("Current Time: %d (Process %d preempted after time slice in Queue 2)\n", current_time, current_process->process_id);
        current_process->remaining_time -= q2_time_slice;
        current_process->completion_time = current_time;
        total_burst_time -= q2_time_slice;
      }

      // Check for new processes and add them to the ready queue or lower priority queues
      printf("\nChecking for new processes after execution in Queue 2\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (current_process->remaining_time > 0){
        int process_age = current_process->arrival_time + current_time;
        if(process_age > 10){
          enqueue(ready_queue, current_process);
          printf("Process %d moved to Queue 1 (Ready Queue)\n", current_process->process_id);
        }
        enqueue(queue3, current_process);
        printf("Process %d moved to Queue 3\n", current_process->process_id);
      }
    }

    if(!isEmpty(queue3)){
      printf("\nLowest Priority Queue:\n");
      printQueue(queue3);
      struct Process *current_process = dequeue(queue3);
      printf("Process %d dequeued\n", current_process->process_id);

      // Execute process to completion
      current_time += current_process->remaining_time;
      printf("Current Time: %d (Process %d executed to completion in Queue 3)\n", current_time, current_process->process_id);
      current_process->completion_time = current_time;
      total_burst_time -= current_process->remaining_time;
      current_process->remaining_time = 0;
      calculateTurnaroundTime(current_process);
      calculateWaitingTime(current_process);

      printf("Process %d completed at time %d, Turnaround Time: %d, Waiting Time: %d\n",
              current_process->process_id,
              current_process->completion_time,
              current_process->turnaround_time,
              current_process->waiting_time);
      printf("Process %d work, sum = %lld\n", current_process->process_id, current_process->work);
    }

    if(isEmpty(ready_queue) && isEmpty(queue2) && isEmpty(queue3)){
      printf("\nCurrent Time: %d\n", current_time);
      printf("Checking for new processes\n");
      for (int j = 0; j<num_processes; j++){
        if (processes[j].arrival_time <= current_time && processes[j].past == false){
          printf("Process %d arrived at time %d\n", processes[j].process_id, processes[j].arrival_time);
          processes[j].past = true;
          enqueue(ready_queue, &processes[j]);
        }
      }

      if (isEmpty(ready_queue)){
        current_time++;
        printf("No new processes have arrived, Current Time: %d\n", current_time);
      }

      if (total_burst_time <= 0){
        break;
      }

      printf("\nCurrent Queue: ");
      printQueue(ready_queue);
    }
  }

  printf("\nMulti-Level Feedback Queue Scheduling Complete\n");
  printf("===============================================\n");
}