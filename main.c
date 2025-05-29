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


Tree Mytree[MAX_NODE_TREE];
GameState currentGameState = GAME_STATE_MAIN_MENU;
// VNTreeNode Slot_1, *Slot_2, *Slot_3;
// AssetLibraryArr Assets;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid Coordinate Debug");
    SetTargetFPS(60);
    InitAssets();
    InitDataCerita(Mytree);
    InitAudioDevice();
    InitButtonRects(currentGameState);

    Music Pusic = LoadMusicStream("Assets/Music/SilentNight.mp3");
    PlayMusicStream(Pusic); 
    SetMusicVolume(Pusic, 1.0f);
    
    
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        BeginDrawing();
        
        if (currentGameState == GAME_STATE_MAIN_MENU ||
            currentGameState == GAME_STATE_PLAY_GAME ||
            currentGameState == GAME_STATE_NEW_CONTINUE_MENU ||
            currentGameState == GAME_STATE_CUSTOM_GAME_MENU ||
            currentGameState == GAME_STATE_CREATE ||
            currentGameState == GAME_STATE_EDIT ||
            currentGameState == GAME_STATE_DELETE ||
            currentGameState == GAME_STATE_CONTINUE_SLOT_1 ||
            currentGameState == GAME_STATE_CONTINUE_SLOT_2 ||
            currentGameState == GAME_STATE_CONTINUE_SLOT_3 ||
            currentGameState == GAME_STATE_ABOUT) {
            UpdateMusicStream(Pusic);
            UpdateMainMenu(&currentGameState); 
            DrawMainMenu(currentGameState);
            DrawDebugGrid(GRID_SIZE); 

        } else if (currentGameState == GAME_STATE_STORY) {
            UpdateCerita(Mytree, &currentGameState); 
            DrawCurrentNodeScreen(Mytree);

        }else if(currentGameState == GAME_STATE_MINI_GAME_STACK){
            InitMiniGameStack();
            currentGameState = GAME_STATE_MINI_GAME_STACK;
        } if(GAME_STATE_CREATE_SLOT_1){
            //StoryCreator(*Slot_1, Assets);
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_CREATE_SLOT_2){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_CREATE_SLOT_3){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if(GAME_STATE_EDIT_SLOT_1){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_EDIT_SLOT_2){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_EDIT_SLOT_3){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if(GAME_STATE_DELETE_SLOT_1){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_DELETE_SLOT_2){
            currentGameState = GAME_STATE_MAIN_MENU;
        }else if (GAME_STATE_DELETE_SLOT_3){
            currentGameState = GAME_STATE_MAIN_MENU;
        } else {
            currentGameState = GAME_STATE_MAIN_MENU;
            InitButtonRects(currentGameState); 
        }
        
        
        ClearBackground(WHITE);
        //currentFrame= 0;
        

        
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 100, 100, 50, BLACK);
        EndDrawing();
    }

    UnloadNodeAssets(Mytree, currentScene);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    //PrintTree(&root);

    // UnloadAssetsSimple(assets);
    return 0;
}
