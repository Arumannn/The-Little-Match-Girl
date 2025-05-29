#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>
#include "mainmenu.h"

// Hapus typedef lama yang tidak digunakan:
/*
typedef struct VNTreeNode *addressTree;
typedef struct SceneNode *addressScene;
typedef struct SceneNode {
    //char *backgroundSound;
    char *dialogue;
    
    char *backgroundPath;
    char *characterPath;

    Texture2D backgroundTex;
    Texture2D characterTex;
    Texture2D particle;

    addressScene next;
} SceneNode;

typedef struct VNTreeNode {
    int id;
    addressScene sceneList;             // Head of this node's scene linked list
    addressTree leftChoice;    // Left child node (e.g., first choice)
    addressTree rightChoice;   // Right child node (e.g., second choice)
    char choices[2];                  // Descriptions or labels for left/right choices
} VNTreeNode;

typedef struct {
    addressTree root;
} VNTree;

typedef struct {
    Texture2D background;
    Texture22 sprite;
} AssetLibrarySimple;
typedef AssetLibrarySimple AssetLibraryArr[2];
*/

#define CHARA_AMMOUNT 8
#define BACKGROUND_AMMOUNT 18

typedef enum
{
    CHOOSINGBACKGROUND,
    CHOOSINGCHARA,
    CONFIRMATION,
    MODE_SELECT_BACKGROUND,
    MODE_SELECT_CHARACTER,
    MODE_INPUT_TEXT,
    MODE_CONFIRM_NODE,
    MODE_REVIEW_SCENE,
    MODE_MOVE_TREE
}CustomStoryMode;

typedef struct SceneNode
{
    Texture2D Background;
    Texture2D Character;
    char *BackgroundPath;
    char *CharacterPath;
    char *Convo;
};



typedef struct ListElements
{
    struct ListElements *Next;   
    struct SceneNode Data;
}*SceneList;

typedef struct Tree
{
    int ID;
    SceneList *NodeContents;
    struct Tree *Left;
    struct Tree *Right;
    struct Tree *Parent;
    char *ChoiceLeftText;         
    char *ChoiceRightText;
}*CustomSceneTree;

typedef struct NodeLoadEntry {
    CustomSceneTree nodePtr; // Pointer ke node yang dimuat
    int leftChildId;         // ID anak kiri yang mentah dari file
    int rightChildId;        // ID anak kanan yang mentah dari file
    struct NodeLoadEntry *next; // Untuk linked list sementara
} NodeLoadEntry;


extern CustomSceneTree *Current;

extern Texture2D FileChara[CHARA_AMMOUNT];
extern Texture2D FileBackground[BACKGROUND_AMMOUNT];
static Texture2D MenuBackground; // Deklarasi statis di sini

void CustomStoryGUI(int state);
void InitiateAssets();
void SaveSlot(CustomSceneTree *ThisSlot, const char* filename);
void MakeCustomStory(CustomSceneTree *ThisSlot, GameState *currentGameState);
void LoadSlot(CustomSceneTree *ThisSlot, const char* filename);

void FreeSceneList(SceneList list);
void FreeCustomStoryTree(CustomSceneTree tree);
void AssignNodeIDs(CustomSceneTree node, int *nextID);

#endif