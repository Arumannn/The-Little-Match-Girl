#ifndef MAINMENU_H
#define MAINMENU_H

#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAX_MENU 19

typedef enum {
    // Main menu states
    GAME_STATE_MAIN_MENU,
    GAME_STATE_PLAY_GAME_MENU,          
    GAME_STATE_NEW_CONTINUE_NON_CUSTOM,  
    GAME_STATE_NEW_CONTINUE_CUSTOM,
    GAME_STATE_STUDIO_MENU,   
    GAME_STATE_ABOUT,    
    
    // Game states
    GAME_STATE_PLAY_GAME,
    GAME_STATE_PLAY_CUSTOM,
    
    // Studio states
    GAME_STATE_CREATE_MENU,
    GAME_STATE_EDIT_MENU,
    GAME_STATE_DELETE_MENU,
    GAME_STATE_CREATE_CUSTOM,
    GAME_STATE_EDIT_CUSTOM,
    GAME_STATE_DELETE_CUSTOM,
    GAME_STATE_PLAY_CUSTOM_MENU,
    
    // Other states
    GAME_STATE_MINI_GAME_STACK,
    GAME_STATE_PAUSE
} GameState;

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

Music PlayBackgroundMusic(const char * pathMusic);

// Variabel global yang digunakan di mainmenu.c
extern Texture2D MenuButtons[MAX_MENU];
extern Rectangle buttonRects[MAX_MENU];
extern int selectedMenu;
extern GameState currentGameState;

// Declare extern variables to be used in menu logic
extern int storyCurrentNode;
extern int storyCurrentScene;

#endif
