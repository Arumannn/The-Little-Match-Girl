#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customstory.h"

void StoryCreator(VNTreeNode **currentNodePtr, AssetLibraryArr assets) {
    static int state = 0; // pilih background & character, masukan dialog, kemudian pemilihan branch
    static int assetSelectState = 0; // 0 = bg, 1 = char
    static int bgIndex = 0;
    static int spriteIndex = 0;
    static char dialogue[256] = "";
    static int dialogueCharCount = 0;

    static SceneNode *lastScene = NULL; // track last scene for linking

    VNTreeNode *currentNode = *currentNodePtr;

    if (state == 0) {
        MenuBackground = LoadTexture("Assets/BackSprites/customstorymenu.png");
        // === Asset Selection (Background / Character) ===
        Texture2D currentTex = (assetSelectState == 0)
            ? assets[bgIndex].background
            : assets[spriteIndex].sprite;

        float scale = 0.5f;
        int texWidth = currentTex.width * scale;
        int texHeight = currentTex.height * scale;
        Vector2 pos = {
            (GetScreenWidth() - texWidth) / 2.0f,
            (GetScreenHeight() - texHeight) / 2.0f
        };
        DrawTextureEx(currentTex, pos, 0.0f, scale, WHITE);

        const char *label = (assetSelectState == 0) ? "Select Background" : "Select Character";
        DrawText(label, GetScreenWidth() / 2 - MeasureText(label, 20) / 2, 20, 20, WHITE);

        int index = (assetSelectState == 0) ? bgIndex : spriteIndex;
        char indexText[64];
        sprintf(indexText, "%d out of %d", index + 1, assetCount);
        DrawText(indexText, GetScreenWidth() / 2 - MeasureText(indexText, 20) / 2, GetScreenHeight() - 80, 20, LIGHTGRAY);

        DrawText("← / → to switch   |   Enter to confirm", GetScreenWidth() / 2 - 160, GetScreenHeight() - 40, 20, DARKGRAY);

        if (IsKeyPressed(KEY_RIGHT)) {
            if (assetSelectState == 0)
                bgIndex = (bgIndex + 1) % assetCount;
            else
                spriteIndex = (spriteIndex + 1) % assetCount;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            if (assetSelectState == 0)
                bgIndex = (bgIndex - 1 + assetCount) % assetCount;
            else
                spriteIndex = (spriteIndex - 1 + assetCount) % assetCount;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            if (assetSelectState == 0) {
                assetSelectState = 1;
            } else {
                assetSelectState = 0;
                dialogueCharCount = 0;
                dialogue[0] = '\0';
                state = 1;
            }
        }

    } else if (state == 1) {
        // === Dialogue Entry ===
        DrawText("Enter Dialogue (Enter to confirm)", 20, 20, 20, WHITE);
        DrawRectangle(20, 60, GetScreenWidth() - 40, 40, DARKGRAY);
        DrawText(dialogue, 30, 70, 20, BLACK);

        int key = GetCharPressed();
        while (key > 0) {
            if (dialogueCharCount < 255) {
                dialogue[dialogueCharCount++] = (char)key;
                dialogue[dialogueCharCount] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && dialogueCharCount > 0) {
            dialogue[--dialogueCharCount] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER) && dialogueCharCount > 0) {
            SceneNode *scene = (SceneNode*)malloc(sizeof(SceneNode));
            scene->backgroundTex = assets[bgIndex].background;
            scene->characterTex = assets[spriteIndex].sprite;
            scene->dialogue = strdup(dialogue);
            scene->backgroundPath = NULL;
            scene->characterPath = NULL;
            scene->next = NULL;

            // Append to list
            if (currentNode->sceneList == NULL) {
                currentNode->sceneList = scene;
                lastScene = scene;
            } else {
                lastScene->next = scene;
                lastScene = scene;
            }

            state = 2; // go to branching or continue choice
        }

    } else if (state == 2) {
        // === Next Step Choice ===
        DrawText("What next?", GetScreenWidth() / 2 - 80, 60, 20, GREEN);
        DrawText("Press N: Add another scene in sequence", 100, 120, 20, LIGHTGRAY);
        DrawText("Press A: Branch LEFT from here (Option A)", 100, 160, 20, LIGHTGRAY);
        DrawText("Press D: Branch RIGHT from here (Option B)", 100, 200, 20, LIGHTGRAY);

        if (IsKeyPressed(KEY_N)) {
            state = 0; // back to new scene in same node
        }

        if (IsKeyPressed(KEY_A)) {
            if (!currentNode->leftChoice) {
                currentNode->leftChoice = (VNTreeNode *)calloc(1, sizeof(VNTreeNode));
                currentNode->leftChoice->id = currentNode->id * 2;
            }
            *currentNodePtr = currentNode->leftChoice;
            lastScene = NULL;
            state = 0;
        }

        if (IsKeyPressed(KEY_D)) {
            if (!currentNode->rightChoice) {
                currentNode->rightChoice = (VNTreeNode *)calloc(1, sizeof(VNTreeNode));
                currentNode->rightChoice->id = currentNode->id * 2 + 1;
            }
            *currentNodePtr = currentNode->rightChoice;
            lastScene = NULL;
            state = 0;
        }
    }
}





void LoadAssetsSimple(AssetLibraryArr assets) {
    char bgPath[128];
    char spritePath[128];

    for (int i = 0; i < 2; i++)
    {
        sprintf(bgPath, "Assets/BackgroundSprites/background%d.png", i + 1);
        sprintf(spritePath, "Assets/CharaSprites/chara%d.png", i + 1);

        assets[i].background = LoadTexture(bgPath);
        assets[i].sprite = LoadTexture(spritePath);
    }
    // assets->background = LoadTexture("Assets/BackgroundSprites/background1.png");
    // assets->sprite = LoadTexture("Assets/CharaSprites/chara1.png");
}

void UnloadAssetsSimple(AssetLibraryArr assets) {
    char bgPath[128];
    char spritePath[128];

    for (int i = 0; i < 2; i++)
    {
        sprintf(bgPath, "Assets/BackgroundSprites/background%d.png", i + 1);
        sprintf(spritePath, "Assets/CharaSprites/chara%d.png", i + 1);

        UnloadTexture(assets[i].background);
        UnloadTexture(assets[i].sprite);
    }
}

void PrintTree(VNTreeNode *node) {
    if (node == NULL) return;

    printf("Node ID: %d\n", node->id);

    SceneNode *scene = node->sceneList;
    int sceneIndex = 1;

    while (scene != NULL) {
        printf("  Scene %d:\n", sceneIndex++);
        
        if (scene->backgroundPath != NULL)
            printf("    Background: %s\n", scene->backgroundPath);
        else
            printf("    Background: [Texture loaded, no path stored]\n");

        printf("    Dialogue: %s\n", scene->dialogue);

        if (scene->characterPath != NULL)
            printf("    Character: %s\n", scene->characterPath);
        else
            printf("    Character: [Texture loaded, no path stored]\n");

        scene = scene->next;
    }

    if (node->leftChoice != NULL) {
        printf("  Branch LEFT:\n");
        PrintTree(node->leftChoice);
    }

    if (node->rightChoice != NULL) {
        printf("  Branch RIGHT:\n");
        PrintTree(node->rightChoice);
    }
}
