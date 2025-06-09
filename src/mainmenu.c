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

extern TreeStory Mytree[MAX_NODE_TREE];
extern int currentScene;
extern int currentFrame;
extern bool exitProgram;
bool showSaveMessage = false;
float saveMessageTimer = 0.0f;

void InitAssets() {
    // =================================== MENU STATE 0 ===================================
    MenuButtons[0] = LoadTexture("Assets/BackSprites/customstorymenu.png");
    MenuButtons[1] = LoadTexture("Assets/mainmenu/Start.png");
    MenuButtons[2] = LoadTexture("Assets/mainmenu/Studio_Game.png");
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
    MenuButtons[16] = LoadTexture("Assets/mainmenu/Continue.png");
    MenuButtons[17] = LoadTexture("Assets/mainmenu/Save_Game.png");
    MenuButtons[18] = LoadTexture("Assets/mainmenu/Main_Menu.png");

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

        case GAME_STATE_PLAY_GAME_MENU:
            printf("currentGameState PLAY_GAME_MENU\n");
            for (int i = 5, idx = 0; i <= 6; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
            printf("currentGameState NEW_CONTINUE_MENU\n");
            for (int i = 7, idx = 0; i <= 8; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;

        case GAME_STATE_STUDIO_MENU:
            printf("currentGameState STUDIO_MENU\n");
            for (int i = 9, idx = 0; i <= 11; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 3 * gapY);
            break;

        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
        case GAME_STATE_DELETE_MENU:
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

        case GAME_STATE_PLAY_GAME:
        case GAME_STATE_MINI_GAME_STACK:
            printf("currentGameState PLAY_GAME or MINIGAME\n");
            break;

        case GAME_STATE_PAUSE:
            printf("currentGameState PAUSE\n");
            for (int i = 16, idx = 0; i <= 18; i++, idx++) {
                buttonRects[i].height = (float)MenuButtons[i].height;
                buttonRects[i].width = (float)MenuButtons[i].width;
                buttonRects[i].x = SCREEN_WIDTH / 2 - buttonRects[i].width / 2;
                buttonRects[i].y = SCREEN_HEIGHT / 2 - 100 + idx * (gapY + 20);
            }
            break;

        default:
            break;
    }
}

void DrawMainMenu(GameState currentGameState) {
    if (currentGameState != GAME_STATE_PAUSE &&
        currentGameState != GAME_STATE_PLAY_GAME &&
        currentGameState != GAME_STATE_MINI_GAME_STACK) {
        DrawTexture(MenuButtons[0], 0, 0, WHITE);
    }

    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
            for (int i = 1; i <= 4; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            printf("DRAWING MENU STATE PLAY GAME\n");
            for (int i = 5; i <= 6; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
            printf("DRAWING MENU STATE NEW CONTINUE\n");
            for (int i = 7; i <= 8; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_STUDIO_MENU:
            for (int i = 9; i <= 11; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
        case GAME_STATE_DELETE_MENU:
            for (int i = 12; i <= 14; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_ABOUT:
            printf("DRAWING MENU STATE ABOUT\n");
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_PAUSE:
            printf("DRAWING GAME STATE PAUSE\n");
            for (int i = 16; i <= 18; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            if (showSaveMessage) {
                DrawText("Game Saved!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 200, 40, GREEN);
            }
            break;

        default:
            break;
    }

    if (selectedMenu != -1) {
        if (currentGameState != GAME_STATE_PAUSE &&
            currentGameState != GAME_STATE_PLAY_GAME &&
            currentGameState != GAME_STATE_MINI_GAME_STACK) {
            DrawRectangleLinesEx(buttonRects[selectedMenu], 10, WHITE);
        }
    }
}

void UpdateMainMenu(GameState *currentGameState) {
    Vector2 mousePoint = GetMousePosition();
    selectedMenu = -1;

    // Don't update menu if in story or minigame states
    if (*currentGameState == GAME_STATE_PLAY_GAME || 
        *currentGameState == GAME_STATE_MINI_GAME_STACK) {
        return;
    }

    int startIndex = 0, endIndex = 0;

    // Determine which buttons are active based on current state
    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU:
            startIndex = 1; endIndex = 4;
            break;
        case GAME_STATE_PLAY_GAME_MENU:
            startIndex = 5; endIndex = 6;
            break;
        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
            startIndex = 7; endIndex = 8;
            break;
        case GAME_STATE_STUDIO_MENU:
            startIndex = 9; endIndex = 11;
            break;
        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
        case GAME_STATE_DELETE_MENU:
            startIndex = 12; endIndex = 14;
            break;
        case GAME_STATE_ABOUT:
            startIndex = 15; endIndex = 15;
            break;
        case GAME_STATE_PAUSE:
            startIndex = 16; endIndex = 18;
            break;
        default:
            return;
    }

    // Check main button interactions
    for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckMenuClick(i, currentGameState)) {
                    InitButtonRects(*currentGameState);
                }
                return;
            }
        }
    }

    // Check back button interaction (except for main menu and about states)
    if (*currentGameState != GAME_STATE_MAIN_MENU && 
        *currentGameState != GAME_STATE_ABOUT && 
        *currentGameState != GAME_STATE_PAUSE) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[15])) {
            selectedMenu = 15;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckMenuClick(15, currentGameState)) {
                    InitButtonRects(*currentGameState);
                }
            }
        }
    }
}

bool CheckMenuClick(int index, GameState *currentGameState) {
    printf("Button %d clicked in state %d\n", index, *currentGameState);

    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU:
            switch (index) {
                case 1: // Start
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    return true;
                case 2: // Studio
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    return true;
                case 3: // About
                    *currentGameState = GAME_STATE_ABOUT;
                    return true;
                case 4: // Exit
                    exitProgram = true;
                    return true;
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            switch (index) {
                case 5: // Play Game
                    *currentGameState = GAME_STATE_NEW_CONTINUE_NON_CUSTOM;
                    return true;
                case 6: // Custom Game
                    *currentGameState = GAME_STATE_NEW_CONTINUE_CUSTOM;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    return true;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            switch (index) {
                case 7: // New Game
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    return true;
                case 8: // Continue
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    return true;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_CUSTOM:
            switch (index) {
                case 7: // New Game
                    *currentGameState = GAME_STATE_CREATE_MENU;
                    return true;
                case 8: // Continue
                    *currentGameState = GAME_STATE_PLAY_CUSTOM_SLOT_1;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    return true;
            }
            break;

        case GAME_STATE_STUDIO_MENU:
            switch (index) {
                case 9: // Create
                    *currentGameState = GAME_STATE_CREATE_MENU;
                    return true;
                case 10: // Edit
                    *currentGameState = GAME_STATE_EDIT_MENU;
                    return true;
                case 11: // Delete
                    *currentGameState = GAME_STATE_DELETE_MENU;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    return true;
            }
            break;

        case GAME_STATE_CREATE_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_CREATE_SLOT_1;
                    return true;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_CREATE_SLOT_2;
                    return true;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_CREATE_SLOT_3;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    return true;
            }
            break;

        case GAME_STATE_EDIT_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_EDIT_SLOT_1;
                    return true;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_EDIT_SLOT_2;
                    return true;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_EDIT_SLOT_3;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    return true;
            }
            break;

        case GAME_STATE_DELETE_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_DELETE_SLOT_1;
                    return true;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_DELETE_SLOT_2;
                    return true;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_DELETE_SLOT_3;
                    return true;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    return true;
            }
            break;

        case GAME_STATE_ABOUT:
            if (index == 15) { // Back
                *currentGameState = GAME_STATE_MAIN_MENU;
                return true;
            }
            break;

        case GAME_STATE_PAUSE:
            switch (index) {
                case 16: // Continue
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    return true;
                case 17: // Save Game
                    showSaveMessage = true;
                    saveMessageTimer = 0.0f;
                    return true;
                case 18: // Main Menu
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    return true;
            }
            break;
    }
    return false;
}

void UpdatePauseMenu(GameState *currentGameState) {
    Vector2 mousePoint = GetMousePosition();
    selectedMenu = -1;

    // Check if mouse is over any button
    for (int i = 16; i <= 18; i++) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckMenuClick(i, currentGameState)) {
                    InitButtonRects(*currentGameState);
                }
                return;
            }
        }
    }

    if (showSaveMessage) {
        saveMessageTimer += GetFrameTime();
        if (saveMessageTimer >= 2.0f) {
            showSaveMessage = false;
            saveMessageTimer = 0.0f;
        }
    }
}

void UnloadAssets() {
    for (int i = 0; i < MAX_MENU; i++) {
        UnloadTexture(MenuButtons[i]);
    }
}

void DrawDebugGrid(int gridSize) {
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

void DrawPauseMenu() {
    DrawTexture(MenuButtons[0], 0, 0, WHITE);

    DrawText("Pause Menu",
              SCREEN_WIDTH / 2 - MeasureText("Pause Menu", 60) / 2,
              SCREEN_HEIGHT / 2 - 250, 60, WHITE);

    for (int i = 16; i <= 18; i++) {
        DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
    }

    if (selectedMenu >= 16 && selectedMenu <= 18) {
        DrawRectangleLinesEx(buttonRects[selectedMenu], 4, YELLOW);
    }
    if (showSaveMessage) {
        DrawText("Story Has Been Saved", 
                 SCREEN_WIDTH / 2 - MeasureText("Story Has Been Saved", 50) / 2, 
                 SCREEN_HEIGHT / 2 - 175, 50, WHITE);
    }
}
