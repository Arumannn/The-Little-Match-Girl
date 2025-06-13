#include "story.h"
#include "mainmenu.h"
#include "Queue.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCENE 10
#define MAX_NODE_TREE 28

// Story manager functions
StoryManager* CreateStoryManager(void) {
    StoryManager* manager = (StoryManager*)malloc(sizeof(StoryManager));
    if (!manager) {
        printf("Failed to allocate StoryManager\n");
        return NULL;
    }
    
    // Initialize manager state
    manager->currentScene = 0;
    manager->currentFrame = 0;
    manager->frameDelay = 3.5f;
    manager->frameTimer = 0.0f;
    manager->isMusicPlaying = false;
    manager->currentSceneMusic = (Music){0};
    
    // Initialize scene tree
    for (int i = 0; i < MAX_NODE_TREE; i++) {
        CreateQueue(&manager->sceneTree[i].Frame);
        manager->sceneTree[i].id = i;
        manager->sceneTree[i].TotalScene = 0;
        manager->sceneTree[i].numChoices = 0;
        manager->sceneTree[i].choiceLeftSon = NULL;
        manager->sceneTree[i].choiceRightSon = NULL;
        manager->sceneTree[i].IdLeftSon = -1;
        manager->sceneTree[i].IdRightSon = -1;
    }
    
    return manager;
}

void DestroyStoryManager(StoryManager* manager) {
    if (!manager) return;
    
    // Unload music if playing
    if (manager->isMusicPlaying) {
        UnloadMusicStream(manager->currentSceneMusic);
    }
    
    // Unload all node assets
    for (int i = 0; i < MAX_NODE_TREE; i++) {
        UnloadNodeAssets(manager, i);
    }
    
    free(manager);
}

// Helper functions
void ResetStoryProgress(StoryManager* manager) {
    if (!manager) return;
    manager->currentScene = 0;
    manager->currentFrame = 0;
    manager->frameTimer = 0.0f;
}

int GetCurrentScene(const StoryManager* manager) {
    return manager ? manager->currentScene : 0;
}

int GetCurrentFrame(const StoryManager* manager) {
    return manager ? manager->currentFrame : 0;
}

void SetCurrentScene(StoryManager* manager, int scene) {
    if (manager && scene >= 0 && scene < MAX_NODE_TREE) {
        manager->currentScene = scene;
    }
}

void SetCurrentFrame(StoryManager* manager, int frame) {
    if (manager && frame >= 0) {
        manager->currentFrame = frame;
    }
}

// Inisialisasi Data Cerita
void InitDataCerita(StoryManager* manager) {
    if (!manager) {
        printf("StoryManager is NULL\n");
        return;
    }
    
    printf("Initializing story data...\n");

    // Node 0
    printf("Setting up Node 0...\n");
    manager->sceneTree[0].id = 0;
    manager->sceneTree[0].TotalScene = 4;
    manager->sceneTree[0].numChoices = 2;
    manager->sceneTree[0].choiceLeftSon = "Go to Alleway";
    manager->sceneTree[0].choiceRightSon = "Go to Open Street";
    manager->sceneTree[0].IdLeftSon = 1;
    manager->sceneTree[0].IdRightSon = 2;

    Scene scenes0[] = {
        {"Assets/Music/myinstants.mp3", "Girl : ......", "Assets/BackgroundSprites/background4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : It is so cold here, but i have to sell all these matches", "Assets/BackgroundSprites/background3.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : What should i go, Alleway?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : Or the Street?", "Assets/BackgroundSprites/background7.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER}
    };
    printf("Adding scenes to Node 0...\n");
    for (int i = 0; i < manager->sceneTree[0].TotalScene; i++) {
        printf("  Scene %d:\n", i);
        printf("    Background path: %s\n", scenes0[i].backgroundPath ? scenes0[i].backgroundPath : "NULL");
        printf("    Character path: %s\n", scenes0[i].characterPath ? scenes0[i].characterPath : "NULL");
        printf("    Dialogue: %s\n", scenes0[i].dialogue ? scenes0[i].dialogue : "NULL");
        printf("    Position: %d\n", scenes0[i].CharPosition);
        EnQueueLast(&manager->sceneTree[0].Frame, scenes0[i]);
    }
    manager->sceneTree[0].TotalScene = QueueSize(manager->sceneTree[0].Frame);
    printf("Node 0 setup complete. Total scenes: %d\n", manager->sceneTree[0].TotalScene);

    // Node 1
    manager->sceneTree[1].id = 1;
    manager->sceneTree[1].TotalScene = 2;
    manager->sceneTree[1].numChoices = 2;
    manager->sceneTree[1].choiceLeftSon = "Light on a few matches";
    manager->sceneTree[1].choiceRightSon = "Ignore it, Keep Walking";
    manager->sceneTree[1].IdLeftSon = 3;
    manager->sceneTree[1].IdRightSon = 4;

    Scene scenes1[] = {
        {NULL, " Girl : I'm Think that guy is feel cold, should i help him?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < manager->sceneTree[1].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[1].Frame, scenes1[i]);
    }
    manager->sceneTree[1].TotalScene = QueueSize(manager->sceneTree[1].Frame);

    // Node 2
    manager->sceneTree[2].id = 2;
    manager->sceneTree[2].TotalScene = 2;
    manager->sceneTree[2].numChoices = 2;
    manager->sceneTree[2].choiceLeftSon = "Walk crosing the street";
    manager->sceneTree[2].choiceRightSon = "Walk along the sidewalks";
    manager->sceneTree[2].IdLeftSon = 5;
    manager->sceneTree[2].IdRightSon = 6;

    Scene scenes2[] = {
        {NULL, "", "Assets/BackgroundSprites/background8.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Would these people be in need of matches?", "Assets/BackgroundSprites/background8.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < manager->sceneTree[2].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[2].Frame, scenes2[i]);
    }
    manager->sceneTree[2].TotalScene = QueueSize(manager->sceneTree[2].Frame);

    // Node 3
    manager->sceneTree[3].id = 3;
    manager->sceneTree[3].TotalScene = 5;
    manager->sceneTree[3].numChoices = 2;
    manager->sceneTree[3].choiceLeftSon = "Give up all the match";
    manager->sceneTree[3].choiceRightSon = "Protect the match";
    manager->sceneTree[3].IdLeftSon = 7;
    manager->sceneTree[3].IdRightSon = 8;

    Scene scenes3[] = {
        {NULL, "Homeless man: Thank you kid, here's 2 dollars for the match", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Yeah, thank you so much", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Give me the match too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Hey I want too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl : I'm Scared, please don't hurt me", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < manager->sceneTree[3].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[3].Frame, scenes3[i]);
    }
    manager->sceneTree[3].TotalScene = QueueSize(manager->sceneTree[3].Frame);

    // Node 4 (Ending: Die Alone)
    manager->sceneTree[4].id = 4;
    manager->sceneTree[4].TotalScene = 5;
    manager->sceneTree[4].numChoices = 1;
    manager->sceneTree[4].IdLeftSon = -1;
    manager->sceneTree[4].IdRightSon = -1;
    manager->sceneTree[4].choiceLeftSon = NULL;
    manager->sceneTree[4].choiceRightSon = NULL;

    Scene scenes4[] = {
        {NULL, "GIrl : Itldlaosdlaokd's cold, should i light a few matches?", "Assets/BackgroundSprites/background14.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : No, i can handle it", "Assets/Endings/diecoldalone1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm still can handle it", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm Just need a rest for a moment......", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < manager->sceneTree[4].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[4].Frame, scenes4[i]);
    }
    manager->sceneTree[4].TotalScene = QueueSize(manager->sceneTree[4].Frame);

    // Node 5 (Ending: Crushed by a Car)
    manager->sceneTree[5].id = 5;
    manager->sceneTree[5].TotalScene = 4;
    manager->sceneTree[5].numChoices = 1;
    manager->sceneTree[5].IdLeftSon = -1;
    manager->sceneTree[5].IdRightSon = -1;
    manager->sceneTree[5].choiceLeftSon = NULL;
    manager->sceneTree[5].choiceRightSon = NULL;

    Scene scenes5[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < manager->sceneTree[5].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[5].Frame, scenes5[i]);
    }
    manager->sceneTree[5].TotalScene = QueueSize(manager->sceneTree[5].Frame);

    // Node 6
    manager->sceneTree[6].id = 6;
    manager->sceneTree[6].TotalScene = 5;
    manager->sceneTree[6].numChoices = 2;
    manager->sceneTree[6].choiceLeftSon = "Try to pickpocket";
    manager->sceneTree[6].choiceRightSon = "Offering the match";
    manager->sceneTree[6].IdLeftSon = 9;
    manager->sceneTree[6].IdRightSon = 10;

    Scene scenes6[] = {
        {NULL, "Walking....", "Assets/BackgroundSprites/background10.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl: Anyone, need a box match?", "Assets/BackgroundSprites/background11.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl: Just only 5$ a box!", "Assets/BackgroundSprites/background12.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : 5$ for a box matach will keep you warm", "Assets/BackgroundSprites/background9.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : Should i offer one to that man?", "Assets/BackgroundSprites/background13.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < manager->sceneTree[6].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[6].Frame, scenes6[i]);
    }
    manager->sceneTree[6].TotalScene = QueueSize(manager->sceneTree[6].Frame);

    // Node 7
    manager->sceneTree[7].id = 7;
    manager->sceneTree[7].TotalScene = 4;
    manager->sceneTree[7].numChoices = 2;
    manager->sceneTree[7].choiceLeftSon = "Take all and escape ";
    manager->sceneTree[7].choiceRightSon = "Accept his offer";
    manager->sceneTree[7].IdLeftSon = 11;
    manager->sceneTree[7].IdRightSon = 12;

    Scene scenes7[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < manager->sceneTree[7].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[7].Frame, scenes7[i]);
    }
    manager->sceneTree[7].TotalScene = QueueSize(manager->sceneTree[7].Frame);

    // Node 8 (Ending: Rumbling) - Kosong
    manager->sceneTree[8].id = 8;
    manager->sceneTree[8].TotalScene = 0;
    manager->sceneTree[8].numChoices = 0;
    manager->sceneTree[8].choiceLeftSon = NULL;
    manager->sceneTree[8].choiceRightSon = NULL;
    manager->sceneTree[8].IdLeftSon = -1;
    manager->sceneTree[8].IdRightSon = -1;

    // Node 9 (Ending: Get Pushed with a Man and Crushed by a Car)
    manager->sceneTree[9].id = 9;
    manager->sceneTree[9].TotalScene = 4;
    manager->sceneTree[9].numChoices = 1;
    manager->sceneTree[9].IdLeftSon = -1;
    manager->sceneTree[9].IdRightSon = -1;
    manager->sceneTree[9].choiceLeftSon = NULL;
    manager->sceneTree[9].choiceRightSon = NULL;

    Scene scenes9[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < manager->sceneTree[9].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[9].Frame, scenes9[i]);
    }
    manager->sceneTree[9].TotalScene = QueueSize(manager->sceneTree[9].Frame);

    // Node 10
    manager->sceneTree[10].id = 10;
    manager->sceneTree[10].TotalScene = 4;
    manager->sceneTree[10].numChoices = 2;
    manager->sceneTree[10].choiceLeftSon = "Tell him";
    manager->sceneTree[10].choiceRightSon = "Be Quite..";
    manager->sceneTree[10].IdLeftSon = 13;
    manager->sceneTree[10].IdRightSon = 14;

    Scene scenes10[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < manager->sceneTree[10].TotalScene; i++) {
        EnQueueLast(&manager->sceneTree[10].Frame, scenes10[i]);
    }
    manager->sceneTree[10].TotalScene = QueueSize(manager->sceneTree[10].Frame);
}

// Menentukan posisi karakter ketika digambar
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

// Memuat aset untuk node
void LoadNodeAssets(StoryManager* manager, int nodeIndex) {
    if (!manager || nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &manager->sceneTree[nodeIndex];
    Queue *queue = &node->Frame;

    for (int i = 0; i < QueueSize(*queue); i++) {
        Scene *scene = &queue->data[(queue->front + i) % MAX_SCENE];
        if (scene->backgroundPath != NULL) {
            scene->backgroundTex = LoadTexture(scene->backgroundPath);
            scene->backgroundTex.height = SCREEN_HEIGHT;
            scene->backgroundTex.width = SCREEN_WIDTH;
        } else {
            scene->backgroundTex = (Texture2D){0};
        }
        if (scene->characterPath != NULL) {
            scene->characterTex = LoadTexture(scene->characterPath);
            scene->characterTex.height /= 2;
            scene->characterTex.width /= 2;
        } else {
            scene->characterTex = (Texture2D){0};
        }
    }
}

// Menghapus aset dari node tertentu
void UnloadNodeAssets(StoryManager* manager, int nodeIndex) {
    if (!manager || nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &manager->sceneTree[nodeIndex];
    Queue *queue = &node->Frame;

    while (!isQueueEmpty(*queue)) {
        DeQueueFirst(queue);
    }
}

// Helper function to draw dialogue box
void DrawDialogueBox(const char* dialogue) {
    if (dialogue != NULL) {
        DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, BLACK);
        DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
        DrawText(dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }
}

// Helper function to draw choice buttons
void DrawChoiceButtons(const TreeStory* node) {
    if (!node || node->numChoices <= 0) return;
    
    int choiceButtonWidth = 400;
    int choiceButtonHeight = 60;
    int yButton = SCREEN_HEIGHT / 2 + 275;

    if (node->numChoices >= 1) {
        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            yButton,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
        DrawText(node->choiceLeftSon,
                 (int)(choiceRectLeft.x + choiceButtonWidth / 2 - MeasureText(node->choiceLeftSon, 25) / 2),
                 (int)(choiceRectLeft.y + choiceButtonHeight / 2 - 25 / 2),
                 25, WHITE);
    }

    if (node->numChoices == 2) {
        Rectangle choiceRectRight = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
            yButton,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
        DrawText(node->choiceRightSon,
                 (int)(choiceRectRight.x + choiceButtonWidth / 2 - MeasureText(node->choiceRightSon, 25) / 2),
                 (int)(choiceRectRight.y + choiceButtonHeight / 2 - 25 / 2),
                 25, WHITE);
    }
}

// Menggambar scene
void DrawCurrentNodeScreen(StoryManager* manager) {
    if (!manager || manager->currentScene < 0 || manager->currentScene >= MAX_NODE_TREE) return;

    TreeStory *node = &manager->sceneTree[manager->currentScene];
    Scene current = Peek(node->Frame);

    if (current.backgroundTex.id != 0) {
        DrawTexture(current.backgroundTex, 0, 0, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }
        }

        if (node->id != 4 && node->id != 5) {
            DrawCharacterAtPosition(current.characterTex, current.CharPosition);

            if (current.dialogue != NULL || (manager->currentFrame == node->TotalScene - 1 && node->numChoices > 0)) {
                DrawDialogueBox(current.dialogue);
            }

            if (manager->currentFrame == node->TotalScene - 1 && node->numChoices > 0) {
                DrawChoiceButtons(node);
            }
        } else {
            DrawCharacterAtPosition(current.characterTex, current.CharPosition);

            if (manager->currentFrame < node->TotalScene - 1 && current.dialogue != NULL) {
                DrawDialogueBox(current.dialogue);
            }
        }
    } else {
        if (current.backgroundTex.id != 0) {
            DrawTexture(current.backgroundTex, 0, 0, WHITE);
        } else {
            ClearBackground(RAYWHITE);
        }
    }
}

// Helper function to handle music updates
void UpdateStoryMusic(StoryManager* manager, const Scene* current) {
    if (!manager) return;
    
    // Update music if it's playing
    if (manager->isMusicPlaying) {
        UpdateMusicStream(manager->currentSceneMusic);
        if (!IsMusicStreamPlaying(manager->currentSceneMusic)) {
            manager->isMusicPlaying = false;
        }
    }
    
    // Check if this scene has music and should start playing
    if (current->backgroundSound != NULL && !manager->isMusicPlaying) {
        UnloadMusicStream(manager->currentSceneMusic);
        manager->currentSceneMusic = LoadMusicStream(current->backgroundSound);
        SetMusicVolume(manager->currentSceneMusic, 0.5f);
        PlayMusicStream(manager->currentSceneMusic);
        manager->isMusicPlaying = true;
        manager->currentSceneMusic.looping = false;
    }
}

// Helper function to handle ending scene updates
void UpdateEndingScene(StoryManager* manager, TreeStory* node, GameState* gameState) {
    if (!manager || !node || !gameState) return;
    
    manager->frameTimer += GetFrameTime();

    if (manager->frameTimer >= manager->frameDelay) {
        manager->frameTimer = 0.0f;
        if (manager->currentFrame < node->TotalScene - 1) {
            manager->currentFrame++;
            DeQueueFirst(&node->Frame);
            Scene nextScene = Peek(node->Frame);
            if (nextScene.backgroundPath != NULL) {
                nextScene.backgroundTex = LoadTexture(nextScene.backgroundPath);
                nextScene.backgroundTex.height = SCREEN_HEIGHT;
                nextScene.backgroundTex.width = SCREEN_WIDTH;
            }
            if (nextScene.characterPath != NULL) {
                nextScene.characterTex = LoadTexture(nextScene.characterPath);
                nextScene.characterTex.height /= 2;
                nextScene.characterTex.width /= 2;
            }
            EnQueueLast(&node->Frame, nextScene);
        } else {
            UnloadNodeAssets(manager, manager->currentScene);
            *gameState = GAME_STATE_MAIN_MENU;
            ResetStoryProgress(manager);
        }
    }
}

// Helper function to handle choice selection
void HandleChoiceSelection(StoryManager* manager, TreeStory* node, GameState* gameState) {
    if (!manager || !node || !gameState) return;
    
    Vector2 mouse = GetMousePosition();
    int choiceButtonWidth = 400;
    int choiceButtonHeight = 60;
    int choiceStartY = SCREEN_HEIGHT / 2 + 275;

    Rectangle choiceRectLeft = {
        SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
        choiceStartY,
        (float)choiceButtonWidth,
        (float)choiceButtonHeight
    };
    if (CheckCollisionPointRec(mouse, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        ProsesChoice(manager, 0);
        return;
    }

    if (node->numChoices == 2) {
        Rectangle choiceRectRight = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        if (CheckCollisionPointRec(mouse, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ProsesChoice(manager, 1);
            return;
        }
    }
}

// Helper function to handle scene progression
void HandleSceneProgression(StoryManager* manager, TreeStory* node) {
    if (!manager || !node) return;
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (manager->currentFrame < node->TotalScene - 1) {
            manager->currentFrame++;
            DeQueueFirst(&node->Frame);
            Scene nextScene = Peek(node->Frame);
            if (nextScene.backgroundPath != NULL) {
                nextScene.backgroundTex = LoadTexture(nextScene.backgroundPath);
                nextScene.backgroundTex.height = SCREEN_HEIGHT;
                nextScene.backgroundTex.width = SCREEN_WIDTH;
            }
            if (nextScene.characterPath != NULL) {
                nextScene.characterTex = LoadTexture(nextScene.characterPath);
                nextScene.characterTex.height /= 2;
                nextScene.characterTex.width /= 2;
            }
            EnQueueLast(&node->Frame, nextScene);
        }
    }
}

// Mengupdate logika cerita
void UpdateCerita(StoryManager* manager, GameState *gameState) {
    if (!manager || manager->currentScene < 0 || manager->currentScene >= MAX_NODE_TREE) {
        return;
    }

    TreeStory *node = &manager->sceneTree[manager->currentScene];
    Scene current = Peek(node->Frame);
    
    // Update music
    UpdateStoryMusic(manager, &current);

    if (IsKeyPressed(KEY_F1)) {
        *gameState = GAME_STATE_PAUSE;
        InitButtonRects(*gameState);
        return;
    }

    if (node->id == 4 || node->id == 5) {
        UpdateEndingScene(manager, node, gameState);
    } else {
        if (manager->currentFrame == node->TotalScene - 1 && node->numChoices > 0) {
            HandleChoiceSelection(manager, node, gameState);
        } else {
            HandleSceneProgression(manager, node);
        }
    }
}

// Memproses pilihan pemain
void ProsesChoice(StoryManager* manager, int choice) {
    if (!manager) return;
    
    int nextNodeIndex = (choice == 0) ? manager->sceneTree[manager->currentScene].IdLeftSon : manager->sceneTree[manager->currentScene].IdRightSon;

    if (nextNodeIndex < 0 || nextNodeIndex >= MAX_NODE_TREE || manager->sceneTree[nextNodeIndex].TotalScene == 0) {
        return;
    }
    UnloadNodeAssets(manager, manager->currentScene);
    manager->currentScene = nextNodeIndex;
    manager->currentFrame = 0;
    LoadNodeAssets(manager, manager->currentScene);
    printf("Berpindah ke scene %d\n", manager->currentScene);
}

// Menyimpan progres cerita
void SaveGameStory(const char *filename, int Node, int Scene) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Nama File Tidak Ditemukan\n");
        return;
    }

    fwrite(&Node, sizeof(int), 1, file);
    fwrite(&Scene, sizeof(int), 1, file);
    fclose(file);
}

// Memuat progres cerita
void LoadGameStory(const char *filename, int *Node, int *Scene) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Nama File Tidak Ditemukan\n");
        *Node = 0;
        *Scene = 0;
        return;
    }

    fread(Node, sizeof(int), 1, file);
    fread(Scene, sizeof(int), 1, file);
    fclose(file);
}

void UnloadStoryAudio(StoryManager* manager) {
    if (!manager) return;
    
    if (manager->isMusicPlaying) {
        UnloadMusicStream(manager->currentSceneMusic);
        manager->isMusicPlaying = false;
    }
}