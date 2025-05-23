#ifndef MAINMENU_H
#define MAINMENU_H

#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAX_MENU 17


// Fungsi utama menu
void InitAssets();
void InitButtonRects();
void DrawMainMenu();
void UpdateMainMenu();
bool CheckMenuClick(int index);
void UnloadAssets();
void DrawDebugGrid(int gridSize);

// Variabel global yang digunakan di mainmenu.c
extern Texture2D MenuButtons[MAX_MENU];
extern Rectangle buttonRects[MAX_MENU];
extern int selectedMenu;
extern int GameState;
extern Music BGMusic;
#endif 
