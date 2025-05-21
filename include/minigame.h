#ifndef MINIGAME_H
#define MINIGAME_H
#define MAX 6

typedef struct{
    char data[MAX];
    int head;
    int tail;
}QTE;


typedef struct {
    char data[MAX];
    int top;
} Stack;


// ===================================== STACK GAME KENANGAN =====================================
void CreateEmptyStack(Stack *S);
void Push (Stack *S, char X);
void Pop (Stack *S, char *X);
//DEBUGGING STACK
int IsStackEmpty(Stack S);
int StackSize(Stack S);        
void PrintStack(Stack S);      
int CompareStack(Stack S1, Stack S2);

//===================================== QUEUE GAME QTE =====================================
void CreateQueue (QTE *Q);
void EnQueue (QTE *Q, char X);
void DeQueue (QTE *Q, char *X);
//DEBUGING QUEUE
int isQueueEmpty(QTE Q);
int QueueSize(QTE Q);
void PrintQueue(QTE S);

// Comparing
int CheckQTEInput(QTE *q, char input); 

// Clearing
#endif