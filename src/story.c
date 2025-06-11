#include "story.h"
#include "mainmenu.h"
#include "Queue.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCENE 10
#define MAX_NODE_TREE 28

int currentScene = 0;
int currentFrame = 0;
float frameDelay = 3.5f;
float frameTimer = 0.0f;
bool isMusicPlaying = false;
Music currentSceneMusic;

TreeStory SceneTree[MAX_NODE_TREE];

// Inisialisasi Data Cerita
void InitDataCerita(TreeStory *SceneTree) {
    printf("Initializing story data...\n");

    // Inisialisasi queue untuk setiap node
    for (int i = 0; i < MAX_NODE_TREE; i++) {
        CreateQueue(&SceneTree[i].Frame);
        SceneTree[i].id = i;
        SceneTree[i].TotalScene = 0;
        SceneTree[i].numChoices = 0;
        SceneTree[i].choiceLeftSon = NULL;
        SceneTree[i].choiceRightSon = NULL;
        SceneTree[i].IdLeftSon = -1;
        SceneTree[i].IdRightSon = -1;
    }

    // Node 0
    printf("Setting up Node 0...\n");
    SceneTree[0].id = 0;
    SceneTree[0].TotalScene = 4;
    SceneTree[0].numChoices = 2;
    SceneTree[0].choiceLeftSon = "Go to Alleway";
    SceneTree[0].choiceRightSon = "Go to Open Street";
    SceneTree[0].IdLeftSon = 1;
    SceneTree[0].IdRightSon = 2;

    Scene scenes0[] = {
        {"Assets/Music/myinstants.mp3", "Girl : ......", "Assets/BackgroundSprites/background4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : It is so cold here, but i have to sell all these matches", "Assets/BackgroundSprites/background3.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : What should i go, Alleway?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : Or the Street?", "Assets/BackgroundSprites/background7.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER}
    };
    printf("Adding scenes to Node 0...\n");
    for (int i = 0; i < SceneTree[0].TotalScene; i++) {
        printf("  Scene %d:\n", i);
        printf("    Background path: %s\n", scenes0[i].backgroundPath ? scenes0[i].backgroundPath : "NULL");
        printf("    Character path: %s\n", scenes0[i].characterPath ? scenes0[i].characterPath : "NULL");
        printf("    Dialogue: %s\n", scenes0[i].dialogue ? scenes0[i].dialogue : "NULL");
        printf("    Position: %d\n", scenes0[i].CharPosition);
        EnQueueLast(&SceneTree[0].Frame, scenes0[i]);
    }
    SceneTree[0].TotalScene = QueueSize(SceneTree[0].Frame);
    printf("Node 0 setup complete. Total scenes: %d\n", SceneTree[0].TotalScene);

    // Node 1
    SceneTree[1].id = 1;
    SceneTree[1].TotalScene = 2;
    SceneTree[1].numChoices = 2;
    SceneTree[1].choiceLeftSon = "Light on a few matches";
    SceneTree[1].choiceRightSon = "Ignore it, Keep Walking";
    SceneTree[1].IdLeftSon = 3;
    SceneTree[1].IdRightSon = 4;

    Scene scenes1[] = {
        {NULL, " Girl : I'm Think that guy is feel cold, should i help him?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[1].TotalScene; i++) {
        EnQueueLast(&SceneTree[1].Frame, scenes1[i]);
    }
    SceneTree[1].TotalScene = QueueSize(SceneTree[1].Frame);

    // Node 2
    SceneTree[2].id = 2;
    SceneTree[2].TotalScene = 2;
    SceneTree[2].numChoices = 2;
    SceneTree[2].choiceLeftSon = "Walk crosing the street";
    SceneTree[2].choiceRightSon = "Walk along the sidewalks";
    SceneTree[2].IdLeftSon = 5;
    SceneTree[2].IdRightSon = 6;

    Scene scenes2[] = {
        {NULL, "", "Assets/BackgroundSprites/background8.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Would these people be in need of matches?", "Assets/BackgroundSprites/background8.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[2].TotalScene; i++) {
        EnQueueLast(&SceneTree[2].Frame, scenes2[i]);
    }
    SceneTree[2].TotalScene = QueueSize(SceneTree[2].Frame);

    // Node 3
    SceneTree[3].id = 3;
    SceneTree[3].TotalScene = 5;
    SceneTree[3].numChoices = 2;
    SceneTree[3].choiceLeftSon = "Give up all the match";
    SceneTree[3].choiceRightSon = "Protect the match";
    SceneTree[3].IdLeftSon = 7;
    SceneTree[3].IdRightSon = 8;

    Scene scenes3[] = {
        {NULL, "Homeless man: Thank you kid, here's 2 dollars for the match", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Yeah, thank you so much", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Give me the match too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Hey I want too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl : I'm Scared, please don't hurt me", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[3].TotalScene; i++) {
        EnQueueLast(&SceneTree[3].Frame, scenes3[i]);
    }
    SceneTree[3].TotalScene = QueueSize(SceneTree[3].Frame);

    // Node 4 (Ending: Die Alone)
    SceneTree[4].id = 4;
    SceneTree[4].TotalScene = 5;
    SceneTree[4].numChoices = 1;
    SceneTree[4].IdLeftSon = -1;
    SceneTree[4].IdRightSon = -1;
    SceneTree[4].choiceLeftSon = NULL;
    SceneTree[4].choiceRightSon = NULL;

    Scene scenes4[] = {
        {NULL, "GIrl : Itldlaosdlaokd's cold, should i light a few matches?", "Assets/BackgroundSprites/background14.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : No, i can handle it", "Assets/Endings/diecoldalone1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm still can handle it", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm Just need a rest for a moment......", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[4].TotalScene; i++) {
        EnQueueLast(&SceneTree[4].Frame, scenes4[i]);
    }
    SceneTree[4].TotalScene = QueueSize(SceneTree[4].Frame);

    // Node 5 (Ending: Crushed by a Car)
    SceneTree[5].id = 5;
    SceneTree[5].TotalScene = 4;
    SceneTree[5].numChoices = 1;
    SceneTree[5].IdLeftSon = -1;
    SceneTree[5].IdRightSon = -1;
    SceneTree[5].choiceLeftSon = NULL;
    SceneTree[5].choiceRightSon = NULL;

    Scene scenes5[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[5].TotalScene; i++) {
        EnQueueLast(&SceneTree[5].Frame, scenes5[i]);
    }
    SceneTree[5].TotalScene = QueueSize(SceneTree[5].Frame);

    // Node 6
    SceneTree[6].id = 6;
    SceneTree[6].TotalScene = 5;
    SceneTree[6].numChoices = 2;
    SceneTree[6].choiceLeftSon = "Try to pickpocket";
    SceneTree[6].choiceRightSon = "Offering the match";
    SceneTree[6].IdLeftSon = 9;
    SceneTree[6].IdRightSon = 10;

    Scene scenes6[] = {
        {NULL, "Walking....", "Assets/BackgroundSprites/background10.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl: Anyone, need a box match?", "Assets/BackgroundSprites/background11.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl: Just only 5$ a box!", "Assets/BackgroundSprites/background12.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : 5$ for a box matach will keep you warm", "Assets/BackgroundSprites/background9.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : Should i offer one to that man?", "Assets/BackgroundSprites/background13.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[6].TotalScene; i++) {
        EnQueueLast(&SceneTree[6].Frame, scenes6[i]);
    }
    SceneTree[6].TotalScene = QueueSize(SceneTree[6].Frame);

    // Node 7
    SceneTree[7].id = 7;
    SceneTree[7].TotalScene = 4;
    SceneTree[7].numChoices = 2;
    SceneTree[7].choiceLeftSon = "Take all and escape ";
    SceneTree[7].choiceRightSon = "Accept his offer";
    SceneTree[7].IdLeftSon = 11;
    SceneTree[7].IdRightSon = 12;

    Scene scenes7[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < SceneTree[7].TotalScene; i++) {
        EnQueueLast(&SceneTree[7].Frame, scenes7[i]);
    }
    SceneTree[7].TotalScene = QueueSize(SceneTree[7].Frame);

    // Node 8 (Ending: Rumbling) - Kosong
    SceneTree[8].id = 8;
    SceneTree[8].TotalScene = 0;
    SceneTree[8].numChoices = 0;
    SceneTree[8].choiceLeftSon = NULL;
    SceneTree[8].choiceRightSon = NULL;
    SceneTree[8].IdLeftSon = -1;
    SceneTree[8].IdRightSon = -1;

    // Node 9 (Ending: Get Pushed with a Man and Crushed by a Car)
    SceneTree[9].id = 9;
    SceneTree[9].TotalScene = 4;
    SceneTree[9].numChoices = 1;
    SceneTree[9].IdLeftSon = -1;
    SceneTree[9].IdRightSon = -1;
    SceneTree[9].choiceLeftSon = NULL;
    SceneTree[9].choiceRightSon = NULL;

    Scene scenes9[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[9].TotalScene; i++) {
        EnQueueLast(&SceneTree[9].Frame, scenes9[i]);
    }
    SceneTree[9].TotalScene = QueueSize(SceneTree[9].Frame);

    // Node 10
    SceneTree[10].id = 10;
    SceneTree[10].TotalScene = 4;
    SceneTree[10].numChoices = 2;
    SceneTree[10].choiceLeftSon = "Tell him";
    SceneTree[10].choiceRightSon = "Be Quite..";
    SceneTree[10].IdLeftSon = 13;
    SceneTree[10].IdRightSon = 14;

    Scene scenes10[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < SceneTree[10].TotalScene; i++) {
        EnQueueLast(&SceneTree[10].Frame, scenes10[i]);
    }
    SceneTree[10].TotalScene = QueueSize(SceneTree[10].Frame);
}

// Menentukan posisi karakter ketika digambar
void DrawCharacterAtPosition(Texture2D tex, CharacterPosition pos) {
    if (tex.id == 0 || pos == CHAR_POS_NONE) return;

    float charX = 0;
    float charY = SCREEN_HEIGHT - tex.height - 100;

    switch (pos) {
        case CHAR_POS_LEFT:
            charX = 50;
            break;
        case CHAR_POS_CENTER:
            charX = SCREEN_WIDTH / 2 - tex.width / 2;
            break;
        case CHAR_POS_RIGHT:
            charX = SCREEN_WIDTH - tex.width - 50;
            break;
        default:
            break;
    }
    DrawTexture(tex, (int)charX, (int)charY, WHITE);
}


// Memuat aset untuk node
void LoadNodeAssets(TreeStory SceneTree[], int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &SceneTree[nodeIndex];
    Queue *queue = &node->Frame;

    for (int i = 0; i < QueueSize(*queue); i++) {
        Scene *scene = &queue->data[(queue->front + i) % MAX_SCENE];
        if (scene->backgroundPath != NULL) {
            scene->backgroundTex = LoadTexture(scene->backgroundPath);
            scene->backgroundTex.height = SCREEN_HEIGHT;
            scene->backgroundTex.width = SCREEN_WIDTH;
        } else {
            scene->backgroundTex = (Texture2D){0};
        }
        if (scene->characterPath != NULL) {
            scene->characterTex = LoadTexture(scene->characterPath);
            scene->characterTex.height /= 2;
            scene->characterTex.width /= 2;
        } else {
            scene->characterTex = (Texture2D){0};
        }
    }
}

// Menghapus aset dari node tertentu
void UnloadNodeAssets(TreeStory SceneTree[], int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &SceneTree[nodeIndex];
    Queue *queue = &node->Frame;

    while (!isQueueEmpty(*queue)) {
        DeQueueFirst(queue);
    }
}

// Menggambar scene
void DrawCurrentNodeScreen(TreeStory SceneTree[]) {
    if (currentScene < 0 || currentScene >= MAX_NODE_TREE) return;

    TreeStory *node = &SceneTree[currentScene];
    Scene current = Peek(node->Frame);

    if (currentGameState == GAME_STATE_PLAY_GAME) {
        if (current.backgroundTex.id != 0) {
            DrawTexture(current.backgroundTex, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);
        }

        if (node->id != 4 && node->id != 5) {
            DrawCharacterAtPosition(current.characterTex, current.CharPosition);

            if (current.dialogue != NULL || (currentFrame == node->TotalScene - 1 && node->numChoices > 0)) {
                DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, BLACK);
                DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);

                if (current.dialogue != NULL) {
                    DrawText(current.dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
                }
            }

            if (currentFrame == node->TotalScene - 1 && node->numChoices > 0) {
                int choiceButtonWidth = 400;
                int choiceButtonHeight = 60;
                int yButton = SCREEN_HEIGHT / 2 + 275;

                if (node->numChoices >= 1) {
                    Rectangle choiceRectLeft = {
                        SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                        yButton,
                        (float)choiceButtonWidth,
                        (float)choiceButtonHeight
                    };
                    DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
                    DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
                    DrawText(node->choiceLeftSon,
                             (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText(node->choiceLeftSon, 25) / 2),
                             (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                             25, WHITE);
                }

                if (node->numChoices == 2) {
                    Rectangle choiceRectRight = {
                        SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                        yButton,
                        (float)choiceButtonWidth,
                        (float)choiceButtonHeight
                    };
                    DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
                    DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
                    DrawText(node->choiceRightSon,
                             (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText(node->choiceRightSon, 25) / 2),
                             (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                             25, WHITE);
                }
            }
        } else {
            DrawCharacterAtPosition(current.characterTex, current.CharPosition);

            if (currentFrame < node->TotalScene - 1 && current.dialogue != NULL) {
                DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, BLACK);
                DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
                DrawText(current.dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
            }
        }
    } else {
        if (current.backgroundTex.id != 0) {
            DrawTexture(current.backgroundTex, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);
        }
    }
}

// Mengupdate logika cerita
void UpdateCerita(TreeStory SceneTree[], GameState *gameState) {
    if (currentScene < 0 || currentScene >= MAX_NODE_TREE) {
        return;
    }

    TreeStory *node = &SceneTree[currentScene];
    Scene current = Peek(node->Frame);
      // Update music if it's playing
    if (isMusicPlaying) {
        UpdateMusicStream(currentSceneMusic);
        if (!IsMusicStreamPlaying(currentSceneMusic)) {
            isMusicPlaying = false;
        }
    }
    
    // Check if this scene has music and should start playing
    if (current.backgroundSound != NULL && !isMusicPlaying) {
        UnloadMusicStream(currentSceneMusic);
        currentSceneMusic = LoadMusicStream(current.backgroundSound);
        SetMusicVolume(currentSceneMusic, 0.5f);
        PlayMusicStream(currentSceneMusic);
        isMusicPlaying = true;
        currentSceneMusic.looping = false;
    }

    Vector2 mouse = GetMousePosition();

    if (IsKeyPressed(KEY_F1)) {
        *gameState = GAME_STATE_PAUSE;
        InitButtonRects(*gameState);
        return;
    }

    if (node->id == 4 || node->id == 5) {
        frameTimer += GetFrameTime();

        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            if (currentFrame < node->TotalScene - 1) {
                currentFrame++;
                DeQueueFirst(&node->Frame);
                Scene nextScene = Peek(node->Frame);
                if (nextScene.backgroundPath != NULL) {
                    nextScene.backgroundTex = LoadTexture(nextScene.backgroundPath);
                    nextScene.backgroundTex.height = SCREEN_HEIGHT;
                    nextScene.backgroundTex.width = SCREEN_WIDTH;
                }
                if (nextScene.characterPath != NULL) {
                    nextScene.characterTex = LoadTexture(nextScene.characterPath);
                    nextScene.characterTex.height /= 2;
                    nextScene.characterTex.width /= 2;
                }
                EnQueueLast(&node->Frame, nextScene);
            } else {
                UnloadNodeAssets(SceneTree, currentScene);
                *gameState = GAME_STATE_MAIN_MENU;
                currentScene = 0;
                currentFrame = 0;
            }
        }
    } else {
        if (currentFrame == node->TotalScene - 1 && node->numChoices > 0) {
            int choiceButtonWidth = 400;
            int choiceButtonHeight = 60;
            int choiceStartY = SCREEN_HEIGHT / 2 + 275;

            Rectangle choiceRectLeft = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            if (CheckCollisionPointRec(mouse, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ProsesChoice(SceneTree, 0);
                return;
            }

            if (node->numChoices == 2) {
                Rectangle choiceRectRight = {
                    SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                    choiceStartY,
                    (float)choiceButtonWidth,
                    (float)choiceButtonHeight
                };
                if (CheckCollisionPointRec(mouse, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ProsesChoice(SceneTree, 1);
                    return;
                }
            }
        } else {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (currentFrame < node->TotalScene - 1) {
                    currentFrame++;
                    DeQueueFirst(&node->Frame);
                    Scene nextScene = Peek(node->Frame);
                    if (nextScene.backgroundPath != NULL) {
                        nextScene.backgroundTex = LoadTexture(nextScene.backgroundPath);
                        nextScene.backgroundTex.height = SCREEN_HEIGHT;
                        nextScene.backgroundTex.width = SCREEN_WIDTH;
                    }
                    if (nextScene.characterPath != NULL) {
                        nextScene.characterTex = LoadTexture(nextScene.characterPath);
                        nextScene.characterTex.height /= 2;
                        nextScene.characterTex.width /= 2;
                    }
                    EnQueueLast(&node->Frame, nextScene);
                }
            }
        }
    }
}

// Memproses pilihan pemain
void ProsesChoice(TreeStory SceneTree[], int choice) {
    int nextNodeIndex = (choice == 0) ? SceneTree[currentScene].IdLeftSon : SceneTree[currentScene].IdRightSon;

    if (nextNodeIndex < 0 || nextNodeIndex >= MAX_NODE_TREE || SceneTree[nextNodeIndex].TotalScene == 0) {
        return;
    }
    UnloadNodeAssets(SceneTree, currentScene);
    currentScene = nextNodeIndex;
    currentFrame = 0;
    LoadNodeAssets(SceneTree, currentScene);
    printf("Berpindah ke scene %d\n", currentScene);
}

// Menyimpan progres cerita
void SaveGameStory(const char *filename, int Node, int Scene) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Nama File Tidak Ditemukan\n");
        return;
    }

    fwrite(&Node, sizeof(int), 1, file);
    fwrite(&Scene, sizeof(int), 1, file);
    fclose(file);
}


// Memuat progres cerita
void LoadGameStory(const char *filename, int *Node, int *Scene) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Nama File Tidak Ditemukan\n");
        *Node = 0;
        *Scene = 0;
        return;
    }

    fread(Node, sizeof(int), 1, file);
    fread(Scene, sizeof(int), 1, file);
    fclose(file);
}

void UnloadStoryAudio() {
    if (isMusicPlaying) {
        UnloadMusicStream(currentSceneMusic);
        isMusicPlaying = false;
    }
}