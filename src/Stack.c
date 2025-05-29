#include "stack.h"
#include <stdio.h>
#include <raylib.h>

void CreateEmptyStack(Stack *S){
    S->top = -1;
}
bool IsStackEmpty(Stack S){
    return S.top == -1;
}

bool IsStackFull(Stack S){
    return S.top == MAX_STACK - 1;
}

void Push (Stack *S, Data X){
    if (!IsStackFull(*S))
    {
        S->top++;
        S->Memory[S->top] = X;
    }else printf("Stack Penuh");
    
}
void Pop (Stack *S, Data *X){
    if (!IsStackEmpty(*S))
    {
        *X = S->Memory[S->top];
        S->top--;
    }
    
}
//DEBUGGING STACK
int StackSize(Stack S){
    return S.top + 1;
} 

void PrintStack(Stack S){
    if (!IsStackEmpty(S))
    {
        for (int i = S.top; i >= MAX_STACK - 1; i--)
        {
            printf("Memory [%d]: Image ID :\n", i, S.Memory[i].Image.id);
        }
        
    }else printf("Stack Masih Kosong\n");
    
}      

