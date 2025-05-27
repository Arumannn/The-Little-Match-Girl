#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "story.h"
#include "customstory.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid Coordinate Debug");
    SetTargetFPS(60);
    InitAssets();
    InitAudioDevice();
    InitButtonRects();
    Music Pusic = LoadMusicStream("Assets/music/Jingle Bells.mp3");
    SetMusicVolume(Pusic, 1.0f);

    assetCount = 2;
    
    PlayMusicStream(Pusic); 
    AssetLibraryArr assets;
    LoadAssetsSimple(assets);

    VNTreeNode root = {0};
    VNTreeNode *currentNode = &root;
    root.id = 1;
   

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) break;
        
        UpdateMainMenu(&currentNode, assets);
        UpdateMusicStream(Pusic);

        BeginDrawing();
        ClearBackground(WHITE);
        DrawMainMenu();
        DrawDebugGrid(GRID_SIZE);  
        

        
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 100, 100, 50, BLACK);
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
    UnloadAssets();
    PrintTree(&root);

    // UnloadAssetsSimple(assets);
    return 0;
}
