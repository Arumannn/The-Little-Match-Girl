#ifndef CUSTOMSTORY_H
#define CUSTOMSTORY_H
#include <raylib.h>
#include <stdlib.h>


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
    Texture2D sprite;
} AssetLibrarySimple;
typedef AssetLibrarySimple AssetLibraryArr[2];

static Texture2D MenuBackground;

static int assetCount;


void StoryCreator(VNTreeNode *currentNodePtr, AssetLibraryArr assets);
void LoadAssetsSimple(AssetLibraryArr assets);
void UnloadAssetsSimple(AssetLibraryArr assets);
void CustomPrintTree(VNTreeNode *node);

#endif
