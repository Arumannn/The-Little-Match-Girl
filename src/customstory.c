#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "story.h"
#include "customstory.h"

CharaArr FileChara[CHARA_AMMOUNT];
BackgroundArr FileBackground[BACKGROUND_AMMOUNT];
AudioArr FileAudio[AUDIO_AMMOUNT];
Texture2D Background, Character;
float RenderTimer = 2.0f;
bool ShowRender;

// Helper functions for PrintTree
typedef struct TempQueueNode {
    CustomSceneTree treeNode;
    int level;
    struct TempQueueNode *next;
} TempQueueNode;

static void Enqueue(TempQueueNode **front, TempQueueNode **rear, CustomSceneTree treeNode, int level) {
    TempQueueNode *newNode = (TempQueueNode *)malloc(sizeof(TempQueueNode));
    newNode->treeNode = treeNode;
    newNode->level = level;
    newNode->next = NULL;
    if (!*rear) {
        *front = *rear = newNode;
    } else {
        (*rear)->next = newNode;
        *rear = newNode;
    }
}

static TempQueueNode *Dequeue(TempQueueNode **front, TempQueueNode **rear) {
    if (!*front) return NULL;
    TempQueueNode *temp = *front;
    *front = (*front)->next;
    if (!*front) *rear = NULL;
    return temp;
}

void CustomStoryGUI(int state, int currentsprite, char *Dialogue, char *BackSprite, char *CharaSprite, CharacterPosition charPosition) {
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
                DrawText("Where do you want to go next?", 40, 30, 30, WHITE);
                DrawText("<-- Go to Next Left Option | Go to Next Right Option --> | Up_Arrow Go To Parent | / Review Scenes", 40, 40, 30, WHITE);
                DrawText(" (A) Delete Lastest Scene | (S) Delete First Scene | (D) Add Scene As The First Scene | (F) Add Scene Into The Last Scene | (G) Overwrite Tree", 40, 80, 25, WHITE);
            }
            else if (currentsprite == 3)
            {
                DrawText("WARNING: You already have a story tree node here!", 40, 30, 30, WHITE);
                DrawText("<-- Go to Next Left Option | Go to Next Right Option --> | Up_Arrow Go To Parent | / Review Scenes | BACKSPACE Delete Tree", 40, 80, 25, WHITE);
                DrawText(" (A) Delete Lastest Scene | (S) Delete First Scene | (D) Add Scene As The First Scene | (F) Add Scene Into The Last Scene | (G) Overwrite Tree", 40, 130, 25, WHITE);
            }
            else if (currentsprite == 4) 
            {
                DrawText("Scene Successfully Added!", 40, 30, 30, WHITE);
            }
            else if (currentsprite == 5)
            {
                DrawText("Scene successfully deleted!", 40, 30, 30, WHITE);
            }
            else if (currentsprite == 6)
            {
                DrawText("Tree and its descendants has been wiped off clean!", 40, 30, 30, WHITE);
            }
            else if (currentsprite == 7)
            {
                DrawText("Tree node has been wiped and ready for overwriting!", 40, 30, 30, WHITE);
            }
            else if (currentsprite == 8)
            {
                DrawText("Story saved successfully!", 40, 30, 30, WHITE);
            }
            break;
            
        case MODE_REVIEW_SCENE:
            Background = LoadTexture(BackSprite);
            Character = LoadTexture(CharaSprite);
            position.x = GetScreenWidth() - Background.width * 0.75f / 2.0f;
            position.y = GetScreenHeight()- Background.height * 0.75f / 2.0f;
            DrawTextureEx(Background, position, 0, 0.75, WHITE);
            
            // Draw character at the specified position
            float charX;
            switch (charPosition) {
                case CHAR_POS_LEFT:
                    charX = position.x + 100;
                    break;
                case CHAR_POS_CENTER:
                    charX = GetScreenWidth()/2.0f - Character.width * 0.75f / 2.0f;
                    break;
                case CHAR_POS_RIGHT:
                    charX = position.x + Background.width * 0.75f - Character.width * 0.75f - 100;
                    break;
                default:
                    charX = GetScreenWidth()/2.0f - Character.width * 0.75f / 2.0f;
                    break;
            }
            position.x = charX;
            position.y = GetScreenHeight()/2.0f - Character.height * 0.75f / 2.0f;
            DrawTextureEx(Character, position, 0, 0.75f, WHITE);
            
            DrawText(Dialogue, position.x, position.y-50, 20, WHITE);
            DrawText("Press <-- to go to previous scene, and press --> to go to the next scene", position.x, position.y-80, 20, WHITE);
            break;
    }
}

void MakeCustomStory(CustomSceneTree *ThisSlot, int SlotNumber)
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
                        CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                        RenderTimer -= GetFrameTime();
                        if (RenderTimer <= 0.0f)
                        {
                            ShowRender = false;
                        }
                    }
                    else
                    {
                        CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                    }
                    switch (KeyPad)
                    {
                        case KEY_LEFT:
                            AddLeftChild(&TempTree, &TempScene, &selectedsprite, &warning, &control);
                        break;

                        case KEY_RIGHT:
                            AddRightChild(&TempTree, &TempScene, &warning, &control, &selectedsprite);
                        break;

                        case KEY_UP:
                            PreviousTree(&TempTree, &TempScene, &selectedsprite, &warning, &control);
                        break;

                        case KEY_A:
                            control = DELETELASTSCENE;
                        break;

                        case KEY_S:
                            control = DELETEFIRSTSCENE;
                        break;

                        case KEY_D:
                            control = ADDFIRSTSCENE;
                        break;

                        case KEY_F:
                            control = ADDLASTSCENE;
                        break;

                        case KEY_G:
                            control = MODE_OVERWRITING;

                        case KEY_ENTER:
                            PrintTree(ThisSlot);
                            SaveSlotToFile(ThisSlot, SlotNumber);
                            
                            selectedsprite = 8; // New confirmation message
                            RenderTimer = 2.0f;
                            ShowRender = true;
                            control = CONFIRMATION;
                            while (ShowRender)
                            {
                                BeginDrawing();
                                ClearBackground(BLACK);
                                if (ShowRender)
                                {
                                    CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                                    RenderTimer -= GetFrameTime();
                                    if (RenderTimer <= 0.0f)
                                    {
                                        ShowRender = false;
                                    }
                                }
                                EndDrawing();
                            }
                            control = ALLDONE;
                        break;

                        case KEY_BACKSPACE:
                            //HandleDeleteCurrentNode(&TempTree, &TempScene, &control, &selectedsprite);
                        break;

                        case KEY_SLASH:
                        {
                            control = MODE_REVIEW_SCENE;
                        }
                    }
                    EndDrawing();
                }
            break;

            case ADDFIRSTSCENE:
                AddSceneFirst(&TempTree, &TempScene, &warning, &control);
            break;

            case ADDLASTSCENE:
                AddSceneLast(&TempTree, &TempScene, &control);
            break;

            case DELETEFIRSTSCENE:
                DeleteSceneFirst(&TempTree, &TempScene, &selectedsprite, &warning, &control);
            break;

            case DELETELASTSCENE:
                DeleteSceneLast(&TempTree, &TempScene, &selectedsprite, &warning, &control);
            break;

            case MODE_REVIEW_SCENE:
                TempScene = TempTree->NodeContents;
                ReviewScene(&control, &TempScene);
                selectedsprite = 2;
            break;

            case MODE_OVERWRITING:
                OverwriteTree(&TempScene, &TempTree, &control);
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
            *selectedsprite = 4;
            RenderTimer = 2.0f;
            while (RenderTimer > 0.0f)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                RenderTimer -= GetFrameTime();
                EndDrawing();
            }
        }
        CustomStoryGUI(*control, *selectedsprite, Convo, NULL, NULL, CHAR_POS_CENTER);
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
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
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
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
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

void OverwriteTree(SceneList *TempScene, CustomSceneTree *TempTree, int *control)
{
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
    (*TempScene)->Data.charPosition = CHAR_POS_CENTER;

    *control = CONFIRMATION;
    RenderTimer = 2.0f;
    while (RenderTimer > 0.0f)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, 7, NULL, NULL, NULL, CHAR_POS_CENTER);
        RenderTimer -= GetFrameTime();
        EndDrawing();
    }

    *control = CHOOSINGBACKGROUND;
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
        *control = CONFIRMATION;
        (*TempTree) = (*TempTree)->Parent;
        (*TempScene) = (*TempTree)->NodeContents;
        *selectedsprite = 3;
    }
}

void AddRightChild(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control, int *selectedsprite)
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
        (*TempScene)->Data.charPosition = CHAR_POS_CENTER;
        *control = CHOOSINGBACKGROUND;
    }
    else
    {
        *TempTree = (*TempTree)->Right;
        *warning = true;
        *selectedsprite = 3;
    }
}

void AddLeftChild(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control)
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
        (*TempScene)->Data.charPosition = CHAR_POS_CENTER;
        *control = CHOOSINGBACKGROUND;
    }
    else
    {
        (*TempTree) = (*TempTree)->Left;
        *warning = true;
        *selectedsprite = 3;
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
    (*ThisSlot)->NodeContents->Data.charPosition = CHAR_POS_CENTER;
}

void AddSceneLast(CustomSceneTree *TempTree, SceneList *TempScene, int *control)
{
    SceneList TravellingScene = (*TempTree)->NodeContents;

    while ((TravellingScene)->Next != NULL)
    {
        TravellingScene = (TravellingScene)->Next;
    }

    TravellingScene->Next = (SceneList)malloc(sizeof(struct ListElements));
    TravellingScene->Next->Before = TravellingScene;
    TravellingScene->Next->Next = NULL;
    TravellingScene->Next->Data.Background = NULL;
    TravellingScene->Next->Data.Character = NULL;
    TravellingScene->Next->Data.Convo = NULL;
    TravellingScene->Next->Data.charPosition = CHAR_POS_CENTER;
    (*TempScene) = TravellingScene->Next;
    *control = CHOOSINGBACKGROUND;
}

void AddSceneFirst(CustomSceneTree *TempTree, SceneList *TempScene, bool *warning, int *control)
{
    SceneList TravellingScene = (*TempTree)->NodeContents;

    TravellingScene->Before = (SceneList)malloc(sizeof(struct ListElements));
    TravellingScene->Before->Next = TravellingScene;
    TravellingScene->Before->Before = NULL;
    TravellingScene->Before->Data.Background = NULL;
    TravellingScene->Before->Data.Character = NULL;
    TravellingScene->Before->Data.Convo = NULL;
    TravellingScene->Before->Data.charPosition = CHAR_POS_CENTER;
    (*TempTree)->NodeContents = TravellingScene->Before;
    (*TempScene) = TravellingScene->Before;
    *control = CHOOSINGBACKGROUND;
}

void DeleteSceneLast(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control)
{
    SceneList TravellingScene = (*TempTree)->NodeContents;

    while ((TravellingScene)->Next != NULL)
    {
        TravellingScene = (TravellingScene)->Next;
    }

    if (TravellingScene->Before != NULL)
    {
        TravellingScene->Before->Next = NULL;
        if (*TempScene == TravellingScene)
        {
            *TempScene = TravellingScene->Before;
        }
    }
    else
    {
        *selectedsprite = 2;
        ShowRender = true;
        RenderTimer = 2.0f;
        *control = CONFIRMATION;
        while (ShowRender)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            if (ShowRender)
            {
                CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                RenderTimer -= GetFrameTime();
                if (RenderTimer <= 0.0f)
                {
                    ShowRender = false;
                }
            }
            EndDrawing();
        }
        return;
    }
    if (TravellingScene->Data.Background) free(TravellingScene->Data.Background);
    if (TravellingScene->Data.Character) free(TravellingScene->Data.Character);
    if (TravellingScene->Data.Convo) free(TravellingScene->Data.Convo);
    free(TravellingScene);
    *warning = false;
    *selectedsprite = 5;
    RenderTimer = 2.0f;
    *control = CONFIRMATION;
    while (RenderTimer > 0.0f)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
        RenderTimer -= GetFrameTime();
        EndDrawing();
    }
}

void DeleteSceneFirst(CustomSceneTree *TempTree, SceneList *TempScene, int *selectedsprite, bool *warning, int *control)
{
    SceneList TravellingScene = (*TempTree)->NodeContents;

    if (TravellingScene->Next != NULL)
    {
        TravellingScene->Next->Before = NULL;
        (*TempTree)->NodeContents = TravellingScene->Next;
    }
    else
    {
        *selectedsprite = 2;
        ShowRender = true;
        RenderTimer = 2.0f;
        *control = CONFIRMATION;
        while (ShowRender)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            if (ShowRender)
            {
                CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                RenderTimer -= GetFrameTime();
                if (RenderTimer <= 0.0f)
                {
                    ShowRender = false;
                }
            }
            EndDrawing();
        }
        return;
    }
    if (TravellingScene->Data.Background) free(TravellingScene->Data.Background);
    if (TravellingScene->Data.Character) free(TravellingScene->Data.Character);
    if (TravellingScene->Data.Convo) free(TravellingScene->Data.Convo);
    free(TravellingScene);
    *warning = false;
    *selectedsprite = 5;
    *control = CONFIRMATION;
    RenderTimer = 2.0f;
    while (RenderTimer > 0.0f)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
        RenderTimer -= GetFrameTime();
        EndDrawing();
    }
}

void DeleteSceneList(SceneList sceneList) 
{
    SceneList current = sceneList;
    while (current != NULL) {
        SceneList next = current->Next;

        free(current->Data.Background);
        free(current->Data.Character);
        free(current->Data.Convo);
        free(current->Data.SFX);

        free(current);
        current = next;
    }
}

void DeleteTreeNode(CustomSceneTree *nodeToDelete) 
{
    if (nodeToDelete == NULL || *nodeToDelete == NULL) return;

    CustomSceneTree node = *nodeToDelete;

    // Post-order traversal: delete children first
    if (node->Left != NULL) DeleteTreeNode(&node->Left);
    if (node->Right != NULL) DeleteTreeNode(&node->Right);

    // Disconnect from parent
    if (node->Parent != NULL) {
        if (node->Parent->Left == node) node->Parent->Left = NULL;
        else if (node->Parent->Right == node) node->Parent->Right = NULL;
    }

    // Delete node content
    if (node->NodeContents) {
        DeleteSceneList(node->NodeContents);
        node->NodeContents = NULL;
    }

    free(node);
    *nodeToDelete = NULL;
}


void HandleDeleteCurrentNode(CustomSceneTree *TempTree, SceneList *TempScene, int *control, int *selectedsprite) 
{
    if (TempTree == NULL || *TempTree == NULL) return;

    CustomSceneTree nodeToDelete = *TempTree;

    // Case 1: Root cannot be deleted
    if (nodeToDelete->Parent == NULL) {
        *selectedsprite = 1; // "Cannot delete root node"
        *control = CONFIRMATION;
        RenderTimer = 2.0f;

        while (RenderTimer > 0.0f) {
            BeginDrawing();
            ClearBackground(BLACK);
            CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
            RenderTimer -= GetFrameTime();
            EndDrawing();
        }

        return;
    }

    // Backup parent BEFORE deletion
    CustomSceneTree parentNode = nodeToDelete->Parent;

    // Safely delete current node and its subtree
    DeleteTreeNode(&nodeToDelete);

    // Update active node to parent
    *TempTree = parentNode;

    // Update TempScene as well
    if (parentNode->NodeContents) {
        *TempScene = parentNode->NodeContents;
    } else {
        parentNode->NodeContents = (SceneList)calloc(1, sizeof(struct ListElements));
        *TempScene = parentNode->NodeContents;
    }

    // Confirmation message
    *selectedsprite = 5; // "Scene successfully deleted"
    *control = CONFIRMATION;

    RenderTimer = 2.0f;
    while (RenderTimer > 0.0f) {
        BeginDrawing();
        ClearBackground(BLACK);
        CustomStoryGUI(*control, *selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
        RenderTimer -= GetFrameTime();
        EndDrawing();
    }
}


void DrawCustomStoryScene(SceneList scene) {
    // Debug print
    printf("BG: %s | CHAR: %s | POS: %d\n", scene->Data.Background ? scene->Data.Background : "(null)", scene->Data.Character ? scene->Data.Character : "(null)", scene->Data.charPosition);
    // Load background jika ada
    Texture2D bgTex = {0};
    if (scene->Data.Background && strlen(scene->Data.Background) > 0) {
        bgTex = LoadTexture(scene->Data.Background);
        if (bgTex.id == 0) printf("Gagal load background: %s\n", scene->Data.Background);
        bgTex.height = SCREEN_HEIGHT;
        bgTex.width = SCREEN_WIDTH;
        DrawTexture(bgTex, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }

    // Load karakter jika ada
    Texture2D charTex = {0};
    if (scene->Data.Character && strlen(scene->Data.Character) > 0) {
        charTex = LoadTexture(scene->Data.Character);
        if (charTex.id == 0) printf("Gagal load karakter: %s\n", scene->Data.Character);
        charTex.height /= 2;
        charTex.width /= 2;
        DrawCharacterAtPosition(charTex, scene->Data.charPosition);
    }

    // Gambar kotak dialog dan teks
    if (scene->Data.Convo && strlen(scene->Data.Convo) > 0) {
        DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, BLACK);
        DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
        DrawText(scene->Data.Convo, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }

    // Jangan unload texture di sini!
    // if (bgTex.id) UnloadTexture(bgTex);
    // if (charTex.id) UnloadTexture(charTex);
}

void ReviewScene(int *control, SceneList *TempScene)
{
    while (*control == MODE_REVIEW_SCENE)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Gambar scene dengan layout seperti story.c
        DrawCustomStoryScene(*TempScene);

        // Navigasi scene
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
            ShowRender = false;
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

void PrintTree(CustomSceneTree *ThisSlot) {
    CustomSceneTree root = *ThisSlot;
    if (!root) 
    {
        printf("Tree is empty.\n");
        return;
    }

    printf("===== DEBUG TREE LOG START =====\n");

    TempQueueNode *front = NULL, *rear = NULL;
    Enqueue(&front, &rear, root, 0);

    while (front) {
        TempQueueNode *qNode = Dequeue(&front, &rear);
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
            Enqueue(&front, &rear, current->Left, level + 1);
        if (current->Right)
            Enqueue(&front, &rear, current->Right, level + 1);

        free(qNode);
    }

    printf("===== DEBUG TREE LOG END =====\n\n");
}

void SerializeSceneList(FILE *file, SceneList sceneList) {
    SceneList current = sceneList;
    int sceneCount = 0;
    
    // First, count the number of scenes
    SceneList counter = sceneList;
    while (counter != NULL) {
        sceneCount++;
        counter = counter->Next;
    }
    
    // Write the scene count
    fwrite(&sceneCount, sizeof(int), 1, file);
    
    // Write each scene
    current = sceneList;
    while (current != NULL) {
        // Write Background string
        int bgLen = current->Data.Background ? strlen(current->Data.Background) + 1 : 0;
        fwrite(&bgLen, sizeof(int), 1, file);
        if (bgLen > 0) {
            fwrite(current->Data.Background, sizeof(char), bgLen, file);
        }
        
        // Write Character string
        int charLen = current->Data.Character ? strlen(current->Data.Character) + 1 : 0;
        fwrite(&charLen, sizeof(int), 1, file);
        if (charLen > 0) {
            fwrite(current->Data.Character, sizeof(char), charLen, file);
        }
        
        // Write Convo string
        int convoLen = current->Data.Convo ? strlen(current->Data.Convo) + 1 : 0;
        fwrite(&convoLen, sizeof(int), 1, file);
        if (convoLen > 0) {
            fwrite(current->Data.Convo, sizeof(char), convoLen, file);
        }
        
        // Write SFX string
        int sfxLen = current->Data.SFX ? strlen(current->Data.SFX) + 1 : 0;
        fwrite(&sfxLen, sizeof(int), 1, file);
        if (sfxLen > 0) {
            fwrite(current->Data.SFX, sizeof(char), sfxLen, file);
        }
        
        current = current->Next;
    }
}

SceneList DeserializeSceneList(FILE *file) {
    int sceneCount;
    if (fread(&sceneCount, sizeof(int), 1, file) != 1) {
        return NULL; // Failed to read scene count
    }
    
    if (sceneCount == 0) {
        return NULL;
    }
    
    SceneList firstScene = NULL;
    SceneList lastScene = NULL;
    
    for (int i = 0; i < sceneCount; i++) {
        // Create new scene
        SceneList newScene = (SceneList)malloc(sizeof(struct ListElements));
        newScene->Next = NULL;
        newScene->Before = lastScene;
        newScene->Data.Background = NULL;
        newScene->Data.Character = NULL;
        newScene->Data.Convo = NULL;
        newScene->Data.SFX = NULL;
        
        // Read Background string
        int bgLen;
        fread(&bgLen, sizeof(int), 1, file);
        if (bgLen > 0) {
            newScene->Data.Background = (char*)malloc(bgLen);
            fread(newScene->Data.Background, sizeof(char), bgLen, file);
        }
        
        // Read Character string
        int charLen;
        fread(&charLen, sizeof(int), 1, file);
        if (charLen > 0) {
            newScene->Data.Character = (char*)malloc(charLen);
            fread(newScene->Data.Character, sizeof(char), charLen, file);
        }
        
        // Read Convo string
        int convoLen;
        fread(&convoLen, sizeof(int), 1, file);
        if (convoLen > 0) {
            newScene->Data.Convo = (char*)malloc(convoLen);
            fread(newScene->Data.Convo, sizeof(char), convoLen, file);
        }
        
        // Read SFX string
        int sfxLen;
        fread(&sfxLen, sizeof(int), 1, file);
        if (sfxLen > 0) {
            newScene->Data.SFX = (char*)malloc(sfxLen);
            fread(newScene->Data.SFX, sizeof(char), sfxLen, file);
        }
        
        // Link the scenes
        if (lastScene != NULL) {
            lastScene->Next = newScene;
        } else {
            firstScene = newScene; // This is the first scene
        }
        lastScene = newScene;
    }
    
    return firstScene;
}

void SerializeTreeNode(FILE *file, CustomSceneTree node) {
    if (node == NULL) {
        // Write a marker for NULL node
        int nullMarker = 0;
        fwrite(&nullMarker, sizeof(int), 1, file);
        return;
    }
    
    // Write a marker for non-NULL node
    int nodeMarker = 1;
    fwrite(&nodeMarker, sizeof(int), 1, file);
    
    // Write node ID
    fwrite(&(node->ID), sizeof(int), 1, file);
    
    // Write the scene list for this node
    SerializeSceneList(file, node->NodeContents);
    
    // Recursively write left and right children
    SerializeTreeNode(file, node->Left);
    SerializeTreeNode(file, node->Right);
}

CustomSceneTree DeserializeTreeNode(FILE *file, CustomSceneTree parent) {
    int nodeMarker;
    if (fread(&nodeMarker, sizeof(int), 1, file) != 1) {
        return NULL; // Failed to read marker
    }
    
    if (nodeMarker == 0) {
        return NULL; // This was a NULL node
    }
    
    // Create new tree node
    CustomSceneTree node = (CustomSceneTree)malloc(sizeof(struct Tree));
    node->Parent = parent;
    node->Left = NULL;
    node->Right = NULL;
    
    // Read node ID
    if (fread(&(node->ID), sizeof(int), 1, file) != 1) {
        free(node);
        return NULL;
    }
    
    // Read the scene list
    node->NodeContents = DeserializeSceneList(file);
    
    // Recursively read left and right children
    node->Left = DeserializeTreeNode(file, node);
    node->Right = DeserializeTreeNode(file, node);
    
    return node;
}

void SaveTreeToFile(CustomSceneTree tree, const char* filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return;
    }
    
    // Write a file header/version for future compatibility
    char header[16] = "VSTORY_V1.0";
    fwrite(header, sizeof(char), 16, file);
    
    // Serialize the entire tree
    SerializeTreeNode(file, tree);
    
    fclose(file);
    printf("Story tree saved to '%s' successfully.\n", filename);
}

CustomSceneTree LoadTreeFromFile(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading.\n", filename);
        return NULL;
    }
    
    // Read and verify the file header
    char header[16];
    if (fread(header, sizeof(char), 16, file) != 16) {
        printf("Error: Invalid file format.\n");
        fclose(file);
        return NULL;
    }
    
    if (strncmp(header, "VSTORY_V1.0", 11) != 0) {
        printf("Error: Unsupported file version or corrupted file.\n");
        fclose(file);
        return NULL;
    }
    
    // Deserialize the tree
    CustomSceneTree tree = DeserializeTreeNode(file, NULL);
    
    fclose(file);
    
    if (tree != NULL) {
        printf("Story tree loaded from '%s' successfully.\n", filename);
    } else {
        printf("Error: Failed to load story tree from '%s'.\n", filename);
    }
    
    return tree;
}

void SaveSlotToFile(CustomSceneTree *ThisSlot, int slotNumber) {
    if (ThisSlot == NULL || *ThisSlot == NULL) {
        printf("Error: No story to save.\n");
        return;
    }
    
    if (slotNumber < 1 || slotNumber > 10) {
        printf("Error: Invalid slot number. Use 1-10.\n");
        return;
    }
    
    char filename[64];
    sprintf(filename, "saves/story_slot_%d.sav", slotNumber);
    
    // Create saves directory if it doesn't exist (platform dependent)
    #ifdef _WIN32
        system("mkdir saves 2>nul");
    #else
        system("mkdir -p saves");
    #endif
    
    SaveTreeToFile(*ThisSlot, filename);
}

/**
 * Loads a story from a numbered save file
 * @param slotNumber - Save slot number to load (1-10)
 * @return Loaded tree, or NULL if failed
 */
CustomSceneTree LoadSlotFromFile(int slotNumber) {
    if (slotNumber < 1 || slotNumber > 10) {
        printf("Error: Invalid slot number. Use 1-10.\n");
        return NULL;
    }
    
    char filename[64];
    sprintf(filename, "saves/story_slot_%d.sav", slotNumber);
    
    return LoadTreeFromFile(filename);
}

void SaveSlot(CustomSceneTree *ThisSlot) {
    if (ThisSlot == NULL || *ThisSlot == NULL) {
        printf("Error: No story to save.\n");
        return;
    }
    
    // For now, save to slot 1 by default
    // You can modify this to ask user for slot number
    SaveSlotToFile(ThisSlot, 1);
    
    // Keep the existing debug output if needed
    printf("===== SAVE SUCCESSFUL =====\n");
    printf("Story saved to slot 1\n");
    printf("===========================\n");
}

void LoadSlot(CustomSceneTree *ThisSlot) {
    if (ThisSlot != NULL && *ThisSlot != NULL) {
        // Clean up existing tree first
        DeleteTreeNode(ThisSlot);
    }
    
    // For now, load from slot 1 by default
    // You can modify this to ask user for slot number
    *ThisSlot = LoadSlotFromFile(1);
    
    if (*ThisSlot != NULL) {
        printf("===== LOAD SUCCESSFUL =====\n");
        printf("Story loaded from slot 1\n");
        printf("===========================\n");
    } else {
        printf("===== LOAD FAILED =====\n");
        printf("Could not load story from slot 1\n");
        printf("Creating new story...\n");
        printf("=======================\n");
        
        // Create a new empty tree if load failed
        InitializeStoryTree(ThisSlot);
    }
}


