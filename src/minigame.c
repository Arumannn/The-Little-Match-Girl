#include "minigame.h"

Stack MemoryStack;
Stack PlayerChoiceStack;
Texture2D memoryImages[MAX_STACK];
float memoryDisplayTimer = 0;
float displayDurationPerMemory = 1.5f; 
float choiceDisplayScale = 0.2f;
int currentMemoryIndex = 0;
bool showingMemories = false;
bool choosingMemories = false;
bool minigameSuccess = false;
bool minigameActive = false; 
int currentChoiceCount = 0; 

void InitMiniGameStack(){
    char filepath[256];
    for (int i = 0; i < MAX_STACK; i++)
    {
        sprintf(filepath, "Assets/Memories/Memory_%d.png", i);
        memoryImages[i] = LoadTexture(filepath);
    }
    CreateEmptyStack(&MemoryStack);
    CreateEmptyStack(&PlayerChoiceStack);

    showingMemories = true;
    choosingMemories = false;
    minigameSuccess = false;
    minigameActive = true;
    currentMemoryIndex = 0;
    currentChoiceCount = 0;
}


void UpdateMiniGameStack(GameState *currentGameState) {
    if (!minigameActive) return;

    if (showingMemories) {
        memoryDisplayTimer += GetFrameTime();
        if (memoryDisplayTimer >= displayDurationPerMemory && currentMemoryIndex < MAX_STACK) {
            Data data = { memoryImages[currentMemoryIndex], (Rectangle){0} };
            Push(&MemoryStack, data);
            currentMemoryIndex++;
            memoryDisplayTimer = 0;
        }

        if (currentMemoryIndex == MAX_STACK) {
            showingMemories = false;
            choosingMemories = true;
            currentChoiceCount = 0;
        }
    } 
    else if (choosingMemories) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < MAX_STACK; i++) {
                Rectangle r = (Rectangle){100 + i * 120, 500, 100, 100};
                if (CheckCollisionPointRec(mouse, r)) {
                    Data chosen = { memoryImages[i], r };
                    Push(&PlayerChoiceStack, chosen);
                    currentChoiceCount++;
                    break;
                }
            }

            if (currentChoiceCount == MAX_STACK) {
                choosingMemories = false;
                if (CompareStack(MemoryStack, PlayerChoiceStack)) {
                    minigameSuccess = true;
                    minigameActive = false;
                    *currentGameState = GAME_STATE_STORY;
                } else {
                    CreateEmptyStack(&PlayerChoiceStack);
                    currentChoiceCount = 0;
                }
            }
        }
    }
}

void DrawMiniGameStack() {
    if (showingMemories) {
        for (int i = 0; i < StackSize(MemoryStack); i++) {
            DrawTextureEx(MemoryStack.Memory[i].Image, (Vector2){100 + i * 120, 200}, 0, 0.3f, WHITE);
        }
    } else if (choosingMemories) {
        for (int i = 0; i < MAX_STACK; i++) {
            DrawTexturePro(memoryImages[i],
                (Rectangle){0, 0, -memoryImages[i].width, memoryImages[i].height},
                (Rectangle){100 + i * 120, 500, 100, 100},
                (Vector2){0, 0}, 0.0f, WHITE);
        }

        // Stack pilihan player
        for (int i = 0; i < StackSize(PlayerChoiceStack); i++) {
            DrawTextureEx(PlayerChoiceStack.Memory[i].Image,
                (Vector2){100 + i * 120, 650}, 0, 0.3f, WHITE);
        }
    }

    if (minigameSuccess) {
        DrawText("Berhasil! Kamu mengingat semuanya!", 500, 100, 30, GREEN);
    }
}

bool CompareStack(Stack S1, Stack S2){
    if (StackSize(S1) == StackSize(S2)) {
        for (int i = 0; i < StackSize(S1); i++) {
            if (S1.Memory[i].Image.id != S2.Memory[i].Image.id) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void UnloadMiniGameStackAssets(Stack *S){
    for (int i = 0; i < StackSize(*S); i++) {
        UnloadTexture(S->Memory[i].Image);
        S->Memory[i].Image = (Texture2D){0};
    }
    CreateEmptyStack(S);
}