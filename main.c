#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "algorithms.h"

int main(){
    // Simulating Round Robin Scheduling
    int num_processes1 = 5;
    int arrival_times1[] = {0, 1, 2, 3, 4};
    int burst_times1[] = {5, 3, 1, 2, 3};
    int priorities1[] = {0, 0, 0, 0};
    struct CPU_Scheduler* scheduler1 = initScheduler(num_processes1, arrival_times1, burst_times1, priorities1);
    printProcesses(scheduler1);
    round_robin(scheduler1->processes,5, 2);
    printProcesses(scheduler1);
    calculateMetrics(scheduler1->processes, num_processes1);

    // Simulating Shortest Job First Scheduling
    // int num_processes2 = 5;
    // int arrival_times2[] = {1, 2, 3, 4, 5};
    // int burst_times2[] = {7, 5, 1, 2, 8};
    // int priorities2[] = {0, 0, 0, 0};
    // struct CPU_Scheduler* scheduler2 = initScheduler(num_processes2, arrival_times2, burst_times2, priorities2);
    // printProcesses(scheduler2);
    // np_sjf(scheduler2->ready_queue);
    // printProcesses(scheduler2);
    // calculateMetrics(scheduler2->processes, num_processes2);

    // Simulating Shortest Job First Scheduling
    // int num_processes3 = 4;
    // // int arrival_times3[] = {2, 5, 1, 0, 4};
    // int arrival_times3[] = {0, 1, 2, 3};
    // int burst_times3[] = {6, 4, 2, 3};
    // int priorities3[] = {0, 0, 0, 0};
    // struct CPU_Scheduler* scheduler3 = initScheduler(num_processes3, arrival_times3, burst_times3, priorities3);
    // printProcesses(scheduler3);
    // preemptive_sjf(scheduler3);
    // printProcesses(scheduler3);
    // calculateMetrics(scheduler3->processes, num_processes3);


    // Simulating Multi-Level Feedback Queue Scheduling
    // int num_processes4 = 4;
    // int arrival_times4[] = {0, 1, 2, 3};
    // int burst_times4[] = {5, 3, 8, 6};
    // int priorities4[] = {0, 0, 0, 0};
    // int q1_time_slice = 2;
    // int q2_time_slice = 4;
    // struct CPU_Scheduler* scheduler4 = initScheduler(num_processes4, arrival_times4, burst_times4, priorities4);
    // printProcesses(scheduler4);
    // mlfq(scheduler4->ready_queue, q1_time_slice, q2_time_slice);
    // calculateMetrics(scheduler4->processes, num_processes4);

}