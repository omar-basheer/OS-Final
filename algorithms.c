#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"
#include "scheduler.h"

#define TIME_SLICE 2

void round_robin(struct Queue *ready_queue)
{
  int current_time = 0;
  while (!isEmpty(ready_queue))
  {
    struct Process *current_process = dequeue(ready_queue);

    int remaining_time = current_process->remaining_time;
    if (remaining_time <= 0)
    {
      // process completes within time slice
      current_time = current_time + remaining_time;
      current_process->completion_time = current_process->completion_time + remaining_time;

      updateProcess(current_process, 0, current_process->completion_time);
      calculateTurnaroundTime(current_process);
      calculateWaitingTime(current_process);

      printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
             current_process->process_id,
             current_process->completion_time,
             current_process->turnaround_time,
             current_process->waiting_time);
    }
    else
    {
      // process needs more time, stop execution at time slice
      if (remaining_time < TIME_SLICE)
      {
        current_time = current_time + remaining_time;
      }
      else
      {
        current_time = current_time + TIME_SLICE;
      }
      current_process->completion_time = current_time;
      remaining_time = remaining_time - TIME_SLICE;
      if (remaining_time < 0)
      {
        remaining_time = 0;
      };
      updateProcess(current_process, remaining_time, current_process->completion_time);
      printf("Process %d preemted at time %d\n", current_process->process_id, current_process->completion_time);
      enqueue(ready_queue, current_process);
    }
  }
};

void np_sjf(struct Queue *ready_queue)
{
  int current_time = 0;

  while (!isEmpty(ready_queue))
  {
    // Find the process with the shortest remaining burst time
    struct Process *shortest_process = NULL;
    struct QueueNode *current_node = ready_queue->head;

    while (current_node != NULL)
    {
      struct Process *current_process = current_node->process;

      // Check if the process has arrived and its burst time is shorter than the current shortest burst time
      if (current_process->arrival_time <= current_time &&
          (shortest_process == NULL || current_process->burst_time < shortest_process->burst_time))
      {
        shortest_process = current_process;
      }

      current_node = current_node->next;
    }

    if (shortest_process == NULL)
    {
      // No process is ready to execute, increment current time
      current_time++;
    }
    else
    {
      // Execute the shortest process
      dequeueProcess(ready_queue, shortest_process);
      shortest_process->completion_time = current_time + shortest_process->burst_time;
      calculateTurnaroundTime(shortest_process);
      calculateWaitingTime(shortest_process);

      printf("Process %d completed at time %d, turnaround time: %d, waiting time: %d\n",
             shortest_process->process_id,
             shortest_process->completion_time,
             shortest_process->turnaround_time,
             shortest_process->waiting_time);

      current_time = shortest_process->completion_time;
    }
  }
}

void mlfq(struct Queue *queue, int q1_time_slice, int q2_time_slice)
{
  struct Queue *Queue1 = queue;
  struct Queue *Queue2 = initQueue();
  struct Queue *Queue3 = initQueue();
  int current_time = 0;

  // while there are processes in any queue
  while (!isEmpty(Queue1) || !isEmpty(Queue2) || !isEmpty(Queue3))
  {
    if (!isEmpty(Queue1))
    {
      printf("Now doing RR in Queue 1\n");
      while (!isEmpty(Queue1))
      {
        struct Process *current_process = dequeue(Queue1);

        // execute process for time slice or until completion
        int remaining_time = current_process->remaining_time;
        if (remaining_time <= q1_time_slice)
        {
          // process completes within queue 1 time slice
          current_time = current_time + remaining_time;
          current_process->completion_time = current_process->completion_time + remaining_time;

          updateProcess(current_process, 0, current_process->completion_time);
          calculateTurnaroundTime(current_process);
          calculateWaitingTime(current_process);

          printf("Process %d completed at time %d in queue 1, turnaround time: %d, waiting time: %d\n",
                 current_process->process_id,
                 current_process->completion_time,
                 current_process->turnaround_time,
                 current_process->waiting_time);
        }
        else
        {
          // process needs more time, move to lower queue
          if (remaining_time < q1_time_slice)
          {
            current_time = current_time + q1_time_slice;
          }
          else
          {
            current_time = current_time + q1_time_slice;
          }
          current_process->completion_time = current_time;
          remaining_time = remaining_time - q1_time_slice;
          if (remaining_time < 0)
          {
            remaining_time = 0;
          };
          updateProcess(current_process, remaining_time, current_process->completion_time);
          if (remaining_time <= q1_time_slice)
          {
            printf("Process %d preemted at time %d in queue 1, enqueued on same priority queue 1\n", current_process->process_id, current_process->completion_time);
            enqueue(Queue1, current_process);
          }
          else
          {
            printf("Process %d preemted at time %d in queue 1, moved to lower priority queue 2\n", current_process->process_id, current_process->completion_time);
            enqueue(Queue2, current_process);
          }
        }
        // add a process that has been waiting in queue 2 for after 10 seconds
        if (current_time == 8 && !isEmpty(Queue2))
        {
          printf("Process %d moved to queue, 1 after %d seconds\n", Queue2->head->process->process_id, current_time);
          enqueue(Queue1, dequeue(Queue2));
        }
      }
    }
    else if (!isEmpty(Queue2))
    {
      printf("Now doing RR in Queue 2\n");
      while (!isEmpty(Queue2))
      {
        struct Process *current_process = dequeue(Queue2);

        // execute process for time slice or until completion
        int remaining_time = current_process->remaining_time;
        if (remaining_time <= q2_time_slice)
        {
          // process completes within queue 2 time slice
          current_time = current_time + remaining_time;
          current_process->completion_time = current_process->completion_time + remaining_time;

          updateProcess(current_process, 0, current_process->completion_time);
          calculateTurnaroundTime(current_process);
          calculateWaitingTime(current_process);

          printf("Process %d completed at time %d in queue 2, turnaround time: %d, waiting time: %d\n",
                 current_process->process_id,
                 current_process->completion_time,
                 current_process->turnaround_time,
                 current_process->waiting_time);
        }
        else
        {
          // process needs more time, move to lower queue
          if (remaining_time < q2_time_slice)
          {
            current_time = current_time + q2_time_slice;
          }
          else
          {
            current_time = current_time + q2_time_slice;
          }
          current_process->completion_time = current_time;
          remaining_time = remaining_time - q2_time_slice;
          if (remaining_time < 0)
          {
            remaining_time = 0;
          };
          updateProcess(current_process, remaining_time, current_process->completion_time);
          if (remaining_time <= q2_time_slice)
          {
            printf("Process %d preemted at time %d in queue 2, enqueued on same priority queue 2\n", current_process->process_id, current_process->completion_time);
            enqueue(Queue2, current_process);
          }
          else
          {
            printf("Process %d preemted at time %d in queue 2, moved to lower priority queue 3\n", current_process->process_id, current_process->completion_time);
            enqueue(Queue3, current_process);
          }
        }
      }
    }
    else if (!isEmpty(Queue3))
    {
      // execute processes until completion (first come first serve)
      printf("Now doing FCFS in Queue 3\n");
      while (!isEmpty(Queue3))
      {
        struct Process *current_process = dequeue(Queue3);
        int remaining_time = current_process->remaining_time;
        current_time = current_time + remaining_time;
        current_process->completion_time = current_process->completion_time + remaining_time;

        updateProcess(current_process, 0, current_process->completion_time);
        calculateTurnaroundTime(current_process);
        calculateWaitingTime(current_process);

        printf("Process %d completed at time %d in queue 3, turnaround time: %d, waiting time: %d\n",
               current_process->process_id,
               current_process->completion_time,
               current_process->turnaround_time,
               current_process->waiting_time);
      }
    }
  }
}

void runPreemptiveSJF(struct CPU_Scheduler *scheduler)
{
  int currentTime = 0;
  struct Process *currentProcess = NULL;
  int isCPUIdle = 1;

  printf("Starting Preemptive SJF Simulation\n");

  while (!isEmpty(scheduler->ready_queue))
  {
    // Check and enqueue newly arrived processes
    for (int i = 0; i < scheduler->num_processes; i++)
    {
      if (scheduler->processes[i].arrival_time == currentTime)
      {
        printf("Time %d: Process %d arrives\n", currentTime, scheduler->processes[i].process_id);
        enqueue(scheduler->ready_queue, &scheduler->processes[i]);
      }
    }

    // Decide on preemption or continuation of the current process
    if (!isCPUIdle && !isEmpty(scheduler->ready_queue) &&
        peek(scheduler->ready_queue)->remaining_time < currentProcess->remaining_time)
    {
      if (peek(scheduler->ready_queue)->remaining_time > 0)
      {
        printf("Time %d: Process %d preempted by Process %d\n", currentTime, currentProcess->process_id, peek(scheduler->ready_queue)->process_id);
        currentProcess->remaining_time -= (currentTime - currentProcess->last_start_time);
        enqueue(scheduler->ready_queue, currentProcess);
        isCPUIdle = 1;
      }

      else
      {
        enqueue(scheduler->ready_queue, currentProcess);
        currentProcess = dequeue(scheduler->ready_queue);
        printf("Time %d: Process %d starts execution\n", currentTime, currentProcess->process_id);
        currentProcess->last_start_time = currentTime;
      }
    }

    if (isCPUIdle && !isEmpty(scheduler->ready_queue))
    {
      currentProcess = dequeue(scheduler->ready_queue);
      if (currentProcess->remaining_time > 0)
      {
        printf("Time %d: Process %d starts execution\n", currentTime, currentProcess->process_id);
        currentProcess->last_start_time = currentTime;
        isCPUIdle = 0;
      }
    }

    // Process execution for a time unit
    if (!isCPUIdle)
    {
      currentProcess->remaining_time--;
      if (currentProcess->remaining_time <= 0)
      {
        printf("Time %d: Process %d completes execution\n", currentTime + 1, currentProcess->process_id);
        currentProcess->completion_time = currentTime + 1;
        calculateTurnaroundTime(currentProcess);
        calculateWaitingTime(currentProcess);
        printf("Process %d turnaround time: %d, waiting time: %d\n", currentProcess->process_id, currentProcess->turnaround_time, currentProcess->waiting_time);
        isCPUIdle = 1;
        currentProcess = NULL;
      }
    }

    currentTime++;
  }

  printf("Preemptive SJF Simulation Complete\n");
}
