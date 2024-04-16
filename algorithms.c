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

// void mlfq (struct Process *processes, int num+processes){
//   struct Queue *
// }
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



void shortestJobFirstPreemptive(struct Process *processes, int num_processes){
    struct Queue *ready_queue = initQueue();
    int current_time = 0;
    int total_burst_time = 0;
    struct Process *current_process = NULL;

    for(int i = 1; i < num_processes-1; i++){
    total_burst_time += processes[i].burst_time;

    }

    printf("Total burst time is %d\n",total_burst_time);




    // Initially, mark all processes as not past and set their remaining times
    for (int i = 0; i < num_processes; i++) {
        processes[i].past = false;
        processes[i].remaining_time = processes[i].burst_time;  // Ensure remaining_time is properly initialized
    }

    while (total_burst_time > 0) {
        // Enqueue new arrivals
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == current_time && !processes[i].past) {
                processes[i].past = true;
                enqueue(ready_queue, &processes[i]);
                printf("Process %d arrived at time %d\n", processes[i].process_id, current_time);
            }
        }

        // Preempt if needed
        if (current_process != NULL && !isEmpty(ready_queue) && peek(ready_queue)->remaining_time < current_process->remaining_time) {
            enqueue(ready_queue, current_process);
            printf("Process %d preempted at time %d with %d remaining time\n", current_process->process_id, current_time, current_process->remaining_time);
            current_process = NULL;
        }

        // Select process to run
        if (current_process == NULL || current_process->remaining_time <= 0) {
            if (!isEmpty(ready_queue)) {
                current_process = dequeue(ready_queue);
                printf("Process %d starts running at time %d\n", current_process->process_id, current_time);
            }
        }

        // Execute current process
        if (current_process != NULL) {
          total_burst_time --;
            current_process->remaining_time--;
            if (current_process->remaining_time == 0) {
                current_process->completion_time = current_time + 1;
                calculateTurnaroundTime(current_process);
                calculateWaitingTime(current_process);
                printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
                       current_process->process_id,
                       current_process->completion_time,
                       current_process->turnaround_time,
                       current_process->waiting_time);
                current_process = NULL;
            }
        } else if (isEmpty(ready_queue)) {
            // If no process is ready and no new arrivals, advance time
            printf("No processes are running or waiting at time %d\n", current_time);
            bool all_done = true;
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].remaining_time > 0) {
                    all_done = false;
                    break;
                }
            }
            if (all_done) break;
        }

        current_time++;
    }
    printf("\n");
    printf("Preemptive Shortest Job First Scheduling Complete\n");
}
