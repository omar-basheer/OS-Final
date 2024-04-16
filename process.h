#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Process {
  int process_id;
  int arrival_time;
  int burst_time;
  int priority;
  int remaining_time;
  int completion_time;
  int turnaround_time;
  int waiting_time;
  int start_time;
  bool past;
};


struct Process* createProcess(int process_id, int arrival_time, int burst_time, int priority);
void updateProcess(struct Process*, int remaining_time, int completion_time);
void calculateTurnaroundTime(struct Process* process);
void calculateWaitingTime(struct Process* process);
void calculateMetrics(struct Process* processes, int num_processes);
#endif /* PROCESS_H */
