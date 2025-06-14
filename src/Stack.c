#include "stack.h"
#include <stdio.h>
#include <raylib.h>

// IS: Stack belum diinisialisasi
// FS: Stack berhasil diinisialisasi kosong
void CreateEmptyStack(Stack *S){
    S->top = -1;
}

// IS: Stack belum dicek kosong/penuh
// FS: Status kosong/penuh stack diketahui
bool IsStackEmpty(Stack S){
    return S.top == -1;
}

bool IsStackFull(Stack S){
    return S.top == MAX_STACK - 1;
}

// IS: Data belum ditambahkan ke stack
// FS: Data berhasil ditambahkan ke stack jika belum penuh
void Push (Stack *S, Data X){
    if (!IsStackFull(*S))
    {
        S->top++;
        S->Memory[S->top] = X;
    }else printf("Stack Penuh");
    
}

// IS: Data belum di-pop dari stack
// FS: Data berhasil di-pop dari stack jika tidak kosong
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

// IS: Ukuran stack belum diketahui
// FS: Ukuran stack berhasil dihitung
int StackSize(Stack S){
    return S.top + 1;
} 

// IS: Stack belum dicetak ke terminal
// FS: Isi stack berhasil dicetak ke terminal
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

