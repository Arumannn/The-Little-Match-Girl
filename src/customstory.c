#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customstory.h"

// void CustomStoryGUI(int state)
// {
//     DrawTexture(MenuBackground, GetScreenWidth()/2, GetScreenHeight()/2, WHITE);
//     switch (state)
//     {
//         case 0:
//         DrawText(("Picture %i of %i", state, BACKGROUND_AMMOUNT), GetScreenWidth()/2, (GetScreenHeight()/2) - 30, 50, WHITE);
//         break;
//         case 1:
//         DrawText(("Picture %i of %i", state, CHARA_AMMOUNT), GetScreenWidth()/2, (GetScreenHeight()/2) - 30, 50, WHITE);
//         break;
//     }
// }
// Global (or passed in context)
int selectedBackground = 0;
int selectedCharacter = 0;
char inputText[256] = "";
SceneList currentSceneList = NULL;

void CustomStoryGUI(int state)
{
    int currentpicture = 0;
    DrawTexture(MenuBackground, 0, 0, WHITE);  // background

    switch (state)
    {
        case MODE_SELECT_BACKGROUND:
            DrawText(TextFormat("Background %d of %d", selectedBackground+1, BACKGROUND_AMMOUNT),
                     50, 50, 30, WHITE);
            DrawTexture(FileBackground[selectedBackground], 200, 100, WHITE);

            // Handle input to change background
            //while (currentpicture >= 0 && currentpicture < BACKGROUND_AMMOUNT)
            //{
                //if (IsKeyPressed(KEY_RIGHT)) selectedBackground = (selectedBackground + 1) % BACKGROUND_AMMOUNT;
                //if (IsKeyPressed(KEY_LEFT)) selectedBackground = (selectedBackground - 1 + BACKGROUND_AMMOUNT) % BACKGROUND_AMMOUNT;
            //}
            if (IsKeyPressed(KEY_RIGHT)) selectedBackground = (selectedBackground + 1) % BACKGROUND_AMMOUNT;
            if (IsKeyPressed(KEY_LEFT)) selectedBackground = (selectedBackground - 1 + BACKGROUND_AMMOUNT) % BACKGROUND_AMMOUNT;
            break;

        case MODE_SELECT_CHARACTER:
            DrawText(TextFormat("Character %d of %d", selectedCharacter+1, CHARA_AMMOUNT),50, 50, 30, WHITE);
            DrawTexture(FileChara[selectedCharacter], 200, 100, WHITE);

            // Handle input to change character
            if (IsKeyPressed(KEY_RIGHT)) selectedCharacter = (selectedCharacter + 1) % CHARA_AMMOUNT;
            if (IsKeyPressed(KEY_LEFT)) selectedCharacter = (selectedCharacter - 1 + CHARA_AMMOUNT) % CHARA_AMMOUNT;
            break;

        case MODE_INPUT_TEXT:
            DrawText("Enter narration text:", 50, 50, 30, WHITE);
            DrawText(inputText, 60, 100, 20, WHITE);

            // Handle text input
            int key = GetCharPressed();
            while (key > 0)
            {
                if (strlen(inputText) < 255)
                {
                    int len = strlen(inputText);
                    inputText[len] = (char)key;
                    inputText[len+1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputText) > 0)
                inputText[strlen(inputText) - 1] = '\0';
            break;

        case MODE_CONFIRM_NODE:
            DrawText("Scene Preview", 50, 50, 30, WHITE);
            DrawTexture(FileBackground[selectedBackground], 0, 0, WHITE);
            DrawTexture(FileChara[selectedCharacter], 400, 200, WHITE);
            DrawText(inputText, 50, 500, 20, WHITE);

            DrawText("Press ENTER to confirm and add to list", 50, 600, 20, GREEN);
            if (IsKeyPressed(KEY_ENTER))
            {
                SceneList newNode = malloc(sizeof(struct ListElements));
                newNode->Next = NULL;

                newNode->Data.Background = FileBackground[selectedBackground];
                newNode->Data.Character = FileChara[selectedCharacter];
                newNode->Data.Convo = strdup(inputText);

                // Add to end of linked list
                if (!(*Current)->NodeContents)
                    (*Current)->NodeContents = newNode;
                else {
                    SceneList iter = (*Current)->NodeContents;
                    while (iter->Next) iter = iter->Next;
                    iter->Next = newNode;
                }

                // Reset for next input
                inputText[0] = '\0';
                // Move back to background selection or show preview mode
            }
            break;

        case MODE_REVIEW_SCENE:
            DrawText("Reviewing Scene", 50, 50, 30, WHITE);

            if (currentSceneList)
            {
                DrawTexture(currentSceneList->Data.Background, 0, 0, WHITE);
                DrawTexture(currentSceneList->Data.Character, 400, 200, WHITE);
                DrawText(currentSceneList->Data.Convo, 50, 500, 20, WHITE);

                if (IsKeyPressed(KEY_RIGHT) && currentSceneList->Next)
                    currentSceneList = currentSceneList->Next;
            }
            else
            {
                DrawText("No scene to review", 100, 200, 30, RED);
            }
            break;

        case MODE_MOVE_TREE:
            DrawText("Move to:", 50, 50, 30, WHITE);
            DrawText("LEFT - Add left node", 60, 100, 20, WHITE);
            DrawText("RIGHT - Add right node", 60, 130, 20, WHITE);
            DrawText("UP - Go to parent", 60, 160, 20, WHITE);

            if (IsKeyPressed(KEY_LEFT) && !(*Current)->Left)
            {
                (*Current)->Left = malloc(sizeof(struct Tree));
                (*Current)->Left->Left = (*Current)->Left->Right = NULL;
                (*Current)->Left->Parent = *Current;
                (*Current)->Left->NodeContents = NULL;
                *Current = (*Current)->Left;
            }
            else if (IsKeyPressed(KEY_RIGHT) && !(*Current)->Right)
            {
                (*Current)->Right = malloc(sizeof(struct Tree));
                (*Current)->Right->Left = (*Current)->Right->Right = NULL;
                (*Current)->Right->Parent = *Current;
                (*Current)->Right->NodeContents = NULL;
                *Current = (*Current)->Right;
            }
            else if (IsKeyPressed(KEY_UP) && (*Current)->Parent)
            {
                *Current = (*Current)->Parent;
            }
            break;,
    }
}

void InitiateAssets()
{
    char Link[100];
    MenuBackground = LoadTexture("Assets/BackSprites/customstorymenu.png");
    for (int i = 0; i < BACKGROUND_AMMOUNT; i++)
    {
        sprintf(Link, "Assets/BackgroundSprites/background%d.png", i + 1);
        FileBackground[i] = LoadTexture(Link);
    }
    for (int i = 0; i < CHARA_AMMOUNT; i++)
    {
        sprintf(Link, "Assets/CharaSprites/chara%d.png", i + 1);
        FileChara[i] = LoadTexture(Link);
    }
}

void SaveSlot(CustomSceneTree *ThisSlot)
{

}

void MakeCustomStory(CustomSceneTree *ThisSlot)
{
    Current = *ThisSlot;

    static int guiState = MODE_SELECT_BACKGROUND;

    BeginDrawing();
    ClearBackground(BLACK);
    CustomStoryGUI(guiState);
    EndDrawing();

    // Add key bindings to move between states
    if (IsKeyPressed(KEY_ENTER)) {
        guiState = (guiState + 1) % 6; // move through modes, or use more precise control
    }
}


void LoadSlot(CustomSceneTree *ThisSlot)
{

}

// void StoryCreator(VNTreeNode *currentNodePtr, AssetLibraryArr assets) {
//     static int state = 0; // pilih background & character, masukan dialog, kemudian pemilihan branch
//     static int assetSelectState = 0; // 0 = bg, 1 = char
//     static int bgIndex = 0;
//     static int spriteIndex = 0;
//     static char dialogue[256] = "";
//     static int dialogueCharCount = 0;

//     static SceneNode *lastScene = NULL; // track last scene for linking

//     VNTreeNode currentNode = *currentNodePtr;

//     CustomLoadAssetsSimple(assets);

//     if (state == 0) {
//         MenuBackground = LoadTexture("Assets/BackSprites/customstorymenu.png");
//         // === Asset Selection (Background / Character) ===
//         Texture2D currentTex = (assetSelectState == 0)
//             ? assets[bgIndex].background
//             : assets[spriteIndex].sprite;

//         float scale = 0.5f;
//         int texWidth = currentTex.width * scale;
//         int texHeight = currentTex.height * scale;
//         Vector2 pos = {
//             (GetScreenWidth() - texWidth) / 2.0f,
//             (GetScreenHeight() - texHeight) / 2.0f
//         };
//         DrawTextureEx(currentTex, pos, 0.0f, scale, WHITE);

//         const char *label = (assetSelectState == 0) ? "Select Background" : "Select Character";
//         DrawText(label, GetScreenWidth() / 2 - MeasureText(label, 20) / 2, 20, 20, WHITE);

//         int index = (assetSelectState == 0) ? bgIndex : spriteIndex;
//         char indexText[64];
//         sprintf(indexText, "%d out of %d", index + 1, assetCount);
//         DrawText(indexText, GetScreenWidth() / 2 - MeasureText(indexText, 20) / 2, GetScreenHeight() - 80, 20, LIGHTGRAY);

//         DrawText("← / → to switch   |   Enter to confirm", GetScreenWidth() / 2 - 160, GetScreenHeight() - 40, 20, DARKGRAY);

//         if (IsKeyPressed(KEY_RIGHT)) {
//             if (assetSelectState == 0)
//                 bgIndex = (bgIndex + 1) % assetCount;
//             else
//                 spriteIndex = (spriteIndex + 1) % assetCount;
//         }
//         if (IsKeyPressed(KEY_LEFT)) {
//             if (assetSelectState == 0)
//                 bgIndex = (bgIndex - 1 + assetCount) % assetCount;
//             else
//                 spriteIndex = (spriteIndex - 1 + assetCount) % assetCount;
//         }

//         if (IsKeyPressed(KEY_ENTER)) {
//             if (assetSelectState == 0) {
//                 assetSelectState = 1;
//             } else {
//                 assetSelectState = 0;
//                 dialogueCharCount = 0;
//                 dialogue[0] = '\0';
//                 state = 1;
//             }
//         }

//     } else if (state == 1) {
//         // === Dialogue Entry ===
//         DrawText("Enter Dialogue (Enter to confirm)", 20, 20, 20, WHITE);
//         DrawRectangle(20, 60, GetScreenWidth() - 40, 40, DARKGRAY);
//         DrawText(dialogue, 30, 70, 20, BLACK);

//         int key = GetCharPressed();
//         while (key > 0) {
//             if (dialogueCharCount < 255) {
//                 dialogue[dialogueCharCount++] = (char)key;
//                 dialogue[dialogueCharCount] = '\0';
//             }
//             key = GetCharPressed();
//         }

//         if (IsKeyPressed(KEY_BACKSPACE) && dialogueCharCount > 0) {
//             dialogue[--dialogueCharCount] = '\0';
//         }

//         if (IsKeyPressed(KEY_ENTER) && dialogueCharCount > 0) {
//             SceneNode *scene = (SceneNode*)malloc(sizeof(SceneNode));
//             scene->backgroundTex = assets[bgIndex].background;
//             scene->characterTex = assets[spriteIndex].sprite;
//             scene->dialogue = strdup(dialogue);
//             scene->backgroundPath = NULL;
//             scene->characterPath = NULL;
//             scene->next = NULL;

//             // Append to list
//             if (currentNode.sceneList == NULL) {
//                 currentNode.sceneList = scene;
//                 lastScene = scene;
//             } else {
//                 lastScene->next = scene;
//                 lastScene = scene;
//             }

//             state = 2; // go to branching or continue choice
//         }

//     } else if (state == 2) {
//         // === Next Step Choice ===
//         DrawText("What next?", GetScreenWidth() / 2 - 80, 60, 20, GREEN);
//         DrawText("Press N: Add another scene in sequence", 100, 120, 20, LIGHTGRAY);
//         DrawText("Press A: Branch LEFT from here (Option A)", 100, 160, 20, LIGHTGRAY);
//         DrawText("Press D: Branch RIGHT from here (Option B)", 100, 200, 20, LIGHTGRAY);

//         if (IsKeyPressed(KEY_N)) {
//             state = 0; // back to new scene in same node
//         }

//         if (IsKeyPressed(KEY_A)) {
//             if (!currentNode.leftChoice) {
//                 currentNode.leftChoice = (VNTreeNode *)calloc(1, sizeof(VNTreeNode));
//                 currentNode.leftChoice->id = currentNode.id * 2;
//             }
//             *currentNodePtr = *currentNode.leftChoice;
//             lastScene = NULL;
//             state = 0;
//         }

//         if (IsKeyPressed(KEY_D)) {
//             if (!currentNode.rightChoice) {
//                 currentNode.rightChoice = (VNTreeNode *)calloc(1, sizeof(VNTreeNode));
//                 currentNode.rightChoice->id = currentNode.id * 2 + 1;
//             }
//             *currentNodePtr = *currentNode.rightChoice;
//             lastScene = NULL;
//             state = 0;
//         }
//     }
//     UnloadAssetsSimple(assets);
// }





// void LoadAssetsSimple(AssetLibraryArr assets) {
//     char bgPath[128];
//     char spritePath[128];

//     for (int i = 0; i < 2; i++)
//     {
//         sprintf(bgPath, "Assets/BackgroundSprites/background%d.png", i + 1);
//         sprintf(spritePath, "Assets/CharaSprites/chara%d.png", i + 1);

//         assets[i].background = LoadTexture(bgPath);
//         assets[i].sprite = LoadTexture(spritePath);
//     }
//     // assets->background = LoadTexture("Assets/BackgroundSprites/background1.png");
//     // assets->sprite = LoadTexture("Assets/CharaSprites/chara1.png");
// }

// void UnloadAssetsSimple(AssetLibraryArr assets) {
//     char bgPath[128];
//     char spritePath[128];

//     for (int i = 0; i < 2; i++)
//     {
//         sprintf(bgPath, "Assets/BackgroundSprites/background%d.png", i + 1);
//         sprintf(spritePath, "Assets/CharaSprites/chara%d.png", i + 1);

//         UnloadTexture(assets[i].background);
//         UnloadTexture(assets[i].sprite);
//     }
// }

// void PrintTree(VNTreeNode *node) {
//     if (node == NULL) return;

//     printf("Node ID: %d\n", node->id);

//     SceneNode *scene = node->sceneList;
//     int sceneIndex = 1;

//     while (scene != NULL) {
//         printf("  Scene %d:\n", sceneIndex++);
        
//         if (scene->backgroundPath != NULL)
//             printf("    Background: %s\n", scene->backgroundPath);
//         else
//             printf("    Background: [Texture loaded, no path stored]\n");

//         printf("    Dialogue: %s\n", scene->dialogue);

//         if (scene->characterPath != NULL)
//             printf("    Character: %s\n", scene->characterPath);
//         else
//             printf("    Character: [Texture loaded, no path stored]\n");

//         scene = scene->next;
//     }

//     if (node->leftChoice != NULL) {
//         printf("  Branch LEFT:\n");
//         PrintTree(node->leftChoice);
//     }

//     if (node->rightChoice != NULL) {
//         printf("  Branch RIGHT:\n");
//         PrintTree(node->rightChoice);
//     }
// }
