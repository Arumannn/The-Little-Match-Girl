#ifndef MAINMENU_H
#define MAINMENU_H

#include <raylib.h>
#include "gamestate.h"
#include "customstory.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAX_MENU 18

// Fungsi utama menu
void InitAssetsMenu();
void InitButtonRects(GameState currentGameState);
void DrawMainMenu(GameState currentGameState);
void UpdateMainMenu(GameState * currentGameState);
bool CheckMenuClick(int index, GameState * currentGameState);
void UnloadAssets();
void DrawDebugGrid(int gridSize);
void UpdatePauseMenu(GameState * GameState);
void DrawPauseMenu();

// Save file and custom story functions
bool CheckSaveFileExists(int slotNumber);
void LoadCustomStoryFromFile(const char* filename, CustomSceneTree* tree);

Music PlayBackgroundMusic(const char * pathMusic);

// Variabel global yang digunakan di mainmenu.c
extern Texture2D MenuButtons[MAX_MENU];
extern Rectangle buttonRects[MAX_MENU];
extern int selectedMenu;
extern GameState currentGameState;

// Declare extern variables to be used in menu logic
extern int storyCurrentScene;
extern int storyCurrentFrame;

#endif
