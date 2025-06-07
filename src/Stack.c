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
    }else{
        printf("Stack kosong\n");
        *X = (Data) {0};
    }
    
}
//DEBUGGING STACK
int StackSize(Stack S){
    return S.top + 1;
} 

void PrintStack(Stack S) {
    if (IsStackEmpty(S)) {
        printf("Stack kosong\n");
        return;
    }
    printf("Isi stack (dari top ke bottom):\n");
    for (int i = S.top; i >= 0; i--) {
        printf("Memory [index %d]: Image ID: %u\n", i, S.Memory[i].Image.id);
    }
}  

