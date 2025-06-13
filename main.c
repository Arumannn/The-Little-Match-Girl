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
                printf("Playing custom story from slot %d\n", currentCustomSlot + 1);
                // Playback custom story sesuai progress
                if (customStorySlots[currentCustomSlot] != NULL) {
                    // Update and draw the custom story
                    GameState nextState = UpdateCustomStory(customStorySlots[currentCustomSlot], &customCurrentNode, &customCurrentScene);
                    DrawCustomStoryScreen(customStorySlots[currentCustomSlot], customCurrentNode, customCurrentScene);
                    
                    // Handle state changes
                    if (nextState != GAME_STATE_PLAY_CUSTOM_STORY) {
                        // Save progress before changing state
                        char savefile[64];
                        sprintf(savefile, "saves/story_slot_%d.sav", currentCustomSlot + 1);
                        SaveCustomStoryProgress(savefile, customCurrentNode, customCurrentScene);
                        currentGameState = nextState;
                    }
                } else {
                    printf("Error: No custom story loaded in slot %d\n", currentCustomSlot + 1);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_MENU;
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
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_SLOT_2:
                if (customStorySlots[1] != NULL) {
                    DeleteTreeNode(&customStorySlots[1]);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_DELETE_SLOT_3:
                if (customStorySlots[2] != NULL) {
                    DeleteTreeNode(&customStorySlots[2]);
                }
                currentGameState = GAME_STATE_MAIN_MENU;
                InitButtonRects(currentGameState);
                break;            case GAME_STATE_PLAY_CUSTOM_SLOT_1:
                currentCustomSlot = 0;
                if (CheckSaveFileExists(1)) {
                    char filename[64];
                    sprintf(filename, "saves/story_slot_1.sav");
                    LoadCustomStoryFromFile(filename, &customStorySlots[0]);
                    customCurrentNode = 0;  // Start from beginning
                    customCurrentScene = 0;
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_PLAY_CUSTOM_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_2:
                currentCustomSlot = 1;
                if (CheckSaveFileExists(2)) {
                    char filename[64];
                    sprintf(filename, "saves/story_slot_2.sav");
                    LoadCustomStoryFromFile(filename, &customStorySlots[1]);
                    customCurrentNode = 0;  // Start from beginning
                    customCurrentScene = 0;
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_PLAY_CUSTOM_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_3:
                currentCustomSlot = 2;
                if (CheckSaveFileExists(3)) {
                    char filename[64];
                    sprintf(filename, "saves/story_slot_3.sav");
                    LoadCustomStoryFromFile(filename, &customStorySlots[2]);
                    customCurrentNode = 0;  // Start from beginning
                    customCurrentScene = 0;
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_PLAY_CUSTOM_MENU;
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
                DrawMiniGameStack();                if (currentGameState != GAME_STATE_MINI_GAME_STACK) {
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