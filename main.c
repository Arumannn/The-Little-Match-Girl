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


int customCurrentNode = 0;
int customCurrentScene = 0;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Little Match Girl");
    SetTargetFPS(60);
    InitAssetsMenu();
    InitDataCerita(Mytree);
    InitAudioDevice();
    InitButtonRects(currentGameState);
    InitiateAssets(); // Initialize custom story assets

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

            case GAME_STATE_PLAY_CUSTOM_STORY:
                // Playback custom story sesuai progress
                if (customStorySlots[currentCustomSlot] != NULL) {
                    currentGameState = UpdateCustomStory(customStorySlots[currentCustomSlot], &customCurrentNode, &customCurrentScene);
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
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_1:
                currentCustomSlot = 0;
                if (customStorySlots[0] != NULL) {
                    char filename[64];
                    sprintf(filename, "saves/custom_save_1.dat");
                    LoadCustomStoryProgress(filename, &customCurrentNode, &customCurrentScene);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_2:
                currentCustomSlot = 1;
                if (customStorySlots[1] != NULL) {
                    char filename[64];
                    sprintf(filename, "saves/custom_save_2.dat");
                    LoadCustomStoryProgress(filename, &customCurrentNode, &customCurrentScene);
                    currentGameState = GAME_STATE_PLAY_CUSTOM_STORY;
                } else {
                    currentGameState = GAME_STATE_MAIN_MENU;
                }
                InitButtonRects(currentGameState);
                break;

            case GAME_STATE_PLAY_CUSTOM_SLOT_3:
                currentCustomSlot = 2;
                if (customStorySlots[2] != NULL) {
                    char filename[64];
                    sprintf(filename, "saves/custom_save_3.dat");
                    LoadCustomStoryProgress(filename, &customCurrentNode, &customCurrentScene);
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
                    LoadNodeAssets(Mytree, currentScene); 

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
    }

    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (customStorySlots[i] != NULL) {
            DeleteTreeNode(&customStorySlots[i]);
        }
    }

    UnloadNodeAssets(Mytree, currentScene);
    UnloadMusicStream(Pusic);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    
    return 0;
}