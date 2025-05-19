#include <raylib.h>
#include "mainmenu.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define GRID_SIZE 50  // Jarak antar garis grid


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid Coordinate Debug");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawDebugGrid(GRID_SIZE);  // Panggil fungsi grid

        // Tampilkan posisi mouse
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: [%.0f, %.0f]", mouse.x, mouse.y), 10, 10, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
