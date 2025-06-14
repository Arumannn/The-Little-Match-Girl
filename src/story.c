// Initial State: Cerita utama belum dimulai
// Final State: Cerita utama selesai dan player dapat melanjutkan ke minigame

#include "story.h"
#include "mainmenu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global state
float frameDelay = 3.5f;
float frameTimer = 0.0f;
bool isMusicPlaying = false;
Music currentSceneMusic;
extern int storyCurrentScene;  // Declared in main.c
extern int storyCurrentFrame;  // Declared in main.c

TreeStory SceneTree[MAX_NODE_TREE];

// IS: Data cerita belum diinisialisasi
// FS: Semua data cerita utama berhasil diinisialisasi
void InitDataCerita(TreeStory *SceneTree) {
    printf("Initializing story data...\n");

    // Inisialisasi scenes untuk setiap node
    for (int i = 0; i < MAX_NODE_TREE; i++) {
        SceneTree[i].id = i;
        SceneTree[i].TotalScene = 0;
        SceneTree[i].currentSceneIndex = 0;
        SceneTree[i].numChoices = 0;
        SceneTree[i].choiceLeftSon = NULL;
        SceneTree[i].choiceRightSon = NULL;
        SceneTree[i].IdLeftSon = -1;
        SceneTree[i].IdRightSon = -1;
        
        // Initialize all scenes to empty
        for (int j = 0; j < MAX_SCENE; j++) {
            SceneTree[i].scenes[j] = (Scene){NULL, NULL, NULL, NULL, {0}, {0}, CHAR_POS_NONE};
        }
    }

    // Node 0
    printf("Setting up Node 0...\n");
    SceneTree[0].id = 0;
    SceneTree[0].TotalScene = 4;
    SceneTree[0].numChoices = 2;
    SceneTree[0].choiceLeftSon = "Go to Alleway";
    SceneTree[0].choiceRightSon = "Go to Open Street";
    SceneTree[0].IdLeftSon = 1;
    SceneTree[0].IdRightSon = 2;

    Scene scenes0[] = {
        {"Assets/Music/myinstants.mp3", "Girl : ......", "Assets/BackgroundSprites/background4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : It is so cold here, but i have to sell all these matches", "Assets/BackgroundSprites/background3.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : What should i go, Alleway?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : Or the Street?", "Assets/BackgroundSprites/background7.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER}
    };
    for (int i = 0; i < SceneTree[0].TotalScene; i++) {
        SceneTree[0].scenes[i] = scenes0[i];
    }

    // Node 1
    SceneTree[1].id = 1;
    SceneTree[1].TotalScene = 2;
    SceneTree[1].numChoices = 2;
    SceneTree[1].choiceLeftSon = "Light on a few matches";
    SceneTree[1].choiceRightSon = "Ignore it, Keep Walking";
    SceneTree[1].IdLeftSon = 3;
    SceneTree[1].IdRightSon = 4;

    Scene scenes1[] = {
        {NULL, " Girl : I'm Think that guy is feel cold, should i help him?", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[1].TotalScene; i++) {
        SceneTree[1].scenes[i] = scenes1[i];
    }

    // Node 2
    SceneTree[2].id = 2;
    SceneTree[2].TotalScene = 2;
    SceneTree[2].numChoices = 2;
    SceneTree[2].choiceLeftSon = "Walk crosing the street";
    SceneTree[2].choiceRightSon = "Walk along the sidewalks";
    SceneTree[2].IdLeftSon = 5;
    SceneTree[2].IdRightSon = 6;

    Scene scenes2[] = {
        {NULL, "", "Assets/BackgroundSprites/background8.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Would these people be in need of matches?", "Assets/BackgroundSprites/background8.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[2].TotalScene; i++) {
        SceneTree[2].scenes[i] = scenes2[i];
    }

    // Node 3
    SceneTree[3].id = 3;
    SceneTree[3].TotalScene = 5;
    SceneTree[3].numChoices = 2;
    SceneTree[3].choiceLeftSon = "Give up all the match";
    SceneTree[3].choiceRightSon = "Protect the match";
    SceneTree[3].IdLeftSon = 7;
    SceneTree[3].IdRightSon = 8;

    Scene scenes3[] = {
        {NULL, "Homeless man: Thank you kid, here's 2 dollars for the match", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Yeah, thank you so much", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Give me the match too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara2.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Homeless man: Hey I want too", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl : I'm Scared, please don't hurt me", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[3].TotalScene; i++) {
        SceneTree[3].scenes[i] = scenes3[i];
    }

    // Node 4 (Ending: Die Alone)
    SceneTree[4].id = 4;
    SceneTree[4].TotalScene = 5;
    SceneTree[4].numChoices = 1;
    SceneTree[4].IdLeftSon = -1;
    SceneTree[4].IdRightSon = -1;
    SceneTree[4].choiceLeftSon = NULL;
    SceneTree[4].choiceRightSon = NULL;

    Scene scenes4[] = {
        {NULL, "GIrl : Itldlaosdlaokd's cold, should i light a few matches?", "Assets/BackgroundSprites/background14.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : No, i can handle it", "Assets/Endings/diecoldalone1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm still can handle it", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm Just need a rest for a moment......", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[4].TotalScene; i++) {
        SceneTree[4].scenes[i] = scenes4[i];
    }

    // Node 5 (Ending: Crushed by a Car)
    SceneTree[5].id = 5;
    SceneTree[5].TotalScene = 4;
    SceneTree[5].numChoices = 1;
    SceneTree[5].IdLeftSon = -1;
    SceneTree[5].IdRightSon = -1;
    SceneTree[5].choiceLeftSon = NULL;
    SceneTree[5].choiceRightSon = NULL;

    Scene scenes5[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[5].TotalScene; i++) {
        SceneTree[5].scenes[i] = scenes5[i];
    }

    // Node 6
    SceneTree[6].id = 6;
    SceneTree[6].TotalScene = 5;
    SceneTree[6].numChoices = 2;
    SceneTree[6].choiceLeftSon = "Try to pickpocket";
    SceneTree[6].choiceRightSon = "Offering the match";
    SceneTree[6].IdLeftSon = 9;
    SceneTree[6].IdRightSon = 10;

    Scene scenes6[] = {
        {NULL, "Walking....", "Assets/BackgroundSprites/background10.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl: Anyone, need a box match?", "Assets/BackgroundSprites/background11.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl: Just only 5$ a box!", "Assets/BackgroundSprites/background12.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : 5$ for a box matach will keep you warm", "Assets/BackgroundSprites/background9.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : Should i offer one to that man?", "Assets/BackgroundSprites/background13.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[6].TotalScene; i++) {
        SceneTree[6].scenes[i] = scenes6[i];
    }

    // Node 7
    SceneTree[7].id = 7;
    SceneTree[7].TotalScene = 4;
    SceneTree[7].numChoices = 2;
    SceneTree[7].choiceLeftSon = "Take all and escape ";
    SceneTree[7].choiceRightSon = "Accept his offer";
    SceneTree[7].IdLeftSon = 11;
    SceneTree[7].IdRightSon = 12;

    Scene scenes7[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < SceneTree[7].TotalScene; i++) {
        SceneTree[7].scenes[i] = scenes7[i];
    }

    // Node 8 (Ending: Rumbling) - Kosong
    SceneTree[8].id = 8;
    SceneTree[8].TotalScene = 0;
    SceneTree[8].numChoices = 0;
    SceneTree[8].choiceLeftSon = NULL;
    SceneTree[8].choiceRightSon = NULL;
    SceneTree[8].IdLeftSon = -1;
    SceneTree[8].IdRightSon = -1;

    // Node 9 (Ending: Get Pushed with a Man and Crushed by a Car)
    SceneTree[9].id = 9;
    SceneTree[9].TotalScene = 4;
    SceneTree[9].numChoices = 1;
    SceneTree[9].IdLeftSon = -1;
    SceneTree[9].IdRightSon = -1;
    SceneTree[9].choiceLeftSon = NULL;
    SceneTree[9].choiceRightSon = NULL;

    Scene scenes9[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[9].TotalScene; i++) {
        SceneTree[9].scenes[i] = scenes9[i];
    }

    // Node 10
    SceneTree[10].id = 10;
    SceneTree[10].TotalScene = 4;
    SceneTree[10].numChoices = 2;
    SceneTree[10].choiceLeftSon = "Tell him";
    SceneTree[10].choiceRightSon = "Be Quite..";
    SceneTree[10].IdLeftSon = 13;
    SceneTree[10].IdRightSon = 14;

    Scene scenes10[] = {
        {NULL, "Girl : Don't Take all the match...", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\n we have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background6.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_RIGHT}
    };
    for (int i = 0; i < SceneTree[10].TotalScene; i++) {
        SceneTree[10].scenes[i] = scenes10[i];
    }
}

// IS: Karakter belum digambar di posisi tertentu
// FS: Karakter berhasil digambar di posisi yang diinginkan
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

// IS: Asset node belum dimuat
// FS: Asset node berhasil dimuat ke memori
void LoadNodeAssets(TreeStory SceneTree[], int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &SceneTree[nodeIndex];
    
    for (int i = 0; i < node->TotalScene; i++) {
        Scene *scene = &node->scenes[i];
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

// IS: Asset node masih termuat di memori
// FS: Asset node berhasil di-unload dari memori
void UnloadNodeAssets(TreeStory SceneTree[], int nodeIndex) {
    if (nodeIndex < 0 || nodeIndex >= MAX_NODE_TREE) {
        printf("Invalid node index: %d\n", nodeIndex);
        return;
    }

    TreeStory *node = &SceneTree[nodeIndex];
    
    for (int i = 0; i < node->TotalScene; i++) {
        Scene *scene = &node->scenes[i];
        if (scene->backgroundTex.id != 0) {
            UnloadTexture(scene->backgroundTex);
            scene->backgroundTex = (Texture2D){0};
        }
        if (scene->characterTex.id != 0) {
            UnloadTexture(scene->characterTex);
            scene->characterTex = (Texture2D){0};
        }
    }
    node->currentSceneIndex = 0;
}

// IS: Layar node cerita belum digambar
// FS: Layar node cerita berhasil digambar sesuai scene
void DrawCurrentNodeScreen(TreeStory SceneTree[]) {
    if (storyCurrentScene < 0 || storyCurrentScene >= MAX_NODE_TREE) return;

    TreeStory *node = &SceneTree[storyCurrentScene];
    Scene *current = &node->scenes[storyCurrentFrame];

    // Draw current scene assets
    if (current->backgroundTex.id != 0) {
        DrawTexture(current->backgroundTex, 0, 0, WHITE);
    }
    if (current->characterTex.id != 0) {
        DrawCharacterAtPosition(current->characterTex, current->CharPosition);
    }

    // Draw dialogue
    if (current->dialogue != NULL) {
        DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, Fade(BLACK, 0.8f));
        DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
        DrawText(current->dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }

    // Draw choice buttons if this is the final scene and there are choices
    if (storyCurrentFrame == node->TotalScene - 1 && node->numChoices > 0) {
        int choiceButtonWidth = 400;
        int choiceButtonHeight = 60;
        int choiceStartY = SCREEN_HEIGHT / 2 + 275;

        // Draw left choice
        Rectangle choiceRectLeft = {
            SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
            choiceStartY,
            (float)choiceButtonWidth,
            (float)choiceButtonHeight
        };
        DrawRectangleRec(choiceRectLeft, Fade(GRAY, 0.8f));
        DrawRectangleLinesEx(choiceRectLeft, 2, WHITE);
        if (node->choiceLeftSon != NULL) {
            Vector2 textPos = {
                choiceRectLeft.x + choiceButtonWidth/2 - MeasureText(node->choiceLeftSon, 25)/2,
                choiceRectLeft.y + choiceButtonHeight/2 - 25/2
            };
            DrawText(node->choiceLeftSon, textPos.x, textPos.y, 25, WHITE);
        }

        // Draw right choice if it exists
        if (node->numChoices == 2) {
            Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            DrawRectangleRec(choiceRectRight, Fade(GRAY, 0.8f));
            DrawRectangleLinesEx(choiceRectRight, 2, WHITE);
            if (node->choiceRightSon != NULL) {
                Vector2 textPos = {
                    choiceRectRight.x + choiceButtonWidth/2 - MeasureText(node->choiceRightSon, 25)/2,
                    choiceRectRight.y + choiceButtonHeight/2 - 25/2
                };
                DrawText(node->choiceRightSon, textPos.x, textPos.y, 25, WHITE);
            }
        }
    }
}

// IS: Cerita belum diperbarui
// FS: Cerita berhasil diperbarui sesuai input user
void UpdateCerita(TreeStory SceneTree[], GameState *gameState) {
    if (storyCurrentScene < 0 || storyCurrentScene >= MAX_NODE_TREE) {
        return;
    }

    TreeStory *node = &SceneTree[storyCurrentScene];
    Scene *current = &node->scenes[storyCurrentFrame];

    // Update music if it's playing
    if (isMusicPlaying) {
        UpdateMusicStream(currentSceneMusic);
        if (!IsMusicStreamPlaying(currentSceneMusic)) {
            isMusicPlaying = false;
        }
    }
    
    // Check if this scene has music and should start playing
    if (current->backgroundSound != NULL && !isMusicPlaying) {
        if (isMusicPlaying) {
            UnloadMusicStream(currentSceneMusic);
        }
        currentSceneMusic = LoadMusicStream(current->backgroundSound);
        SetMusicVolume(currentSceneMusic, 0.5f);
        PlayMusicStream(currentSceneMusic);
        isMusicPlaying = true;
        currentSceneMusic.looping = false;
    }

    Vector2 mouse = GetMousePosition();

    if (IsKeyPressed(KEY_F1)) {
        *gameState = GAME_STATE_PAUSE;
        InitButtonRects(*gameState);
        return;
    }

    if (node->id == 4 || node->id == 5) {
        frameTimer += GetFrameTime();

        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            if (storyCurrentFrame < node->TotalScene - 1) {
                storyCurrentFrame++;
            } else {
                UnloadNodeAssets(SceneTree, storyCurrentScene);
                *gameState = GAME_STATE_MAIN_MENU;
                storyCurrentScene = 0;
                storyCurrentFrame = 0;
            }
        }
    } else {
        if (storyCurrentFrame == node->TotalScene - 1 && node->numChoices > 0) {
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
                ProsesChoice(SceneTree, 0);
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
                    ProsesChoice(SceneTree, 1);
                    return;
                }
            }
        } else {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (storyCurrentFrame < node->TotalScene - 1) {
                    storyCurrentFrame++;
                }
            }
        }
    }
}

// IS: Pilihan cerita belum diproses
// FS: Pilihan cerita berhasil diproses dan state berpindah
void ProsesChoice(TreeStory SceneTree[], int choice) {
    int nextNodeIndex = (choice == 0) ? SceneTree[storyCurrentScene].IdLeftSon : SceneTree[storyCurrentScene].IdRightSon;

    if (nextNodeIndex < 0 || nextNodeIndex >= MAX_NODE_TREE || SceneTree[nextNodeIndex].TotalScene == 0) {
        return;
    }
    
    UnloadNodeAssets(SceneTree, storyCurrentScene);
    storyCurrentScene = nextNodeIndex;
    storyCurrentFrame = 0;
    LoadNodeAssets(SceneTree, storyCurrentScene);
    printf("Berpindah ke scene %d\n", storyCurrentScene);
}

// IS: Progress cerita belum disimpan
// FS: Progress cerita berhasil disimpan ke file
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

// IS: Progress cerita belum dimuat
// FS: Progress cerita berhasil dimuat dari file
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

// IS: Musik cerita masih termuat di memori
// FS: Musik cerita berhasil di-unload dari memori
void UnloadStoryAudio() {
    if (isMusicPlaying) {
        UnloadMusicStream(currentSceneMusic);
        isMusicPlaying = false;
    }
}