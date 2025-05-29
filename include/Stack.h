#ifndef STACK_H
#define STACK_H
#define MAX_STACK 6
#include "raylib.h"
#include "stdbool.h"

typedef struct{
    Texture2D Image;
    Rectangle ImageReacts;
}Data;


typedef struct {
    Data Memory[MAX_STACK];
    int top;
} Stack;


// ===================================== STACK GAME KENANGAN =====================================
void CreateEmptyStack(Stack *S);
void Push (Stack *S, Data X);
void Pop (Stack *S, Data *X);
//DEBUGGING STACK
bool IsStackEmpty(Stack S);
int StackSize(Stack S);  
bool IsStackFull(Stack S);
void PrintStack(Stack S);      


// Clearing
#endif