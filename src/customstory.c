#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "customstory.h"

CharaArr FileChara[CHARA_AMMOUNT];
BackgroundArr FileBackground[BACKGROUND_AMMOUNT];
AudioArr FileAudio[AUDIO_AMMOUNT];
Texture2D Background, Character;
float RenderTimer = 2.0f;
bool ShowRender;

void CustomStoryGUI(int state, int currentsprite, char *Dialogue, char *BackSprite, char *CharaSprite) {
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
            else if (!ShowRender && currentsprite < 3)
            {
                DrawText("Pick where to go next:", 40, 40, 30, WHITE);
                DrawText("<--: Left child | -->: Right child | ↑: Parent | ↓: Add Scene | BACKSPACE: Delete The Latest Scene | ENTER: Save", 40, 80, 25, WHITE);
            }
            else if (currentsprite == 3) //FOR TREE
            {
                DrawText("WARNING: You already have a story tree node here!", 40, 30, 30, WHITE);
                DrawText("<-- Review | / : Delete Tree Node | BACKSPACE: Delete Latest Scene | Overwrite -->", 40, 80, 25, WHITE);
            }
            else if (currentsprite == 4) // FOR LINKEDLIST
            {
                DrawText("WARNING: You already have a scene here!", 40, 30, 30, WHITE);
                DrawText("<-- Review | / : Deletes | Overwrite -->", 40, 80, 25, WHITE);
            }
            else if (currentsprite == 5)
            {
                DrawText("Scene successfully deleted!", 40, 30, 30, WHITE);
            }
            break;
        case MODE_REVIEW_SCENE:
            Background = LoadTexture(BackSprite);
            Character = LoadTexture(CharaSprite);
            position.x = GetScreenWidth() - Background.width * 0.75f / 2.0f;
            position.y = GetScreenHeight()- Background.height * 0.75f / 2.0f;
            DrawTextureEx(Background, position, 0, 0.75, WHITE);
            position.x = GetScreenWidth()/2.0f - Character.width * 0.75f / 2.0f;
            position.y = GetScreenHeight()/2.0f - Character.height * 0.75f / 2.0f;
            DrawTextureEx(Character, position, 0, 0.75f, WHITE);
            DrawText(Dialogue, position.x, position.y-50, 20, WHITE);
            DrawText("Press <-- to go to previous scene, and press --> to go to the next scene", position.x, position.y-80, 20, WHITE);
            // UnloadTexture(Background);
            // UnloadTexture(Character);
            break;
    }
}

void MakeCustomStory(CustomSceneTree *ThisSlot)
{
    ShowRender = false;
    char Convo[128];
    int selectedsprite = 0;
    int control = CHOOSINGBACKGROUND;

    InitializeStoryTree(ThisSlot);

    CustomSceneTree TempTree = *ThisSlot;
    SceneList TempScene = TempTree->NodeContents;
    TempScene->Data.Background = NULL;
    TempScene->Data.Character = NULL;
    TempScene->Data.Convo = NULL;
    
    while (control != ALLDONE)
    {
        switch (control)
        {
            case CHOOSINGBACKGROUND:
                ChoosingBackground(&selectedsprite, &control, &TempScene);
            break;

            case CHOOSINGCHARA:
                ChoosingChara(&selectedsprite, &control, &TempScene);
            break;

            case CHOOSINGDIALOGUE:
                ChoosingDialogue(Convo, &selectedsprite, &control, &TempScene);
            break;

            case CONFIRMATION:
                bool warning = false;
                selectedsprite = 0;
                int KeyPad;
                while (control == CONFIRMATION)
                {
                    KeyPad = GetKeyPressed();
                    BeginDrawing();
                    ClearBackground(BLACK);
                    if (ShowRender)
                    {
                        CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL);
                        RenderTimer -= GetFrameTime();
                        if (RenderTimer <= 0.0f)
                        {
                            ShowRender = false;
                        }
                    }
                    else
                    {
                        CustomStoryGUI(control, 0, NULL, NULL, NULL);
                    }
                    switch (KeyPad)
                    {
                        case KEY_LEFT:
                            AddLeftChild(&TempTree, &TempScene, &warning, &control);
                        break;

                        case KEY_RIGHT:
                            AddRightChild(&TempTree, &TempScene, &warning, &control);
                        break;

                        case KEY_UP:
                            EndDrawing();
                            PreviousTree(&TempTree, &TempScene, &selectedsprite, &warning, &control);
                        break;

                        case KEY_DOWN:
                            AddNewScene(&TempScene, &warning, &control);
                        break;

                        case KEY_ENTER:
                            SaveSlot(ThisSlot);
                            control = ALLDONE;
                        break;

                        case KEY_BACKSPACE:
                            PreviousScene(&TempTree, &TempScene, &selectedsprite, &warning, &control);
                        break;
                    }
                    EndDrawing();
                }
            break;

            case MODE_REVIEW_SCENE:
                ReviewScene(&control, &TempScene);
            break;
        }
    }
    EndDrawing();
}

void ChoosingDialogue(char *Convo, int *selectedsprite, int *control, SceneList *TempScene)
{
    int letterCount = 0;
    int key = 0;
    memset(Convo, 0, sizeof(Convo));
    while (*control == CHOOSINGDIALOGUE)
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
            (*TempScene)->Data.Convo = malloc(strlen(Convo) + 1);
            strcpy((*TempScene)->Data.Convo, Convo);
            *control = CONFIRMATION;
        }
        CustomStoryGUI(*control, *selectedsprite, Convo, NULL, NULL);
        EndDrawing();
    }
}

void ChoosingChara(int *selectedsprite, int *control, SceneList *TempScene)
{
    *selectedsprite = 0;
    while ((*control) == CHOOSINGCHARA)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL);
        if (IsKeyPressed(KEY_ENTER))
        {
            char TempFileName[128];
            sprintf(TempFileName, "Assets/CharaSprites/chara%i.png", (*selectedsprite)+1);
            (*TempScene)->Data.Character = malloc(strlen(TempFileName) + 1);
            strcpy((*TempScene)->Data.Character, TempFileName);
            *control = CHOOSINGDIALOGUE;
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            if (*selectedsprite > 0)
            {
                (*selectedsprite)--;
            }
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            if (*selectedsprite < CHARA_AMMOUNT - 1)
            {
                (*selectedsprite)++;
            }
        }
        EndDrawing();
    }
}

void ChoosingBackground(int *selectedsprite, int *control, SceneList *TempScene)
{
    *selectedsprite = 0;
    while (*control == CHOOSINGBACKGROUND)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL);
        if (IsKeyPressed(KEY_ENTER))
        {
            char TempFileName[128];
            sprintf(TempFileName, "Assets/BackgroundSprites/background%i.png", (*selectedsprite)+1);
            (*TempScene)->Data.Background = malloc(strlen(TempFileName) + 1);
            strcpy((*TempScene)->Data.Background, TempFileName);
            *control = CHOOSINGCHARA;
        }
        else if (IsKeyPressed(KEY_LEFT))
        {
            if (*selectedsprite > 0)
            {
                (*selectedsprite)--;
            }
        }
        else if (IsKeyPressed(KEY_RIGHT))
        {
            if (*selectedsprite < BACKGROUND_AMMOUNT-1)
            {
                (*selectedsprite)++;
            }
        }
        EndDrawing();
    }
}

void AddNewScene(SceneList *TempScene, bool *warning, int *control)
{
    if ((*TempScene)->Next == NULL)
    {
        (*TempScene)->Next = (SceneList)malloc(sizeof(struct ListElements));
        (*TempScene)->Next->Before = (*TempScene);
        (*TempScene) = (*TempScene)->Next;
        (*TempScene)->Next = NULL;
        (*TempScene)->Data.Background = NULL;
        (*TempScene)->Data.Character = NULL;
        (*TempScene)->Data.Convo = NULL;
        *control = CHOOSINGBACKGROUND;
    }
    else
    {
        *warning = true;
        while (*warning == true)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, 4, NULL, NULL, NULL);
            if (IsKeyPressed(KEY_RIGHT))
            {
                if ((*TempScene)->Next->Data.Background) free((*TempScene)->Next->Data.Background);
                if ((*TempScene)->Next->Data.Character) free((*TempScene)->Next->Data.Character);
                if ((*TempScene)->Next->Data.Convo) free((*TempScene)->Next->Data.Convo);
                (*TempScene)->Next->Data.Background = NULL;
                (*TempScene)->Next->Data.Character = NULL;
                (*TempScene)->Next->Data.Convo = NULL;
                *warning = false;
                (*TempScene) = (*TempScene)->Next;
                *control = CHOOSINGBACKGROUND;
            }
            else if (IsKeyPressed(KEY_LEFT))
            {
                *control = MODE_REVIEW_SCENE;
                (*TempScene) = (*TempScene)->Next;
                *warning = false;
            }
            EndDrawing();
        }
    }
}

void PreviousTree(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control)
{
    if ((*TempTree)->Parent == NULL)
    {
        ShowRender = true;
        *selectedsprite = 1;
        RenderTimer = 2.0f;
    }
    else
    {
        *warning = true;
        (*TempTree) = (*TempTree)->Parent;
        (*TempScene) = (*TempTree)->NodeContents;
        while (*warning)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, 3, NULL, NULL, NULL);
            if (IsKeyPressed(KEY_RIGHT))
            {
                SceneList toDelete = (*TempScene);
                SceneList next;
                while (toDelete != NULL)
                {
                    next = toDelete->Next;

                    if (toDelete->Data.Background) free(toDelete->Data.Background);
                    if (toDelete->Data.Character) free(toDelete->Data.Character);
                    if (toDelete->Data.Convo) free(toDelete->Data.Convo);
                    free(toDelete);
                    toDelete = next;
                }
                (*TempTree)->NodeContents = (SceneList)calloc(1, sizeof(struct ListElements));
                (*TempScene) = (*TempTree)->NodeContents;
                (*TempScene)->Next = NULL;
                (*TempScene)->Before = NULL;
                (*TempScene)->Data.Background = NULL;
                (*TempScene)->Data.Character = NULL;
                (*TempScene)->Data.Convo = NULL;
                *control = CHOOSINGBACKGROUND;
                *warning = false;
            }
            else if (IsKeyPressed(KEY_LEFT) && (*TempScene) != NULL)
            {
                *control = MODE_REVIEW_SCENE;
                *warning = false;
                CustomStoryGUI(*control, 0, (*TempScene)->Data.Convo, (*TempScene)->Data.Background, (*TempScene)->Data.Character);
            }
            else if (IsKeyPressed(KEY_SLASH))
            {
                SceneList DeleteTemp = (*TempScene);
                if (DeleteTemp->Before != NULL)
                {
                    DeleteTemp->Before->Next = DeleteTemp->Next;
                }
                if (DeleteTemp->Next != NULL)
                {
                    DeleteTemp->Next->Before = DeleteTemp->Before;
                }
                if (DeleteTemp->Before != NULL)
                {
                    (*TempScene) = (*TempScene)->Before;
                }
                else if (DeleteTemp->Next != NULL)
                {
                    (*TempScene) = (*TempScene)->Next;
                }
                else
                {
                    (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                    (*TempScene) = (*TempTree)->NodeContents;
                    (*TempScene)->Data.Background = NULL;
                    (*TempScene)->Data.Character = NULL;
                    (*TempScene)->Data.Convo = NULL;
                    (*TempScene)->Next = NULL;
                    (*TempScene)->Before = NULL;
                }
                if (DeleteTemp->Data.Background) free(DeleteTemp->Data.Background);
                if (DeleteTemp->Data.Character) free(DeleteTemp->Data.Character);
                if (DeleteTemp->Data.Convo) free(DeleteTemp->Data.Convo);
                
                DeleteTemp = (*TempTree)->NodeContents;

                while (DeleteTemp->Next != NULL)
                {
                    DeleteTemp = DeleteTemp->Next;
                }
                DeleteTemp->Before->Next = NULL;
                if (DeleteTemp->Data.Background) free(DeleteTemp->Data.Background);
                if (DeleteTemp->Data.Character) free(DeleteTemp->Data.Character);
                if (DeleteTemp->Data.Convo) free(DeleteTemp->Data.Convo);
                free(DeleteTemp);
                *warning = false;
                ShowRender = true;
                *selectedsprite = 5;
                RenderTimer = 2.0f;
                CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL);
            }
            EndDrawing();
        }
    }
}

void AddRightChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control)
{
    if ((*TempTree)->Right == NULL)
    {
        (*TempTree)->Right = (CustomSceneTree)malloc(sizeof(struct Tree));
        (*TempTree)->Right->Parent = (*TempTree);
        (*TempTree) = (*TempTree)->Right;
        (*TempTree)->ID = rand() % 100000000;
        (*TempTree)->Left = NULL;
        (*TempTree)->Right = NULL;
        (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
        (*TempScene) = (*TempTree)->NodeContents;
        (*TempScene)->Next = NULL;
        (*TempScene)->Before = NULL;
        (*TempScene)->Data.Background = NULL;
        (*TempScene)->Data.Character = NULL;
        (*TempScene)->Data.Convo = NULL;
        *control = CHOOSINGBACKGROUND;
    }
    else
    {
        *warning = true;
        while(*warning)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, 3, NULL, NULL, NULL);
            if (IsKeyPressed(KEY_RIGHT))
            {
                (*TempTree) = (*TempTree)->Right;
                (*TempScene) = (*TempTree)->NodeContents;
                SceneList toDelete = (*TempScene);
                SceneList next;
                while (toDelete != NULL)
                {
                    next = toDelete->Next;

                    if (toDelete->Data.Background) free(toDelete->Data.Background);
                    if (toDelete->Data.Character) free(toDelete->Data.Character);
                    if (toDelete->Data.Convo) free(toDelete->Data.Convo);
                    free(toDelete);
                    toDelete = next;
                }
                (*TempTree)->NodeContents = (SceneList)calloc(1, sizeof(struct ListElements));
                (*TempScene) = (*TempTree)->NodeContents;
                (*TempScene)->Next = NULL;
                (*TempScene)->Before = NULL;
                (*TempScene)->Data.Background = NULL;
                (*TempScene)->Data.Character = NULL;
                (*TempScene)->Data.Convo = NULL;
                *control = CHOOSINGBACKGROUND;
                *warning = false;
            }
            else if (IsKeyPressed(KEY_LEFT))
            {
                *warning = false;
                (*TempTree) = (*TempTree)->Right;
            }
            EndDrawing();
        }
    }
}

void AddLeftChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control)
{
    if ((*TempTree)->Left == NULL)
    {
        (*TempTree)->Left = (CustomSceneTree)malloc(sizeof(struct Tree));
        (*TempTree)->Left->Parent = (*TempTree);
        (*TempTree) = (*TempTree)->Left;
        (*TempTree)->ID = rand() % 100000000;
        (*TempTree)->Left = NULL;
        (*TempTree)->Right = NULL;
        (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
        (*TempScene) = (*TempTree)->NodeContents;
        (*TempScene)->Next = NULL;
        (*TempScene)->Before = NULL;
        (*TempScene)->Data.Background = NULL;
        (*TempScene)->Data.Character = NULL;
        (*TempScene)->Data.Convo = NULL;
        *control = CHOOSINGBACKGROUND;
    }
    else
    {
        *warning = true;
        while(*warning)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, 3, NULL, NULL, NULL);
            if (IsKeyPressed(KEY_RIGHT))
            {
                (*TempTree) = (*TempTree)->Left;
                (*TempScene) = (*TempTree)->NodeContents;
                SceneList toDelete = (*TempScene);
                SceneList next;
                while (toDelete != NULL)
                {
                    next = toDelete->Next;

                    if (toDelete->Data.Background) free(toDelete->Data.Background);
                    if (toDelete->Data.Character) free(toDelete->Data.Character);
                    if (toDelete->Data.Convo) free(toDelete->Data.Convo);
                    free(toDelete);
                    toDelete = next;
                }
                (*TempTree)->NodeContents = (SceneList)calloc(1, sizeof(struct ListElements));
                (*TempScene) = (*TempTree)->NodeContents;
                (*TempScene)->Next = NULL;
                (*TempScene)->Before = NULL;
                (*TempScene)->Data.Background = NULL;
                (*TempScene)->Data.Character = NULL;
                (*TempScene)->Data.Convo = NULL;
                *control = CHOOSINGBACKGROUND;
                *warning = false;
            }
            else if (IsKeyPressed(KEY_LEFT))
            {
                *warning = false;
                (*TempTree) = (*TempTree)->Left;
            }
            else if (IsKeyPressed(KEY_BACKSPACE))
            {
                SceneList DeleteNode = (*TempTree)->NodeContents;
                while (DeleteNode->Next != NULL)
                {
                    DeleteNode = DeleteNode->Next;
                }
                if (DeleteNode->Data.Background) free(DeleteNode->Data.Background);
                if (DeleteNode->Data.Character) free(DeleteNode->Data.Character);
                if (DeleteNode->Data.Convo) free(DeleteNode->Data.Convo);
                free(DeleteNode);

            }
            EndDrawing();
        }
    }
}

void InitializeStoryTree(CustomSceneTree *ThisSlot)
{
    *ThisSlot = (CustomSceneTree)malloc(sizeof(struct Tree));
    (*ThisSlot)->Parent = NULL;
    (*ThisSlot)->Left = NULL;
    (*ThisSlot)->Right = NULL;
    (*ThisSlot)->ID = rand() % 100000000;
    (*ThisSlot)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
    (*ThisSlot)->NodeContents->Next = NULL;
    (*ThisSlot)->NodeContents->Before = NULL;
    (*ThisSlot)->NodeContents->Data.Background = NULL;
    (*ThisSlot)->NodeContents->Data.Character = NULL;
    (*ThisSlot)->NodeContents->Data.Convo = NULL;
}

void PreviousScene(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control)
{
    if ((*TempScene)->Before == NULL)
    {
        ShowRender = true;
        *selectedsprite = 2;
        RenderTimer = 2.0f;
    }
    else
    {
        *warning = true;
        (*TempScene) = (*TempScene)->Before;
        while (*warning)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, 4, NULL, NULL, NULL);
            if (IsKeyPressed(KEY_RIGHT))
            {
                if ((*TempScene)->Data.Background) free((*TempScene)->Data.Background);
                if ((*TempScene)->Data.Character) free((*TempScene)->Data.Character);
                if ((*TempScene)->Data.Convo) free((*TempScene)->Data.Convo);
                (*TempScene)->Data.Background = NULL;
                (*TempScene)->Data.Character = NULL;
                (*TempScene)->Data.Convo = NULL;
                *warning = false;
                *control = CHOOSINGBACKGROUND;
            }
            else if (IsKeyPressed(KEY_LEFT))
            {
                *control = MODE_REVIEW_SCENE;
                *warning = false;
                CustomStoryGUI(*control, 0, (*TempScene)->Data.Convo, (*TempScene)->Data.Background, (*TempScene)->Data.Character);
            }
            else if (IsKeyPressed(KEY_SLASH))
            {
                SceneList DeleteTemp = (*TempScene);
                if (DeleteTemp->Before != NULL)
                {
                    DeleteTemp->Before->Next = DeleteTemp->Next;
                }
                if (DeleteTemp->Next != NULL)
                {
                    DeleteTemp->Next->Before = DeleteTemp->Before;
                }
                if (DeleteTemp->Before != NULL)
                {
                    (*TempScene) = (*TempScene)->Before;
                }
                else if (DeleteTemp->Next != NULL)
                {
                    (*TempScene) = (*TempScene)->Next;
                }
                else
                {
                    (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
                    (*TempScene) = (*TempTree)->NodeContents;
                    (*TempScene)->Data.Background = NULL;
                    (*TempScene)->Data.Character = NULL;
                    (*TempScene)->Data.Convo = NULL;
                    (*TempScene)->Next = NULL;
                    (*TempScene)->Before = NULL;
                }
                if (DeleteTemp->Data.Background) free(DeleteTemp->Data.Background);
                if (DeleteTemp->Data.Character) free(DeleteTemp->Data.Character);
                if (DeleteTemp->Data.Convo) free(DeleteTemp->Data.Convo);
                free(DeleteTemp);
                *warning = false;
                ShowRender = true;
                *selectedsprite = 5;
                RenderTimer = 2.0f;
                CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL);
            }
            EndDrawing();
        }
    }
}

void ReviewScene(int *control, SceneList *TempScene)
{
    char LinkBack[128];
    char LinkChara[128];
    char LinkAudio[128];
    char LinkConvo[128];
    while (*control == MODE_REVIEW_SCENE)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        sprintf(LinkBack, (*TempScene)->Data.Background);
        sprintf(LinkChara, (*TempScene)->Data.Character);
        sprintf(LinkConvo, (*TempScene)->Data.Convo);
        CustomStoryGUI(*control, 0, LinkConvo, LinkBack, LinkChara);
        if (IsKeyPressed(KEY_RIGHT) && (*TempScene)->Next != NULL)
        {
            (*TempScene) = (*TempScene)->Next;
        }
        else if(IsKeyPressed(KEY_LEFT) && (*TempScene)->Before != NULL)
        {
            (*TempScene) = (*TempScene)->Before;
        }
        else if(IsKeyPressed(KEY_ENTER))
        {
            *control = CONFIRMATION;
        }
        EndDrawing();
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

