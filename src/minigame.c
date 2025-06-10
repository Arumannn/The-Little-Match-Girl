#include "minigame.h"
#include <stdio.h>

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
int memorySequence[MAX_STACK];   // Urutan memori yang ditampilkan (berurutan)
int gridDisplayOrder[MAX_STACK]; // Urutan tampilan di grid (acak)
bool correctChoices[MAX_STACK];  // Menandai pilihan yang benar

void InitMiniGameStack() {
    char filepath[256];
    
    for (int i = 0; i < MAX_STACK; i++) {
        sprintf(filepath, "Assets/Memories/Memory_%d.png", i);
        memoryImages[i] = LoadTexture(filepath);
        if (memoryImages[i].id == 0) {
            printf("Gagal memuat gambar %s\n", filepath);
        }
    }
    
    // Inisialisasi stack
    CreateEmptyStack(&MemoryStack);
    CreateEmptyStack(&PlayerChoiceStack);

    // Inisialisasi urutan memori (0, 1, 2, ..., MAX_STACK-1)
    for (int i = 0; i < MAX_STACK; i++) {
        memorySequence[i] = i;
    }
    
    // Inisialisasi dan acak urutan grid
    for (int i = 0; i < MAX_STACK; i++) {
        gridDisplayOrder[i] = i;
    }
    // Fisher-Yates shuffle untuk gridDisplayOrder
    for (int i = MAX_STACK - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        int temp = gridDisplayOrder[i];
        gridDisplayOrder[i] = gridDisplayOrder[j];
        gridDisplayOrder[j] = temp;
    }

    // Inisialisasi state minigame
    showingMemories = true;
    choosingMemories = false;
    minigameSuccess = false;
    minigameActive = true;
    currentMemoryIndex = 0;
    currentChoiceCount = 0;
    memoryDisplayTimer = 0;
    
    // Load music once
    static Music minigameMusic;
    minigameMusic = LoadMusicStream("Assets/Music/Melody of Memories.mp3");
    PlayMusicStream(minigameMusic);
    
    // Reset array correctChoices
    for (int i = 0; i < MAX_STACK; i++) {
        correctChoices[i] = false;
    }
    
    // Debugging: Cetak urutan
    printf("Urutan memori yang ditampilkan: ");
    for (int i = 0; i < MAX_STACK; i++) {
        printf("%d ", memorySequence[i]);
    }
    printf("\nUrutan tampilan grid: ");
    for (int i = 0; i < MAX_STACK; i++) {
        printf("%d ", gridDisplayOrder[i]);
    }
    printf("\n");
}

void UpdateMiniGameStack(GameState *currentGameState) {
    static Music minigameMusic;
    UpdateMusicStream(minigameMusic);  // Update existing music stream
    if (!minigameActive) return;

    static Stack TempStack; // Stack sementara untuk perbandingan dengan Pop
    static bool tempStackInitialized = false; // Flag inisialisasi TempStack

    if (showingMemories) {
        memoryDisplayTimer += GetFrameTime();

        // Delay 5 detik sebelum menampilkan memori
        if (memoryDisplayTimer < 5.0f) {
            return;
        }

        // Hitung waktu relatif dan indeks memori yang diharapkan
        float timeAfterStart = memoryDisplayTimer - 5.0f;
        int expectedMemoryIndex = (int)(timeAfterStart / displayDurationPerMemory);
        
        // Push memori ke MemoryStack
        if (expectedMemoryIndex >= currentMemoryIndex && currentMemoryIndex < MAX_STACK) {
            Data memoryData = { 
                memoryImages[memorySequence[currentMemoryIndex]], 
                (Rectangle){0, 0, memoryImages[memorySequence[currentMemoryIndex]].width, 
                           memoryImages[memorySequence[currentMemoryIndex]].height}
            };
            Push(&MemoryStack, memoryData);
            currentMemoryIndex++;
        }

        // Pindah ke fase pemilihan setelah semua memori ditampilkan
        if (currentMemoryIndex >= MAX_STACK && timeAfterStart >= MAX_STACK * displayDurationPerMemory) {
            showingMemories = false;
            choosingMemories = true;
            currentChoiceCount = 0;
            tempStackInitialized = false; // Reset flag TempStack
            printf("Memory Stack : Memulai pemilihan memori (urutan terbalik)\n");
            PrintStack(MemoryStack); // Debugging: Cetak MemoryStack
        }
    } 
    else if (choosingMemories) {
        // Inisialisasi TempStack dengan salinan MemoryStack
        if (!tempStackInitialized) {
            CreateEmptyStack(&TempStack);
            for (int i = 0; i < StackSize(MemoryStack); i++) {
                Push(&TempStack, MemoryStack.Memory[i]);
            }
            tempStackInitialized = true;
            printf("Menyalin MemoryStack ke TempStack\n");
            PrintStack(TempStack); // Debugging: Cetak TempStack
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            for (int i = 0; i < MAX_STACK; i++) {
                // Skip jika kotak sudah dipilih
                if (correctChoices[i]) continue;

                // Hitung posisi grid 3x2
                Rectangle choiceRect;
                int col = i % 3;
                int row = i / 3;
                int gridStartX = SCREEN_WIDTH / 2 - (3 * 640) / 2;
                int gridStartY = SCREEN_HEIGHT / 2 - (2 * 360) / 2;
                choiceRect.x = gridStartX + col * 640;
                choiceRect.y = gridStartY + row * 360;
                choiceRect.width = 640;
                choiceRect.height = 360;

                if (CheckCollisionPointRec(mouse, choiceRect)) {
                    // Mendapatkan indeks memori dari grid yang diacak
                    int actualMemoryIndex = gridDisplayOrder[i];
                    
                    // Pop elemen teratas dari TempStack
                    Data expectedMemory;
                    Pop(&TempStack, &expectedMemory);

                    // Cek apakah Pop berhasil
                    if (expectedMemory.Image.id != 0) { 
                        if (memoryImages[actualMemoryIndex].id == expectedMemory.Image.id) {
                            Data chosenData = { memoryImages[actualMemoryIndex], choiceRect };
                            Push(&PlayerChoiceStack, chosenData);
                            correctChoices[i] = true;
                            currentChoiceCount++;
                            printf("Pilihan benar: Memori %d (ID: %u)\n", actualMemoryIndex, memoryImages[actualMemoryIndex].id);
                            PrintStack(PlayerChoiceStack); // Debugging: Cetak PlayerChoiceStack
                            
                            // Cek apakah minigame selesai
                            if (currentChoiceCount >= MAX_STACK) {
                                choosingMemories = false;
                                minigameSuccess = true;
                                printf("Minigame berhasil diselesaikan!\n");
                            }
                        } else {
                            // Pilihan salah, reset PlayerChoiceStack dan TempStack
                            CreateEmptyStack(&PlayerChoiceStack);
                            currentChoiceCount = 0;
                            for (int j = 0; j < MAX_STACK; j++) {
                                correctChoices[j] = false;
                            }
                            CreateEmptyStack(&TempStack);
                            for (int j = 0; j < StackSize(MemoryStack); j++) {
                                Push(&TempStack, MemoryStack.Memory[j]);
                            }
                            printf("Pilihan salah: Memori %d (ID: %u), mereset pilihan\n", 
                                   actualMemoryIndex, memoryImages[actualMemoryIndex].id);
                            PrintStack(TempStack); // Debugging: Cetak TempStack
                        }
                    } else {
                        printf("Error: TempStack kosong, mereset TempStack\n");
                        CreateEmptyStack(&TempStack);
                        for (int j = 0; j < StackSize(MemoryStack); j++) {
                            Push(&TempStack, MemoryStack.Memory[j]);
                        }
                    }
                    break;
                }
            }
        }
    }
    
    if (minigameSuccess && IsKeyPressed(KEY_SPACE)) {
        minigameActive = false;
        CreateEmptyStack(&MemoryStack);
        CreateEmptyStack(&PlayerChoiceStack);
        CreateEmptyStack(&TempStack); 
        *currentGameState = GAME_STATE_PLAY_GAME;
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
        DrawText("How can you become like this.... (choose reverse sequence)", SCREEN_WIDTH / 2 - 250, 50, 30, WHITE);
        
        // Menggambar grid 3x2
        for (int i = 0; i < MAX_STACK; i++) {
            int col = i % 3;
            int row = i / 3;
            int gridStartX = SCREEN_WIDTH / 2 - (3 * 640) / 2;
            int gridStartY = SCREEN_HEIGHT / 2 - (2 * 360) / 2;
            
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
                borderColor = GREEN;
                borderThickness = 4;
            } else {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, choiceRect)) {
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

void UnloadMiniGameStackAssets(Stack *S) {
    CreateEmptyStack(S);
}