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

int main() {
    
    InitWindow(1920, 1080, "The Little Match Girls");
    
    SetTargetFPS(60);

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

        EndDrawing();
    }
    UnloadTexture(LoadTexture("img/background.png"));
    CloseWindow();
    UnloadAssetsSimple(assets);
    return 0;
}
