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

void InitAssets(){
    MenuButtons[0] = LoadTexture("Assets/BackSprites/customstorymenu.png");
    MenuButtons[1] = LoadTexture("Assets/mainmenu/Start.png");
    MenuButtons[2] = LoadTexture("Assets/mainmenu/Studio_Game.png");
    MenuButtons[3] = LoadTexture("Assets/mainmenu/About.png");
    MenuButtons[4] = LoadTexture("Assets/mainmenu/Exit.png");
    MenuButtons[5] = LoadTexture("Assets/mainmenu/Play_Game.png");
    MenuButtons[6] = LoadTexture("Assets/mainmenu/Custom_Game.png");
    MenuButtons[7] = LoadTexture("Assets/mainmenu/New_Game.png");
    MenuButtons[8] = LoadTexture("Assets/mainmenu/Continue.png");
    MenuButtons[9] = LoadTexture("Assets/mainmenu/Create_Game.png");
    MenuButtons[10] = LoadTexture("Assets/mainmenu/Edit_Game.png");
    MenuButtons[11] = LoadTexture("Assets/mainmenu/Delete_Game.png");
    MenuButtons[12] = LoadTexture("Assets/mainmenu/Slot_1.png");
    MenuButtons[13] = LoadTexture("Assets/mainmenu/Slot_2.png");
    MenuButtons[14] = LoadTexture("Assets/mainmenu/Slot_3.png");
    MenuButtons[15] = LoadTexture("Assets/mainmenu/Back.png");
    MenuButtons[16] = LoadTexture("Assets/mainmenu/Continue.png");
    MenuButtons[17] = LoadTexture("Assets/mainmenu/Save_Game.png");
    MenuButtons[18] = LoadTexture("Assets/mainmenu/Main_Menu.png");

    MenuButtons[0].height = SCREEN_HEIGHT;
    MenuButtons[0].width = SCREEN_WIDTH;

    for (int i = 1; i <= MAX_MENU; i++) {
        MenuButtons[i].height = MenuButtons[i].height / 2;
        MenuButtons[i].width = MenuButtons[i].width / 2;
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
    int gapY = 120;

    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
            printf("InitButtonRects: GAME_STATE_MAIN_MENU\n");
            for (int i = 1, idx = 0; i <= 4; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            printf("InitButtonRects: GAME_STATE_PLAY_GAME_MENU\n");
            for (int i = 5, idx = 0; i <= 6; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            printf("InitButtonRects: GAME_STATE_NEW_CONTINUE_NON_CUSTOM\n");
            for (int i = 7, idx = 0; i <= 8; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 2 * gapY);
            break;

        case GAME_STATE_STUDIO_MENU:
            printf("InitButtonRects: GAME_STATE_STUDIO_MENU\n");
            for (int i = 9, idx = 0; i <= 11; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 3 * gapY);
            break;

        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
        case GAME_STATE_DELETE_MENU:
            printf("InitButtonRects: GAME_STATE_CREATE_MENU/EDIT/DELETE (SLOTS)\n");
            for (int i = 12, idx = 0; i <= 14; i++, idx++) {
                SetButtonRect(i, startX, startY + idx * gapY);
            }
            SetButtonRect(15, startX, startY + 3 * gapY);
            break;

        case GAME_STATE_ABOUT:
            printf("InitButtonRects: GAME_STATE_ABOUT\n");
            SetButtonRect(15, SCREEN_WIDTH / 2 - MenuButtons[15].width / 2, SCREEN_HEIGHT / 2 + 100);
            break;

        case GAME_STATE_PLAY_GAME:
        case GAME_STATE_MINI_GAME_STACK:
            printf("InitButtonRects: GAME_STATE_PLAY_GAME or MINIGAME (no main menu buttons)\n");
            break;

        case GAME_STATE_PAUSE:
            printf("InitButtonRects: GAME_STATE_PAUSE\n");
            for (int i = 16, idx = 0; i <= 18; i++, idx++) {
                buttonRects[i].height = (float)MenuButtons[i].height;
                buttonRects[i].width = (float)MenuButtons[i].width;
                buttonRects[i].x = SCREEN_WIDTH / 2 - buttonRects[i].width / 2;
                buttonRects[i].y = SCREEN_HEIGHT / 2 - 100 + idx * (gapY + 20);
            }
            break;

        default:
            printf("InitButtonRects: DEFAULT (no specific initialization)\n");
            break;
    }
}

void DrawMainMenu(GameState currentGameState){

    if (currentGameState != GAME_STATE_PAUSE &&
        currentGameState != GAME_STATE_PLAY_GAME &&
        currentGameState != GAME_STATE_MINI_GAME_STACK)
    {
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
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
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
        case GAME_STATE_PLAY_GAME:
            printf("DRAWING GAME STATE PLAY GAME (no main menu buttons)\n");
            break;
        case GAME_STATE_MINI_GAME_STACK:
            break;
        case GAME_STATE_PAUSE:
            printf("DRAWING GAME STATE PAUSE (handled by DrawPauseMenu)\n");
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
    Vector2 mousePos = GetMousePosition();
    selectedMenu = -1;

    if (*currentGameState == GAME_STATE_PLAY_GAME ||
        *currentGameState == GAME_STATE_MINI_GAME_STACK ||
        *currentGameState == GAME_STATE_PAUSE) {
        return;
    }

    int startIndex = 0, endIndex = 0;
    int backButtonIndex = -1;

    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU:
            startIndex = 1; endIndex = 4;
            break;
        case GAME_STATE_PLAY_GAME_MENU:
            startIndex = 5; endIndex = 6;
            backButtonIndex = 15;
            break;
        case GAME_STATE_NEW_CONTINUE_CUSTOM:
        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            startIndex = 7; endIndex = 8;
            backButtonIndex = 15;
            break;
        case GAME_STATE_STUDIO_MENU:
            startIndex = 9; endIndex = 11;
            backButtonIndex = 15;
            break;
        case GAME_STATE_DELETE_MENU:
        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
            startIndex = 12; endIndex = 14;
            backButtonIndex = 15;
            break;
        case GAME_STATE_ABOUT:
            startIndex = 15; endIndex = 15;
            backButtonIndex = 15;
            break;
        default:
            return;
    }

    for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePos, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i, currentGameState);
            }
        }
    }

    if (backButtonIndex != -1) {
        if (CheckCollisionPointRec(mousePos, buttonRects[backButtonIndex])) {
            selectedMenu = backButtonIndex;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(backButtonIndex, currentGameState);
            }
        }
    }
}

bool CheckMenuClick(int index, GameState *currentGameState) {
    printf("Button %d clicked in state %d\n", index, *currentGameState);

    GameState previousState = *currentGameState;

    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU:
            switch (index) {
                case 1:
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
                case 2:
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    break;
                case 3:
                    *currentGameState = GAME_STATE_ABOUT;
                    break;
                case 4:
                    exitProgram = true;
                    return false;
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            switch (index) {
                case 5:
                    *currentGameState = GAME_STATE_NEW_CONTINUE_NON_CUSTOM;
                    break;
                case 6:
                    *currentGameState = GAME_STATE_NEW_CONTINUE_CUSTOM;
                    break;
                case 15:
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            switch(index){
                case 7:
                    *currentGameState = GAME_STATE_MINI_GAME_STACK;
                    currentScene = 0;
                    currentFrame = 0;
                    break;
                case 8:
                    LoadGameStory("savefile.txt", &currentScene, &currentFrame);
                    LoadNodeAssets(Mytree, currentScene);
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    break;
                case 15:
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
            }
            break;
            case GAME_STATE_NEW_CONTINUE_CUSTOM:
            switch(index){
                case 7:
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
                case 8:
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
                case 15:
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
            }
            break;

        case GAME_STATE_STUDIO_MENU:
            switch (index) {
                case 9:
                    *currentGameState = GAME_STATE_CREATE_MENU;
                    break;
                case 10:
                    *currentGameState = GAME_STATE_EDIT_MENU;
                    break;
                case 11:
                    *currentGameState = GAME_STATE_DELETE_MENU;
                    break;
                case 15:
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
            }
            break;

        case GAME_STATE_CREATE_MENU:
            switch (index) {
                case 12: printf("CREATE SLOT 1 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 13: printf("CREATE SLOT 2 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 14: printf("CREATE SLOT 3 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 15: *currentGameState = GAME_STATE_STUDIO_MENU; break;
            }
            break;

        case GAME_STATE_EDIT_MENU:
            switch (index) {
                case 12: printf("EDIT SLOT 1 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 13: printf("EDIT SLOT 2 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 14: printf("EDIT SLOT 3 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 15: *currentGameState = GAME_STATE_STUDIO_MENU; break;
            }
            break;

        case GAME_STATE_DELETE_MENU:
            switch (index) {
                case 12: printf("DELETE SLOT 1 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 13: printf("DELETE SLOT 2 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 14: printf("DELETE SLOT 3 selected\n"); *currentGameState = GAME_STATE_STUDIO_MENU; break;
                case 15: *currentGameState = GAME_STATE_STUDIO_MENU; break;
            }
            break;

        case GAME_STATE_CONTINUE_SLOT_1:
        case GAME_STATE_CONTINUE_SLOT_2:
        case GAME_STATE_CONTINUE_SLOT_3:
            printf("Attempted to click in a CONTINUE_SLOT state, unexpected.\n");
            *currentGameState = GAME_STATE_PLAY_GAME;
            break;

        case GAME_STATE_ABOUT:
            if (index == 15) {
                *currentGameState = GAME_STATE_MAIN_MENU;
            }
            break;

        case GAME_STATE_PAUSE:
            switch (index) {
                case 16:
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    break;
                case 17:
                    SaveGameStory("savefile.txt", currentScene, currentFrame);
                    showSaveMessage = true;
                    saveMessageTimer = 0.0f;
                    break;
                case 18:
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    currentFrame = 0;
                    currentScene = 0;
                    break;
                default:
                    break;
            }
            break;
    }

    if (previousState != *currentGameState) {
        InitButtonRects(*currentGameState);
        selectedMenu = -1;
    }

    return true;
}

void UnloadAssets() {
    for (int i = 0; i < MAX_MENU; i++) {
        UnloadTexture(MenuButtons[i]);
    }
}

void DrawDebugGrid(int gridSize) {
    Color gridColor = DARKGRAY;

    for (int x = 0; x <= SCREEN_WIDTH; x += gridSize) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, gridColor);
        DrawText(TextFormat("%d", x), x + 2, 0, 10, GRAY);
    }

    for (int y = 0; y <= SCREEN_HEIGHT; y += gridSize) {
        DrawLine(0, y, SCREEN_WIDTH, y, gridColor);
        DrawText(TextFormat("%d", y), 0, y + 2, 10, GRAY);
    }
}

void UpdatePauseMenu(GameState * currentGameState){
    Vector2 mousePos = GetMousePosition();
    selectedMenu = -1;

    if (showSaveMessage) {
        saveMessageTimer += GetFrameTime();
        if (saveMessageTimer >= 2.0f) { 
            showSaveMessage = false;
            saveMessageTimer = 0.0f;
        }
    }

    for (int i = 16; i <= 18; i++) {
        if (CheckCollisionPointRec(mousePos, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i, currentGameState);
                return;
            }
        }
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
                 SCREEN_HEIGHT / 2 - 175, 50, WHITE) ; 
    }
}
