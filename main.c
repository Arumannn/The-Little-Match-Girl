#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
<<<<<<< Updated upstream
#include "include/headers.h"
=======
#include "story.h"
#include "customstory.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid
>>>>>>> Stashed changes

int main() {
    
    InitWindow(1920, 1080, "The Little Match Girls");
    
    SetTargetFPS(60);
<<<<<<< Updated upstream
    ToggleFullscreen();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(LoadTexture("Assets/BackSprites/diner-scene.png"), 0, 0, WHITE);
        DrawText("Welcome", 860, 540, 20, DARKGRAY);
=======

    AssetLibraryArr assets;
    LoadAssetsSimple(assets);

    VNTreeNode root = {0};
    root.id = 1;

    bool showStoryCreator = true;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (showStoryCreator) 
        {
            StoryCreator(&root, assets, 2);
        } 
        else
        {
            DrawDebugGrid(GRID_SIZE);  // Panggil fungsi grid

            // Tampilkan posisi mouse
            Vector2 mouse = GetMousePosition();
            DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 10, 10, 20, YELLOW);
        }

>>>>>>> Stashed changes
        EndDrawing();
    }
    UnloadTexture(LoadTexture("img/background.png"));
    CloseWindow();
<<<<<<< Updated upstream
    return 0; 
=======
    UnloadAssetsSimple(assets);
    return 0;
>>>>>>> Stashed changes
}
