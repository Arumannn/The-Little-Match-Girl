#ifndef STORY_H
#define STORY_H
#include <raylib.h>
#include "mainmenu.h"
#include "Queue.h"

#define MAX_NODE_TREE 28

typedef struct {
    int id;                     // ID node cerita
    Queue Frame;                // Queue statis untuk menyimpan frame/scene
    char *choiceLeftSon;        // Teks untuk pilihan kiri
    char *choiceRightSon;       // Teks untuk pilihan kanan
    int IdLeftSon;              // ID node tujuan untuk pilihan kiri
    int IdRightSon;             // ID node tujuan untuk pilihan kanan
    int TotalScene;             // Jumlah total scene dalam node
    int numChoices;             // Jumlah pilihan (0, 1, atau 2)
} TreeStory;

extern TreeStory SceneTree[MAX_NODE_TREE];
extern int currentScene;
extern int currentFrame;

void InitDataCerita(TreeStory SceneTree[]);
void LoadNodeAssets(TreeStory SceneTree[], int nodeIndex);
void UnloadNodeAssets(TreeStory SceneTree[], int nodeIndex);
void DrawCurrentNodeScreen(TreeStory SceneTree[]);
void UpdateCerita(TreeStory SceneTree[], GameState *gameState);
void ProsesChoice(TreeStory SceneTree[], int choice);
void SaveGameStory(const char *filename, int Node, int Scene);
void LoadGameStory(const char *filename, int *Node, int *Scene);
void UnloadStoryAudio(void);

#endif