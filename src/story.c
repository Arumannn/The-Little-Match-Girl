#include "story.h"
#include "mainmenu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h> 

#define MAX_SCENE 10
#define MAX_NODE_TREE 28

Tree SceneTree[MAX_NODE_TREE];
int currentScene = 0; 
int currentFrameIndex = 0;

void InitDataCerita(){
 
    SceneTree[0].id = 0;
    SceneTree[0].TotalScene = 4;
    SceneTree[0].numChoices = 2;
    SceneTree[0].choiceLeftSon = "Go to Alleway";
    SceneTree[0].choiceRightSon = "Go to Open Street";

    // Scenes 0
    SceneTree[0].Frame[0].backgroundPath = "Assets/BackgroundSprites/background4.png";
    SceneTree[0].Frame[0].characterPath = NULL;
    SceneTree[0].Frame[0].backgroundSound = NULL; // Masih Test
    SceneTree[0].Frame[0].dialogue = "......";

    SceneTree[0].Frame[1].backgroundPath = "Assets/BackgroundSprites/background3.png";
    SceneTree[0].Frame[1].characterPath = "Assets/CharaSprites/chara3.png";
    SceneTree[0].Frame[1].backgroundSound = NULL; // Masih Test
    SceneTree[0].Frame[1].dialogue = "It is so cold here, but i have to sell all these matches";
    SceneTree[0].Frame[1].CharPosition = CHAR_POS_CENTER;

    SceneTree[0].Frame[2].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[0].Frame[2].characterPath = "Asssets/CharaSprites/chara3.png";
    SceneTree[0].Frame[2].backgroundSound = NULL; // Masih Test
    SceneTree[0].Frame[2].CharPosition = CHAR_POS_CENTER;
    SceneTree[0].Frame[2].dialogue = "What should i go, Alleway?";

    SceneTree[0].Frame[3].backgroundPath = "Assets/BackgroundSprites/background7.png";
    SceneTree[0].Frame[3].characterPath = "Asssets/CharaSprites/chara3.png";
    SceneTree[0].Frame[3].backgroundSound = NULL; // Masih Test
    SceneTree[0].Frame[3].CharPosition = CHAR_POS_CENTER;
    SceneTree[0].Frame[3].dialogue = "Or the Street?";

    SceneTree[1].id = 1;
    SceneTree[1].TotalScene = 1;
    SceneTree[1].numChoices = 2;
    SceneTree[1].choiceLeftSon = "Light on a few matches";
    SceneTree[1].choiceRightSon = "Ignore it, Keep Walking";

    SceneTree[1].Frame[0].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[1].Frame[0].characterPath = "Assets/CharaSprites/chara3.png";
    SceneTree[1].Frame[0].backgroundSound = NULL; // Masih Test
    SceneTree[1].Frame[0].CharPosition = CHAR_POS_LEFT;
    SceneTree[1].Frame[0].dialogue = "I'm Think that guy is feel cold, should i help him?";


    //ENDING
    SceneTree[13].id = 13;
    SceneTree[13].TotalScene = 1;
    SceneTree[13].numChoices = 0;

    SceneTree[13].Frame[1].backgroundPath = "Assets/BackgroundSprites/background5.png";
    SceneTree[13].Frame[1].backgroundSound = NULL; // Masih Test
    SceneTree[13].Frame[1].dialogue = "I'm Think that guy is feel cold, should i help him?";

    SceneTree[13].Frame[2].backgroundPath = "Assets/BackgroundSprites/background5.png";
    SceneTree[13].Frame[2].backgroundSound = NULL; // Masih Test
    SceneTree[13].Frame[2].dialogue = "No, I can handle it";

    SceneTree[13].Frame[3].backgroundPath = "Assets/BackgroundSprites/background5.png";
    SceneTree[13].Frame[3].backgroundSound = NULL; // Masih Test
    SceneTree[13].Frame[3].dialogue = ".....";


}

static void DrawCharacterAtPosition(Texture2D tex, CharacterPosition pos) {
    if (tex.id == 0 || pos == CHAR_POS_NONE) return;

    float charX = 0;
    // Asumsi SCREEN_HEIGHT sudah dijangkau dari mainmenu.h atau didefinisikan ulang di story.c
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

void UnloadNodeAssets(int nodeIndex){
    
    if (nodeIndex < 0 || nodeIndex > MAX_NODE_TREE)
    {
        printf("Node dengan index tersebut tidak ada\n");
        return;
    }
    
    // Unload node assets
    Tree *UnloadTemp = &SceneTree[nodeIndex];

    for (int i = 0; i <= UnloadTemp->TotalScene; i++)
    {
        Scene *SceneData = &UnloadTemp->Frame[i];
        if (SceneData->backgroundTex.id != 0)
        {
            UnloadTexture(SceneData->backgroundTex);
            SceneData->backgroundTex = (Texture2D) {0};
        }
        if (SceneData->characterTex.id != 0)
        {
            UnloadTexture(SceneData->characterTex);
            SceneData->characterTex = (Texture2D) {0};
        }
    }
}

void LoadNodeAssets(int nodeIndex){
    Tree *NodeCur = &SceneTree[nodeIndex];
    for (int i = 0; i < NodeCur->TotalScene; i++)
    {
        Scene *SceneDataCur = &NodeCur->Frame[i];
        if (SceneDataCur->backgroundPath != NULL)
        {
            SceneDataCur->backgroundTex = LoadTexture(SceneDataCur->backgroundPath);
            SceneDataCur->backgroundTex.height = SCREEN_HEIGHT;
            SceneDataCur->backgroundTex.width = SCREEN_WIDTH;
        }else SceneDataCur->backgroundTex = (Texture2D) {0};
        if (SceneDataCur->characterPath != NULL)
        {
            SceneDataCur->characterTex = LoadTexture(SceneDataCur->characterPath);
            SceneDataCur->characterTex.height /= 2;
            SceneDataCur->characterTex.width /= 2;
        }else SceneDataCur->characterTex = (Texture2D) {0};
    }
    
}

void DrawCurrentNodeScreen(){
    Tree *NodeCur = &SceneTree[currentScene];
    if (currentScene < 0 || currentScene >= MAX_NODE_TREE || NodeCur->TotalScene == 0) {
        ClearBackground(BLACK);
        //EACH ENDING
    }

    Scene *SceneDataCur = &NodeCur->Frame[currentFrameIndex];

    if (SceneDataCur->backgroundTex.id != 0) {
        DrawTexture(SceneDataCur->backgroundTex, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }
    DrawCharacterAtPosition(SceneDataCur->characterTex, SceneDataCur->CharPosition);

    DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 150, Fade(BLACK, 0.7f));
    DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 150, WHITE);

    if (SceneDataCur->dialogue != NULL) {
        DrawText(SceneDataCur->dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }

    if (currentFrameIndex == NodeCur->TotalScene - 1 && NodeCur->numChoices > 0) {
        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT - 400;

        if (NodeCur->numChoices >= 1) {
            Rectangle choiceRectLeft = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectLeft, Fade(BLUE, 0.8f));
            DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
            DrawText(NodeCur->choiceLeftSon,
                     (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText(NodeCur->choiceLeftSon, 25) / 2),
                     (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                     25, WHITE);
        }

        if (NodeCur->numChoices == 2) {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2,
                choiceStartY + choiceButtonHeight + 10,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectRight, Fade(BLUE, 0.8f));
            DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
            DrawText(NodeCur->choiceRightSon,
                     (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText(NodeCur->choiceRightSon, 25) / 2),
                     (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                     25, WHITE);
        }
    }
}




void UpdateCerita(int *GameState){
    Tree NodeCur = SceneTree[currentScene];
    Vector2 mouse = GetMousePosition();
    if (currentScene < 0 || currentScene > MAX_NODE_TREE || NodeCur.TotalScene == 0)
    {
        if (*GameState != 0)
        {
            *GameState = 0;
            // PlayBackGroundMusic(NULL);
        }
        return;        
    }

    if (currentFrameIndex == NodeCur.TotalScene - 1 && NodeCur.numChoices > 0)
    {
        Vector2 mousePos = GetMousePosition();

        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT - 400;

        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        if (CheckCollisionPointRec(mousePos, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ProsesChoice(0);
            return;
        }
        
        if (currentFrameIndex == NodeCur.TotalScene - 1)
        {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2,
                choiceStartY + choiceButtonHeight + 10,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            if (CheckCollisionPointRec(mousePos, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ProsesChoice(1);
                return;
            }
        }
    } else{
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER))
        {
            if (currentFrameIndex < NodeCur.TotalScene - 1)
            {
                currentFrameIndex++;
            }else printf("ENDING\n");
            
        }
    }
    // if (currentBGM.stream.buffer != NULL) {
    //     UpdateMusicStream(currentBGM);
    // }
}

void ProsesChoice(int choice){
    int nextNodeIndex = -1;
    extern int currentGameState;

    if (choice == 0)
    {
        nextNodeIndex = currentScene * 2 + 1;
    }else if(choice == 1){
        nextNodeIndex = currentScene * 2 + 2;
    }
    
    if (nextNodeIndex < 0 || nextNodeIndex > MAX_NODE_TREE || SceneTree[nextNodeIndex].TotalScene == 0)
    {
        printf("Ending Cerita or invalid cerita\n");
    }
    
    UnloadNodeAssets(currentScene);
    currentScene = nextNodeIndex;
    currentFrameIndex = 0;

    LoadNodeAssets(currentScene);
    printf("Berpindah ke scene %d\n", currentScene);
}