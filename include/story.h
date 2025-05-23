#ifndef STORY_H
#define STORY_H
#include <raylib.h>
#include "mainmenu.h"

#define MAX_SCENE 10
#define MAX_NODE_TREE 28

typedef enum {
    CHAR_POS_NONE = 0, // Tidak ada karakter
    CHAR_POS_LEFT,     // Karakter di kiri
    CHAR_POS_CENTER,   // Karakter di tengah
    CHAR_POS_RIGHT     // Karakter di kanan
} CharacterPosition;


typedef struct Scene{
    char *backgroundSound;                 
    char *dialogue;       

    char *backgroundPath;   // Direktori untuk menuju ke image Background       
    char *characterPath;    // Direktori untuk menuju ke image karakter       

    Texture2D backgroundTex;       
    Texture2D characterTex;
    CharacterPosition CharPosition;

}Scene;

typedef struct{
    int id;
    Scene Frame[MAX_SCENE];
    char *choiceLeftSon;
    char *choiceRightSon;
    int TotalScene;
    int numChoices;
}Tree;


extern Tree SceneTree[MAX_NODE_TREE];
extern int currentScene;
extern int currentFrameIndex;

void InitDataCerita();
void LoadNodeAssets(int nodeIndex);
void UnloadNodeAssets(int nodeIndex);
void DrawCurrentNodeScreen();
void UpdateCerita(int *gameState);
void ProsesChoice(int choice);
void PlayBackgroundMusic(const char *musicPath);
void UnloadStoryAudio();
#endif