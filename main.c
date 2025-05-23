#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "story.h"
// #include "customstory.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid

#define GAME_STATE_MAIN_MENU 0
#define GAME_STATE_PLAY_GAME 1          
#define GAME_STATE_NEW_CONTINUE_MENU 2  
#define GAME_STATE_CUSTOM_GAME_MENU 3   
#define GAME_STATE_ABOUT 5              
#define GAME_STATE_STORY 6 


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid Coordinate Debug");
    SetTargetFPS(60);
    InitAssets();
    InitDataCerita();
    InitAudioDevice();
    InitButtonRects();

    Music Pusic = LoadMusicStream("Assets/music/Jingle Bells.mp3");
    SetMusicVolume(Pusic, 1.0f);
    
    PlayMusicStream(Pusic); 
   

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        BeginDrawing();
        
        if (GameState != GAME_STATE_STORY)
        {
            /* code */
            UpdateMusicStream(Pusic);
            UpdateMainMenu();
            DrawMainMenu();
            DrawDebugGrid(GRID_SIZE);  
        }else{
            UpdateCerita(&GameState);
            DrawCurrentNodeScreen();
        }
        
        
        ClearBackground(WHITE);
        

        
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 100, 100, 50, BLACK);
        EndDrawing();
    }

    UnloadNodeAssets(currentScene);
    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();

    // UnloadAssetsSimple(assets);
    return 0;
}
