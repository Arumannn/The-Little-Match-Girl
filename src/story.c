#include "story.h"
#include "mainmenu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h> 

#define MAX_SCENE 10
#define MAX_NODE_TREE 28


int currentScene = 0; 
int currentFrame = 0;
float frameDelay = 2.0f;
float frameTimer = 0.0f;


// Inisialisasi Data untuk cerita dan juga setiap frame dari node
void InitDataCerita(Tree * SceneTree){
 
    SceneTree[0].id = 0;
    SceneTree[0].TotalScene = 4;
    SceneTree[0].numChoices = 2;
    SceneTree[0].choiceLeftSon = "Go to Alleway";
    SceneTree[0].choiceRightSon = "Go to Open Street";
    SceneTree[0].IdLeftSon = 1;
    SceneTree[0].IdRightSon = 2;

    // Scenes 0
    SceneTree[0].Frame[0].backgroundPath = "Assets/BackgroundSprites/background4.png";
    SceneTree[0].Frame[0].characterPath = NULL;
    SceneTree[0].Frame[0].backgroundSound = NULL; 
    SceneTree[0].Frame[0].dialogue = "Girl : ......";

    SceneTree[0].Frame[1].backgroundPath = "Assets/BackgroundSprites/background3.png";
    SceneTree[0].Frame[1].characterPath = "Assets/CharaSprites/chara3.png";
    SceneTree[0].Frame[1].backgroundSound = NULL; 
    SceneTree[0].Frame[1].dialogue = "Girl : It is so cold here, but i have to sell all these matches";
    SceneTree[0].Frame[1].CharPosition = CHAR_POS_CENTER;

    SceneTree[0].Frame[2].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[0].Frame[2].characterPath = "Asssets/CharaSprites/chara3.png";
    SceneTree[0].Frame[2].backgroundSound = NULL; 
    SceneTree[0].Frame[2].CharPosition = CHAR_POS_CENTER;
    SceneTree[0].Frame[2].dialogue = "Girl : What should i go, Alleway?";

    SceneTree[0].Frame[3].backgroundPath = "Assets/BackgroundSprites/background7.png";
    SceneTree[0].Frame[3].characterPath = "Asssets/CharaSprites/chara3.png";
    SceneTree[0].Frame[3].backgroundSound = NULL; 
    SceneTree[0].Frame[3].CharPosition = CHAR_POS_CENTER;
    SceneTree[0].Frame[3].dialogue = "Girl : Or the Street?";

    SceneTree[1].id = 1;
    SceneTree[1].TotalScene = 2;
    SceneTree[1].numChoices = 2;
    SceneTree[1].choiceLeftSon = "Light on a few matches";
    SceneTree[1].choiceRightSon = "Ignore it, Keep Walking";
    SceneTree[1].IdLeftSon = 3;
    SceneTree[1].IdRightSon = 4;

    SceneTree[1].Frame[0].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[1].Frame[0].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[1].Frame[0].backgroundSound = NULL; 
    SceneTree[1].Frame[0].CharPosition = CHAR_POS_LEFT;
    SceneTree[1].Frame[0].dialogue = " Girl : I'm Think that guy is feel cold, should i help him?";

    SceneTree[1].Frame[1].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[1].Frame[1].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[1].Frame[1].backgroundSound = NULL; 
    SceneTree[1].Frame[1].CharPosition = CHAR_POS_LEFT;
    SceneTree[1].Frame[1].dialogue = "";

    SceneTree[2].id = 2;
    SceneTree[2].TotalScene = 2;
    SceneTree[2].numChoices = 2;
    SceneTree[2].choiceLeftSon = "Walk crosing the street";
    SceneTree[2].choiceRightSon = "Walk along the sidewalks";
    SceneTree[2].IdLeftSon = 5;
    SceneTree[2].IdRightSon = 6;

    SceneTree[2].Frame[0].backgroundPath = "Assets/BackgroundSprites/background8.png";
    SceneTree[2].Frame[0].characterPath = NULL;
    SceneTree[2].Frame[0].backgroundSound = NULL; 
    SceneTree[2].Frame[0].CharPosition = CHAR_POS_NONE;
    SceneTree[2].Frame[0].dialogue = "";

    SceneTree[2].Frame[1].backgroundPath = "Assets/BackgroundSprites/background8.png";
    SceneTree[2].Frame[1].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[2].Frame[1].backgroundSound = NULL; 
    SceneTree[2].Frame[1].CharPosition = CHAR_POS_LEFT;
    SceneTree[2].Frame[1].dialogue = "Girl : Would these people be in need of matches?";

    
    SceneTree[3].id = 3;
    SceneTree[3].TotalScene = 5;
    SceneTree[3].numChoices = 2;
    SceneTree[3].choiceLeftSon = "Give up all the match";
    SceneTree[3].choiceRightSon = "Protect the match";
    SceneTree[3].IdLeftSon = 7;
    SceneTree[3].IdRightSon = 8;
    
    SceneTree[3].Frame[0].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[0].characterPath = "Assets/CharaSprites/chara2.png";
    SceneTree[3].Frame[0].backgroundSound = NULL; 
    SceneTree[3].Frame[0].CharPosition = CHAR_POS_RIGHT;
    SceneTree[3].Frame[0].dialogue = "Homeless man: Thank you kid, here's 2 dollars for the match";

    SceneTree[3].Frame[1].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[1].characterPath = "Assets/CharaSprites/chara4.png";
    SceneTree[3].Frame[1].backgroundSound = NULL; 
    SceneTree[3].Frame[1].CharPosition = CHAR_POS_RIGHT;
    SceneTree[3].Frame[1].dialogue = "Homeless man: Yeah, thank you so much";

    SceneTree[3].Frame[2].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[2].characterPath = "Assets/CharaSprites/chara2.png";
    SceneTree[3].Frame[2].backgroundSound = NULL; 
    SceneTree[3].Frame[2].CharPosition = CHAR_POS_RIGHT;
    SceneTree[3].Frame[2].dialogue = "Homeless man: Give me the match too";

    SceneTree[3].Frame[3].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[3].characterPath = "Assets/CharaSprites/chara4.png";
    SceneTree[3].Frame[3].backgroundSound = NULL; 
    SceneTree[3].Frame[3].CharPosition = CHAR_POS_RIGHT;
    SceneTree[3].Frame[3].dialogue = "Homeless man: Hey I want too";

    SceneTree[3].Frame[4].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[4].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[3].Frame[4].backgroundSound = NULL; 
    SceneTree[3].Frame[4].CharPosition = CHAR_POS_LEFT;
    SceneTree[3].Frame[4].dialogue = "Girl : I'm Scared, please don't hurt me";
    
    SceneTree[3].Frame[5].backgroundPath = "Assets/BackgroundSprites/background6.png";
    SceneTree[3].Frame[5].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[3].Frame[5].backgroundSound = NULL; 
    SceneTree[3].Frame[5].CharPosition = CHAR_POS_LEFT;
    SceneTree[3].Frame[5].dialogue = "";
    

    // NODE 4 dan 5 ENDING SCENE, nanti
    SceneTree[5].id = 5;
    SceneTree[5].TotalScene = 4;
    SceneTree[5].numChoices = 1;
    SceneTree[5].IdLeftSon = -1;
    SceneTree[5].IdRightSon = -1;
    SceneTree[5].choiceLeftSon = NULL;
    SceneTree[5].choiceRightSon = NULL;

    SceneTree[5].Frame[0].backgroundPath = "Assets/Endings/crossingtheroad1.png";
    SceneTree[5].Frame[0].characterPath = NULL;
    SceneTree[5].Frame[0].backgroundSound = NULL; 
    SceneTree[5].Frame[0].CharPosition = CHAR_POS_NONE;
    SceneTree[5].Frame[0].dialogue = NULL;

    SceneTree[5].Frame[1].backgroundPath = "Assets/Endings/crossingtheroad2.png";
    SceneTree[5].Frame[1].characterPath = NULL;
    SceneTree[5].Frame[1].backgroundSound = NULL; 
    SceneTree[5].Frame[1].CharPosition = CHAR_POS_NONE;
    SceneTree[5].Frame[1].dialogue = NULL;

    SceneTree[5].Frame[2].backgroundPath = "Assets/Endings/crossingtheroad3.png";
    SceneTree[5].Frame[2].characterPath = NULL;
    SceneTree[5].Frame[2].backgroundSound = NULL; 
    SceneTree[5].Frame[2].CharPosition = CHAR_POS_NONE;
    SceneTree[5].Frame[2].dialogue = NULL;

    SceneTree[5].Frame[3].backgroundPath = "Assets/Endings/crossingtheroad4.png";
    SceneTree[5].Frame[3].characterPath = NULL;
    SceneTree[5].Frame[3].backgroundSound = NULL; 
    SceneTree[5].Frame[3].CharPosition = CHAR_POS_NONE;
    SceneTree[5].Frame[3].dialogue = NULL;
    //========================================


    SceneTree[6].id = 6;
    SceneTree[6].TotalScene = 5;
    SceneTree[6].numChoices = 2;
    SceneTree[6].choiceLeftSon = "Try to pickpocket";
    SceneTree[6].choiceRightSon = "Offering the match";
    SceneTree[6].IdLeftSon = 9;
    SceneTree[6].IdRightSon = 10;

    
    SceneTree[6].Frame[0].backgroundPath = "Assets/BackgroundSprites/background10.png";
    SceneTree[6].Frame[0].characterPath = "Assets/CharaSprites/chara3.png";
    SceneTree[6].Frame[0].backgroundSound = NULL; 
    SceneTree[6].Frame[0].CharPosition = CHAR_POS_CENTER;
    SceneTree[6].Frame[0].dialogue = "Walking....";

    SceneTree[6].Frame[1].backgroundPath = "Assets/BackgroundSprites/background11.png";
    SceneTree[6].Frame[1].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[6].Frame[1].backgroundSound = NULL; 
    SceneTree[6].Frame[1].CharPosition = CHAR_POS_LEFT;
    SceneTree[6].Frame[1].dialogue = "Girl: Anyone, need a box match?";

    SceneTree[6].Frame[2].backgroundPath = "Assets/BackgroundSprites/background12.png";
    SceneTree[6].Frame[2].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[6].Frame[2].backgroundSound = NULL; 
    SceneTree[6].Frame[2].CharPosition = CHAR_POS_LEFT;
    SceneTree[6].Frame[2].dialogue = "Girl: Just only 5$ a box!";

    SceneTree[6].Frame[3].backgroundPath = "Assets/BackgroundSprites/background9.png";
    SceneTree[6].Frame[3].characterPath = "Assets/CharaSprites/chara3.png";
    SceneTree[6].Frame[3].backgroundSound = NULL; 
    SceneTree[6].Frame[3].CharPosition = CHAR_POS_LEFT;
    SceneTree[6].Frame[3].dialogue = "Girl : 5$ for a box matach will keep you warm";

    SceneTree[6].Frame[4].backgroundPath = "Assets/BackgroundSprites/background13.png";
    SceneTree[6].Frame[4].characterPath = "Assets/CharaSprites/chara5.png";
    SceneTree[6].Frame[4].backgroundSound = NULL; 
    SceneTree[6].Frame[4].CharPosition = CHAR_POS_LEFT;
    SceneTree[6].Frame[4].dialogue = "Girl : Should i offer one to that man?";
    
}  

//Menentukan posisi karakter ketika digambar
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


//Menghapus gambar dari layar, agar tidak memberatkan
void UnloadNodeAssets(Tree SceneTree[], int nodeIndex){
    
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

void LoadNodeAssets(Tree SceneTree[], int nodeIndex){
    Tree *NodeCur = &SceneTree[nodeIndex];

    if (NodeCur->TotalScene == 0) {
        printf("node %d nilai TotalScene = 0\n", nodeIndex);
        return;
    }
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

void DrawCurrentNodeScreen(Tree SceneTree[]){
    Tree *NodeCur = &SceneTree[currentScene];
    Scene *SceneDataCur = &NodeCur->Frame[currentFrame];

    if (currentGameState == GAME_STATE_STORY) {
        
        if (SceneDataCur->backgroundTex.id != 0) {
            DrawTexture(SceneDataCur->backgroundTex, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE); 
        }

      
        if (NodeCur->id != 5) {
            DrawCharacterAtPosition(SceneDataCur->characterTex, SceneDataCur->CharPosition);

  
            if (SceneDataCur->dialogue != NULL || (currentFrame == NodeCur->TotalScene - 1 && NodeCur->numChoices > 0)) {
                DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, BLACK);
                DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);

                if (SceneDataCur->dialogue != NULL) {
                    DrawText(SceneDataCur->dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
                }
            }

            if (currentFrame == NodeCur->TotalScene - 1 && NodeCur->numChoices > 0) {
                int choiceButtonWidth = 400;
                int choiceButtonHeight = 60;
                int yButton = SCREEN_HEIGHT / 2 + 275;

                if (NodeCur->numChoices >= 1) {
                    Rectangle choiceRectLeft = {
                        SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                        yButton,
                        (float)choiceButtonWidth,
                        (float)choiceButtonHeight
                    };
                    DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
                    DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
                    DrawText(NodeCur->choiceLeftSon,
                             (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText(NodeCur->choiceLeftSon, 30) / 2),
                             (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                             25, WHITE);
                }

                if (NodeCur->numChoices == 2) {
                    Rectangle choiceRectRight = {
                        SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                        yButton,
                        (float)choiceButtonWidth,
                        (float)choiceButtonHeight
                    };
                    DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
                    DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
                    DrawText(NodeCur->choiceRightSon,
                             (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText(NodeCur->choiceRightSon, 25) / 2),
                             (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                             25, WHITE);
                }
            }
        }
        
    }
    else { 
        if(SceneDataCur->backgroundTex.id != 0) {
            DrawTexture(SceneDataCur->backgroundTex, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE); 
        }
    }
}




void UpdateCerita(Tree SceneTree[], GameState *GameState){
    Tree NodeCur = SceneTree[currentScene];
    Vector2 mouse = GetMousePosition();
     
    if (currentScene < 0 || currentScene > MAX_NODE_TREE || NodeCur.TotalScene == 0)
    {
        if (*GameState != GAME_STATE_MAIN_MENU)
        {
            *GameState = GAME_STATE_MAIN_MENU;
            // PlayBackGroundMusic(NULL);
        }
        return;        
    }

    if (currentScene != 5)
    {
        if (currentFrame == NodeCur.TotalScene - 1 && NodeCur.numChoices > 0)
    {
        Vector2 mousePos = GetMousePosition();

        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT / 2 + 275;

        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        if (CheckCollisionPointRec(mousePos, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ProsesChoice(SceneTree, 0);
            return;
        }
        
        if (currentFrame == NodeCur.TotalScene - 1)
        {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight

                
            };
            if (CheckCollisionPointRec(mousePos, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                ProsesChoice(SceneTree, 1);
                return;
            }
        }
    } else{
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER))
        {
            if (currentFrame< NodeCur.TotalScene - 1)
            {
                currentFrame++;
            }
            
        }
     }

    }else{
        frameTimer += GetFrameTime(); 

        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f; 
            if (currentFrame < NodeCur.TotalScene - 1) {
                currentFrame++;
            } else {
                UnloadNodeAssets(SceneTree, currentScene); 
                *GameState = GAME_STATE_MAIN_MENU; 
                currentScene = 0;        
                currentFrame = 0; 
                // PlayBackGroundMusic(NULL); 
            }
        }
    }    
    // if (currentBGM.stream.buffer != NULL) {
    //     UpdateMusicStream(currentBGM);
    // }
}

void ProsesChoice(Tree SceneTree[], int choice){
    int nextNodeIndex = -1;
    

    if (choice == 0)
    {
        nextNodeIndex = SceneTree[currentScene].IdLeftSon;
    }else if(choice == 1){
        nextNodeIndex = SceneTree[currentScene].IdRightSon;
    }
    
    if (nextNodeIndex < 0 || nextNodeIndex > MAX_NODE_TREE || SceneTree[nextNodeIndex].TotalScene == 0)
    {

    }
    
    UnloadNodeAssets(SceneTree, currentScene);
    currentScene = nextNodeIndex;
    currentFrame= 0;

    LoadNodeAssets(SceneTree, currentScene);
    printf("Berpindah ke scene %d\n", currentScene);
}
