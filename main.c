#include <raylib.h>

int main() {
    
    InitWindow(1920, 1080, "The Little Match Girls");
    
    SetTargetFPS(60);
    ToggleFullscreen();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(LoadTexture("img/background.png"), 0, 0, WHITE);
        DrawText("Welcome", 860, 540, 20, DARKGRAY);
        EndDrawing();
    }
    UnloadTexture(LoadTexture("img/background.png"));
    CloseWindow();
    return 0;
}
