#ifndef STORY_H
#define STORY_H

#include <raylib.h>
#include "gamestate.h"

#define MAX_SCENE 10

// External declarations for story state
extern int storyCurrentScene;
extern int storyCurrentFrame;
#define MAX_NODE_TREE 28

typedef enum {
    CHAR_POS_NONE,
    CHAR_POS_LEFT,
    CHAR_POS_CENTER,
    CHAR_POS_RIGHT
} CharacterPosition;

typedef struct {
    char *backgroundSound;
    char *dialogue;
    char *backgroundPath;
    char *characterPath;
    Texture2D backgroundTex;
    Texture2D characterTex;
    CharacterPosition CharPosition;
} Scene;

typedef struct {
    int id;
    Scene scenes[MAX_SCENE];  // Array of scenes instead of Queue
    int currentSceneIndex;    // Track current scene
    int TotalScene;
    int numChoices;
    char *choiceLeftSon;
    char *choiceRightSon;
    int IdLeftSon;
    int IdRightSon;
} TreeStory;

void InitDataCerita(TreeStory *SceneTree);
void DrawCharacterAtPosition(Texture2D tex, CharacterPosition pos);
void LoadNodeAssets(TreeStory SceneTree[], int nodeIndex);
void UnloadNodeAssets(TreeStory SceneTree[], int nodeIndex);
void DrawCurrentNodeScreen(TreeStory SceneTree[]);
void UpdateCerita(TreeStory SceneTree[], GameState *gameState);
void ProsesChoice(TreeStory SceneTree[], int choice);
void SaveGameStory(const char *filename, int Node, int Scene);
void LoadGameStory(const char *filename, int *Node, int *Scene);
void UnloadStoryAudio();

#endif // STORY_H