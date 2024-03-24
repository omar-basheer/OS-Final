#include <stdio.h>
#include <stdlib.h>

#include "process.h"

struct Process* createProcess(int process_id, int arrival_time, int burst_time, int priority){
  struct Process* process = (struct Process*)malloc(sizeof(struct Process));
  if (process == NULL) {
    printf("Memory allocation failed for process.\n");
    exit(EXIT_FAILURE);
  }
  process->process_id = process_id;
  process->arrival_time = arrival_time;
  process->burst_time = burst_time;
  process->priority = priority;
  process->remaining_time = burst_time; // Initially, remaining time is same as burst time
  process->completion_time = 0; // Initialize completion time to 0
  process->turnaround_time = 0; // Initialize turnaround time to 0
  process->waiting_time = 0; // Initialize waiting time to 0

  return process;
};

void updateProcess(struct Process* process, int remaining_time, int completion_time) {
    process->remaining_time = remaining_time;
    process->completion_time = completion_time;
}

void calculateTurnaroundTime(struct Process* process) {
    process->turnaround_time = process->completion_time - process->arrival_time;
}

void calculateWaitingTime(struct Process* process, int total_time) {
    process->waiting_time = process->turnaround_time - process->burst_time;
}