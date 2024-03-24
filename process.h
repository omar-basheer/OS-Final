#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include <stdlib.h>

struct Process {
  int process_id;
  int arrival_time;
  int burst_time;
  int priority;
  int remaining_time;
  int completion_time;
  int turnaround_time;
  int waiting_time;
};



#endif /* PROCESS_H */
