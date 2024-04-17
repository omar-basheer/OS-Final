# Process Scheduler Simulator

## Description
This project is designed to simulate real-world process scheduling using the C programming language. It allows the user to create processes using the `fork()` system call and to choose from among four different scheduling algorithms to manage these processes. The available scheduling algorithms include:

- Round Robin (RR)
- Multi-Level Feedback Queue (MLFQ)
- Shortest Job First (SJF) Preemptive
- Shortest Job First (SJF) Non-Preemptive

Users can specify the number of processes they wish to simulate and select the scheduling algorithm to observe its behavior.

## Objectives
- Aptitude to demonstrate skills acquired during the previous sessions in both classroom and outside classroom environments.
- Ability to build and simulate OS system programs.
- Capacity to implement low level systems that solve modern-day real-world problems using different methodologies and algorithms.
- Techniques of testing system programs

## Features
- Simulation of process creation using the `fork()` system call.
- Four different CPU scheduling algorithms to choose from.
- Customizable number of processes to simulate.
- Observation of scheduling behavior and performance.

## Deliverables

- C code with clear comments and documentation.
- A Youtube link to the  simulation video demonstrating the program running correctly.

## How to Run?
- gcc -o my_program main.c process.c scheduler.c algorithms.c queue.c
- ./my_program  
- chmod +x run_project.sh  
- ./run_project.sh  

## YouTube Demo
https://youtu.be/VlBOXr7dI-A


## Contributors
- [Omar Basheer](https://github.com/omar-basheer)
- [Selom Caleb Arcmann-Ackummey](https://github.com/selomcaleb)
