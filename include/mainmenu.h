#ifndef MAINMENU_H
#define MAINMENU_H

#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAX_MENU 17

typedef enum{
    //MAIN MENU
    GAME_STATE_MAIN_MENU,
    GAME_STATE_PLAY_GAME,          
    GAME_STATE_NEW_CONTINUE_MENU,  
    GAME_STATE_CUSTOM_GAME_MENU,   
    GAME_STATE_ABOUT,    
    GAME_STATE_STORY,
    GAME_STATE_CREATE,
    GAME_STATE_EDIT,
    GAME_STATE_DELETE,
    GAME_STATE_CREATE_SLOT_1,
    GAME_STATE_CREATE_SLOT_2,
    GAME_STATE_CREATE_SLOT_3,
    GAME_STATE_EDIT_SLOT_1,
    GAME_STATE_EDIT_SLOT_2,
    GAME_STATE_EDIT_SLOT_3,
    GAME_STATE_DELETE_SLOT_1,
    GAME_STATE_DELETE_SLOT_2,
    GAME_STATE_DELETE_SLOT_3,
    GAME_STATE_PLAY_CUSTOM_SLOT_1,
    GAME_STATE_PLAY_CUSTOM_SLOT_2,
    GAME_STATE_PLAY_CUSTOM_SLOT_3,
    GAME_STATE_CONTINUE_SLOT_1,
    GAME_STATE_CONTINUE_SLOT_2,
    GAME_STATE_CONTINUE_SLOT_3,

    //STORY
    END_GAME_ALONE, //NODE 4 dan 20
    END_GAME_DIE_BECAUSE_HOMELESS, // NODE 8
    END_GAME_FOOD_FROM_HOMELESS, // NODE 12
    END_GAME_DIE_FAILED_ESCAPE, // NODE 16
    END_GAME_NEW_FAMILY, // NODE 24
    END_GAME_ALONE_IN_FEAR, // NODE 23
    END_GAME_CRUSHED_BY_CAR, //NODE 5, 9 dan 10
    END_GAME_AFFORD_HOT_SOUP, // NODE 17 dan 27
    END_GAME_GET_STAB_BY_HOMELESS, // NODE 21
    END_GAME_DIE_ALONE_UNDER_CHRISTMAS_TREE, //NODE 25
    END_GAME_LOST_MATCH_THEN_DIE 
}GameState;

// Fungsi utama menu
void InitAssets();
void InitButtonRects(GameState currentGameState);
void DrawMainMenu(GameState currentGameState);
void UpdateMainMenu(GameState * currentGameState);
bool CheckMenuClick(int index, GameState * currentGameState);
void UnloadAssets();
void DrawDebugGrid(int gridSize);

// Variabel global yang digunakan di mainmenu.c
extern Texture2D MenuButtons[MAX_MENU];
extern Rectangle buttonRects[MAX_MENU];
extern int selectedMenu;
extern GameState currentGameState;
extern Music BGMusic;
#endif 
