#ifndef HEADER_H
#define HEADER_H
#include <raylib.h>

typedef struct Scene *address_Scene;
typedef struct Scene {
    char *backgroundSound;                 
    char *dialogue;       

    char *backgroundPath;          
    char *characterPath;           

    Texture2D backgroundTex;       
    Texture2D characterTex;
    Texture2D particle;

    float delay;

    address_Scene next;
}Scene;

typedef struct StoryNode *address_Tree;
typedef struct StoryNode {
    int id;                        
    address_Scene scene;
    address_Tree leftson;
    address_Tree rightson;

    char choices[2];            
} StoryNode;



typedef struct {
    address_Tree Root;
} Tree;

typedef struct {
    address_Scene Head;
} List_Scene;


#endif
