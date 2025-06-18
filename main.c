#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "customstory.h"
#include "minigame.h"
#include "story.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid

TreeStory Mytree[MAX_NODE_TREE];
GameState currentGameState = GAME_STATE_MAIN_MENU;
GameState previousGameState = GAME_STATE_MAIN_MENU; 
bool minigameInitialized = false; 
bool exitProgram = false;
// Custom story slots
CustomSceneTree customStorySlots[3] = {NULL, NULL, NULL};
int currentCustomSlot = 0;

// Story state variables 
int storyCurrentNode = 0;
int storyCurrentScene = 0;

int customCurrentNode = 0;
int customCurrentScene = 0;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Little Match Girl");
    InitAudioDevice();
    SetTargetFPS(60);
    InitAssetsMenu();
    InitDataCerita(Mytree);
    InitButtonRects(currentGameState);
    InitiateAssets();
    CreateSaveDirectories();
    LoadCustomStorySlots(customStorySlots);

    
    
    while (!WindowShouldClose() && !exitProgram) {
         
        
        BeginDrawing();
        
        switch (currentGameState) {
            case GAME_STATE_MAIN_MENU:
            case GAME_STATE_PLAY_GAME_MENU:
            case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            case GAME_STATE_NEW_CONTINUE_CUSTOM:
            case GAME_STATE_STUDIO_MENU:
            case GAME_STATE_ABOUT:
            case GAME_STATE_CREATE_MENU:
            case GAME_STATE_EDIT_MENU:
            case GAME_STATE_DELETE_MENU:
            case GAME_STATE_PLAY_CUSTOM_MENU:
                printf("Current Game State: %d\n", currentGameState);
                ClearBackground(WHITE); 
                printf("Updating menu...\n");
                UpdateMainMenu(&currentGameState);
                printf("Drawing menu...\n"); 
                DrawMainMenu(currentGameState);
                printf("Menu drawn.\n");
                //DrawDebugGrid(GRID_SIZE);
                break;
                
            case GAME_STATE_PLAY_GAME:
                previousGameState = currentGameState;
                UpdateCerita(Mytree, &currentGameState); 
                DrawCurrentNodeScreen(Mytree);
                break;

            case GAME_STATE_PLAY_CUSTOM:
                previousGameState = currentGameState;
                if (customStorySlots[currentCustomSlot] != NULL) {
                    currentGameState = UpdateCustomStory(customStorySlots[currentCustomSlot], &customCurrentNode, &customCurrentScene, currentCustomSlot + 1);
                    DrawCustomStoryScreen(customStorySlots[currentCustomSlot], customCurrentNode, customCurrentScene);
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                break;

            case GAME_STATE_CREATE_CUSTOM:
                MakeCustomStory(&customStorySlots[currentCustomSlot], currentCustomSlot + 1);
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_EDIT_CUSTOM:
                if (customStorySlots[currentCustomSlot] != NULL) {
                    MakeCustomStory(&customStorySlots[currentCustomSlot], currentCustomSlot + 1);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_CUSTOM:
                if (customStorySlots[currentCustomSlot] != NULL) {
                    DeleteTreeNode(&customStorySlots[currentCustomSlot]);
                }
                DeleteSlotFiles(currentCustomSlot + 1);
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
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
                    storyCurrentNode = 0;  
                    storyCurrentScene = 0;  
                    LoadNodeAssets(Mytree, storyCurrentNode);  
                }
                break;

            default:
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;
        }
        
        // Vector2 mouse = GetMousePosition();
        // DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 10, 10, 20, WHITE);
        // DrawText(TextFormat("Current State: %d", currentGameState), 10, 40, 20, BLACK);
        
        EndDrawing();
    }  
    CleanupCustomStorySlots(customStorySlots);

    UnloadNodeAssets(Mytree, storyCurrentNode);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    
    return 0;
}