#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include "mainmenu.h"
#include "customstory.h"
#include "story.h"


Texture2D MenuButtons[MAX_MENU];
Rectangle buttonRects[MAX_MENU];
Music BGMusic;
int selectedMenu = -1;
int MenuState = 0;
bool showStoryCreator = false;


extern Tree Mytree[MAX_NODE_TREE];
extern int currentScene; 
extern int currentFrame;

void InitAssets(){

    
    // =================================== MENU STATE 0 ===================================
    
    MenuButtons[0] = LoadTexture("Assets/BackSprites/customstorymenu.png");
    MenuButtons[1] = LoadTexture("Assets/mainmenu/Start.png");
    MenuButtons[2] = LoadTexture("Assets/mainmenu/Studio Game.png");
    MenuButtons[3] = LoadTexture("Assets/mainmenu/About.png");
    MenuButtons[4] = LoadTexture("Assets/mainmenu/Exit.png");
    // =================================== MENU STATE 1 ===================================
    MenuButtons[5] = LoadTexture("Assets/mainmenu/Play_Game.png");
    MenuButtons[6] = LoadTexture("Assets/mainmenu/Custom_Game.png");
    // =================================== MENU STATE 2 ===================================
    MenuButtons[7] = LoadTexture("Assets/mainmenu/New_Game.png");
    MenuButtons[8] = LoadTexture("Assets/mainmenu/Continue.png");
    // =================================== MENU STATE 3 ===================================
    MenuButtons[9] = LoadTexture("Assets/mainmenu/Create_Game.png");
    MenuButtons[10] = LoadTexture("Assets/mainmenu/Edit_Game.png");
    MenuButtons[11] = LoadTexture("Assets/mainmenu/Delete_Game.png");
    // =================================== MENU STATE 4 ===================================
    MenuButtons[12] = LoadTexture("Assets/mainmenu/Slot_1.png");
    MenuButtons[13] = LoadTexture("Assets/mainmenu/Slot_2.png");
    MenuButtons[14] = LoadTexture("Assets/mainmenu/Slot_3.png");
    MenuButtons[15] = LoadTexture("Assets/mainmenu/Back.png");
    
    MenuButtons[0].height = SCREEN_HEIGHT;
    MenuButtons[0].width = SCREEN_WIDTH;
    
    for (int i = 1; i < MAX_MENU; i++)
    {
        MenuButtons[i].height = 230 / 2;
        MenuButtons[i].width = 630 / 2;
    }   
}           

void SetButtonRect(int id, int x, int y) {
    buttonRects[id].x = x;
    buttonRects[id].y = y;
    buttonRects[id].width = (float)MenuButtons[id].width;
    buttonRects[id].height = (float)MenuButtons[id].height;
}

void InitButtonRects(GameState currentGameState) {
    int startX = 100;
    int startY = 300;
    int gapY = 150;

    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
            printf("currentGameState MAIN_MENU\n");
            for (int i = 1, idx = 0; i <= 4; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            break;

        case GAME_STATE_PLAY_GAME:
            printf("currentGameState PLAY_GAME\n");
            for (int i = 5, idx = 0; i <= 6; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;

        case GAME_STATE_NEW_CONTINUE_MENU: 
            printf("currentGameState NEW_CONTINUE_MENU\n");
            for (int i = 7, idx = 0; i <= 8; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;

        case GAME_STATE_CUSTOM_GAME_MENU:
            printf("currentGameState CUSTOM_GAME_MENU\n");
            for (int i = 9, idx = 0; i <= 11; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 3 * gapY);
            break;

        case GAME_STATE_CREATE: 
        case GAME_STATE_EDIT:   
        case GAME_STATE_DELETE: 
            printf("currentGameState CREATE/EDIT/DELETE (SLOTS)\n");
            for (int i = 12, idx = 0; i <= 14; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 3 * gapY);
            break;

        case GAME_STATE_ABOUT:
            printf("currentGameState ABOUT\n");
            SetButtonRect(15, SCREEN_WIDTH / 2 - MenuButtons[15].width / 2, SCREEN_HEIGHT - 500); // Tombol Back di tengah
            break;
        case GAME_STATE_STORY:
            printf("currentGameState STORY\n");
          
            break;
        default:
            break;
    }
}

void DrawMainMenu(GameState currentGameState){
    PlayMusicStream(BGMusic);
    DrawTexture(MenuButtons[0], 0, 0, WHITE);
    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
            for (int i = 1; i <= 4; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            break;
        case GAME_STATE_PLAY_GAME:
            printf("DRAWING MENU STATE PLAY GAME\n");
            for (int i = 5; i <= 6; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case GAME_STATE_NEW_CONTINUE_MENU:
            printf("DRAWING MENU STATE NEW CONTINUE\n");
            for (int i = 7; i <= 8; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case GAME_STATE_CUSTOM_GAME_MENU:
            for (int i = 9; i <= 11; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case GAME_STATE_CREATE:
        case GAME_STATE_EDIT:
        case GAME_STATE_DELETE:
            for (int i = 12; i <= 14; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case GAME_STATE_ABOUT:
            printf("DRAWING MENU STATE ABOUT\n");
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case GAME_STATE_STORY:
            break;
        default:
            break;
    }

    if (selectedMenu >= 0 && selectedMenu < MAX_MENU) {
        DrawRectangleLinesEx(buttonRects[selectedMenu], 10, YELLOW);
    }
}

void UpdateMainMenu(GameState *currentGameState) {
    Vector2 mousePos = GetMousePosition();
    selectedMenu = -1;

    if (*currentGameState == GAME_STATE_STORY)
    {
        return;
    }
    
    int startIndex = 0, endIndex = 0;

    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU: startIndex = 1; endIndex = 4; break;
        case GAME_STATE_PLAY_GAME: startIndex = 5; endIndex = 6; break;
        case GAME_STATE_NEW_CONTINUE_MENU: startIndex = 7; endIndex = 8; break;
        case GAME_STATE_CUSTOM_GAME_MENU: startIndex = 9; endIndex = 11; break;
        case GAME_STATE_CREATE:
        case GAME_STATE_EDIT:
        case GAME_STATE_DELETE: startIndex = 12; endIndex = 14; break;
        case GAME_STATE_ABOUT: startIndex = 15; endIndex = 15; break; 
        default: return; 
    }

    for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePos, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i, currentGameState); 
            }
            return; 
        }
    }


    if (*currentGameState != GAME_STATE_PLAY_GAME && *currentGameState != GAME_STATE_PLAY_GAME) {
        if (CheckCollisionPointRec(mousePos, buttonRects[15])) {
            selectedMenu = 15;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(15, currentGameState, currentNodePtr, assets);
            }
        }
    }
}

bool CheckMenuClick(int index, GameState * currentGameState) {
    printf("Tombol tertekan\n");
    switch (*currentGameState) { 
        case GAME_STATE_MAIN_MENU:
            switch (index) {
                case 1: 
                    *currentGameState = GAME_STATE_PLAY_GAME; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 2: 
                    *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 3: 
                    *currentGameState = GAME_STATE_ABOUT; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 4: 
                    CloseWindow(); 
                    break; // Keluar dari aplikasi
            }
            break;

        case GAME_STATE_PLAY_GAME:
            switch (index) {
                case 5: 
                    *currentGameState = GAME_STATE_STORY; 
                    InitButtonRects(*currentGameState); 
                    currentScene = 0; 
                    currentFrame = 0; 
                    LoadNodeAssets(Mytree, currentScene); 
                    break;
                case 6: 
                    *currentGameState = GAME_STATE_NEW_CONTINUE_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 15: 
                    *currentGameState = GAME_STATE_MAIN_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_MENU:
            switch(index){
                case 7: // New Game
                    printf("Masuk ke New Game\n");
                    *currentGameState = GAME_STATE_CREATE; 
                    InitButtonRects(*currentGameState);
                    break;
                case 8: // Continue
                    printf("Masuk ke Load Game\n");
                    *currentGameState = GAME_STATE_CONTINUE_SLOT_1; 
                    InitButtonRects(*currentGameState);
                    break;
                case 15: // Tombol Back
                    *currentGameState = GAME_STATE_PLAY_GAME; 
                    InitButtonRects(*currentGameState);
                    break;
            }
            break;

        case GAME_STATE_CUSTOM_GAME_MENU:
            switch (index) {
                case 9: // Create Game
                    *currentGameState = GAME_STATE_CREATE; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 10: // Edit Game
                    *currentGameState = GAME_STATE_EDIT; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 11: // Delete Game
                    *currentGameState = GAME_STATE_DELETE; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 15: // Tombol Back
                    *currentGameState = GAME_STATE_MAIN_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;

        case GAME_STATE_CREATE:
            switch (index) {
                case 12: printf("CREATE SLOT 1 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 13: printf("CREATE SLOT 2 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 14: printf("CREATE SLOT 3 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 15: *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; InitButtonRects(*currentGameState); break;
            }
            break;
        case GAME_STATE_EDIT:
            switch (index) {
                case 12: printf("EDIT SLOT 1 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 13: printf("EDIT SLOT 2 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 14: printf("EDIT SLOT 3 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 15: *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; InitButtonRects(*currentGameState); break;
            }
            break;
        case GAME_STATE_DELETE:
            switch (index) {
                case 12: printf("DELETE SLOT 1 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 13: printf("DELETE SLOT 2 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 14: printf("DELETE SLOT 3 Ditekan\n"); *currentGameState = GAME_STATE_MAIN_MENU; InitButtonRects(*currentGameState); break;
                case 15: *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; InitButtonRects(*currentGameState); break;
            }
            break;
        
        case GAME_STATE_CONTINUE_SLOT_1:
            printf("Continue dari Slot 1\n");
            *currentGameState = GAME_STATE_STORY;
            InitButtonRects(*currentGameState);
            currentScene = 0; 
            currentFrame = 0; 
            LoadNodeAssets(Mytree, currentScene);
            break;
        case GAME_STATE_CONTINUE_SLOT_2:
            printf("Continue dari Slot 2\n");
            *currentGameState = GAME_STATE_STORY;
            InitButtonRects(*currentGameState);
            currentScene = 0; 
            currentFrame = 0; 
            LoadNodeAssets(Mytree, currentScene);
            break;
        case GAME_STATE_CONTINUE_SLOT_3:
            printf("Continue dari Slot 3\n");
            *currentGameState = GAME_STATE_STORY;
            InitButtonRects(*currentGameState);
            currentScene = 0; 
            currentFrame = 0; 
            LoadNodeAssets(Mytree, currentScene);
            break;
        
        case GAME_STATE_ABOUT:
            if (index == 15) { 
                *currentGameState = GAME_STATE_MAIN_MENU; 
                InitButtonRects(*currentGameState); 
            }
            break;

        default:
            break;
    }
    return false;
}

void UnloadAssets() {
    for (int i = 0; i < MAX_MENU; i++) {
        UnloadTexture(MenuButtons[i]);
    }
}         


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


