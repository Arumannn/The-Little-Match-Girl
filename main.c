#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <unistd.h>
#include "story.h"
#include "customstory.h"
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid Coordinate Debug");
    
    SetTargetFPS(60);

    AssetLibraryArr assets;
    LoadAssetsSimple(assets);

    VNTreeNode root = {0};
    VNTreeNode *currentNode = &root;
    root.id = 1;

    bool showStoryCreator = true;
    MenuBackground = LoadTexture("Assets/BackSprites/customstorymenu.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(MenuBackground, (Vector2){0, 0}, 0.0f,
              fminf((float)GetScreenWidth() / MenuBackground.width,
                    (float)GetScreenHeight() / MenuBackground.height),
              WHITE);

        if (showStoryCreator)
        {
            StoryCreator(&currentNode, assets, 2);
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

    CloseWindow();
    UnloadAssetsSimple(assets);
    PrintTree(&root);
    sleep(100);
    return 0;
}
