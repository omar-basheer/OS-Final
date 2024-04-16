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
// void mlfq(struct Queue *queue, int q1_time_slice, int q2_time_slice)
// {
//   printf("Multi-Level Feedback Queue Scheduling\n");
//   struct Queue *Queue1 = queue;
//   struct Queue *Queue2 = initQueue();
//   struct Queue *Queue3 = initQueue();
//   int current_time = 0;

//   // while there are processes in any queue
//   while (!isEmpty(Queue1) || !isEmpty(Queue2) || !isEmpty(Queue3))
//   {
//     if (!isEmpty(Queue1))
//     {
//       printf("Now doing RR in Queue 1\n");
//       while (!isEmpty(Queue1))
//       {
//         struct Process *current_process = dequeue(Queue1);

//         // execute process for time slice or until completion
//         int remaining_time = current_process->remaining_time;
//         if (remaining_time <= q1_time_slice)
//         {
//           // process completes within queue 1 time slice
//           current_time = current_time + remaining_time;
//           current_process->completion_time = current_process->completion_time + remaining_time;

//           updateProcess(current_process, 0, current_process->completion_time);
//           calculateTurnaroundTime(current_process);
//           calculateWaitingTime(current_process);

//           printf("Process %d completed at time %d in queue 1, turnaround time: %d, waiting time: %d\n",
//                  current_process->process_id,
//                  current_process->completion_time,
//                  current_process->turnaround_time,
//                  current_process->waiting_time);
//         }
//         else
//         {
//           // process needs more time, move to lower queue
//           if (remaining_time < q1_time_slice)
//           {
//             current_time = current_time + q1_time_slice;
//           }
//           else
//           {
//             current_time = current_time + q1_time_slice;
//           }
//           current_process->completion_time = current_time;
//           remaining_time = remaining_time - q1_time_slice;
//           if (remaining_time < 0)
//           {
//             remaining_time = 0;
//           };
//           updateProcess(current_process, remaining_time, current_process->completion_time);
//           if (remaining_time <= q1_time_slice)
//           {
//             printf("Process %d preemted at time %d in queue 1, enqueued on same priority queue 1\n", current_process->process_id, current_process->completion_time);
//             enqueue(Queue1, current_process);
//           }
//           else
//           {
//             printf("Process %d preemted at time %d in queue 1, moved to lower priority queue 2\n", current_process->process_id, current_process->completion_time);
//             enqueue(Queue2, current_process);
//           }
//         }
//         // add a process that has been waiting in queue 2 for after 10 seconds
//         if (current_time == 8 && !isEmpty(Queue2))
//         {
//           printf("Process %d moved to queue, 1 after %d seconds\n", Queue2->head->process->process_id, current_time);
//           enqueue(Queue1, dequeue(Queue2));
//         }
//       }
//     }
//     else if (!isEmpty(Queue2))
//     {
//       printf("Now doing RR in Queue 2\n");
//       while (!isEmpty(Queue2))
//       {
//         struct Process *current_process = dequeue(Queue2);

//         // execute process for time slice or until completion
//         int remaining_time = current_process->remaining_time;
//         if (remaining_time <= q2_time_slice)
//         {
//           // process completes within queue 2 time slice
//           current_time = current_time + remaining_time;
//           current_process->completion_time = current_process->completion_time + remaining_time;

//           updateProcess(current_process, 0, current_process->completion_time);
//           calculateTurnaroundTime(current_process);
//           calculateWaitingTime(current_process);

//           printf("Process %d completed at time %d in queue 2, turnaround time: %d, waiting time: %d\n",
//                  current_process->process_id,
//                  current_process->completion_time,
//                  current_process->turnaround_time,
//                  current_process->waiting_time);
//         }
//         else
//         {
//           // process needs more time, move to lower queue
//           if (remaining_time < q2_time_slice)
//           {
//             current_time = current_time + q2_time_slice;
//           }
//           else
//           {
//             current_time = current_time + q2_time_slice;
//           }
//           current_process->completion_time = current_time;
//           remaining_time = remaining_time - q2_time_slice;
//           if (remaining_time < 0)
//           {
//             remaining_time = 0;
//           };
//           updateProcess(current_process, remaining_time, current_process->completion_time);
//           if (remaining_time <= q2_time_slice)
//           {
//             printf("Process %d preemted at time %d in queue 2, enqueued on same priority queue 2\n", current_process->process_id, current_process->completion_time);
//             enqueue(Queue2, current_process);
//           }
//           else
//           {
//             printf("Process %d preemted at time %d in queue 2, moved to lower priority queue 3\n", current_process->process_id, current_process->completion_time);
//             enqueue(Queue3, current_process);
//           }
//         }
//       }
//     }
//     else if (!isEmpty(Queue3))
//     {
//       // execute processes until completion (first come first serve)
//       printf("Now doing FCFS in Queue 3\n");
//       while (!isEmpty(Queue3))
//       {
//         struct Process *current_process = dequeue(Queue3);
//         int remaining_time = current_process->remaining_time;
//         current_time = current_time + remaining_time;
//         current_process->completion_time = current_process->completion_time + remaining_time;

//         updateProcess(current_process, 0, current_process->completion_time);
//         calculateTurnaroundTime(current_process);
//         calculateWaitingTime(current_process);

//         printf("Process %d completed at time %d in queue 3, turnaround time: %d, waiting time: %d\n",
//                current_process->process_id,
//                current_process->completion_time,
//                current_process->turnaround_time,
//                current_process->waiting_time);
//       }
//     }
//   }
// }

// void preemptive_sjf(struct CPU_Scheduler *scheduler)
// {
//   printf("Preemptive Shortest Job First Scheduling\n");
//   int currentTime = 0;
//   struct Process *currentProcess = NULL;
//   int isCPUIdle = 1;

//   printf("Starting Preemptive SJF Simulation\n");

//   while (!isEmpty(scheduler->ready_queue))
//   {
//     // Check and enqueue newly arrived processes
//     for (int i = 0; i < scheduler->num_processes; i++)
//     {
//       if (scheduler->processes[i].arrival_time == currentTime)
//       {
//         printf("Time %d: Process %d arrives\n", currentTime, scheduler->processes[i].process_id);
//         enqueue(scheduler->ready_queue, &scheduler->processes[i]);
//       }
//     }

//     // Decide on preemption or continuation of the current process
//     if (!isCPUIdle && !isEmpty(scheduler->ready_queue) &&
//         peek(scheduler->ready_queue)->remaining_time < currentProcess->remaining_time)
//     {
//       if (peek(scheduler->ready_queue)->remaining_time > 0)
//       {
//         printf("Time %d: Process %d preempted by Process %d\n", currentTime, currentProcess->process_id, peek(scheduler->ready_queue)->process_id);
//         currentProcess->remaining_time -= (currentTime - currentProcess->last_start_time);
//         enqueue(scheduler->ready_queue, currentProcess);
//         isCPUIdle = 1;
//       }

//       else
//       {
//         enqueue(scheduler->ready_queue, currentProcess);
//         currentProcess = dequeue(scheduler->ready_queue);
//         printf("Time %d: Process %d starts execution\n", currentTime, currentProcess->process_id);
//         currentProcess->last_start_time = currentTime;
//       }
//     }

//     if (isCPUIdle && !isEmpty(scheduler->ready_queue))
//     {
//       currentProcess = dequeue(scheduler->ready_queue);
//       if (currentProcess->remaining_time > 0)
//       {
//         printf("Time %d: Process %d starts execution\n", currentTime, currentProcess->process_id);
//         currentProcess->last_start_time = currentTime;
//         isCPUIdle = 0;
//       }
//     }

//     // Process execution for a time unit
//     if (!isCPUIdle)
//     {
//       currentProcess->remaining_time--;
//       if (currentProcess->remaining_time <= 0)
//       {
//         printf("Time %d: Process %d completes execution\n", currentTime + 1, currentProcess->process_id);
//         currentProcess->completion_time = currentTime + 1;
//         calculateTurnaroundTime(currentProcess);
//         calculateWaitingTime(currentProcess);
//         printf("Process %d turnaround time: %d, waiting time: %d\n", currentProcess->process_id, currentProcess->turnaround_time, currentProcess->waiting_time);
//         isCPUIdle = 1;
//         currentProcess = NULL;
//       }
//     }

//     currentTime++;
//   }

//   printf("Preemptive SJF Simulation Complete\n");
// }
