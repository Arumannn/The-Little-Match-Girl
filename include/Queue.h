#ifndef QUEUE_H
#define QUEUE_H
#define MAX 6
#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#include "Stack.h"





typedef struct{
    Data data[MAX];
    int front;
    int rear;
}Queue;



void CreateQueue (Queue *Q);
void EnQueue (Queue *Q, Data X);
void DeQueue (Queue *Q, Data *X);
//DEBUGING QUEUE
bool isQueueEmpty(Queue Q);
bool isQueueFull(Queue Q);
int QueueSize(Queue Q);
void PrintQueue(Queue S);

#endif