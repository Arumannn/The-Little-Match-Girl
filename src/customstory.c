#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "customstory.h"

CharaArr FileChara[CHARA_AMMOUNT];
BackgroundArr FileBackground[BACKGROUND_AMMOUNT];
float RenderTimer = 2.0f;
bool ShowRender;

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
        if (ShowRender && currentsprite == 1)
        {
            DrawText("You already are in the earliest Node Possible!", 40, 30, 30, WHITE);
        }
        else if (ShowRender && currentsprite == 2)
        {
            DrawText("You already are in the earliest Scene Possible!", 40, 30, 30, WHITE);
        }
        else if (!ShowRender)
        {
            DrawText("Pick where to go next:", 40, 40, 30, WHITE);
            DrawText("<--: Left child | -->: Right child | ↑: Parent | ↓: Add Scene | BACKSPACE: Previous Scene  | ENTER: Save", 40, 80, 25, WHITE);
        }
        break;
    }
}

void MakeCustomStory(CustomSceneTree *ThisSlot)
{
    ShowRender = false;
    char Convo[128];
    int selectedsprite = 0;
    int control = CHOOSINGBACKGROUND;

    *ThisSlot = (CustomSceneTree)malloc(sizeof(struct Tree));
    (*ThisSlot)->Parent = NULL;
    (*ThisSlot)->Left = NULL;
    (*ThisSlot)->Right = NULL;
    (*ThisSlot)->ID = rand() % 100000000;
    (*ThisSlot)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
    (*ThisSlot)->NodeContents->Next = NULL;
    (*ThisSlot)->NodeContents->Before = NULL;

    CustomSceneTree TempTree = *ThisSlot;
    SceneList TempScene = TempTree->NodeContents;
    TempScene->Data.Background = NULL;
    TempScene->Data.Character = NULL;
    TempScene->Data.Convo = NULL;
    
    while (control != ALLDONE)
    {
        if (control == CHOOSINGBACKGROUND)
        {
            selectedsprite = 0;
            while (control == CHOOSINGBACKGROUND)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                CustomStoryGUI(control, selectedsprite, NULL);
                if (IsKeyPressed(KEY_ENTER))
                {
                    char TempFileName[128];
                    sprintf(TempFileName, "Assets/BackgroundSprites/background%i.png", selectedsprite+1);
                    TempScene->Data.Background = malloc(strlen(TempFileName) + 1);
                    strcpy(TempScene->Data.Background, TempFileName);
                    control = CHOOSINGCHARA;
                }
                else if (IsKeyPressed(KEY_LEFT))
                {
                    if (selectedsprite > 0)
                    {
                        selectedsprite--;
                    }
                }
                else if (IsKeyPressed(KEY_RIGHT))
                {
                    if (selectedsprite < BACKGROUND_AMMOUNT-1)
                    {
                        selectedsprite++;
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
                if (IsKeyPressed(KEY_ENTER))
                {
                    char TempFileName[128];
                    sprintf(TempFileName, "Assets/CharaSprites/chara%i.png", selectedsprite+1);
                    TempScene->Data.Character = malloc(strlen(TempFileName) + 1);
                    strcpy(TempScene->Data.Character, TempFileName);
                    control = CHOOSINGDIALOGUE;
                }
                else if (IsKeyPressed(KEY_LEFT))
                {
                    if (selectedsprite > 0)
                    {
                        selectedsprite--;
                    }
                }
                else if (IsKeyPressed(KEY_RIGHT))
                {
                    if (selectedsprite < CHARA_AMMOUNT - 1)
                    {
                        selectedsprite++;
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
                if (IsKeyPressed(KEY_ENTER))
                {
                    TempScene->Data.Convo = malloc(strlen(Convo) + 1);
                    strcpy(TempScene->Data.Convo, Convo);
                    control = CONFIRMATION;
                }
                CustomStoryGUI(control, selectedsprite, Convo);
                EndDrawing();
            }
        }
        else if (control == CONFIRMATION)
        {
            selectedsprite = 0;
            while (control == CONFIRMATION)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                if (ShowRender)
                {
                    CustomStoryGUI(control, selectedsprite, NULL);
                    RenderTimer -= GetFrameTime();
                    if (RenderTimer <= 0.0f)
                    {
                        ShowRender = false;
                    }
                }
                else
                {
                    CustomStoryGUI(control, 0, NULL);
                }
                if (IsKeyPressed(KEY_LEFT))
                {
                    TempTree->Left = (CustomSceneTree)malloc(sizeof(struct Tree));
                    TempTree->Left->Parent = TempTree;
                    TempTree = TempTree->Left;
                    TempTree->ID = rand() % 100000000;
                    TempTree->Left = NULL;
                    TempTree->Right = NULL;
                    TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                    TempScene = TempTree->NodeContents;
                    TempScene->Next = NULL;
                    TempScene->Before = NULL;
                    TempScene->Data.Background = NULL;
                    TempScene->Data.Character = NULL;
                    TempScene->Data.Convo = NULL;
                    control = CHOOSINGBACKGROUND;
                }
                else if(IsKeyPressed(KEY_RIGHT))
                {
                    TempTree->Right = (CustomSceneTree)malloc(sizeof(struct Tree));
                    TempTree->Right->Parent = TempTree;
                    TempTree = TempTree->Right;
                    TempTree->ID = rand() % 100000000;
                    TempTree->Left = NULL;
                    TempTree->Right = NULL;
                    TempTree->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                    TempScene = TempTree->NodeContents;
                    TempScene->Next = NULL;
                    TempScene->Before = NULL;
                    TempScene->Data.Background = NULL;
                    TempScene->Data.Character = NULL;
                    TempScene->Data.Convo = NULL;
                    control = CHOOSINGBACKGROUND;
                }
                else if(IsKeyPressed(KEY_UP))
                {
                    if (TempTree->Parent == NULL)
                    {
                        ShowRender = true;
                        selectedsprite = 1;
                        RenderTimer = 2.0f;
                    }
                    else
                    {
                        TempTree = TempTree->Parent;
                        TempScene = TempTree->NodeContents;
                        control = CHOOSINGBACKGROUND;
                    }
                }
                else if(IsKeyPressed(KEY_DOWN))
                {
                    TempScene->Next = (SceneList)malloc(sizeof(struct ListElements));
                    TempScene->Next->Before = TempScene;
                    TempScene = TempScene->Next;
                    TempScene->Next = NULL;
                    TempScene->Data.Background = NULL;
                    TempScene->Data.Character = NULL;
                    TempScene->Data.Convo = NULL;
                    control = CHOOSINGBACKGROUND;
                }
                else if (IsKeyPressed(KEY_ENTER))
                {
                    SaveSlot(ThisSlot);
                    control = ALLDONE;
                }
                else if (IsKeyPressed(KEY_BACKSPACE))
                {
                    if (TempScene->Before == NULL)
                    {
                        ShowRender = true;
                        selectedsprite = 2;
                        RenderTimer = 2.0f;
                    }
                    else
                    {
                        TempScene = TempScene->Before;
                        control = CHOOSINGBACKGROUND;
                    }
                }
                EndDrawing();
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
    CustomSceneTree root = *ThisSlot;
    if (!root) 
    {
        printf("Tree is empty.\n");
        return;
    }


    printf("===== DEBUG TREE LOG START =====\n");

    // Define queue inside this function
    typedef struct TempQueueNode {
        CustomSceneTree treeNode;
        int level;
        struct TempQueueNode *next;
    } TempQueueNode;

    TempQueueNode *front = NULL, *rear = NULL;

    // Enqueue function
    auto void Enqueue(CustomSceneTree treeNode, int level) {
        TempQueueNode *newNode = (TempQueueNode *)malloc(sizeof(TempQueueNode));
        newNode->treeNode = treeNode;
        newNode->level = level;
        newNode->next = NULL;
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // Dequeue function
    auto TempQueueNode *Dequeue() {
        if (!front) return NULL;
        TempQueueNode *temp = front;
        front = front->next;
        if (!front) rear = NULL;
        return temp;
    }

    Enqueue(root, 0);

    while (front) {
        TempQueueNode *qNode = Dequeue();
        CustomSceneTree current = qNode->treeNode;
        int level = qNode->level;

        printf("---- Tree Node ID: %d | Level: %d ----\n", current->ID, level);

        SceneList scene = current->NodeContents;
        int sceneIndex = 1;

        while (scene) {
            printf("   Scene %d:\n", sceneIndex);
            printf("     Background: %s\n", scene->Data.Background ? scene->Data.Background : "(none)");
            printf("     Character : %s\n", scene->Data.Character ? scene->Data.Character : "(none)");
            printf("     Dialogue  : %s\n", scene->Data.Convo ? scene->Data.Convo : "(none)");
            scene = scene->Next;
            sceneIndex++;
        }

        if (sceneIndex == 1) {
            printf("   [No scenes in this node]\n");
        }

        if (current->Left)
            Enqueue(current->Left, level + 1);
        if (current->Right)
            Enqueue(current->Right, level + 1);

        free(qNode);
    }

    printf("===== DEBUG TREE LOG END =====\n\n");

}



void LoadSlot(CustomSceneTree *ThisSlot)
{

}

