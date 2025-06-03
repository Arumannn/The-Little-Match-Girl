#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customstory.h"

CharaArr FileChara[CHARA_AMMOUNT];
BackgroundArr FileBackground[BACKGROUND_AMMOUNT];

void CustomStoryGUI(int state, int currentsprite, char *Dialogue) {
    char buffer[128];
    Vector2 position;
    switch (state) {
        case CHOOSINGBACKGROUND:
            sprintf(buffer, "Background %i of %i", currentsprite+1, BACKGROUND_AMMOUNT);
            position.x = GetScreenWidth()/2.0f - FileBackground[currentsprite].Background.width * 0.75f / 2.0f;
            position.y = GetScreenHeight()/2.0f - FileBackground[currentsprite].Background.height * 0.75f / 2.0f;
            DrawTextureEx(FileBackground[currentsprite].Background, position, 0, 0.75f, WHITE);
            DrawText(buffer, position.x, position.y-50, 20, WHITE);
            break;

        case CHOOSINGCHARA:
            sprintf(buffer, "Character %i of %i", currentsprite+1, CHARA_AMMOUNT);
            position.x = GetScreenWidth()/2.0f - FileChara[currentsprite].Chara.width * 0.75f / 2.0f;
            position.y = GetScreenHeight()/2.0f - FileChara[currentsprite].Chara.height * 0.75f / 2.0f;
            DrawTextureEx(FileChara[currentsprite].Chara, position, 0, 0.75f, WHITE);
            DrawText(buffer, position.x, position.y-50, 20, WHITE);
            break;

        case CHOOSINGDIALOGUE:
            DrawText("Type your dialogue and press ENTER", 40, 30, 30, WHITE);
            DrawRectangleLines(40, 80, 720, 40, WHITE);
            DrawText(Dialogue, 50, 85, 20, LIGHTGRAY);
            break;

        case CONFIRMATION:
            DrawText("Pick where to go next:", 40, 40, 30, WHITE);
            DrawText("←: Left child | →: Right child | ↑: Parent | ↓: Add node | ENTER: Save", 40, 80, 25, WHITE);
            break;
    }
}


void MakeCustomStory(CustomSceneTree *ThisSlot)
{
    char Convo[128];
    int selectedsprite = 0;
    //char inputtext[30];
    int control = CHOOSINGBACKGROUND;

    *ThisSlot = (CustomSceneTree)malloc(sizeof(struct Tree));
    (*ThisSlot)->Parent = NULL;
    (*ThisSlot)->Left = NULL;
    (*ThisSlot)->Right = NULL;
    (*ThisSlot)->ID = rand() % 100000000;
    (*ThisSlot)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
    (*ThisSlot)->NodeContents->Next = NULL;

    CustomSceneTree TempTree = *ThisSlot;

    while (control != ALLDONE)
    {
        if (control == CHOOSINGBACKGROUND)
        {
            selectedsprite = 0;
            while (control == CHOOSINGBACKGROUND)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                //selectedsprite = 1;
                CustomStoryGUI(control, selectedsprite, NULL);
                if (IsKeyPressed(KEY_ENTER)) //selectedsprite = (selectedsprite + 1) % BACKGROUND_AMMOUNT;
                {
                    char TempFileName[128];
                    sprintf(TempFileName, "Assets/BackgroundSprites/background%i.png", selectedsprite+1);
                    TempTree->NodeContents->Data.Background = malloc(strlen(TempFileName) + 1);
                    strcpy(TempTree->NodeContents->Data.Background, TempFileName);
                    control = CHOOSINGCHARA;
                }
                else if (IsKeyPressed(KEY_LEFT))
                {
                    if (selectedsprite > 0)
                    {
                        selectedsprite--;// = (selectedsprite - 1 + BACKGROUND_AMMOUNT) % BACKGROUND_AMMOUNT;
                    }
                }
                else if (IsKeyPressed(KEY_RIGHT)) //selectedsprite = (selectedsprite + 1) % BACKGROUND_AMMOUNT;
                {
                    if (selectedsprite < BACKGROUND_AMMOUNT-1)
                    {
                        selectedsprite++; //+ 1) % BACKGROUND_AMMOUNT;
                    }
                }
                EndDrawing();
            }
        }

        else if (control == CHOOSINGCHARA)
        {
            selectedsprite = 0;
            while (control == CHOOSINGCHARA)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                CustomStoryGUI(control, selectedsprite, NULL);
                if (IsKeyPressed(KEY_ENTER)) //selectedsprite = (selectedsprite + 1) % BACKGROUND_AMMOUNT;
                {
                    char TempFileName[128];
                    sprintf(TempFileName, "Assets/CharaSprites/chara%i.png", selectedsprite+1);
                    TempTree->NodeContents->Data.Character = malloc(strlen(TempFileName) + 1);
                    strcpy(TempTree->NodeContents->Data.Character, TempFileName);
                    control = CHOOSINGDIALOGUE;
                }
                else if (IsKeyPressed(KEY_LEFT))
                {
                    if (selectedsprite > 0)
                    {
                        selectedsprite--;// = (selectedsprite - 1 + BACKGROUND_AMMOUNT) % BACKGROUND_AMMOUNT;
                    }
                }
                else if (IsKeyPressed(KEY_RIGHT)) //selectedsprite = (selectedsprite + 1) % BACKGROUND_AMMOUNT;
                {
                    if (selectedsprite < CHARA_AMMOUNT - 1)
                    {
                        selectedsprite++; //+ 1) % BACKGROUND_AMMOUNT;
                    }
                }
                EndDrawing();
            }
        }

        else if (control == CHOOSINGDIALOGUE)
        {
            int letterCount = 0;
            int key = 0;
            memset(Convo, 0, sizeof(Convo));
            while (control == CHOOSINGDIALOGUE)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                key = GetCharPressed();
                while (key > 0)
                {
                    if (letterCount < 127 && key >= 32 && key <= 125)
                    {
                        Convo[letterCount++] = (char)key;
                        Convo[letterCount] = '\0';
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    if (letterCount > 0) Convo[--letterCount] = '\0';
                }
                if (IsKeyPressed(KEY_ENTER)) //selectedsprite = (selectedsprite + 1) % BACKGROUND_AMMOUNT;
                {
                    TempTree->NodeContents->Data.Convo = malloc(strlen(Convo) + 1);
                    strcpy(TempTree->NodeContents->Data.Convo, Convo);
                    control = CONFIRMATION;
                }
                CustomStoryGUI(control, selectedsprite, Convo);
                EndDrawing();
            }
        }
        
        if (control == CONFIRMATION)
        {
            CustomStoryGUI(control, selectedsprite, NULL);
            if (IsKeyPressed(KEY_LEFT))
            {
                TempTree->Left = (CustomSceneTree)malloc(sizeof(struct Tree));
                TempTree->Left->Parent = TempTree;
                TempTree = TempTree->Left;
                TempTree->ID = rand() % 100000000;
                TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));;
                control = CHOOSINGBACKGROUND;
            }
            else if(IsKeyPressed(KEY_RIGHT))
            {
                TempTree->Right = (CustomSceneTree)malloc(sizeof(struct Tree));
                TempTree->Right->Parent = TempTree;
                TempTree = TempTree->Right;
                TempTree->ID = rand() % 100000000;
                TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                control = CHOOSINGBACKGROUND;
            }
            else if(IsKeyPressed(KEY_UP))
            {
                if (TempTree->Parent != NULL)
                {
                    TempTree = TempTree->Parent;
                }
                else
                {
                    DrawText("Type your dialogue and press ENTER", 40, 30, 30, WHITE);
                    control = CONFIRMATION;
                }
            }
            else if(IsKeyPressed(KEY_DOWN))
            {
                TempTree->NodeContents->Next = (SceneList)malloc(sizeof(struct ListElements));
                TempTree->NodeContents = TempTree->NodeContents->Next;
                TempTree->NodeContents->Next = NULL;
                control = CHOOSINGBACKGROUND;
            }
            else if (IsKeyPressed(KEY_ENTER))
            {
                SaveSlot(&TempTree);
                control = ALLDONE;
            }
        }
    }
}

void InitiateAssets()
{
    for (int i = 0; i < BACKGROUND_AMMOUNT; i++) 
    {
        char path[128];
        sprintf(path, "Assets/BackgroundSprites/background%i.png", i+1);
        strcpy(FileBackground[i].FileName, path);
        FileBackground[i].Background = LoadTexture(path);
    }
    for (int i = 0; i < CHARA_AMMOUNT; i++)
    {
        char path[128];
        sprintf(path, "Assets/CharaSprites/chara%i.png", i+1);
        strcpy(FileChara[i].FileName, path);
        FileChara[i].Chara = LoadTexture(path);
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

