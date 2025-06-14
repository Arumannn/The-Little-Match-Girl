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
GameState previousGameState = GAME_STATE_MAIN_MENU; // Track previous state for pause menu
bool minigameInitialized = false; 
bool exitProgram = false;
// Custom story slots
CustomSceneTree customStorySlots[3] = {NULL, NULL, NULL};
int currentCustomSlot = 0;

// Story state variables 
int storyCurrentScene = 0;
int storyCurrentFrame = 0;

int customCurrentNode = 0;
int customCurrentScene = 0;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Little Match Girl");
    InitAudioDevice();
    SetTargetFPS(60);
    InitAssetsMenu();
    InitDataCerita(Mytree);
    InitButtonRects(currentGameState);
    InitiateAssets(); // Initialize custom story assets
    CreateSaveDirectories(); // Create save directories

    // Load existing custom story slots from files
    for (int i = 0; i < 3; i++) {
        customStorySlots[i] = LoadSlotFromFile(i + 1);
        if (customStorySlots[i] != NULL) {
            printf("Loaded custom story slot %d\n", i + 1);
        }
    }

    
    
    while (!WindowShouldClose() && !exitProgram) {
         
        
        BeginDrawing();
        
        switch (currentGameState) {
            case GAME_STATE_MAIN_MENU:
            case GAME_STATE_PLAY_GAME_MENU:
            case GAME_STATE_NEW_CONTINUE_NON_CUSTOM:
            case GAME_STATE_NEW_CONTINUE_CUSTOM:
            case GAME_STATE_PLAY_CUSTOM_MENU:
            case GAME_STATE_STUDIO_MENU:
            case GAME_STATE_CREATE_MENU:
            case GAME_STATE_EDIT_MENU:
            case GAME_STATE_DELETE_MENU:
            case GAME_STATE_ABOUT:
                printf("Current Game State: %d\n", currentGameState);
                ClearBackground(WHITE); 
                printf("Updating main menu...\n");
                UpdateMainMenu(&currentGameState);
                printf("Drawing main menu...\n"); 
                DrawMainMenu(currentGameState);
                printf("Main menu drawn.\n");
                DrawDebugGrid(GRID_SIZE);
                break;
                
            case GAME_STATE_PLAY_GAME:
                UpdateCerita(Mytree, &currentGameState); 
                DrawCurrentNodeScreen(Mytree);
                break;

            case GAME_STATE_PLAY_CUSTOM_STORY:
                if (customStorySlots[currentCustomSlot] != NULL) {
                    GameState nextState = UpdateCustomStory(customStorySlots[currentCustomSlot], &customCurrentNode, &customCurrentScene, currentCustomSlot + 1);
                    if (nextState == GAME_STATE_PAUSE) {
                        currentGameState = GAME_STATE_PAUSE;
                        break;
                    }
                    currentGameState = nextState;
                    DrawCustomStoryScreen(customStorySlots[currentCustomSlot], customCurrentNode, customCurrentScene);
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                break;

            case GAME_STATE_CREATE_SLOT_1:
                currentCustomSlot = 0;
                MakeCustomStory(&customStorySlots[0], 1);
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_CREATE_SLOT_2:
                currentCustomSlot = 1;
                MakeCustomStory(&customStorySlots[1], 2);
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_CREATE_SLOT_3:
                currentCustomSlot = 2;
                MakeCustomStory(&customStorySlots[2], 3);
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_EDIT_SLOT_1:
                currentCustomSlot = 0;
                if (customStorySlots[0] != NULL) {
                    MakeCustomStory(&customStorySlots[0], 1);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_EDIT_SLOT_2:
                currentCustomSlot = 1;
                if (customStorySlots[1] != NULL) {
                    MakeCustomStory(&customStorySlots[1], 2);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_EDIT_SLOT_3:
                currentCustomSlot = 2;
                if (customStorySlots[2] != NULL) {
                    MakeCustomStory(&customStorySlots[2], 3);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_SLOT_1:
                if (customStorySlots[0] != NULL) {
                    DeleteTreeNode(&customStorySlots[0]);
                }
                DeleteSlotFiles(1);  // Delete files from disk
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_SLOT_2:
                if (customStorySlots[1] != NULL) {
                    DeleteTreeNode(&customStorySlots[1]);
                }
                DeleteSlotFiles(2);  // Delete files from disk
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_SLOT_3:
                if (customStorySlots[2] != NULL) {
                    DeleteTreeNode(&customStorySlots[2]);
                }
                DeleteSlotFiles(3);  // Delete files from disk
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_1:
                currentCustomSlot = 0;
                if (customStorySlots[0] != NULL) {
                    LoadCustomStoryProgressBySlot(1, &customCurrentNode, &customCurrentScene);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_2:
                currentCustomSlot = 1;
                if (customStorySlots[1] != NULL) {
                    LoadCustomStoryProgressBySlot(2, &customCurrentNode, &customCurrentScene);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_3:
                currentCustomSlot = 2;
                if (customStorySlots[2] != NULL) {
                    LoadCustomStoryProgressBySlot(3, &customCurrentNode, &customCurrentScene);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
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
                    storyCurrentScene = 0;  // Reset to first scene
                    storyCurrentFrame = 0;  // Reset to first frame
                    LoadNodeAssets(Mytree, storyCurrentScene);  // Load assets for the first scene
                }
                break;

            default:
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;
        }
        
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 10, 10, 20, WHITE);
        DrawText(TextFormat("Current State: %d", currentGameState), 10, 40, 20, BLACK);
        
        EndDrawing();
    }    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (customStorySlots[i] != NULL) {
            DeleteTreeNode(&customStorySlots[i]);
        }
    }

    UnloadNodeAssets(Mytree, storyCurrentScene);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    
    return 0;
}