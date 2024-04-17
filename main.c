#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "algorithms.h"
#include <time.h>


int main(){
    srand(time(NULL));
    int num_processes1;
    int arrival_times1[100];
    int burst_times1[100];
    int time_slice1;

    int num_processes2;
    int arrival_times2[100];
    int burst_times2[100];

    int num_processes3;
    int arrival_times3[100];
    int burst_times3[100];

    int num_processes4;
    int arrival_times4[100];
    int burst_times4[100];
    int time_slice4_1;
    int time_slice4_2;

    printf("\n");
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("Welcome to the CPU Scheduler Simulator!\n");
    printf("\n");
    printf("Please open your terminal to at least half of your screen to view the output properly.\n");
    printf("\n");
    int choice;
    // Interactive menu
    printf("Select a scheduling algorithm to simulate:\n");
    printf("1. Round Robin\n");
    printf("2. Shortest Job First (Non-Preemptive)\n");
    printf("3. Shortest Job First (Preemptive)\n");
    printf("4. Multi-Level Feedback Queue\n");
    printf("\n");

    printf("Enter your choice (enter the corresponding number): ");
    scanf("%d", &choice);
    printf("\n");

    switch(choice) {
        case 1:
            // Round Robin
            printf("You selected Round Robin Scheduling.\n");
            printf("\n");

            // Get inputs for Round Robin parameters
            // Number of processes
            printf("Enter the number of processes: ");
            scanf("%d", &num_processes1);
            printf("\n");
            
            // Arrival times and burst times for each process
            printf("Enter arrival times for each process:\n");
            for (int i = 0; i < num_processes1; i++) {
                printf("Arrival time for process %d: ", i+1);
                scanf("%d", &arrival_times1[i]);
            }
            printf("\n");

            printf("Enter burst times for each process:\n");
            for (int i = 0; i < num_processes1; i++) {
                printf("Burst time for process %d: ", i+1);
                scanf("%d", &burst_times1[i]);
            }
            printf("\n");

            // Time slice
            printf("Enter time slice: ");
            scanf("%d", &time_slice1);
            printf("\n");
            printf("\n");

            // Run Round Robin algorithm
            printf("Running Round Robin algorithm...\n");
            printf("\n");
            struct CPU_Scheduler* scheduler1 = initScheduler(num_processes1, arrival_times1, burst_times1);
            printf("\n");
            printf("Initial Processes:\n");
            printProcesses(scheduler1);
            round_robin(scheduler1->processes, num_processes1, time_slice1);
            printf("\n");
            printf("Final Processes:\n");
            printProcesses(scheduler1);
            printf("\n");
            printf("Algorithm metrics:\n");
            printf("\n");
            calculateMetrics(scheduler1->processes, num_processes1);
            printf("--------------------------------------------------------------------------------------------------------------\n");
            break;

        case 2:
            printf("You selected Shortest Job First Scheduling (Non-Preemptive).\n");
            printf("\n");

            // Get inputs for Shortest Job First parameters
            // Number of processes
            printf("Enter the number of processes: ");
            scanf("%d", &num_processes2);
            printf("\n");

            // Arrival times and burst times for each process
            printf("Enter arrival times for each process:\n");
            for (int i = 0; i < num_processes2; i++) {
                printf("Arrival time for process %d: ", i+1);
                scanf("%d", &arrival_times2[i]);
            }
            printf("\n");

            printf("Enter burst times for each process:\n");
            for (int i = 0; i < num_processes2; i++) {
                printf("Burst time for process %d: ", i+1);
                scanf("%d", &burst_times2[i]);
            }
            printf("\n");

            // Run Shortest Job First algorithm
            printf("Running Shortest Job First (Non-Preemptive) algorithm...\n");
            printf("\n");
            struct CPU_Scheduler* scheduler2 = initScheduler(num_processes2, arrival_times2, burst_times2);
            printf("\n");
            printf("Initial Processes:\n");
            printProcesses(scheduler2);
            np_sjf(scheduler2->processes, num_processes2);
            printf("\n");
            printf("Final Processes:\n");
            printProcesses(scheduler2);
            printf("\n");
            printf("Algorithm metrics:\n");
            printf("\n");
            calculateMetrics(scheduler2->processes, num_processes2);
            printf("--------------------------------------------------------------------------------------------------------------\n");
            break;

        case 3:
            printf("You selected Shortest Job First Scheduling (Preemptive).\n");
            printf("\n");

            // Get inputs for Shortest Job First parameters
            // Number of processes
            printf("Enter the number of processes: ");
            scanf("%d", &num_processes3);
            printf("\n");

            // Arrival times and burst times for each process
            printf("Enter arrival times for each process:\n");
            for (int i = 0; i < num_processes3; i++) {
                printf("Arrival time for process %d: ", i+1);
                scanf("%d", &arrival_times3[i]);
            }
            printf("\n");

            printf("Enter burst times for each process:\n");
            for (int i = 0; i < num_processes3; i++) {
                printf("Burst time for process %d: ", i+1);
                scanf("%d", &burst_times3[i]);
            }
            printf("\n");

            // Run Shortest Job First algorithm
            printf("Running Shortest Job First (Preemptive) algorithm...\n");
            printf("\n");
            struct CPU_Scheduler* scheduler3 = initScheduler(num_processes3, arrival_times3, burst_times3);
            printf("\n");
            printf("Initial Processes:\n");
            printProcesses(scheduler3);
            pp_sjf(scheduler3->processes, num_processes3);
            printf("\n");
            printf("Final Processes:\n");
            printProcesses(scheduler3);
            printf("\n");
            printf("Algorithm metrics:\n");
            printf("\n");
            calculateMetrics(scheduler3->processes, num_processes3);
            printf("--------------------------------------------------------------------------------------------------------------\n");
            break;

        case 4:
            printf("You selected Multi-Level Feedback Queue Scheduling.\n");
            printf("\n");

            // Get inputs for Multi-Level Feedback Queue parameters
            // Number of processes
            printf("Enter the number of processes: ");
            scanf("%d", &num_processes4);
            printf("\n");

            // Arrival times and burst times for each process
            printf("Enter arrival times for each process:\n");
            for (int i = 0; i < num_processes4; i++) {
                printf("Arrival time for process %d: ", i+1);
                scanf("%d", &arrival_times4[i]);
            }
            printf("\n");

            printf("Enter burst times for each process:\n");
            for (int i = 0; i < num_processes4; i++) {
                printf("Burst time for process %d: ", i+1);
                scanf("%d", &burst_times4[i]);
            }
            printf("\n");

            // Time slices for each queue
            printf("Enter time slice for Queue 1: ");
            scanf("%d", &time_slice4_1);
            printf("\n");

            printf("Enter time slice for Queue 2: ");
            scanf("%d", &time_slice4_2);
            printf("\n");

            // Run Multi-Level Feedback Queue algorithm
            printf("Running Multi-Level Feedback Queue algorithm...\n");
            printf("\n");
            struct CPU_Scheduler* scheduler4 = initScheduler(num_processes4, arrival_times4, burst_times4);
            printf("\n");
            printf("Initial Processes:\n");
            printProcesses(scheduler4);
            mlfq(scheduler4->processes, num_processes4, time_slice4_1, time_slice4_2);
            printf("\n");
            printf("Final Processes:\n");
            printProcesses(scheduler4);
            printf("\n");
            printf("Algorithm metrics:\n");
            printf("\n");
            calculateMetrics(scheduler4->processes, num_processes4);
            printf("--------------------------------------------------------------------------------------------------------------\n");
            break;

        default:
            printf("Invalid choice. Exiting...\n");
            printf("--------------------------------------------------------------------------------------------------------------\n");
            exit(EXIT_FAILURE);
    }


}