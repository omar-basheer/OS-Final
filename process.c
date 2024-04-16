#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "process.h"

struct Process* createProcess(int process_id, int arrival_time, int burst_time){
  struct Process* process = (struct Process*)malloc(sizeof(struct Process));
//   if (process == NULL) {
//     printf("Memory allocation failed for process.\n");
//     exit(EXIT_FAILURE);
//   }

  process->process_id = fork();
  process->arrival_time = arrival_time;
  process->burst_time = burst_time;
  process->remaining_time = burst_time; // Initially, remaining time is same as burst time
  process->completion_time = 0; // Initialize completion time to 0
  process->turnaround_time = 0; // Initialize turnaround time to 0
  process->waiting_time = 0; // Initialize waiting time to 0
  process->last_start_time = 0; // Initialize waiting time to 0
  process->past = false; 

if (process->process_id == -1){
    printf("Fork failed.\n");
    exit(EXIT_FAILURE);
  }
  else if(process->process_id == 0){
    exit(EXIT_SUCCESS);
  }
  else{
    return process;
  }
  return NULL;
};


/**
 * Updates the process with the given remaining time and completion time.
 *
 * @param process The process to be updated.
 * @param remaining_time The new remaining time for the process.
 * @param completion_time The new completion time for the process.
 */
void updateProcess(struct Process* process, int remaining_time, int completion_time) {
    process->remaining_time = remaining_time;
    process->completion_time = completion_time;
}

/**
 * Calculates the turnaround time for a given process.
 * The turnaround time is the difference between the completion time and the arrival time.
 *
 * @param process The process for which to calculate the turnaround time.
 */
void calculateTurnaroundTime(struct Process* process) {
    process->turnaround_time = process->completion_time - process->arrival_time;
}

/**
 * Calculates the waiting time for a given process.
 * The waiting time is calculated by subtracting the burst time from the turnaround time.
 *
 * @param process The process for which to calculate the waiting time.
 */
void calculateWaitingTime(struct Process* process) {
    process->waiting_time = process->turnaround_time - process->burst_time;
}

/**
 * Calculates various metrics for a given array of processes.
 *
 * @param processes The array of processes.
 * @param num_processes The number of processes in the array.
 */
void calculateMetrics(struct Process* processes, int num_processes) {
    printf("\n");
    printf("Metrics:\n");
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    int total_context_switches = 0;
    int total_deadline_misses = 0;

    int total_burst_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_burst_time += processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
        total_response_time += processes[i].waiting_time; // Assuming response time is same as waiting time

        // Calculate context switches (assuming each process starts and ends with a context switch)
        if (i == 0 || i == num_processes - 1) {
            total_context_switches += 1; // Process start or end
        } else {
            if (processes[i].arrival_time != processes[i-1].completion_time) {
                total_context_switches += 1; // Context switch needed
            }
        }

        // Calculate deadline misses (assuming completion time > arrival time + burst time)
        if (processes[i].completion_time > processes[i].arrival_time + processes[i].burst_time) {
            total_deadline_misses += 1;
        }
    }

    float average_turnaround_time = total_turnaround_time / num_processes;
    float average_waiting_time = total_waiting_time / num_processes;
    float average_response_time = total_response_time / num_processes;
    float cpu_utilization = (total_burst_time / total_turnaround_time) * 100;
    float average_context_switches = (float)total_context_switches / num_processes;
    float deadline_miss_ratio = (float)total_deadline_misses / num_processes;

    printf("Average Turnaround Time: %.2f\n", average_turnaround_time);
    printf("Average Waiting Time: %.2f\n", average_waiting_time);
    printf("Average Response Time: %.2f\n", average_response_time);
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
    printf("Average Context Switches: %.2f\n", average_context_switches);
    printf("Deadline Miss Ratio: %.2f\n", deadline_miss_ratio);
    printf("\n");
}
