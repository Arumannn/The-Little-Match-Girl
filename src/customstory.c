#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customstory.h"

void CustomStoryGUI(int state, char Link[128], int currentsprite) {
    char buffer[128];

    switch (state) {
        case CHOOSINGBACKGROUND:
            sprintf(Link, "Assets/BackgroundSprites/background%i.png", currentsprite);
            sprintf(buffer, "Background %i", currentsprite);
            {
                Texture2D bg = LoadTexture(Link);
                DrawTexture(bg, 0, 0, WHITE);
                DrawText(buffer, 30, 30, 30, WHITE);
                UnloadTexture(bg);
            }
            break;

        case CHOOSINGCHARA:
            sprintf(Link, "Assets/CharaSprites/chara%i.png", currentsprite);
            sprintf(buffer, "Character %i", currentsprite);
            {
                Texture2D ch = LoadTexture(Link);
                DrawTexture(ch, GetScreenWidth()/2 - ch.width/2, GetScreenHeight()/2 - ch.height/2, WHITE);
                DrawText(buffer, 30, 30, 30, WHITE);
                UnloadTexture(ch);
            }
            break;

        case CHOOSINGDIALOGUE:
            DrawText("Type your dialogue and press ENTER", 40, 30, 30, WHITE);
            break;

        case CONFIRMATION:
            DrawText("Pick where to go next:", 40, 40, 30, WHITE);
            DrawText("←: Left child | →: Right child | ↑: Parent | ↓: Add node | ENTER: Save", 40, 80, 25, WHITE);
            break;
    }
}


void MakeCustomStory(CustomSceneTree* ThisSlot) {
    int selectedsprite = 1;
    int control = CHOOSINGBACKGROUND;
    char link[128] = "Assets/BackgroundSprites/background1.png";

    *ThisSlot = (CustomSceneTree)malloc(sizeof(struct Tree));
    (*ThisSlot)->Parent = NULL;
    (*ThisSlot)->Left = NULL;
    (*ThisSlot)->Right = NULL;
    (*ThisSlot)->ID = rand() % 100000000;
    (*ThisSlot)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
    (*ThisSlot)->NodeContents->Next = NULL;

    CustomSceneTree TempTree = *ThisSlot;

    char dialogueInput[512] = "";
    int letterCount = 0;

    while (!WindowShouldClose() && control != ALLDONE) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (control == CHOOSINGBACKGROUND) {
            CustomStoryGUI(control, link, selectedsprite);

            if (IsKeyPressed(KEY_ENTER)) {
                sprintf(link, "Assets/BackgroundSprites/background%i.png", selectedsprite);
                strcpy(TempTree->NodeContents->Data.Background, link);
                control = CHOOSINGCHARA;
            } else if (IsKeyPressed(KEY_LEFT) && selectedsprite > 1) {
                selectedsprite--;
            } else if (IsKeyPressed(KEY_RIGHT) && selectedsprite < BACKGROUND_AMMOUNT) {
                selectedsprite++;
            }
        }

        else if (control == CHOOSINGCHARA) {
            CustomStoryGUI(control, link, selectedsprite);

            if (IsKeyPressed(KEY_ENTER)) {
                sprintf(link, "Assets/CharaSprites/chara%i.png", selectedsprite);
                strcpy(TempTree->NodeContents->Data.Character, link);
                control = CHOOSINGDIALOGUE;
            } else if (IsKeyPressed(KEY_LEFT) && selectedsprite > 1) {
                selectedsprite--;
            } else if (IsKeyPressed(KEY_RIGHT) && selectedsprite < CHARA_AMMOUNT) {
                selectedsprite++;
            }
        }

        else if (control == CHOOSINGDIALOGUE) {
            CustomStoryGUI(control, link, selectedsprite);

            DrawRectangle(40, 80, GetScreenWidth() - 80, 60, DARKGRAY);
            DrawText(dialogueInput, 50, 90, 30, WHITE);

            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && letterCount < 511) {
                    dialogueInput[letterCount++] = (char)key;
                    dialogueInput[letterCount] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
                letterCount--;
                dialogueInput[letterCount] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER)) {
                strncpy(TempTree->NodeContents->Data.Convo, dialogueInput, 511);
                TempTree->NodeContents->Data.Convo[511] = '\0';
                dialogueInput[0] = '\0';
                letterCount = 0;
                control = CONFIRMATION;
            }
        }

        else if (control == CONFIRMATION) {
            CustomStoryGUI(control, link, selectedsprite);

            if (IsKeyPressed(KEY_LEFT)) {
                TempTree->Left = (CustomSceneTree)malloc(sizeof(struct Tree));
                TempTree->Left->Parent = TempTree;
                TempTree->Left->Left = NULL;
                TempTree->Left->Right = NULL;
                TempTree = TempTree->Left;
                TempTree->ID = rand() % 100000000;
                TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                TempTree->NodeContents->Next = NULL;
                control = CHOOSINGBACKGROUND;
            } else if (IsKeyPressed(KEY_RIGHT)) {
                TempTree->Right = (CustomSceneTree)malloc(sizeof(struct Tree));
                TempTree->Right->Parent = TempTree;
                TempTree->Right->Left = NULL;
                TempTree->Right->Right = NULL;
                TempTree = TempTree->Right;
                TempTree->ID = rand() % 100000000;
                TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                TempTree->NodeContents->Next = NULL;
                control = CHOOSINGBACKGROUND;
            } else if (IsKeyPressed(KEY_UP)) {
                if (TempTree->Parent) TempTree = TempTree->Parent;
            } else if (IsKeyPressed(KEY_DOWN)) {
                SceneList newNode = (SceneList)malloc(sizeof(struct ListElements));
                newNode->Next = NULL;
                TempTree->NodeContents->Next = newNode;
                TempTree->NodeContents = newNode;
                control = CHOOSINGBACKGROUND;
            } else if (IsKeyPressed(KEY_ENTER)) {
                SaveSlot(&TempTree);
                control = ALLDONE;
            }
        }

        EndDrawing();
    }
}



void SaveSlot(CustomSceneTree *ThisSlot)
{
    // if (node == NULL) {
    // int nullFlag = 0;
    // fwrite(&nullFlag, sizeof(int), 1, file); // Null marker
    // return;
    // }

    // int presentFlag = 1;
    // fwrite(&presentFlag, sizeof(int), 1, file);
    // fwrite(&node->ID, sizeof(int), 1, file);

    // SaveSceneList(node->NodeContents, file);

    // SaveTree(node->Left, file);
    // SaveTree(node->Right, file);
}



void LoadSlot(CustomSceneTree *ThisSlot)
{

}

