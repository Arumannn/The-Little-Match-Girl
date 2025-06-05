#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include "mainmenu.h"
#include "story.h"

Texture2D MenuButtons[MAX_MENU];
Rectangle buttonRects[MAX_MENU];
Music BGMusic;
int selectedMenu = -1;

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
    
    // Set background image menjadi full Screen
    MenuButtons[0].height = SCREEN_HEIGHT;
    MenuButtons[0].width = SCREEN_WIDTH;
    
    // Set semua button dengan ukuran yang sama
    for (int i = 1; i < MAX_MENU; i++) {
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
            SetButtonRect(15, SCREEN_WIDTH / 2 - MenuButtons[15].width / 2, SCREEN_HEIGHT - 500);
            break;
            
        case GAME_STATE_STORY:
        case GAME_STATE_MINI_GAME_STACK:
            // These states don't need button rectangles
            printf("currentGameState STORY or MINIGAME\n");
            break;
            
        default:
            break;
    }
}

void DrawMainMenu(GameState currentGameState){
    PlayMusicStream(BGMusic);
    DrawTexture(MenuButtons[0], 0, 0, WHITE); // Draw background
    
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
        case GAME_STATE_MINI_GAME_STACK:
            // These states don't draw menu buttons
            break;
            
        default:
            break;
    }

    // Draw selection highlight
    if (selectedMenu >= 0 && selectedMenu < MAX_MENU) {
        DrawRectangleLinesEx(buttonRects[selectedMenu], 10, WHITE);
    }
}

void UpdateMainMenu(GameState *currentGameState) {
    Vector2 mousePos = GetMousePosition();
    selectedMenu = -1;

    // Don't update menu if in story or minigame states
    if (*currentGameState == GAME_STATE_STORY || *currentGameState == GAME_STATE_MINI_GAME_STACK) {
        return;
    }
    
    int startIndex = 0, endIndex = 0;

    // Determine which buttons are active based on current state
    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU: 
            startIndex = 1; endIndex = 4; 
            break;
        case GAME_STATE_PLAY_GAME: 
            startIndex = 5; endIndex = 6; 
            break;
        case GAME_STATE_NEW_CONTINUE_MENU: 
            startIndex = 7; endIndex = 8; 
            break;
        case GAME_STATE_CUSTOM_GAME_MENU: 
            startIndex = 9; endIndex = 11; 
            break;
        case GAME_STATE_DELETE: 
        case GAME_STATE_CREATE:
        case GAME_STATE_EDIT: 
            startIndex = 12; endIndex = 14; 
            break;
        case GAME_STATE_ABOUT: 
            startIndex = 15; endIndex = 15; 
            break; 
        default: 
            return; 
    }

    // Check main button interactions
    for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePos, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i, currentGameState); 
            }
            return; 
        }
    }

    // Check back button interaction (except for main menu and about states that handle it differently)
    if (*currentGameState != GAME_STATE_MAIN_MENU && *currentGameState != GAME_STATE_ABOUT) {
        if (CheckCollisionPointRec(mousePos, buttonRects[15])) {
            selectedMenu = 15;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(15, currentGameState);
            }
        }
    } else if (*currentGameState == GAME_STATE_ABOUT) {
        // Special handling for about state back button
        if (CheckCollisionPointRec(mousePos, buttonRects[15])) {
            selectedMenu = 15;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(15, currentGameState);
            }
        }
    }
}

bool CheckMenuClick(int index, GameState *currentGameState) {
    printf("Button %d clicked in state %d\n", index, *currentGameState);
    
    switch (*currentGameState) { 
        case GAME_STATE_MAIN_MENU:
            switch (index) {
                case 1: // Start Game
                    *currentGameState = GAME_STATE_PLAY_GAME; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 2: // Studio Game (Custom Games)
                    *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 3: // About
                    *currentGameState = GAME_STATE_ABOUT; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 4: // Exit
                    CloseWindow(); 
                    break;
            }
            break;

        case GAME_STATE_PLAY_GAME:
            switch (index) {
                case 5: // Play Game - Start with minigame first
                    printf("Starting minigame before story\n");
                    *currentGameState = GAME_STATE_MINI_GAME_STACK; 
                    // Don't initialize button rects for minigame
                    break;
                case 6: // Custom Game
                    *currentGameState = GAME_STATE_NEW_CONTINUE_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_MENU:
            switch(index){
                case 7: // New Game
                    printf("Starting new custom game\n");
                    *currentGameState = GAME_STATE_CREATE; 
                    InitButtonRects(*currentGameState);
                    break;
                case 8: // Continue
                    printf("Loading saved game\n");
                    *currentGameState = GAME_STATE_CONTINUE_SLOT_1; 
                    InitButtonRects(*currentGameState);
                    break;
                case 15: // Back
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
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;

        case GAME_STATE_CREATE:
            switch (index) {
                case 12: 
                    printf("CREATE SLOT 1 selected\n"); 
                    *currentGameState = GAME_STATE_CREATE_SLOT_1; 
                    break;
                case 13: 
                    printf("CREATE SLOT 2 selected\n"); 
                    *currentGameState = GAME_STATE_CREATE_SLOT_2; 
                    break;
                case 14: 
                    printf("CREATE SLOT 3 selected\n"); 
                    *currentGameState = GAME_STATE_CREATE_SLOT_3; 
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;
            
        case GAME_STATE_EDIT:
            switch (index) {
                case 12: 
                    printf("EDIT SLOT 1 selected\n"); 
                    *currentGameState = GAME_STATE_EDIT_SLOT_1; 
                    break;
                case 13: 
                    printf("EDIT SLOT 2 selected\n"); 
                    *currentGameState = GAME_STATE_EDIT_SLOT_2; 
                    break;
                case 14: 
                    printf("EDIT SLOT 3 selected\n"); 
                    *currentGameState = GAME_STATE_EDIT_SLOT_3; 
                    break;
                case 15: 
                    *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;
            
        case GAME_STATE_DELETE:
            switch (index) {
                case 12: 
                    printf("DELETE SLOT 1 selected\n"); 
                    *currentGameState = GAME_STATE_DELETE_SLOT_1; 
                    break;
                case 13: 
                    printf("DELETE SLOT 2 selected\n"); 
                    *currentGameState = GAME_STATE_DELETE_SLOT_2; 
                    break;
                case 14: 
                    printf("DELETE SLOT 3 selected\n"); 
                    *currentGameState = GAME_STATE_DELETE_SLOT_3; 
                    break;
                case 15: 
                    *currentGameState = GAME_STATE_CUSTOM_GAME_MENU; 
                    InitButtonRects(*currentGameState); 
                    break;
            }
            break;
        
        case GAME_STATE_CONTINUE_SLOT_1:
            printf("Continue from Slot 1\n");
            *currentGameState = GAME_STATE_STORY;
            currentScene = 0; 
            currentFrame = 0; 
            LoadNodeAssets(Mytree, currentScene);
            break;
            
        case GAME_STATE_CONTINUE_SLOT_2:
            printf("Continue from Slot 2\n");
            *currentGameState = GAME_STATE_STORY;
            currentScene = 0; 
            currentFrame = 0; 
            LoadNodeAssets(Mytree, currentScene);
            break;
            
        case GAME_STATE_CONTINUE_SLOT_3:
            printf("Continue from Slot 3\n");
            *currentGameState = GAME_STATE_STORY;
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
    return true; 
}

void UnloadAssets() {
    for (int i = 0; i < MAX_MENU; i++) {
        UnloadTexture(MenuButtons[i]);
    }
}         

v oid DrawDebugGrid(int gridSize) {
    Color gridColor = DARKGRAY;

    // Draw vertical lines
    for (int x = 0; x <= SCREEN_WIDTH; x += gridSize) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, gridColor);
        DrawText(TextFormat("%d", x), x + 2, 0, 10, GRAY);
    }

    // Draw horizontal lines
    for (int y = 0; y <= SCREEN_HEIGHT; y += gridSize) {
        DrawLine(0, y, SCREEN_WIDTH, y, gridColor);
        DrawText(TextFormat("%d", y), 0, y + 2, 10, GRAY);
    }
}