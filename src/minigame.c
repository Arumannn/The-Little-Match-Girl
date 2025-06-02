// minigame.c - Modified for Random Grid Display
#include "minigame.h"

Stack MemoryStack;
Stack PlayerChoiceStack;
Texture2D memoryImages[MAX_STACK];
float memoryDisplayTimer = 0;
float displayDurationPerMemory = 1.5f; 
int currentMemoryIndex = 0;
bool showingMemories = false;
bool choosingMemories = false;
bool minigameSuccess = false;
bool minigameActive = false; 
int currentChoiceCount = 0;
int memorySequence[MAX_STACK];       // Urutan memori yang ditampilkan besar (tetap berurutan)
int gridDisplayOrder[MAX_STACK];     // Urutan tampilan di grid pilihan (yang akan diacak)
bool correctChoices[MAX_STACK];      // Array untuk menandai pilihan yang benar

void InitMiniGameStack(){
    char filepath[256];
    
    // Load textures
    for (int i = 0; i < MAX_STACK; i++) {
        sprintf(filepath, "Assets/Memories/Memory_%d.png", i);
        memoryImages[i] = LoadTexture(filepath);
        
        if (memoryImages[i].id == 0) {
            printf("Gambar masih kosong %s\n", filepath);
        }
    }
    
    CreateEmptyStack(&MemoryStack);
    CreateEmptyStack(&PlayerChoiceStack);

    // Inisialisasi urutan memori yang ditampilkan (tetap berurutan)
    for (int i = 0; i < MAX_STACK; i++) {
        memorySequence[i] = i; 
    }
    
    // Inisialisasi dan acak urutan tampilan grid
    for (int i = 0; i < MAX_STACK; i++) {
        gridDisplayOrder[i] = i;
    }
    
    // Mengacak array gridDisplayOrder menggunakan Fisher-Yates shuffle
    for (int i = MAX_STACK - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        int temp = gridDisplayOrder[i];
        gridDisplayOrder[i] = gridDisplayOrder[j];
        gridDisplayOrder[j] = temp;
    }

    showingMemories = true;
    choosingMemories = false;
    minigameSuccess = false;
    minigameActive = true;
    currentMemoryIndex = 0;
    currentChoiceCount = 0;
    memoryDisplayTimer = 0;
    
    // Reset array correctChoices
    for (int i = 0; i < MAX_STACK; i++) {
        correctChoices[i] = false;
    }
    
    printf("Urutan memori yang ditampilkan: ");
    for (int i = 0; i < MAX_STACK; i++) {
        printf("%d ", memorySequence[i]);
    }
    printf("\n");
    
    printf("Urutan tampilan grid: ");
    for (int i = 0; i < MAX_STACK; i++) {
        printf("%d ", gridDisplayOrder[i]);
    }
    printf("\n");
}

void UpdateMiniGameStack(GameState *currentGameState) {
    if (!minigameActive) return;

    if (showingMemories) {
        memoryDisplayTimer += GetFrameTime();

        // Delay 5 detik sebelum mulai menampilkan memory
        if (memoryDisplayTimer < 5.0f) {
            return; // Belum saatnya menampilkan memory
        }

        // Hitung waktu relatif setelah delay 5 detik
        float timeAfterStart = memoryDisplayTimer - 5.0f;
        int expectedMemoryIndex = (int)(timeAfterStart / displayDurationPerMemory);
        
        // Jika sudah saatnya untuk memory berikutnya dan masih ada memory yang belum ditampilkan
        if (expectedMemoryIndex >= currentMemoryIndex && currentMemoryIndex < MAX_STACK) {
            Data memoryData = { 
                memoryImages[memorySequence[currentMemoryIndex]], 
                (Rectangle){0, 0, memoryImages[memorySequence[currentMemoryIndex]].width, 
                           memoryImages[memorySequence[currentMemoryIndex]].height}
            };
            Push(&MemoryStack, memoryData);
            
            printf("Memory %d (Gambar : %d)\n", currentMemoryIndex, memorySequence[currentMemoryIndex]);
            
            currentMemoryIndex++;
        }

        // Jika semua memory sudah ditampilkan dan waktu cukup untuk memory terakhir
        if (currentMemoryIndex >= MAX_STACK && timeAfterStart >= MAX_STACK * displayDurationPerMemory) {
            showingMemories = false;
            choosingMemories = true;
            currentChoiceCount = 0;
            printf("Pemilihan memori\n");
        }
    } 
    else if (choosingMemories) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            for (int i = 0; i < MAX_STACK; i++) {
                Rectangle choiceRect;
                
                // Posisi grid 3x2 (3 kolom, 2 baris) dengan center alignment
                int col = i % 3;
                int row = i / 3;
                int gridStartX = SCREEN_WIDTH / 2 - (3 * 640) / 2; // Center grid horizontal untuk ukuran 640
                int gridStartY = SCREEN_HEIGHT / 2 - (2 * 360) / 2; // Center grid vertical untuk ukuran 360
                
                choiceRect.x = gridStartX + col * 640; 
                choiceRect.y = gridStartY + row * 360; 
                choiceRect.width = 640;
                choiceRect.height = 360;

                if (CheckCollisionPointRec(mouse, choiceRect)) {
                    // Mendapatkan index memori yang sebenarnya dari grid yang diacak
                    int actualMemoryIndex = gridDisplayOrder[i];
                    
                    int expectedMemoryIndex = StackSize(MemoryStack) - 1 - currentChoiceCount;
                    
                    if (expectedMemoryIndex >= 0 && expectedMemoryIndex < StackSize(MemoryStack)) {
                        Data expectedMemory = MemoryStack.Memory[expectedMemoryIndex];
                        
                        // Membandingkan dengan memori yang sebenarnya
                        if (memoryImages[actualMemoryIndex].id == expectedMemory.Image.id) {
                            Data chosenData = { memoryImages[actualMemoryIndex], choiceRect };
                            Push(&PlayerChoiceStack, chosenData);
                            correctChoices[i] = true; // Menandai pilihan ini sebagai benar
                            currentChoiceCount++;
                            if (currentChoiceCount >= MAX_STACK) {
                                choosingMemories = false;
                                minigameSuccess = true;
                                printf("Minigame completed successfully!\n");
                            }
                        } else {
                            
                            CreateEmptyStack(&PlayerChoiceStack);
                            currentChoiceCount = 0;
                            // Reset semua correctChoices
                            for (int j = 0; j < MAX_STACK; j++) {
                                correctChoices[j] = false;
                            }
                        }
                    }
                    break; 
                }
            }
        }
    }
    
    // Handle success state
    if (minigameSuccess && IsKeyPressed(KEY_SPACE)) {
        minigameActive = false;
        *currentGameState = GAME_STATE_STORY;
    }
}

void DrawMiniGameStack() {
    ClearBackground(BLACK);
    
    if (showingMemories) {
        
        if (memoryDisplayTimer < 5.0f) {
            DrawText("Remember everything.........", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 42, WHITE);
            return;
        }
        
        float timeAfterStart = memoryDisplayTimer - 5.0f;
        int displayIndex = (int)(timeAfterStart / displayDurationPerMemory);
        
        if (displayIndex < MAX_STACK && displayIndex >= 0) {
            Texture2D currentTexture = memoryImages[memorySequence[displayIndex]];
            
            float scaleX = (float)SCREEN_WIDTH / currentTexture.width;
            float scaleY = (float)SCREEN_HEIGHT / currentTexture.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY; 
            
            int drawWidth = (int)(currentTexture.width * scale);
            int drawHeight = (int)(currentTexture.height * scale);
            int x = (SCREEN_WIDTH - drawWidth) / 2;
            int y = (SCREEN_HEIGHT - drawHeight) / 2;
            
            DrawTextureEx(currentTexture, (Vector2){x, y}, 0, scale, WHITE);
            
            DrawText(TextFormat("Memory %d of %d", displayIndex + 1, MAX_STACK), 
                    SCREEN_WIDTH / 2 - 80, 100, 20, LIGHTGRAY);
        }
    }
    else if (choosingMemories) {
        DrawText("How can you become like this.... (choice reverse sequence)", SCREEN_WIDTH / 2 - 250, 50, 30, WHITE);
        
        // Menggambar grid dengan urutan yang sudah diacak
        for (int i = 0; i < MAX_STACK; i++) {
            int col = i % 3;
            int row = i / 3;
            int gridStartX = SCREEN_WIDTH / 2 - (3 * 640) / 2; // Center grid horizontal untuk ukuran 640
            int gridStartY = SCREEN_HEIGHT / 2 - (2 * 360) / 2; // Center grid vertical untuk ukuran 360
            
            Rectangle choiceRect = {gridStartX + col * 640, gridStartY + row * 360, 640, 360};
            
            int memoryToDisplay = gridDisplayOrder[i];
            
            DrawTexturePro(
                memoryImages[memoryToDisplay],
                (Rectangle){0, 0, memoryImages[memoryToDisplay].width, memoryImages[memoryToDisplay].height},
                choiceRect,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
            
            Color borderColor = DARKGRAY;
            int borderThickness = 2;
            
            if (correctChoices[i]) {
                // Jika pilihan benar, border hijau tebal
                borderColor = GREEN;
                borderThickness = 4;
            } else {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, choiceRect)) {
                    // Hover effect hanya untuk yang belum dipilih
                    borderColor = YELLOW;
                    borderThickness = 3;
                }
            }
            
            DrawRectangleLinesEx(choiceRect, borderThickness, borderColor);
        }
    }

    if (minigameSuccess) {
        DrawText("Now you have to fight with your life.....", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 40, WHITE);
        DrawText("Press SPACE to continue...", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 20, LIGHTGRAY);
    }
}

bool CompareStacksReverse(Stack original, Stack playerChoice) {
    if (StackSize(original) != StackSize(playerChoice)) {
        return false;
    }
    
    for (int i = 0; i < StackSize(original); i++) {
        int originalIndex = StackSize(original) - 1 - i;
        if (original.Memory[originalIndex].Image.id != playerChoice.Memory[i].Image.id) {
            return false;
        }
    }
    return true;
}

void UnloadMiniGameStackAssets(Stack *S) {
    for (int i = 0; i < StackSize(*S); i++) {
        if (S->Memory[i].Image.id != 0) {
            UnloadTexture(S->Memory[i].Image);
            S->Memory[i].Image = (Texture2D){0};
        }
    }
    CreateEmptyStack(S);
    
    for (int i = 0; i < MAX_STACK; i++) {
        if (memoryImages[i].id != 0) {
            UnloadTexture(memoryImages[i]);
            memoryImages[i] = (Texture2D){0};
        }
    }
}