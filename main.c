#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
//#include "customstory.h"
#include "minigame.h"
#include "story.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid

TreeStory Mytree[MAX_NODE_TREE];
GameState currentGameState = GAME_STATE_MAIN_MENU; 
bool minigameInitialized = false; 
bool exitProgram = false;
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Little Match Girl");
    SetTargetFPS(60);
    InitAssets();
    InitDataCerita(Mytree);
    InitAudioDevice();
    InitButtonRects(currentGameState);

    Music Pusic = LoadMusicStream("Assets/Music/SilentNight.mp3");
    PlayMusicStream(Pusic); 
    SetMusicVolume(Pusic, 1.0f);
    
    while (!WindowShouldClose() && !exitProgram) {
        
        UpdateMusicStream(Pusic); 
        
        BeginDrawing();
        
        switch (currentGameState) {
            case GAME_STATE_MAIN_MENU:
            case GAME_STATE_PLAY_GAME_MENU:
            case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            case GAME_STATE_NEW_CONTINUE_CUSTOM:
            case GAME_STATE_STUDIO_MENU:
            case GAME_STATE_CREATE_MENU:
            case GAME_STATE_EDIT_MENU:
            case GAME_STATE_DELETE_MENU:
            // case GAME_STATE_CONTINUE_SLOT_1:
            // case GAME_STATE_CONTINUE_SLOT_2:
            // case GAME_STATE_CONTINUE_SLOT_3:
            case GAME_STATE_ABOUT:
                ClearBackground(WHITE); 
                UpdateMainMenu(&currentGameState); 
                DrawMainMenu(currentGameState);
                DrawDebugGrid(GRID_SIZE);
                break;
                
            case GAME_STATE_PLAY_GAME:
                UpdateCerita(Mytree, &currentGameState); 
                DrawCurrentNodeScreen(Mytree);
                break;
            case GAME_STATE_PAUSE:
                UpdatePauseMenu(&currentGameState);
                DrawPauseMenu();
                break;
            case GAME_STATE_MINI_GAME_STACK:
                if (!minigameInitialized) {
                    InitMiniGameStack();
                    minigameInitialized = true;
                    printf("Minigame initialized!\n");
                }

                UpdateMiniGameStack(&currentGameState);
                DrawMiniGameStack();

                if (currentGameState != GAME_STATE_MINI_GAME_STACK) {
                    minigameInitialized = false;
                    printf("Exiting minigame, transitioning to story\n");
                    LoadNodeAssets(Mytree, currentScene); 
                    
                }
                break;   
            case GAME_STATE_CREATE_SLOT_1:
            case GAME_STATE_CREATE_SLOT_2:
            case GAME_STATE_CREATE_SLOT_3:
            case GAME_STATE_EDIT_SLOT_1:
            case GAME_STATE_EDIT_SLOT_2:
            case GAME_STATE_EDIT_SLOT_3:
            case GAME_STATE_DELETE_SLOT_1:
            case GAME_STATE_DELETE_SLOT_2:
            case GAME_STATE_DELETE_SLOT_3:
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;
                
            default:
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;
        }
        
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 10, 10, 20, WHITE     );
        DrawText(TextFormat("Current State: %d", currentGameState), 10, 40, 20, BLACK);
        
        EndDrawing();
    }

    UnloadNodeAssets(Mytree, currentScene);
    UnloadMusicStream(Pusic);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    
    return 0;
}