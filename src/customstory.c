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

// Global variables for custom story management
int KeyPad;  // For keyboard input
int storedKeyPad;  // For storing branch direction
bool warning = false;  // For error states and warnings

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
                DrawText("<-- Create Left Branch (Set choice text first)", 40, 70, 25, WHITE);
                DrawText("--> Create Right Branch (Set choice text first)", 40, 100, 25, WHITE);
                DrawText("Up Arrow: Go To Parent | /: Review Scenes", 40, 130, 25, WHITE);
                DrawText("A: Delete Last Scene | S: Delete First Scene", 40, 160, 25, WHITE);
                DrawText("D: Add Scene As First | F: Add Scene As Last", 40, 190, 25, WHITE);
                DrawText("G: Overwrite Tree | ENTER: Save and Exit", 40, 220, 25, WHITE);
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
          case CHOOSINGCHOICETEXT:
            DrawText("Before creating a new node, set the choice button text", 40, 30, 30, WHITE);
            
            // Draw left choice input area
            DrawText("Left Choice Text:", 40, 100, 25, WHITE);
            DrawRectangleLines(40, 130, 720, 40, WHITE);
            DrawText(Dialogue, 50, 140, 20, LIGHTGRAY);

            // Draw right choice input area
            DrawText("Right Choice Text:", 40, 200, 25, WHITE);
            DrawRectangleLines(40, 230, 720, 40, WHITE);

            // Draw example choice buttons like in story mode
            int choiceButtonWidth = 400;
            int choiceButtonHeight = 60;
            int choiceStartY = SCREEN_HEIGHT / 2 + 275;

            // Left choice button
            Rectangle choiceRectLeft = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
            DrawText("Preview Left Choice", 
                    (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText("Preview Left Choice", 25) / 2),
                    (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                    25, WHITE);

            // Right choice button
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
            DrawText("Preview Right Choice",
                    (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText("Preview Right Choice", 25) / 2),
                    (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                    25, WHITE);

            DrawText("Press TAB to switch between choices | ENTER when done", 40, SCREEN_HEIGHT - 50, 20, LIGHTGRAY);
            break;
    }
}

void MakeCustomStory(CustomSceneTree *ThisSlot, int SlotNumber)
{
    ShowRender = false;
    char Convo[DUMMY_MAX_CONVO];
    char LeftText[DUMMY_MAX_CONVO];   // Buffer for left choice text
    char RightText[DUMMY_MAX_CONVO];  // Buffer for right choice text
    memset(LeftText, 0, DUMMY_MAX_CONVO);
    memset(RightText, 0, DUMMY_MAX_CONVO);
    int selectedsprite = 0;
    int control = CHOOSINGBACKGROUND;
    CustomSceneTree TempTree;
    SceneList TempScene;
    bool needChoiceText = false;  // Flag to track if we need to get choice text

    if (*ThisSlot == NULL)
    {
        InitializeStoryTree(ThisSlot);
        TempTree = *ThisSlot;
        TempScene = TempTree->NodeContents;
        TempScene->Data.Background = NULL;
        TempScene->Data.Character = NULL;
        TempScene->Data.Convo = NULL;
    }
    else
    {
        TempTree = *ThisSlot;
        TempScene = TempTree->NodeContents;
        control = MODE_REVIEW_SCENE;
    }
    
    while (control != ALLDONE)
    {
        switch (control)
        {
            case CONFIRMATION:
                {
                    KeyPad = GetKeyPressed();
                    BeginDrawing();
                    ClearBackground(BLACK);
                    CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                      if (KeyPad == KEY_LEFT || KeyPad == KEY_RIGHT) {
                        storedKeyPad = KeyPad;  // Store which direction was chosen
                        control = CHOOSINGCHOICETEXT;
                        needChoiceText = true;
                        break;
                    }
                    
                    // Handle other keys
                    switch (KeyPad)
                    {
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
                            break;                        case KEY_ENTER:
                            // Save the current story first
                            PrintTree(ThisSlot);
                            SaveSlotToFile(ThisSlot, SlotNumber);
                            printf("Saving to slot %d...\n", SlotNumber);
                            
                            // Show save confirmation message
                            selectedsprite = 8;  // Save confirmation sprite
                            ShowRender = true;
                            RenderTimer = 2.0f;

                            while (RenderTimer > 0.0f)
                            {
                                BeginDrawing();
                                ClearBackground(BLACK);
                                CustomStoryGUI(control, selectedsprite, NULL, NULL, NULL, CHAR_POS_CENTER);
                                EndDrawing();
                                RenderTimer -= GetFrameTime();
                            }
                            
                            // Add a small delay to ensure user sees the message
                            WaitTime(0.5f);
                            
                            // Now we can safely exit
                            control = ALLDONE;
                            break;

                        case KEY_SLASH:
                            control = MODE_REVIEW_SCENE;
                            break;
                    }
                    EndDrawing();
                }
                break;            case CHOOSINGCHOICETEXT:
                ChoosingChoiceText(LeftText, RightText, &selectedsprite, &TempTree);
                if (selectedsprite == 4) {  // Choice text input complete
                    if (needChoiceText) {
                        if (storedKeyPad == KEY_LEFT) {
                            AddLeftChild(&TempTree, &TempScene, &selectedsprite, &warning, &control);
                        } else if (storedKeyPad == KEY_RIGHT) {
                            AddRightChild(&TempTree, &TempScene, &warning, &control, &selectedsprite);
                        }
                        needChoiceText = false;
                        control = CHOOSINGBACKGROUND;
                    }
                }
                break;

            case CHOOSINGBACKGROUND:
                ChoosingBackground(&selectedsprite, &control, &TempScene);
            break;

            case CHOOSINGCHARA:
                ChoosingChara(&selectedsprite, &control, &TempScene);
            break;

            case CHOOSINGCHARPOSITION:
                ChoosingCharaPosition(&selectedsprite, &control, &TempScene);
            break;

            case CHOOSINGDIALOGUE:
                printf("Masuk ChoosingDialogue, control = %d\n", control);
                memset(Convo, 0, sizeof(Convo));
                control = ChoosingDialogue(Convo, &selectedsprite, &TempScene);
            break;            // Remove duplicate CONFIRMATION case since it's already handled above

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
                // Keep current TempScene position, don't reset to NodeContents
                ReviewScene(&control, &TempScene, &TempTree);
                selectedsprite = 2;
            break;

            case MODE_OVERWRITING:
                OverwriteTree(&TempScene, &TempTree, &control);
            break;            // Remove duplicate CHOOSINGCHOICETEXT case since it's already handled above
        }
    }
    EndDrawing();
}

int ChoosingDialogue(char *Convo, int *selectedsprite, SceneList *TempScene)
{
    printf("Masuk ChoosingDialogue\n");
    int letterCount = 0;
    int key = 0;
    int control = CHOOSINGDIALOGUE;
    memset(Convo, 0, DUMMY_MAX_CONVO);
    while (control == CHOOSINGDIALOGUE)
    {
        printf("Loop ChoosingDialogue, control = %d, Convo='%s'\n", control, Convo);
        BeginDrawing();
        ClearBackground(BLACK);
        key = GetCharPressed();
        while (key > 0)
        {
            if (letterCount < DUMMY_MAX_CONVO-1 && key >= 32 && key <= 125)
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
            printf("User tekan ENTER, Convo='%s'\n", Convo);            if (strlen(Convo) > 0) {
                (*TempScene)->Data.Convo = malloc(strlen(Convo) + 1);
                strcpy((*TempScene)->Data.Convo, Convo);
                
                // Show success message briefly
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("Scene content saved! Press any key to continue...", 40, 30, 30, WHITE);
                EndDrawing();
                WaitTime(1.0f);
                
                // Return to branch creation menu
                control = CONFIRMATION;
                *selectedsprite = 0;  // Reset sprite to show branch options
                ShowRender = false;
            }else {
                // Tampilkan pesan error
                DrawText("Dialog tidak boleh kosong!", 100, 200, 30, RED);
            }
        }
        CustomStoryGUI(control, *selectedsprite, Convo, NULL, NULL, CHAR_POS_CENTER);
        EndDrawing();
    }
    return control;
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
            *control = CHOOSINGCHARPOSITION;
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

void ChoosingCharaPosition(int *selectedsprite, int *control, SceneList *TempScene) {
    int pos = CHAR_POS_CENTER;
    while (*control == CHOOSINGCHARPOSITION) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Pilih posisi karakter: [LEFT: <-] [CENTER: UP] [RIGHT: ->] [ENTER: OK]", 100, 100, 30, WHITE);

        // Preview posisi karakter
        Texture2D charTex = {0};
        if ((*TempScene)->Data.Character && strlen((*TempScene)->Data.Character) > 0) {
            charTex = LoadTexture((*TempScene)->Data.Character);
            charTex.height /= 2;
            charTex.width /= 2;
            DrawCharacterAtPosition(charTex, pos);
            // UnloadTexture(charTex); // optional
        }

        EndDrawing();

        if (IsKeyPressed(KEY_LEFT)) pos = CHAR_POS_LEFT;
        if (IsKeyPressed(KEY_UP)) pos = CHAR_POS_CENTER;
        if (IsKeyPressed(KEY_RIGHT)) pos = CHAR_POS_RIGHT;
        if (IsKeyPressed(KEY_ENTER)) {
            printf("KEY_ENTER detected, lanjut ke CHOOSINGDIALOGUE\n");
            (*TempScene)->Data.charPosition = pos;
            *control = CHOOSINGDIALOGUE;
        }
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
        if (toDelete->Data.SFX) free(toDelete->Data.SFX);  // Free SFX if exists
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
    (*TempScene)->Data.SFX = NULL;  // Initialize SFX to NULL
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
        
        // Move to parent tree node
        (*TempTree) = (*TempTree)->Parent;
        
        // Reset TempScene to start of parent's NodeContents
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
        (*TempTree)->TextLeft = NULL;  // Initialize choice text fields
        (*TempTree)->TextRight = NULL;
        (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
        (*TempScene) = (*TempTree)->NodeContents;
        (*TempScene)->Next = NULL;
        (*TempScene)->Before = NULL;
        (*TempScene)->Data.Background = NULL;
        (*TempScene)->Data.Character = NULL;
        (*TempScene)->Data.Convo = NULL;
        (*TempScene)->Data.SFX = NULL;  // Initialize SFX to NULL
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
        (*TempTree)->TextLeft = NULL;  // Initialize choice text fields
        (*TempTree)->TextRight = NULL;
        (*TempTree)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
        (*TempScene) = (*TempTree)->NodeContents;
        (*TempScene)->Next = NULL;
        (*TempScene)->Before = NULL;
        (*TempScene)->Data.Background = NULL;
        (*TempScene)->Data.Character = NULL;
        (*TempScene)->Data.Convo = NULL;
        (*TempScene)->Data.SFX = NULL;  // Initialize SFX to NULL
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
    (*ThisSlot)->TextLeft = NULL;  // Initialize choice text fields to NULL
    (*ThisSlot)->TextRight = NULL;
    (*ThisSlot)->NodeContents = (SceneList)malloc(sizeof(struct ListElements));
    (*ThisSlot)->NodeContents->Next = NULL;
    (*ThisSlot)->NodeContents->Before = NULL;
    (*ThisSlot)->NodeContents->Data.Background = NULL;
    (*ThisSlot)->NodeContents->Data.Character = NULL;
    (*ThisSlot)->NodeContents->Data.Convo = NULL;
    (*ThisSlot)->NodeContents->Data.SFX = NULL;  // Initialize SFX to NULL
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
    TravellingScene->Next->Data.SFX = NULL;  // Initialize SFX to NULL
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
    TravellingScene->Before->Data.SFX = NULL;  // Initialize SFX to NULL
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
    if (TravellingScene->Data.SFX) free(TravellingScene->Data.SFX);  // Free SFX if exists
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
    if (TravellingScene->Data.SFX) free(TravellingScene->Data.SFX);  // Free SFX if exists
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

    // Free choice text fields
    if (node->TextLeft != NULL) {
        free(node->TextLeft);
        node->TextLeft = NULL;
    }
    if (node->TextRight != NULL) {
        free(node->TextRight);
        node->TextRight = NULL;
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
        // UnloadTexture(charTex); // (optional, but don't unload if reused)
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

void ReviewScene(int *control, SceneList *TempScene, CustomSceneTree *TempTree)
{
    Vector2 mouse = {0};
    bool showWarning = false;
    const char* warningText = "";
    float warningTimer = 0;

    while (*control == MODE_REVIEW_SCENE)
    {
        mouse = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw current scene with layout like story.c
        DrawCustomStoryScene(*TempScene);

        // Debug information showing current node and scene navigation options
        char debugText[128];
        sprintf(debugText, "Current Node ID: %d | Scene Position: %s%s%s", 
                (*TempTree)->ID,
                (*TempScene)->Before ? "<- " : "",
                "[]",
                (*TempScene)->Next ? " ->" : "");
        DrawText(debugText, 10, 10, 20, RED);

        // Always draw choice buttons, regardless of scene position
        if ((*TempScene)->Next == NULL){

            int choiceButtonWidth = 400;
            int choiceButtonHeight = 60;
            int choiceStartY = SCREEN_HEIGHT / 2 + 275;
    
            // Left choice button
            Rectangle choiceRectLeft = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
            DrawText((*TempTree)->TextLeft ? (*TempTree)->TextLeft : "Go Left", 
                    (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText((*TempTree)->TextLeft ? (*TempTree)->TextLeft : "Go Left", 25) / 2),
                    (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                    25, WHITE);
    
            // Right choice button
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
            DrawText((*TempTree)->TextRight ? (*TempTree)->TextRight : "Go Right",
                    (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText((*TempTree)->TextRight ? (*TempTree)->TextRight : "Go Right", 25) / 2),
                    (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                    25, WHITE);
    
            // Handle button hovers and clicks
            if (CheckCollisionPointRec(mouse, choiceRectLeft)) {
                if ((*TempTree)->Left != NULL) {
                    DrawRectangleLinesEx(choiceRectLeft, 3, GREEN);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        *TempTree = (*TempTree)->Left;
                        *TempScene = (*TempTree)->NodeContents;  // Reset to first scene of new node
                    }
                } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    showWarning = true;
                    warningText = "There's no left node yet!";
                    warningTimer = 2.0f;
                }
            }
            
            if (CheckCollisionPointRec(mouse, choiceRectRight)) {
                if ((*TempTree)->Right != NULL) {
                    DrawRectangleLinesEx(choiceRectRight, 3, GREEN);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        *TempTree = (*TempTree)->Right;
                        *TempScene = (*TempTree)->NodeContents;  // Reset to first scene of new node
                    }
                } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    showWarning = true;
                    warningText = "There's no right node yet!";
                    warningTimer = 2.0f;
                }
            }
        }

        // Show warning if needed
        if (showWarning) {
            DrawText(warningText, SCREEN_WIDTH/2 - MeasureText(warningText, 30)/2, 50, 30, RED);
            warningTimer -= GetFrameTime();
            if (warningTimer <= 0) {
                showWarning = false;
            }
        }

        // Scene navigation
        if (IsKeyPressed(KEY_RIGHT) && (*TempScene)->Next != NULL) {
            (*TempScene) = (*TempScene)->Next;
        }
        else if(IsKeyPressed(KEY_LEFT) && (*TempScene)->Before != NULL) {
            (*TempScene) = (*TempScene)->Before;
        }
        else if(IsKeyPressed(KEY_ENTER)) {
            ShowRender = false;
            *control = CONFIRMATION;
        }
        else if(IsKeyPressed(KEY_BACKSPACE)) {
            if ((*TempTree)->Parent != NULL) {
                *TempTree = (*TempTree)->Parent;
                *TempScene = (*TempTree)->NodeContents;
            }
        }

        // Navigation instructions
        DrawText("LEFT/RIGHT: Change scenes | BACKSPACE: Return to parent | ENTER: Exit review", 
                SCREEN_WIDTH/2 - MeasureText("LEFT/RIGHT: Change scenes | BACKSPACE: Return to parent | ENTER: Exit review", 20)/2, 
                SCREEN_HEIGHT - 30, 20, LIGHTGRAY);

        EndDrawing();
    }
}

void InitiateAssets()
{
    for (int i = 0; i < BACKGROUND_AMMOUNT; i++) 
    {
        char path[MAX_PATH_LENGTH];
        if (snprintf(path, MAX_PATH_LENGTH, "Assets/BackgroundSprites/background%i.png", i+1) >= MAX_PATH_LENGTH) {
            printf("Error: Background path too long for index %d\n", i+1);
            continue;
        }
        strncpy(FileBackground[i].FileName, path, MAX_FILENAME_LENGTH-1);
        FileBackground[i].FileName[MAX_FILENAME_LENGTH-1] = '\0';
        FileBackground[i].Background = LoadTexture(path);
    }
    
    for (int i = 0; i < CHARA_AMMOUNT; i++)
    {
        char path[MAX_PATH_LENGTH];
        if (snprintf(path, MAX_PATH_LENGTH, "Assets/CharaSprites/chara%i.png", i+1) >= MAX_PATH_LENGTH) {
            printf("Error: Character path too long for index %d\n", i+1);
            continue;
        }
        strncpy(FileChara[i].FileName, path, MAX_FILENAME_LENGTH-1);
        FileChara[i].FileName[MAX_FILENAME_LENGTH-1] = '\0';
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
    
    printf("DEBUG: Serializing %d scenes\n", sceneCount);
    
    // Write the scene count
    fwrite(&sceneCount, sizeof(int), 1, file);
    
    // Write each scene
    current = sceneList;
    while (current != NULL) {
        // Write Background string
        int bgLen = (current->Data.Background != NULL) ? strlen(current->Data.Background) + 1 : 0;
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
        printf("Error: Failed to read scene count\n");
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
        if (!newScene) {
            printf("Error: Failed to allocate memory for scene\n");
            return NULL;
        }
        newScene->Next = NULL;
        newScene->Before = lastScene;
        newScene->Data.Background = NULL;
        newScene->Data.Character = NULL;
        newScene->Data.Convo = NULL;
        newScene->Data.SFX = NULL;  // Initialize SFX to NULL
        newScene->Data.charPosition = CHAR_POS_NONE;  // Default position
        
        // Read Background string
        int bgLen;
        if (fread(&bgLen, sizeof(int), 1, file) != 1) {
            printf("Error: Failed to read background length\n");
            free(newScene);
            return NULL;
        }
        if (bgLen > 0) {
            newScene->Data.Background = (char*)malloc(bgLen);
            if (!newScene->Data.Background) {
                printf("Error: Failed to allocate memory for background\n");
                free(newScene);
                return NULL;
            }
            if (fread(newScene->Data.Background, sizeof(char), bgLen, file) != bgLen) {
                printf("Error: Failed to read background data\n");
                free(newScene->Data.Background);
                free(newScene);
                return NULL;
            }
        }
        
        // Read Character string
        int charLen;
        if (fread(&charLen, sizeof(int), 1, file) != 1) {
            printf("Error: Failed to read character length\n");
            if (newScene->Data.Background) free(newScene->Data.Background);
            free(newScene);
            return NULL;
        }
        if (charLen > 0) {
            newScene->Data.Character = (char*)malloc(charLen);
            if (!newScene->Data.Character) {
                printf("Error: Failed to allocate memory for character\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                free(newScene);
                return NULL;
            }
            if (fread(newScene->Data.Character, sizeof(char), charLen, file) != charLen) {
                printf("Error: Failed to read character data\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                free(newScene->Data.Character);
                free(newScene);
                return NULL;
            }
        }
        
        // Read Convo string
        int convoLen;
        if (fread(&convoLen, sizeof(int), 1, file) != 1) {
            printf("Error: Failed to read convo length\n");
            if (newScene->Data.Background) free(newScene->Data.Background);
            if (newScene->Data.Character) free(newScene->Data.Character);
            free(newScene);
            return NULL;
        }
        if (convoLen > 0) {
            newScene->Data.Convo = (char*)malloc(convoLen);
            if (!newScene->Data.Convo) {
                printf("Error: Failed to allocate memory for convo\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                if (newScene->Data.Character) free(newScene->Data.Character);
                free(newScene);
                return NULL;
            }
            if (fread(newScene->Data.Convo, sizeof(char), convoLen, file) != convoLen) {
                printf("Error: Failed to read convo data\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                if (newScene->Data.Character) free(newScene->Data.Character);
                free(newScene->Data.Convo);
                free(newScene);
                return NULL;
            }
        }
        
        // Read SFX string
        int sfxLen;
        if (fread(&sfxLen, sizeof(int), 1, file) != 1) {
            printf("Error: Failed to read SFX length\n");
            if (newScene->Data.Background) free(newScene->Data.Background);
            if (newScene->Data.Character) free(newScene->Data.Character);
            if (newScene->Data.Convo) free(newScene->Data.Convo);
            free(newScene);
            return NULL;
        }
        if (sfxLen > 0) {
            newScene->Data.SFX = (char*)malloc(sfxLen);
            if (!newScene->Data.SFX) {
                printf("Error: Failed to allocate memory for SFX\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                if (newScene->Data.Character) free(newScene->Data.Character);
                if (newScene->Data.Convo) free(newScene->Data.Convo);
                free(newScene);
                return NULL;
            }
            if (fread(newScene->Data.SFX, sizeof(char), sfxLen, file) != sfxLen) {
                printf("Error: Failed to read SFX data\n");
                if (newScene->Data.Background) free(newScene->Data.Background);
                if (newScene->Data.Character) free(newScene->Data.Character);
                if (newScene->Data.Convo) free(newScene->Data.Convo);
                free(newScene->Data.SFX);
                free(newScene);
                return NULL;
            }
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
    
    printf("DEBUG: Serializing node ID: %d\n", node->ID);
    
    // Write a marker for non-NULL node
    int nodeMarker = 1;
    fwrite(&nodeMarker, sizeof(int), 1, file);
    
    // Write node ID
    fwrite(&(node->ID), sizeof(int), 1, file);
    
    // Write TextLeft
    int textLeftLen = node->TextLeft ? strlen(node->TextLeft) + 1 : 0;
    fwrite(&textLeftLen, sizeof(int), 1, file);
    if (textLeftLen > 0) {
        printf("DEBUG: Writing TextLeft: %s\n", node->TextLeft);
        fwrite(node->TextLeft, sizeof(char), textLeftLen, file);
    }
    
    // Write TextRight
    int textRightLen = node->TextRight ? strlen(node->TextRight) + 1 : 0;
    fwrite(&textRightLen, sizeof(int), 1, file);
    if (textRightLen > 0) {
        printf("DEBUG: Writing TextRight: %s\n", node->TextRight);
        fwrite(node->TextRight, sizeof(char), textRightLen, file);
    }
    
    printf("DEBUG: Serializing scene list for node %d\n", node->ID);
    // Write the scene list for this node
    SerializeSceneList(file, node->NodeContents);
    
    printf("DEBUG: Recursively serializing left child of node %d\n", node->ID);
    // Recursively write left and right children
    SerializeTreeNode(file, node->Left);
    printf("DEBUG: Recursively serializing right child of node %d\n", node->ID);
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
    node->TextLeft = NULL;
    node->TextRight = NULL;
    
    // Read node ID
    if (fread(&(node->ID), sizeof(int), 1, file) != 1) {
        free(node);
        return NULL;
    }
    
    // Read TextLeft
    int textLeftLen;
    if (fread(&textLeftLen, sizeof(int), 1, file) != 1) {
        free(node);
        return NULL;
    }
    if (textLeftLen > 0) {
        node->TextLeft = (char*)malloc(textLeftLen);
        if (fread(node->TextLeft, sizeof(char), textLeftLen, file) != textLeftLen) {
            free(node->TextLeft);
            free(node);
            return NULL;
        }
    }
    
    // Read TextRight
    int textRightLen;
    if (fread(&textRightLen, sizeof(int), 1, file) != 1) {
        free(node);
        return NULL;
    }
    if (textRightLen > 0) {
        node->TextRight = (char*)malloc(textRightLen);
        if (fread(node->TextRight, sizeof(char), textRightLen, file) != textRightLen) {
            free(node->TextLeft);
            free(node->TextRight);
            free(node);
            return NULL;
        }
    }
    
    // Read the scene list
    node->NodeContents = DeserializeSceneList(file);
    if (!node->NodeContents) {
        if (node->TextLeft) free(node->TextLeft);
        if (node->TextRight) free(node->TextRight);
        free(node);
        return NULL;
    }
    
    // Recursively read left and right children
    node->Left = DeserializeTreeNode(file, node);
    node->Right = DeserializeTreeNode(file, node);
    
    return node;
}

void SaveTreeToFile(CustomSceneTree tree, const char* filename) {
    printf("DEBUG: SaveTreeToFile called with filename: %s\n", filename);
    
    if (tree == NULL) {
        printf("Error: Cannot save NULL tree.\n");
        return;
    }

    printf("DEBUG: Opening file for writing...\n");
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return;
    }
    
    printf("DEBUG: Writing file header...\n");
    // Write a file header/version for future compatibility
    char header[16] = "VSTORY_V1.0";
    size_t written = fwrite(header, sizeof(char), 16, file);
    if (written != 16) {
        printf("Error: Failed to write file header.\n");
        fclose(file);
        return;
    }
    
    printf("DEBUG: Starting tree serialization...\n");
    // Serialize the entire tree
    SerializeTreeNode(file, tree);
    
    printf("DEBUG: Checking for write errors...\n");
    // Check for any write errors
    if (ferror(file)) {
        printf("Error: Write error occurred while saving.\n");
        clearerr(file);
        fclose(file);
        return;
    }
    
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
    char header[MAX_HEADER_LENGTH] = {0};
    if (fread(header, sizeof(char), MAX_HEADER_LENGTH, file) != MAX_HEADER_LENGTH) {
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
    printf("DEBUG: SaveSlotToFile called with slot %d\n", slotNumber);
    
    if (ThisSlot == NULL || *ThisSlot == NULL) {
        printf("Error: No story to save.\n");
        return;
    }
    
    if (slotNumber < 1 || slotNumber > 10) {
        printf("Error: Invalid slot number. Use 1-10.\n");
        return;
    }
    
    char filename[MAX_PATH_LENGTH];
    if (snprintf(filename, MAX_PATH_LENGTH, "saves/story_slot_%d.sav", slotNumber) >= MAX_PATH_LENGTH) {
        printf("Error: Save filename too long\n");
        return;
    }
    
    printf("DEBUG: Creating saves directory...\n");
    // Create saves directory if it doesn't exist (platform dependent)
    #ifdef _WIN32
        system("mkdir saves 2>nul");
    #else
        system("mkdir -p saves");
    #endif
    
    printf("DEBUG: Testing file creation...\n");
    // Try to create/open the file first to verify we can write
    FILE *testFile = fopen(filename, "wb");
    if (testFile == NULL) {
        printf("Error: Cannot create save file. Check permissions and disk space.\n");
        return;
    }
    fclose(testFile);
    
    printf("DEBUG: Calling SaveTreeToFile...\n");
    // Now actually save the tree
    SaveTreeToFile(*ThisSlot, filename);
    printf("DEBUG: SaveSlotToFile completed\n");
}


CustomSceneTree LoadSlotFromFile(int slotNumber) {
    if (slotNumber < 1 || slotNumber > 3) {
        printf("Error: Invalid slot number. Use 1-3.\n");
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

void LoadSlot(CustomSceneTree *ThisSlot, int slotnumber) {
    if (ThisSlot != NULL && *ThisSlot != NULL) {
        // Clean up existing tree first
        DeleteTreeNode(ThisSlot);
    }
    
    // For now, load from slot 1 by default
    // You can modify this to ask user for slot number
    *ThisSlot = LoadSlotFromFile(slotnumber);
    
    if (*ThisSlot != NULL) {
        printf("===== LOAD SUCCESSFUL =====\n");
        printf("Story loaded from slot %i\n");
        printf("===========================\n");
    } else {
        printf("===== LOAD FAILED =====\n");
        printf("Could not load story from slot 1\n");
        printf("Creating new story...\n");
        printf("=======================\n");
        
        // Create a new empty tree if load failed
        InitializeStoryTree(ThisSlot);
    }
    MakeCustomStory(ThisSlot, slotnumber);
}

void SaveCustomStoryProgress(const char *filename, int node, int scene) {
    // Create saves directory if it doesn't exist
    #ifdef _WIN32
        system("mkdir saves 2>nul");
    #else
        system("mkdir -p saves");
    #endif

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Could not save progress.\n");
        return;
    }

    // Save node and scene indices
    fwrite(&node, sizeof(int), 1, file);
    fwrite(&scene, sizeof(int), 1, file);

    fclose(file);
}

void LoadCustomStoryProgress(const char *filename, int *node, int *scene) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("No saved progress found.\n");
        return;
    }

    // Read node and scene indices
    fread(node, sizeof(int), 1, file);
    fread(scene, sizeof(int), 1, file);

    fclose(file);
}

int UpdateCustomStory(CustomSceneTree tree, int *currentNode, int *currentScene) {
    // Navigasi node (LEFT/RIGHT) dan scene (UP/DOWN)
    CustomSceneTree node = tree;
    // Cari node saat ini berdasarkan ID
    TempQueueNode *front = NULL, *rear = NULL;
    Enqueue(&front, &rear, tree, 0);
    while (front) {
        TempQueueNode *qNode = Dequeue(&front, &rear);
        CustomSceneTree current = qNode->treeNode;
        if (current->ID == *currentNode) {
            node = current;
            free(qNode);
            break;
        }
        if (current->Left) Enqueue(&front, &rear, current->Left, 0);
        if (current->Right) Enqueue(&front, &rear, current->Right, 0);
        free(qNode);
    }
    while (front) { TempQueueNode *temp = Dequeue(&front, &rear); free(temp); }

    // Get current scene
    SceneList scene = node->NodeContents;
    for (int i = 0; i < *currentScene && scene != NULL; i++) scene = scene->Next;
    if (!scene) return GAME_STATE_MAIN_MENU;

    // Check if this is the last scene in the node
    bool isLastScene = (scene->Next == NULL);
    
    Vector2 mouse = GetMousePosition();

    // Handle pause menu
    if (IsKeyPressed(KEY_F1)) {
        return GAME_STATE_PAUSE;
    }

    // Handle choice selection if this is the last scene and node has choices
    if (isLastScene && (node->Left != NULL || node->Right != NULL)) {
        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT / 2 + 275;

        // Left choice button
        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        
        if (CheckCollisionPointRec(mouse, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (node->Left != NULL) {
                node = node->Left;
                *currentNode = node->ID;
                *currentScene = 0;
                return GAME_STATE_PLAY_CUSTOM_STORY;
            }
        }

        // Right choice button
        if (node->Right != NULL) {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            
            if (CheckCollisionPointRec(mouse, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                node = node->Right;
                *currentNode = node->ID;
                *currentScene = 0;
                return GAME_STATE_PLAY_CUSTOM_STORY;
            }
        }
    } else {
        // Handle scene progression with mouse click or keyboard
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (scene->Next != NULL) {
                (*currentScene)++;
            } else if (node->Left == NULL && node->Right == NULL) {
                // End of story - return to main menu
                return GAME_STATE_MAIN_MENU;
            }
        }
    }

    // Handle keyboard navigation (for debugging/development)
    if (IsKeyPressed(KEY_LEFT) && node->Left != NULL) {
        node = node->Left;
        *currentNode = node->ID;
        *currentScene = 0;
    } else if (IsKeyPressed(KEY_RIGHT) && node->Right != NULL) {
        node = node->Right;
        *currentNode = node->ID;
        *currentScene = 0;
    }
    
    // Navigasi scene dengan keyboard
    if (IsKeyPressed(KEY_UP) && scene && scene->Before != NULL) {
        (*currentScene)--;
    } else if (IsKeyPressed(KEY_DOWN) && scene && scene->Next != NULL) {
        (*currentScene)++;
    }
    
    // Keluar
    if (IsKeyPressed(KEY_ESCAPE)) {
        char filename[64];
        sprintf(filename, "saves/custom_save_%d.dat", *currentNode);
        SaveCustomStoryProgress(filename, *currentNode, *currentScene);
        return GAME_STATE_MAIN_MENU;
    }
    
    return GAME_STATE_PLAY_CUSTOM_STORY;
}

void DrawCustomStoryScreen(CustomSceneTree tree, int currentNode, int currentScene) {
    // Cari node saat ini berdasarkan ID
    CustomSceneTree node = tree;
    TempQueueNode *front = NULL, *rear = NULL;
    Enqueue(&front, &rear, tree, 0);
    while (front) {
        TempQueueNode *qNode = Dequeue(&front, &rear);
        CustomSceneTree current = qNode->treeNode;
        if (current->ID == currentNode) {
            node = current;
            free(qNode);
            break;
        }
        if (current->Left) Enqueue(&front, &rear, current->Left, 0);
        if (current->Right) Enqueue(&front, &rear, current->Right, 0);
        free(qNode);
    }
    while (front) { TempQueueNode *temp = Dequeue(&front, &rear); free(temp); }

    // Ambil scene ke-currentScene
    SceneList scene = node->NodeContents;
    for (int i = 0; i < currentScene && scene != NULL; i++) scene = scene->Next;
    if (!scene) return;

    // Gambar background
    Texture2D bgTex = {0};
    if (scene->Data.Background && strlen(scene->Data.Background) > 0) {
        bgTex = LoadTexture(scene->Data.Background);
        bgTex.height = SCREEN_HEIGHT;
        bgTex.width = SCREEN_WIDTH;
        DrawTexture(bgTex, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }
    
    // Gambar karakter
    Texture2D charTex = {0};
    if (scene->Data.Character && strlen(scene->Data.Character) > 0) {
        charTex = LoadTexture(scene->Data.Character);
        charTex.height /= 2;
        charTex.width /= 2;
        DrawCharacterAtPosition(charTex, scene->Data.charPosition);
        // UnloadTexture(charTex); // (optional, but don't unload if reused)
    }
    
    // Gambar kotak dialog
    if (scene->Data.Convo && strlen(scene->Data.Convo) > 0) {
        DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, Fade(BLACK, 0.8f));
        DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
        DrawText(scene->Data.Convo, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }
    
    // Check if this is the last scene and node has choices
    bool isLastScene = (scene->Next == NULL);
    if (isLastScene && (node->Left != NULL || node->Right != NULL)) {
        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT / 2 + 275;

        // Draw left choice button
        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
        
        const char* leftText = node->TextLeft ? node->TextLeft : "Go Left";
        Vector2 leftTextPos = {
            choiceRectLeft.x + choiceButtonWidth/2 - MeasureText(leftText, 25)/2,
            choiceRectLeft.y + choiceButtonHeight/2 - 25/2
        };
        DrawText(leftText, leftTextPos.x, leftTextPos.y, 25, WHITE);

        // Draw right choice button if it exists
        if (node->Right != NULL) {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
            
            const char* rightText = node->TextRight ? node->TextRight : "Go Right";
            Vector2 rightTextPos = {
                choiceRectRight.x + choiceButtonWidth/2 - MeasureText(rightText, 25)/2,
                choiceRectRight.y + choiceButtonHeight/2 - 25/2
            };
            DrawText(rightText, rightTextPos.x, rightTextPos.y, 25, WHITE);
        }
    }
    
    // Petunjuk navigasi (only show in debug mode or when needed)
    // DrawText("[LEFT/RIGHT]: Pindah node | [UP/DOWN]: Scene | [ESC]: Kembali", 60, 60, 28, YELLOW);
    
    // Don't unload textures here as they might be reused
}

void ChoosingChoiceText(char *LeftText, char *RightText, int *selectedsprite, CustomSceneTree *TempTree)
{
    static bool isLeftActive = true;  // Track which text field is active
    static int leftCount = 0;
    static int rightCount = 0;
    int key;

    while (1)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // Display input fields with instructions
        DrawText("Enter text for both choices:", 40, 30, 30, WHITE);
        DrawText("TAB to switch between choices, ENTER when done", 40, 60, 20, LIGHTGRAY);
        
        // Draw left choice input field with active highlighting
        DrawText("Left Choice:", 40, 100, 20, WHITE);
        DrawRectangleLines(40, 130, 720, 40, isLeftActive ? YELLOW : WHITE);
        DrawText(LeftText, 50, 140, 20, isLeftActive ? YELLOW : LIGHTGRAY);
        
        // Draw right choice input field with active highlighting
        DrawText("Right Choice:", 40, 200, 20, WHITE);
        DrawRectangleLines(40, 230, 720, 40, !isLeftActive ? YELLOW : WHITE);
        DrawText(RightText, 50, 240, 20, !isLeftActive ? YELLOW : LIGHTGRAY);

        // Draw preview choice buttons matching story.c style
        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT / 2 + 275;

        // Left choice preview button
        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
        DrawText(strlen(LeftText) > 0 ? LeftText : "Preview Left Choice",
                (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText(strlen(LeftText) > 0 ? LeftText : "Preview Left Choice", 25) / 2),
                (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                25, WHITE);

        // Right choice preview button
        Rectangle choiceRectRight = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
        DrawText(strlen(RightText) > 0 ? RightText : "Preview Right Choice",
                (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText(strlen(RightText) > 0 ? RightText : "Preview Right Choice", 25) / 2),
                (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                25, WHITE);

        // Handle keyboard input
        key = GetCharPressed();
        while (key > 0)
        {
            if (isLeftActive && leftCount < DUMMY_MAX_CONVO-1)
            {
                if (key >= 32 && key <= 125)
                {
                    LeftText[leftCount] = (char)key;
                    LeftText[++leftCount] = '\0';
                }
            }
            else if (!isLeftActive && rightCount < DUMMY_MAX_CONVO-1)
            {
                if (key >= 32 && key <= 125)
                {
                    RightText[rightCount] = (char)key;
                    RightText[++rightCount] = '\0';
                }
            }
            key = GetCharPressed();
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (isLeftActive && leftCount > 0)
            {
                LeftText[--leftCount] = '\0';
            }
            else if (!isLeftActive && rightCount > 0)
            {
                RightText[--rightCount] = '\0';
            }
        }

        // Switch between fields with TAB
        if (IsKeyPressed(KEY_TAB))
        {
            isLeftActive = !isLeftActive;
        }

        // Complete input and show confirmation when both fields have text
        if (IsKeyPressed(KEY_ENTER) && strlen(LeftText) > 0 && strlen(RightText) > 0)
        {
            // Save the choice texts to the tree node
            (*TempTree)->TextLeft = malloc(strlen(LeftText) + 1);
            strcpy((*TempTree)->TextLeft, LeftText);
            (*TempTree)->TextRight = malloc(strlen(RightText) + 1);
            strcpy((*TempTree)->TextRight, RightText);

            // Show success confirmation
            float confirmTimer = 2.0f;
            while (confirmTimer > 0.0f)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("Choice texts set successfully!", 40, 30, 30, GREEN);
                DrawText("Left Choice: ", 40, 100, 20, WHITE);
                DrawText(LeftText, 160, 100, 20, YELLOW);
                DrawText("Right Choice: ", 40, 150, 20, WHITE); 
                DrawText(RightText, 160, 150, 20, YELLOW);
                EndDrawing();
                confirmTimer -= GetFrameTime();
            }

            // Reset for next time
            isLeftActive = true;
            leftCount = 0;
            rightCount = 0;
            *selectedsprite = 4; // Signal successful completion
            return;
        }

        EndDrawing();
    }
}


