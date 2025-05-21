#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include "mainmenu.h"

Texture2D MenuButtons[MAX_MENU];
Rectangle buttonRects[MAX_MENU];
Music BGMusic;
int selectedMenu = -1;
int MenuState = 0;

void InitAssets(){

    InitAudioDevice();
    BGMusic = LoadMusicStream("Assets/music/Jingle Bells.mp3");
    PlayMusicStream(BGMusic); 
    SetMusicVolume(BGMusic, 1.0f);
    // =================================== MENU STATE 0 ===================================
    MenuButtons[0] = LoadTexture("Assets/mainmenu/background_starter.jpeg");
    MenuButtons[1] = LoadTexture("Assets/mainmenu/Start.png");
    MenuButtons[2] = LoadTexture("Assets/mainmenu/Studio Game.png");
    MenuButtons[3] = LoadTexture("Assets/mainmenu/About.png");
    MenuButtons[4] = LoadTexture("Assets/mainmenu/Exit.png");
    // =================================== MENU STATE 1 ===================================
    MenuButtons[5] = LoadTexture("Assets/mainmenu/Play_Game.png");
    MenuButtons[6] = LoadTexture("Assets/mainmenu/Costum_Game.png");
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
    
    
    for (int i = 1; i < MAX_MENU; i++)
    {
        MenuButtons[i].height = 230 / 2;
        MenuButtons[i].width = 630 / 2;
    }   
}           

void InitButtonRects() {
    int startX = 100;
    int startY = 300;
    int gapY = 150;
    
// =================================== MENU STATE 0 ===================================
    switch (MenuState)
    {
    case 0:
        printf("MENUSTATE 0\n");
        for (int i = 1; i <= 4; i++) {
            buttonRects[i].x = startX;
            buttonRects[i].y = startY + i * gapY;
            buttonRects[i].width = (float)MenuButtons[i].width;
            buttonRects[i].height = (float)MenuButtons[i].height;
        } 
        break;
// =================================== MENU STATE 1 ===================================
    case 1:
        printf("MENUSTATE 1\n");
        for (int i = 5; i <= 6; i++) {
            buttonRects[i].x = startX;
            buttonRects[i].y = startY + (i-5) * gapY;
            buttonRects[i].width = (float)MenuButtons[i].width;
            buttonRects[i].height = (float)MenuButtons[i].height;
        }
            buttonRects[15].x = startX;
            buttonRects[15].y = startY + 2 * gapY;
            buttonRects[15].width = (float)MenuButtons[15].width;
            buttonRects[15].height = (float)MenuButtons[15].height;
            
        break;
// =================================== MENU STATE 2 ===================================

    case 2:
        printf("MENUSTATE 2\n");
        for (int i = 7; i <= 8; i++) {
            buttonRects[i].x = startX;
            buttonRects[i].y = startY + (i-6) * gapY;
            buttonRects[i].width = (float)MenuButtons[i].width;
            buttonRects[i].height = (float)MenuButtons[i].height;
        }
            buttonRects[15].x = startX;
            buttonRects[15].y = startY + 3 * gapY;
            buttonRects[15].width = (float)MenuButtons[15].width;
            buttonRects[15].height = (float)MenuButtons[15].height;
        break;
// =================================== MENU STATE 3 ===================================

    case 3:
        printf("MENUSTATE 3\n");
        for (int i = 9; i <= 11; i++) {
            buttonRects[i].x = startX;
            buttonRects[i].y = startY + (i-9) * gapY;
            buttonRects[i].width = (float)MenuButtons[i].width;
            buttonRects[i].height = (float)MenuButtons[i].height;
        }
            buttonRects[15].x = startX;
            buttonRects[15].y = startY + 3 * gapY;
            buttonRects[15].width = (float)MenuButtons[15].width;
            buttonRects[15].height = (float)MenuButtons[15].height;
        break;
    case 4:
// =================================== MENU STATE 4 ===================================
        printf("MENUSTATE 4\n");
        for (int i = 12; i <= 14; i++) {
            buttonRects[i].x = startX;
            buttonRects[i].y = startY + (i-13) * gapY;
            buttonRects[i].width = (float)MenuButtons[i].width;
            buttonRects[i].height = (float)MenuButtons[i].height;
        }
            buttonRects[15].x = startX;
            buttonRects[15].y = startY + 3 * gapY;
            buttonRects[15].width = (float)MenuButtons[15].width;
            buttonRects[15].height = (float)MenuButtons[15].height;
        break;
    case 5:
    // =================================== MENU STATE 5 ===================================
    printf("MENUSTATE 5\n");
        buttonRects[15].x = 950;
        buttonRects[15].y = 930;
        buttonRects[15].width = (float)MenuButtons[15].width;
        buttonRects[15].height = (float)MenuButtons[15].height;
    default:
        break;
    }
}

void DrawMainMenu(){
    PlayMusicStream(BGMusic);
    DrawTexture(MenuButtons[0], 0, 0, WHITE);
    switch (MenuState) {
        case 0:
            for (int i = 1; i <= 4; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            break;
        case 1:
            printf("DRAWING MENU STATE 1\n");
            for (int i = 5; i <= 6; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case 2:
            printf("DRAWING MENU STATE 2\n");
            for (int i = 7; i <= 8; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            DrawText("About this game: Visual Novel by Hans Christian Andersen", SCREEN_WIDTH / 2 - 200, 100, 20, BLACK);
            break;
        case 3:
            for (int i = 9; i <= 11; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
        case 4:
            for (int i = 12; i <= 14; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;
    }

    if (selectedMenu >= 0 && selectedMenu < MAX_MENU) {
        DrawRectangleLinesEx(buttonRects[selectedMenu], 3, YELLOW);
    }
}

void UpdateMainMenu() {
    Vector2 mousePos = GetMousePosition();
    selectedMenu = -1;

    int startIndex = 0, endIndex = 0;

    switch (MenuState) {
        case 0: startIndex = 1; endIndex = 4; break;
        case 1: startIndex = 5; endIndex = 6; break;
        case 2: startIndex = 7; endIndex = 8; break;
        case 3: startIndex = 9; endIndex = 11; break;
        case 4: startIndex = 12; endIndex = 14; break;
    }

    for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePos, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i);
            }
            return; 
        }
    }

    if (MenuState >= 1 && MenuState <= 4) {
        if (CheckCollisionPointRec(mousePos, buttonRects[15])) {
            selectedMenu = 15;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(15);
            }
        }
    }
}

bool CheckMenuClick(int index) {
    printf("Tombol tertekan\n");
    if (MenuState == 0) {
        switch (index) {
            case 1: MenuState = 1; InitButtonRects(); return true;
            case 2: MenuState = 3; InitButtonRects(); return true;
            case 3: MenuState = 5; InitButtonRects(); return true;
        }
    } else if (MenuState == 1) {
        switch (index) {
            case 5: MenuState = 2; InitButtonRects(); return true;
            case 6: MenuState = 2; InitButtonRects(); return true;
            case 15: MenuState = 0; InitButtonRects(); return true;
        }
    } else if (MenuState == 3){
        switch (index) {
            case 9: MenuState = 4; InitButtonRects(); return true;
            case 10: MenuState = 4; InitButtonRects(); return true;
            case 11: MenuState = 4; InitButtonRects(); return true;
            case 15: MenuState = 0; InitButtonRects(); return true;

        }
    } else if (MenuState == 1 || MenuState == 2 || MenuState == 4 || MenuState == 3) {
        if (index == 15) {
            MenuState = 0;
            InitButtonRects();
            return true;
        }
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


