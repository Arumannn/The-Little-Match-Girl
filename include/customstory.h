#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>

#define CHARA_AMMOUNT 11
#define BACKGROUND_AMMOUNT 23

typedef enum
{
    CHOOSINGBACKGROUND,
    CHOOSINGCHARA,
    CHOOSINGDIALOGUE,
    CONFIRMATION,
    MODE_REVIEW_SCENE,
    MODE_MOVE_TREE,
    CHANGE_TREENODE,
    CHANGE_LINKEDNODE,
    ALLDONE
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

struct SceneNode
{
    char *Background;
    char *Character;
    char *Convo;
};

typedef struct ListElements 
{
    struct ListElements *Next;
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
static Texture2D MenuBackground;

void CustomStoryGUI(int state, int currentsprite, char *Dialogue);
void InitiateAssets();
void SaveSlot(CustomSceneTree *ThisSlot);
void MakeCustomStory(CustomSceneTree *ThisSlot);
void LoadSlot(CustomSceneTree *ThisSlot);


// void StoryCreator(VNTreeNode *currentNodePtr, AssetLibraryArr assets);
// void LoadAssetsSimple(AssetLibraryArr assets);
// void UnloadAssetsSimple(AssetLibraryArr assets);
// void CustomPrintTree(VNTreeNode *node);

#endif
