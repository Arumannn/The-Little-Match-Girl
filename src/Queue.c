#include "queue.h"


//===================================== QUEUE GAME QTE =====================================
void CreateQueue (Queue *Q){
    Q->front = -1;
    Q->rear = -1;
}
bool isQueueEmpty(Queue Q){
    return (Q.front == -1) && (Q.rear == -1);
}
bool isQueueFull(Queue Q){
    return (Q.front == (Q.rear + 1) % MAX);
}
int QueueSize(Queue Q){
    return Q.rear - 1;
}
void EnQueue (Queue *Q, Data X){
    if (!isQueueFull(*Q)){
        if (Q->front == -1)
        {
            Q->front = 0;
            Q->rear = 0;
        }else {
            Q->rear = Q->rear + 1;
        }
        Q->data[Q->rear] = X;
    }else printf("Queue Full\n");
}
void DeQueue (Queue *Q, Data *X){
    if (!isQueueEmpty(*Q))
    {
        *X = Q->data[Q->rear];
        if (Q->front == Q->rear)
        {
            Q->front = -1;
            Q->rear = -1;
        }else Q->front = Q->front + 1;
        
    }else printf("Queue Kosong\n");
    
}
void PrintQueue(Queue Q){
    if (!isQueueEmpty(Q))
    {
        for (int i = Q.front; i <= Q.rear; i++)
        {
            printf("-> %d", Q.data[i].Image.id);
        }
        printf("\n");
    }else printf("Queue Kosong\n");
    
}

