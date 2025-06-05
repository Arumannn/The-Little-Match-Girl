#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>

#define CHARA_AMMOUNT 11
#define BACKGROUND_AMMOUNT 23
#define AUDIO_AMMOUNT 6

typedef enum
{
    CHOOSINGBACKGROUND,
    CHOOSINGCHARA,
    CHOOSINGDIALOGUE,
    CONFIRMATION,
    ALLDONE,
    MODE_REVIEW_SCENE,
    MODE_OVERWRITING,
    OUTOFBOUNDS,
};

typedef struct BackgroundFileData
{
    Texture2D Background;
    char FileName[124];
}BackgroundArr;

typedef struct CharaFileData
{
    Texture2D Chara;
    char FileName[124];
}CharaArr;

typedef struct AudioFileData
{
    Texture2D Audio;
    char FileName[124];
}AudioArr;

struct SceneNode
{
    char *Background;
    char *Character;
    char *Convo;
    char *SFX;
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
    struct Tree *Left;
    struct Tree *Right;
    struct Tree *Parent;
}*CustomSceneTree;

//extern CustomSceneTree *Current;

extern CharaArr FileChara[CHARA_AMMOUNT];
extern BackgroundArr FileBackground[BACKGROUND_AMMOUNT];
extern AudioArr FileAudio[AUDIO_AMMOUNT];
static Texture2D MenuBackground;


void InitializeStoryTree(CustomSceneTree *ThisSlot);
void CustomStoryGUI(int state, int currentsprite, char *Dialogue, char *BackSprite, char *CharaSprite);
void InitiateAssets();
void SaveSlot(CustomSceneTree *ThisSlot);
void ChoosingBackground(int *selectedsprite, int *control, SceneList *TempScene);
void ChoosingChara(int *selectedsprite, int *control, SceneList *TempScene);
void ChoosingDialogue(char *Convo, int *selectedsprite, int *control, SceneList *TempScene);
void MakeCustomStory(CustomSceneTree *ThisSlot);
void LoadSlot(CustomSceneTree *ThisSlot);
void AddLeftChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control);
void AddRightChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control);
void PreviousScene(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control); //soon to be deleted
void PreviousTree(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control);
void AddNewScene(SceneList *TempScene, bool *warning, int *control); //soon to be deleted
void AddSceneLast(CustomSceneTree *ThisSlot);
void AddSceneFirst(CustomSceneTree *ThisSlot);
void DeleteSceneFirst(CustomSceneTree *ThisSlot);
void DeleteSceneLast(CustomSceneTree *ThisSlot);
void ReviewScene(int *control, SceneList *TempScene);



// void StoryCreator(VNTreeNode *currentNodePtr, AssetLibraryArr assets);
// void LoadAssetsSimple(AssetLibraryArr assets);
// void UnloadAssetsSimple(AssetLibraryArr assets);
// void CustomPrintTree(VNTreeNode *node);

#endif
