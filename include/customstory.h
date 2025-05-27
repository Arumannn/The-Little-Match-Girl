#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>



typedef struct SceneNode {
    char *backgroundSound;
    char *dialogue;

    char *backgroundPath;
    char *characterPath;

    Texture2D backgroundTex;
    Texture2D characterTex;
    Texture2D particle;

    float delay;

    struct SceneNode *next;
} SceneNode;

typedef struct VNTreeNode {
    int id;
    SceneNode *sceneList;             // Head of this node's scene linked list
    struct VNTreeNode *leftChoice;    // Left child node (e.g., first choice)
    struct VNTreeNode *rightChoice;   // Right child node (e.g., second choice)
    char choices[2];                  // Descriptions or labels for left/right choices
} VNTreeNode;

typedef struct {
    VNTreeNode *root;
} VNTree;

typedef struct {
    Texture2D background;
    Texture2D sprite;
} AssetLibrarySimple;
typedef AssetLibrarySimple AssetLibraryArr[2];

static Texture2D MenuBackground;

static int assetCount;


void StoryCreator(VNTreeNode **currentNodePtr, AssetLibraryArr assets);
void LoadAssetsSimple(AssetLibraryArr assets);
void UnloadAssetsSimple(AssetLibraryArr assets);
void PrintTree(VNTreeNode *node);

#endif
