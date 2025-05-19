#include <raylib.h>
#include "mainmenu.h"

Texture2D backgroundMenu, buttonStart, buttonAbout, buttonExit, buttonNewGame, buttonLoadGame, buttonBack;
void InitMenuAssets(){
    // Load menu background texture
    backgroundMenu = LoadTexture("img/background/background_cadangan.png");
    buttonStart = LoadTexture("img/mainmenu/Start.png");
    buttonAbout = LoadTexture("img/mainmenu/About.png");
    buttonExit = LoadTexture("img/mainmenu/Exit.png");
    buttonNewGame = LoadTexture("img/mainmenu/New_Game.png");
    buttonLoadGame = LoadTexture("img/mainmenu/Load_Game.png");
    buttonBack = LoadTexture("img/mainmenu/Back.png");
}           
void DrawMainMenu();
void UpdateMainMenu();
int GetSelectedMenu();            

bool CheckMenuClick(int index);
void UnloadMenuAssets();         


void DrawDebugGrid(int gridSize) {
    Color gridColor = DARKGRAY;

    // Garis vertikal
    for (int x = 0; x <= SCREEN_WIDTH; x += gridSize) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, gridColor);
        DrawText(TextFormat("%d", x), x + 2, 0, 10, GRAY);
    }

    // Garis horizontal
    for (int y = 0; y <= SCREEN_HEIGHT; y += gridSize) {
        DrawLine(0, y, SCREEN_WIDTH, y, gridColor);
        DrawText(TextFormat("%d", y), 0, y + 2, 10, GRAY);
    }
}
