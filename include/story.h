#ifndef STORY_H
#define STORY_H
#include "raylib.h"

#define MAX_SCENE 10
#define MAX_NODE_TREE 28

typedef struct Scene{
    char *backgroundSound;                 
    char *dialogue;       

    char *backgroundPath;          
    char *characterPath;           

    Texture2D backgroundTex;       
    Texture2D characterTex;
    Texture2D particle;

    float delay;
}Scene;

typedef struct{
    int id;
    Scene Scenes;
}Tree;


extern Tree SceneTree[MAX_NODE_TREE];
extern int currentScene;

#endif