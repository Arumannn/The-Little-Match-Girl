// Initial State: Menu utama belum ditampilkan
// Final State: Menu utama ditampilkan dan user dapat memilih opsi

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raylib.h>
#include "mainmenu.h"
#include "story.h"
#include "customstory.h"

Texture2D MenuButtons[MAX_MENU];
Rectangle buttonRects[MAX_MENU];
Music BGMusic;
int selectedMenu = -1;

extern TreeStory Mytree[MAX_NODE_TREE];
extern int storyCurrentScene;
extern int storyCurrentFrame;
extern bool exitProgram;
extern CustomSceneTree customStorySlots[3];
extern int currentCustomSlot;
extern int customCurrentNode;
extern int customCurrentScene;
bool showSaveMessage = false;
float saveMessageTimer = 0.0f;

// IS: Asset menu belum dimuat
// FS: Semua asset menu berhasil dimuat dan siap digunakan
void InitAssetsMenu() {
    // Initialize menu BGM
    BGMusic = LoadMusicStream("Assets/Music/SilentNight.mp3");
    SetMusicVolume(BGMusic, 0.5f);
    PlayMusicStream(BGMusic);
    BGMusic.looping = true;

    // =================================== MENU STATE 0 ===================================
    MenuButtons[0] = LoadTexture("Assets/mainmenu/customstorymenu.png");
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
    MenuButtons[19] = LoadTexture("Assets/mainmenu/Reference.png");
    

    // Set background image menjadi full Screen
    MenuButtons[0].height = SCREEN_HEIGHT;
    MenuButtons[0].width = SCREEN_WIDTH;
    
    MenuButtons[19].height = MenuButtons[19].height / 5;
    MenuButtons[19].width = MenuButtons[19].width / 5;
    // Set semua button dengan ukuran yang sama
    for (int i = 1; i < MAX_MENU; i++) {
        MenuButtons[i].height = 230 / 2;
        MenuButtons[i].width = 630 / 2;
    }
}

// IS: Posisi button belum ditentukan
// FS: Posisi button berhasil ditentukan sesuai koordinat yang diberikan
void SetButtonRect(int id, int x, int y) {
    buttonRects[id].x = x;
    buttonRects[id].y = y;
    buttonRects[id].width = (float)MenuButtons[id].width;
    buttonRects[id].height = (float)MenuButtons[id].height;
}

// IS: Layout button belum diatur sesuai game state
// FS: Layout button berhasil diatur sesuai game state yang aktif
void InitButtonRects(GameState currentGameState) {
    // Center all buttons horizontally
    int centerX = SCREEN_WIDTH / 2 - (630 / 2) / 2; // Center button width
    int startY = 250; // Start a bit higher to make room for title
    int gapY = 150; // Slightly smaller gap for better spacing

    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
            printf("currentGameState MAIN_MENU\n");
            for (int i = 1, idx = 0; i <= 4; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            printf("currentGameState PLAY_GAME_MENU\n");
            for (int i = 5, idx = 0; i <= 6; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 2 * gapY);
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            printf("currentGameState NEW_CONTINUE_MENU\n");
            for (int i = 7, idx = 0; i <= 8; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 2 * gapY);
            break;
        case GAME_STATE_STUDIO_MENU:
            printf("currentGameState STUDIO_MENU\n");
            for (int i = 9, idx = 0; i <= 11; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 3 * gapY);
            break;

        case GAME_STATE_CREATE_MENU:
        case GAME_STATE_EDIT_MENU:
        case GAME_STATE_DELETE_MENU:
            printf("currentGameState CREATE/EDIT/DELETE (SLOTS)\n");
            for (int i = 12, idx = 0; i <= 14; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 3 * gapY);
            break;

        case GAME_STATE_ABOUT:
            printf("currentGameState ABOUT\n");
            SetButtonRect(15, centerX, SCREEN_HEIGHT - 200);
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

        case GAME_STATE_NEW_CONTINUE_CUSTOM: // New case for "New Game" / "Continue" for custom stories
            printf("currentGameState NEW_CONTINUE_CUSTOM\n");
            for (int i = 7, idx = 0; i <= 8; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 2 * gapY);
            break;

        case GAME_STATE_PLAY_CUSTOM_MENU: // New case for displaying custom story slots
            printf("currentGameState PLAY_CUSTOM_MENU\n");
            for (int i = 12, idx = 0; i <= 14; i++, idx++) {
                SetButtonRect(i, centerX, startY + idx * gapY);
            }
            SetButtonRect(15, centerX, startY + 3 * gapY);
            break;

        default:
            break;
    }
}

void DrawMainMenu(GameState currentGameState) {
    printf("Drawing main menu with currentGameState: %d\n", currentGameState);
    if (currentGameState != GAME_STATE_PAUSE &&
        currentGameState != GAME_STATE_PLAY_GAME &&
        currentGameState != GAME_STATE_MINI_GAME_STACK) {
        DrawTexture(MenuButtons[0], 0, 0, WHITE);
        
        // Draw the title "The Little Match Girl" at the top
        
        DrawText("The Little Match Girl", SCREEN_WIDTH / 2 - MeasureText("The Little Match Girl", 60) / 2, 80, 60, WHITE);
    }
    printf("Switch case currentGameState: %d\n", currentGameState);
    
    // Define button positioning variables
    int startX = 100;
    int startY = 300;
    
    switch (currentGameState) {
        case GAME_STATE_MAIN_MENU:
        printf("Entering switch case for currentGameState: %d\n", currentGameState);
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
            // Draw the reference image centered on screen with its specific size
            int imagePosX = SCREEN_WIDTH / 2 - MenuButtons[19].width / 2;
            int imagePosY = SCREEN_HEIGHT / 2 - MenuButtons[19].height / 2;
            
            // Right side: Game information
            int textStartX = 750;
            int textStartY = 275;
            int lineHeight = 35;
            
            // Title
            DrawText("The Little Match Girl", textStartX, textStartY, 40, WHITE);
            
            // Version
            DrawText("Version 1.0", textStartX, textStartY + 60, 25, LIGHTGRAY);
            
            // Game Description
            DrawText("Game Description", textStartX, textStartY + 120, 30, YELLOW);
            DrawText("An interactive visual novel game based on the classic fairy tale.", textStartX, textStartY + 160, 20, WHITE);
            
            // Story Synopsis
            DrawText("Story Synopsis", textStartX, textStartY + 220, 30, YELLOW);
            DrawText("Follow the journey of a little girl selling matches on a cold winter night.", textStartX, textStartY + 260, 20, WHITE);
            DrawText("Experience her dreams and hopes through interactive storytelling.", textStartX, textStartY + 285, 20, WHITE);
            
            // Development Team
            DrawText("Development Team", textStartX, textStartY + 340, 30, YELLOW);
            DrawText("Arman Yusuf Rifandi & Andreas Devan Pandu N.", textStartX, textStartY + 380, 20, WHITE);
            
            // Special Thanks
            DrawText("Special Thanks", textStartX, textStartY + 430, 30, YELLOW);
            DrawText("Cursor AI, ChatGPT, Gemini, Claude, Grok & GitHub Copilot", textStartX, textStartY + 470, 20, WHITE);
            
            DrawTexture(MenuButtons[19], imagePosX - 575, imagePosY, WHITE);
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

        case GAME_STATE_NEW_CONTINUE_CUSTOM: // Draw New Game / Continue buttons for custom story
            for (int i = 7; i <= 8; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
            break;

        case GAME_STATE_PLAY_CUSTOM_MENU: // Draw custom story slots
            for (int i = 12; i <= 14; i++) {
                DrawTexture(MenuButtons[i], (int)buttonRects[i].x, (int)buttonRects[i].y, WHITE);
                // Draw "Empty Slot" text if the slot is NULL
                if (customStorySlots[i - 12] == NULL) { 
                    DrawText("Empty Slot", (int)buttonRects[i].x + MenuButtons[i].width + 20, (int)buttonRects[i].y + MenuButtons[i].height / 2 - 10, 30, RED);
                }
            }
            DrawTexture(MenuButtons[15], (int)buttonRects[15].x, (int)buttonRects[15].y, WHITE);
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

// IS: State menu belum diperbarui
// FS: State menu berhasil diperbarui sesuai input user
void UpdateMainMenu(GameState *currentGameState) {
    printf("Updating main menu with currentGameState: %d\n", *currentGameState);
    UpdateMusicStream(BGMusic);
    // Update main menu music
    
    Vector2 mousePoint = GetMousePosition();
    selectedMenu = -1;

    // Don't update menu if in story or minigame states
    if (*currentGameState == GAME_STATE_PLAY_GAME || 
        *currentGameState == GAME_STATE_MINI_GAME_STACK ||
        *currentGameState == GAME_STATE_PAUSE ) {
        printf("Skipping main menu update in GAME_STATE_PLAY_GAME or GAME_STATE_MINI_GAME_STACK\n");
        return;
    }
    printf("Indexing buttonRects based on currentGameState: %d\n", *currentGameState);
    int startIndex = 0, endIndex = 0;
    int backButtonIndex = 15;

    // Determine which buttons are active based on current state
    switch (*currentGameState) {
        case GAME_STATE_MAIN_MENU:
            startIndex = 1; endIndex = 4;
            break;
        case GAME_STATE_PLAY_GAME_MENU:
            startIndex = 5; endIndex = 6;
            break;
        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
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
        case GAME_STATE_NEW_CONTINUE_CUSTOM: // Handle interaction for "New Game" / "Continue"
            startIndex = 7; endIndex = 8;
            break;
        case GAME_STATE_PLAY_CUSTOM_MENU: // Handle interaction for custom story slots
            startIndex = 12; endIndex = 14;
            break;
        default:
            return;
    }

    // Check main button interactions
   for (int i = startIndex; i <= endIndex; i++) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CheckMenuClick(i, currentGameState);
            }
        }
    }

    if (backButtonIndex != -1) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[backButtonIndex])) {
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
                case 1: // Start
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
                case 2: // Studio
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    break;
                case 3: // About
                    *currentGameState = GAME_STATE_ABOUT;
                    break;
                case 4: // Exit
                    exitProgram = true;
                    return false;
            }
            break;

        case GAME_STATE_PLAY_GAME_MENU:
            switch (index) {
                case 5: // Play Game
                    *currentGameState = GAME_STATE_NEW_CONTINUE_NON_CUSTOM;
                    break;
                case 6: // Custom Game
                    *currentGameState = GAME_STATE_NEW_CONTINUE_CUSTOM;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            switch (index) {               
                 case 7: // New Game
                    *currentGameState = GAME_STATE_MINI_GAME_STACK;
                    storyCurrentScene = 0;
                    storyCurrentFrame = 0;
                    LoadNodeAssets(Mytree, storyCurrentScene);
                    break;
                case 8: // Continue
                    LoadGameStory("saves/story/progress_slot_1.dat", &storyCurrentScene, &storyCurrentFrame);
                    LoadNodeAssets(Mytree, storyCurrentScene);
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
            }
            break;

        case GAME_STATE_NEW_CONTINUE_CUSTOM: // Handle actions for "New Game" / "Continue" in custom story
            switch (index) {
                case 7: // New Game
                case 8: // Continue (For now, both lead to slot selection)
                    *currentGameState = GAME_STATE_PLAY_CUSTOM_MENU; // Transition to a state for slot selection
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_PLAY_GAME_MENU;
                    break;
            }
            break;

        case GAME_STATE_STUDIO_MENU:
            switch (index) {
                case 9: // Create
                    *currentGameState = GAME_STATE_CREATE_MENU;
                    break;
                case 10: // Edit
                    *currentGameState = GAME_STATE_EDIT_MENU;
                    break;
                case 11: // Delete
                    *currentGameState = GAME_STATE_DELETE_MENU;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    break;
            }
            break;

        case GAME_STATE_CREATE_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_CREATE_SLOT_1;
                    break;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_CREATE_SLOT_2;
                    break;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_CREATE_SLOT_3;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    break;
            }
            break;

        case GAME_STATE_EDIT_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_EDIT_SLOT_1;
                    break;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_EDIT_SLOT_2;
                    break;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_EDIT_SLOT_3;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    break;
            }
            break;

        case GAME_STATE_DELETE_MENU:
            switch (index) {
                case 12: // Slot 1
                    *currentGameState = GAME_STATE_DELETE_SLOT_1;
                    break;
                case 13: // Slot 2
                    *currentGameState = GAME_STATE_DELETE_SLOT_2;
                    break;
                case 14: // Slot 3
                    *currentGameState = GAME_STATE_DELETE_SLOT_3;
                    break;
                case 15: // Back
                    *currentGameState = GAME_STATE_STUDIO_MENU;
                    break;
            }
            break;

        case GAME_STATE_ABOUT:
            if (index == 15) { // Back
                *currentGameState = GAME_STATE_MAIN_MENU;
                break;
            }
            break;
        case GAME_STATE_PAUSE:
            switch (index) {
                case 16: // Continue
                    *currentGameState = GAME_STATE_PLAY_GAME;
                    break;                case 17: // Save Game
                    SaveGameStory("saves/story/progress_slot_1.dat", storyCurrentScene, storyCurrentFrame);
                    showSaveMessage = true;
                    saveMessageTimer = 0.0f;
                    break;
                case 18: // Main Menu
                    printf("Transitioning to main menu...\n");
                    // Re-initialize the BGMusic before going back to main menu
                    BGMusic = LoadMusicStream("Assets/Music/SilentNight.mp3");
                    SetMusicVolume(BGMusic, 0.5f);
                    PlayMusicStream(BGMusic);
                    BGMusic.looping = true;
                    
                    // Reset game state
                    storyCurrentFrame = 0;
                    storyCurrentScene = 0;
                    printf("Resetting storyCurrentScene and storyCurrentFrame to 0\n");
                    *currentGameState = GAME_STATE_MAIN_MENU;
                    printf("Main menu initialized\n");
                    break;
            }
            break;

        case GAME_STATE_PLAY_CUSTOM_MENU: // Handle slot selection for playing custom stories
            switch (index) {
                case 12: // Slot 1
                    // Load slot 1 if it exists
                    if (customStorySlots[0] == NULL) {
                        customStorySlots[0] = LoadSlotFromFile(1);
                    }
                    if (customStorySlots[0] != NULL) {
                        // Reset to beginning of story
                        currentCustomSlot = 0;
                        customCurrentNode = customStorySlots[0]->ID;
                        customCurrentScene = 0;
                        *currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                    }
                    break;
                case 13: // Slot 2
                    // Load slot 2 if it exists
                    if (customStorySlots[1] == NULL) {
                        customStorySlots[1] = LoadSlotFromFile(2);
                    }
                    if (customStorySlots[1] != NULL) {
                        // Reset to beginning of story
                        currentCustomSlot = 1;
                        customCurrentNode = customStorySlots[1]->ID;
                        customCurrentScene = 0;
                        *currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                    }
                    break;
                case 14: // Slot 3
                    // Load slot 3 if it exists
                    if (customStorySlots[2] == NULL) {
                        customStorySlots[2] = LoadSlotFromFile(3);
                    }
                    if (customStorySlots[2] != NULL) {
                        // Reset to beginning of story
                        currentCustomSlot = 2;
                        customCurrentNode = customStorySlots[2]->ID;
                        customCurrentScene = 0;
                        *currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                    }
                    break;
                case 15: *currentGameState = GAME_STATE_NEW_CONTINUE_CUSTOM; break; // Back to "New Game" / "Continue" screen
            }
            break;
        default:
            break;
    }
    if (previousState != *currentGameState) {
        InitButtonRects(*currentGameState);
        selectedMenu = -1;
    }

    return false;
}

// IS: Pause menu belum diperbarui
// FS: Pause menu berhasil diperbarui sesuai input user
void UpdatePauseMenu(GameState *currentGameState) {
    Vector2 mousePoint = GetMousePosition();
    selectedMenu = -1;    // Check if mouse is over any button
    for (int i = 16; i <= 18; i++) {
        if (CheckCollisionPointRec(mousePoint, buttonRects[i])) {
            selectedMenu = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Clicked pause menu button %d\n", i);
                CheckMenuClick(i, currentGameState);
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
    UnloadMusicStream(BGMusic);
    for (int i = 0; i < MAX_MENU; i++) {
        UnloadTexture(MenuButtons[i]);
    }
}

// IS: Grid debug belum digambar di layar
// FS: Grid debug berhasil digambar di layar
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

// IS: Pause menu belum digambar di layar
// FS: Pause menu berhasil digambar di layar
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