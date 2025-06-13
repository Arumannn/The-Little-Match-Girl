#ifndef MINIGAME_H
#define MINIGAME_H
#include <raylib.h>

#include "mainmenu.h"
#include "Stack.h"

extern Stack MemoryStack;
extern Stack PlayerChoiceStack;
extern Texture2D memoryImages[MAX_STACK];
extern float memoryDisplayTimer;
extern int currentMemoryIndex;
extern bool showingMemories;
extern bool choosingMemories;
extern bool minigameSuccess;
extern bool minigameActive;
extern int currentChoiceCount;

//MINIGAME STACK

void InitMiniGameStack();
void UpdateMiniGameStack(GameState *currentGameState);
void DrawMiniGameStack();
void UnloadMiniGameStackAssets(Stack * S); 

#endif