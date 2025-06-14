#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>

#include "mainmenu.h"  // Include mainmenu.h to get game state definitions
#include "story.h"

// Safe buffer sizes
#define MAX_PATH_LENGTH 256
#define MAX_FILENAME_LENGTH 128
#define MAX_HEADER_LENGTH 16
#define MAX_DIALOG_LENGTH 1024

#define CHARA_AMMOUNT 11
#define BACKGROUND_AMMOUNT 23
#define AUDIO_AMMOUNT 6

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef enum
{
    CHOOSINGBACKGROUND,
    CHOOSINGCHARA,
    CHOOSINGCHARPOSITION,
    CHOOSINGDIALOGUE,
    CHOOSINGCHOICETEXT,  // New state for setting choice text
    CONFIRMATION,
    ALLDONE,
    ADDFIRSTSCENE,
    ADDLASTSCENE,
    DELETEFIRSTSCENE,
    DELETELASTSCENE,
    MODE_REVIEW_SCENE,
    MODE_OVERWRITING,
} CustomStoryState;

typedef struct BackgroundFileData
{
    Texture2D Background;
    char FileName[MAX_FILENAME_LENGTH];
}BackgroundArr;

typedef struct CharaFileData
{
    Texture2D Chara;
    char FileName[MAX_FILENAME_LENGTH];
}CharaArr;

typedef struct AudioFileData
{
    Texture2D Audio;
    char FileName[MAX_FILENAME_LENGTH];
}AudioArr;

struct SceneNode
{
    char *Background;
    char *Character;
    char *Convo;
    char *SFX;
    CharacterPosition charPosition;
};

typedef struct ListElements 
{
    struct ListElements *Next;
    struct ListElements *Before;
    struct SceneNode Data;
} *SceneList;

typedef struct Tree
{
    int ID;
    SceneList NodeContents;
    char *TextLeft;      // Text to show for left choice button
    char *TextRight;     // Text to show for right choice button
    struct Tree *Left;
    struct Tree *Right;
    struct Tree *Parent;
}*CustomSceneTree;

//extern CustomSceneTree *Current;

extern CharaArr FileChara[CHARA_AMMOUNT];
extern BackgroundArr FileBackground[BACKGROUND_AMMOUNT];
extern AudioArr FileAudio[AUDIO_AMMOUNT];
static Texture2D MenuBackground;

CustomSceneTree LoadTreeFromFile(const char* filename);
CustomSceneTree LoadSlotFromFile(int slotNumber);
CustomSceneTree DeserializeTreeNode(FILE *file, CustomSceneTree parent);
SceneList DeserializeSceneList(FILE *file);

void InitializeStoryTree(CustomSceneTree *ThisSlot);
void CustomStoryGUI(int state, int currentsprite, char *Dialogue, char *BackSprite, char *CharaSprite, CharacterPosition charPosition, CustomSceneTree TempTree, SceneList TempScene);
void InitiateAssets();
void PrintTree(CustomSceneTree *ThisSlot);
void ChoosingBackground(int *selectedsprite, int *control, SceneList *TempScene, CustomSceneTree TempTree);
void ChoosingChara(int *selectedsprite, int *control, SceneList *TempScene, CustomSceneTree TempTree);
enum { DUMMY_MAX_CONVO = 128 };
int ChoosingDialogue(char *Convo, int *selectedsprite, SceneList *TempScene, CustomSceneTree TempTree);
void ChoosingCharaPosition(int *selectedsprite, int *control, SceneList *TempScene);
void ChoosingChoiceText(char *LeftText, char *RightText, int *selectedsprite, CustomSceneTree *TempTree);  // New function
void MakeCustomStory(CustomSceneTree *ThisSlot, int SlotNumber);
void OverwriteTree(SceneList *TempScene, CustomSceneTree *TempTree, int *control);
void AddLeftChild(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control);
void AddRightChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control, int *selectedsprite);
void PreviousTree(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control);
void AddSceneLast(CustomSceneTree *TempTree, SceneList *TempScene, int *control);
void AddSceneFirst(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control);
void DeleteSceneFirst(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control);
void DeleteSceneLast(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control);
void ReviewScene(int *control, SceneList *TempScene, CustomSceneTree *TempTree);
void DeleteTreeNode(CustomSceneTree *nodeToDelete);
void DeleteSceneList(SceneList sceneList);
void HandleDeleteCurrentNode(CustomSceneTree *TempTree, SceneList *TempScene, int *control, int *selectedsprite);
void SaveTreeToFile(CustomSceneTree tree, const char* filename);
void SaveSlotToFile(CustomSceneTree *ThisSlot, int slotNumber);
void SerializeTreeNode(FILE *file, CustomSceneTree node);
void SerializeSceneList(FILE *file, SceneList sceneList);
void LoadSlot(CustomSceneTree *ThisSlot, int slotnumber);

// Save/Load progress functions
void SaveCustomStoryProgress(const char *filename, int node, int scene);
void LoadCustomStoryProgress(const char *filename, int *node, int *scene);
void SaveCustomStoryProgressBySlot(int slotNumber, int node, int scene);
void LoadCustomStoryProgressBySlot(int slotNumber, int *node, int *scene);
void SaveRegularStoryProgressBySlot(int slotNumber, int node, int scene);
void LoadRegularStoryProgressBySlot(int slotNumber, int *node, int *scene);

// Custom story playback functions
int UpdateCustomStory(CustomSceneTree tree, int *currentNode, int *currentScene, int currentSlot);
void DrawCustomStoryScreen(CustomSceneTree tree, int currentNode, int currentScene);

// DeleteSlotFiles function declaration
void DeleteSlotFiles(int slotNumber);

// Clear slot from memory function declaration
void ClearSlotFromMemory(int slotNumber);

// Create save directories function declaration
void CreateSaveDirectories(void);

#endif
