#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "customstory.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAX_MENU 17


// Fungsi utama menu
void InitAssets();
void InitButtonRects();
void DrawMainMenu();
void UpdateMainMenu(VNTreeNode **currentNode, AssetLibraryArr assets);
bool CheckMenuClick(int index, VNTreeNode **currentNodePtr, AssetLibraryArr assets);
void UnloadAssets();
void DrawDebugGrid(int gridSize);

// Variabel global yang digunakan di mainmenu.c
extern Texture2D MenuButtons[MAX_MENU];
extern Rectangle buttonRects[MAX_MENU];
extern int selectedMenu;
extern int MenuState;
extern Music BGMusic;
#endif 
