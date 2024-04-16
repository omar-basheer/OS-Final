#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "algorithms.h"

int main(){
    // Simulating Round Robin Scheduling
    // int num_processes1 = 4;
    // int arrival_times1[] = {0, 1, 3, 5};
    // int burst_times1[] = {6, 3, 7, 4};
    // int priorities1[] = {0, 0, 0, 0};
    // struct CPU_Scheduler* scheduler1 = initScheduler(num_processes1, arrival_times1, burst_times1, priorities1);
    // printProcesses(scheduler1);
    // round_robin(scheduler1->ready_queue);

    // // Simulating Shortest Job First Scheduling
    // int num_processes2 = 4;
    // int arrival_times2[] = {0, 1, 2, 3};
    // int burst_times2[] = {6, 3, 7, 4};
    // int priorities2[] = {0, 0, 0, 0};
    // struct CPU_Scheduler* scheduler2 = initScheduler(num_processes2, arrival_times2, burst_times2, priorities2);
    // printProcesses(scheduler2);
    // np_sjf(scheduler2->ready_queue);



// Simulating Shortest Job First Scheduling
    int num_processes3 = 3;
    int arrival_times3[] = {0, 1,2};
    int burst_times3[] = {6, 4,2};
    int priorities3[] = {0, 0};
    struct CPU_Scheduler* scheduler3 = initScheduler(num_processes3, arrival_times3, burst_times3, priorities3);
    printProcesses(scheduler3);
    runPreemptiveSJF(scheduler3);
    // Simulating Multi-Level Feedback Queue Scheduling
    // int num_processes4 = 4;
    // int arrival_times4[] = {0, 1, 3, 5};
    // int burst_times4[] = {6, 3, 7, 4};
    // int priorities4[] = {0, 0, 0, 0};
    // int q1_time_slice = 2;
    // int q2_time_slice = 4;
    // struct CPU_Scheduler* scheduler4 = initScheduler(num_processes4, arrival_times4, burst_times4, priorities4);
    // printProcesses(scheduler4);
    // mlfq(scheduler4->ready_queue, q1_time_slice, q2_time_slice);

}