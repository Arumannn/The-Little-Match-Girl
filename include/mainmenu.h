<<<<<<< Updated upstream
=======
#ifndef MAINMENU_H
#define MAINMENU_H
#include <raylib.h>
#include <stdlib.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid

extern Rectangle menuButtons[5];
extern Texture2D backgroundMenu;
extern Texture2D buttonStart, buttonAbout, buttonExit, buttonBack, buttonNewGame, buttonLoadGame;




void DrawMainMenu();
void UpdateMainMenu();
int GetSelectedMenu();            

bool CheckMenuClick(int index);
void InitMenuAssets();           
void UnloadMenuAssets();
void DrawDebugGrid(int gridSize);         


#endif
>>>>>>> Stashed changes
