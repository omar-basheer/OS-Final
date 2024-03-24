#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"


void priority_scheduler(struct Queue* queue){

    if(isEmpty(queue)){
        printf("Queue is empty. Cannot schedule.\n");
        return;
    };

    struct Queue* tempQueue = initQueue();
    struct Process* currentProcess = NULL;
    struct Process* highestPriorityProcess = NULL;
    int totalProcessses = queue ->size;
    int currentTime = 0;

    while (totalProcessses > 0){
        while(!isEmpty(queue) && queue->head->process->arrival_time <= currentTime){
            currentProcess = dequeue(queue);
            enqueue(tempQueue, currentProcess);
        }

        highestPriorityProcess = NULL;
        struct QueueNode* tempNode = tempQueue->head;

        while (tempNode != NULL){
            if(highestPriorityProcess == NULL || tempNode->process->priority < highestPriorityProcess->priority){
                highestPriorityProcess = tempNode ->process;
            }
            tempNode = tempNode->next;
        }

        if(highestPriorityProcess != NULL){
            int remainingTime = highestPriorityProcess->remaining_time - 1;
            currentTime++;
            if(remainingTime == 0){
                highestPriorityProcess->completion_time = currentTime;
                calculateTurnaroundTime(highestPriorityProcess);
                calculateWaitingTime(highestPriorityProcess, currentTime);
                totalProcessses--;
            }
            else{
                highestPriorityProcess->remaining_time = remainingTime;
                enqueue(tempQueue, highestPriorityProcess);
            }
        }

        else{
            currentTime++;
        }

    }


}



void shortestTimeRemaining(struct Queue* queue){
    
}




