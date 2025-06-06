#ifndef QUEUE_H
#define QUEUE_H
#include <raylib.h>
#include <stdbool.h>
#define MAX_SCENE 10

typedef enum {
    CHAR_POS_NONE = 0,  // Tidak ada karakter
    CHAR_POS_LEFT,      // Karakter di kiri
    CHAR_POS_CENTER,    // Karakter di tengah
    CHAR_POS_RIGHT      // Karakter di kanan
} CharacterPosition;

typedef struct Scene {
    char *backgroundSound;         // Path ke file suara latar
    char *dialogue;               // Teks dialog
    char *backgroundPath;         // Path ke gambar background
    char *characterPath;          // Path ke gambar karakter
    Texture2D backgroundTex;      // Tekstur background
    Texture2D characterTex;       // Tekstur karakter
    CharacterPosition CharPosition; // Posisi karakter
} Scene;

typedef struct {
    Scene data[MAX_SCENE]; // Array statis untuk menyimpan scene
    int front;             // Indeks elemen pertama
    int rear;              // Indeks elemen terakhir
} Queue;

void CreateQueue(Queue *Q);
void EnQueueLast(Queue *Q, Scene scene);
Scene Peek(Queue Q);
void DeQueueFirst(Queue *Q);
bool isQueueEmpty(Queue Q);
bool isQueueFull(Queue Q);
int QueueSize(Queue Q);
void PrintQueue(Queue Q);

#endif