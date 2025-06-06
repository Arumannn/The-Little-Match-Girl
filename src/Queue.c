#include "Queue.h"
#include <stdio.h>
#include <string.h>

void CreateQueue(Queue *Q) {
    Q->front = -1;
    Q->rear = -1;
}

bool isQueueEmpty(Queue Q) {
    return (Q.front == -1) && (Q.rear == -1);
}

bool isQueueFull(Queue Q) {
    return ((Q.rear + 1) % MAX_SCENE) == Q.front;
}

int QueueSize(Queue Q) {
    if (isQueueEmpty(Q)) return 0;
    if (isQueueFull(Q)) return MAX_SCENE;
    return (Q.rear - Q.front + MAX_SCENE) % MAX_SCENE + 1;
}

void EnQueueLast(Queue *Q, Scene scene) {
    if (isQueueFull(*Q)) {
        printf("Queue Full\n");
        return;
    }
    
    if (isQueueEmpty(*Q)) {
        Q->front = 0;
        Q->rear = 0;
    } else {
        Q->rear = (Q->rear + 1) % MAX_SCENE;
    }
    
    Q->data[Q->rear] = scene;
}

Scene Peek(Queue Q) {
    Scene emptyScene = {NULL, NULL, NULL, NULL, {0}, {0}, CHAR_POS_NONE};
    if (isQueueEmpty(Q)) {
        printf("Queue Kosong\n");
        return emptyScene;
    }
    return Q.data[Q.front];
}

void DeQueueFirst(Queue *Q) {
    if (isQueueEmpty(*Q)) {
        printf("Queue Kosong\n");
        return;
    }
    
    if (Q->data[Q->front].backgroundTex.id != 0) {
        UnloadTexture(Q->data[Q->front].backgroundTex);
    }
    if (Q->data[Q->front].characterTex.id != 0) {
        UnloadTexture(Q->data[Q->front].characterTex);
    }
    
    if (Q->front == Q->rear) {
        Q->front = -1;
        Q->rear = -1;
    } else {
        Q->front = (Q->front + 1) % MAX_SCENE;
    }
}

void PrintQueue(Queue Q) {
    if (isQueueEmpty(Q)) {
        printf("Queue Kosong\n");
        return;
    }
    
    int i = Q.front;
    while (i != Q.rear) {
        printf("Scene: %s, Dialogue: %s, Pos: %d\n", 
               Q.data[i].backgroundPath ? Q.data[i].backgroundPath : "None",
               Q.data[i].dialogue ? Q.data[i].dialogue : "None",
               Q.data[i].CharPosition);
        i = (i + 1) % MAX_SCENE;
    }
    // Print last element
    printf("Scene: %s, Dialogue: %s, Pos: %d\n", 
           Q.data[i].backgroundPath ? Q.data[i].backgroundPath : "None",
           Q.data[i].dialogue ? Q.data[i].dialogue : "None",
           Q.data[i].CharPosition);
}