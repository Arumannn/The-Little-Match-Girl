// Initial State: Cerita utama belum dimulai
// Final State: Cerita utama selesai dan player dapat melanjutkan ke minigame

#include "story.h"
#include "mainmenu.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float frameDelay = 3.5f;
float frameTimer = 0.0f;
bool isMusicPlaying = false;
Music currentSceneMusic;
extern int storyCurrentNode;  
extern int storyCurrentScene;

TreeStory SceneTree[MAX_NODE_TREE];

// IS: Data cerita belum diinisialisasi
// FS: Semua data cerita utama berhasil diinisialisasi
void InitDataCerita(TreeStory *SceneTree) {
    printf("Initializing story data...\n");

    // Inisialisasi scenes untuk setiap node
    for (int i = 0; i < MAX_NODE_TREE; i++) {
        SceneTree[i].id = i;
        SceneTree[i].TotalScene = 0;
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
        {"Assets/Music/snowstorm.mp3", "Girl : ......", "Assets/BackgroundSprites/background41.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : It is so cold here, but i have to sell all these matches", "Assets/BackgroundSprites/background48.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : What should i go, Open Street?", "Assets/BackgroundSprites/background29.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/crowdincity.mp3", "Girl : Or the Alleway?", "Assets/BackgroundSprites/background29.png", NULL, {0}, {0}, CHAR_POS_NONE}
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
        {NULL, " Girl : I'm Think that guy is feel cold, should i help him?", "Assets/BackgroundSprites/background38.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : But this match is for my survival, i can't give it to them", "Assets/BackgroundSprites/background38.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
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
        {"Assets/Music/crowd.mp3", "", "Assets/BackgroundSprites/background8.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/crowd.mp3", "Girl : Would these people be in need of matches?", "Assets/BackgroundSprites/background8.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT}
    };
    for (int i = 0; i < SceneTree[2].TotalScene; i++) {
        SceneTree[2].scenes[i] = scenes2[i];
    }

    // Node 3
    SceneTree[3].id = 3;
    SceneTree[3].TotalScene = 6;
    SceneTree[3].numChoices = 2;
    SceneTree[3].choiceLeftSon = "Give up all the match";
    SceneTree[3].choiceRightSon = "Protect the match";
    SceneTree[3].IdLeftSon = 7;
    SceneTree[3].IdRightSon = 8;

    Scene scenes3[] = {
        {"Assets/Music/ignitematch.mp3", NULL, "Assets/BackgroundSprites/background39.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/firesound.mp3", "Homeless man: Thank you kid, for the fire", "Assets/BackgroundSprites/background53.png", "Assets/CharaSprites/chara4.png", {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/firesound.mp3", NULL, "Assets/BackgroundSprites/background70.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "..........", "Assets/BackgroundSprites/background55.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/homelessangry.mp3", "Other Homeless man: Hey! Why the fuck is the fire down, I'm freezing my ass here! ", "Assets/BackgroundSprites/background71.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/homelessangry.mp3", "Other Homeless man : Give me your fucking match", "Assets/BackgroundSprites/background35.png", "Assets/CharaSprites/chara22.png", {0}, {0}, CHAR_POS_LEFT}
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
        {"Assets/Music/snowstorm.mp3", "Girl : It's cold, should i light a few matches?", "Assets/BackgroundSprites/background14.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {"Assets/Music/footsteps.mp3", "Girl : No, i can handle it", "Assets/Endings/diecoldalone1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm still can handle it", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm Just need a rest for a moment......", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic1.mp3", NULL, "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[4].TotalScene; i++) {
        SceneTree[4].scenes[i] = scenes4[i];
    }

    // Node 5 (Ending: Crushed by a Car)
    SceneTree[5].id = 5;
    SceneTree[5].TotalScene = 5;
    SceneTree[5].numChoices = 1;
    SceneTree[5].IdLeftSon = -1;
    SceneTree[5].IdRightSon = -1;
    SceneTree[5].choiceLeftSon = NULL;
    SceneTree[5].choiceRightSon = NULL;

    Scene scenes5[] = {
        {NULL, NULL, "Assets/Endings/crossingtheroad1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/carhorn.mp3", NULL, "Assets/Endings/crossingtheroad2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/panicwoman.mp3", NULL, "Assets/Endings/crossingtheroad3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/acident.mp3", NULL, "Assets/Endings/crossingtheroad4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic3.mp3", NULL, "Assets/Endings/crossingtheroad5.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[5].TotalScene; i++) {
        SceneTree[5].scenes[i] = scenes5[i];
    }

    // Node 6
    SceneTree[6].id = 6;
    SceneTree[6].TotalScene = 6;
    SceneTree[6].numChoices = 2;
    SceneTree[6].choiceLeftSon = "Try to pickpocket";
    SceneTree[6].choiceRightSon = "Offering the match";
    SceneTree[6].IdLeftSon = 9;
    SceneTree[6].IdRightSon = 10;

    Scene scenes6[] = {
        {"Assets/Music/footsteps.mp3", "Walking....", "Assets/BackgroundSprites/background18.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {"Assets/Music/footsteps.mp3", "Girl: Anyone, need a box match?", "Assets/BackgroundSprites/background40.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {"Assets/Music/footsteps.mp3", "Girl: Just only 5$ a box!", "Assets/BackgroundSprites/background12.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {"Assets/Music/footsteps.mp3", "Girl : 5$ for a box matach will keep you warm", "Assets/BackgroundSprites/background9.png", "Assets/CharaSprites/chara3.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : Should i offer one to that man?", "Assets/BackgroundSprites/background27.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Girl : But he has a lot money, i can steal it", "Assets/BackgroundSprites/background44.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[6].TotalScene; i++) {
        SceneTree[6].scenes[i] = scenes6[i];
    }

    // Node 7
    SceneTree[7].id = 7;
    SceneTree[7].TotalScene = 5;
    SceneTree[7].numChoices = 2;
    SceneTree[7].choiceLeftSon = "Take The Match and escape ";
    SceneTree[7].choiceRightSon = "Accept his offer";
    SceneTree[7].IdLeftSon = 11;
    SceneTree[7].IdRightSon = 12;

    Scene scenes7[] = {
        {"Assets/Music/girlcry.mp3", "Girl : Don't Take all the match...", "Assets/BackgroundSprites/Background55.png" , NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/Punch.mp3", "Girl : Please", NULL, "Assets/BackgroundSprites/Background56.png", {0}, {0}, CHAR_POS_NONE}, // Fighting sound
        {NULL, "Homeless man : Sorry for the trouble kid.", "Assets/BackgroundSprites/background35.png", NULL, {0}, {0}, CHAR_POS_RIGHT},
        {NULL, "Girl: ........", "Assets/BackgroundSprites/background35.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {NULL, "Homeless Man : You know how desperate we bums are. Here's your match back. If you want,\nwe have some food for you—it's our way of apologizing", "Assets/BackgroundSprites/background54.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl: Can I trust him?", "Assets/BackgroundSprites/background54.png", NULL, {0}, {0}, CHAR_POS_NONE},

    };
    for (int i = 0; i < SceneTree[7].TotalScene; i++) {
        SceneTree[7].scenes[i] = scenes7[i];
    }

    // Node 8 (Ending: Rumbling) - Kosong
    SceneTree[8].id = 8;
    SceneTree[8].TotalScene = 5;
    SceneTree[8].numChoices = 0;
    SceneTree[8].choiceLeftSon = NULL;
    SceneTree[8].choiceRightSon = NULL;
    SceneTree[8].IdLeftSon = -1;
    SceneTree[8].IdRightSon = -1;

    Scene scenes8[] = {
        {"Assets/Music/homelessangry.mp3", "Homeless : GIVE ME THE FUCKING MATCH.", "Assets/Endings/rumbling1.png" , NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/homelessangry.mp3", "Another Homeless : I WANT ONE TOO", "Assets/Endings/rumbling2.png", NULL, {0}, {0}, CHAR_POS_NONE}, 
        {"Assets/Music/girlcry.mp3", "Girl : Please don't hurt me", "Assets/Endings/rumbling3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/CrowdRunning2.mp3", NULL, "Assets/Endings/rumbling4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic2.mp3", NULL, "Assets/Endings/rumbling5.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[8].TotalScene; i++) {
        SceneTree[8].scenes[i] = scenes8[i];
    }

    // Node 9 (Ending: Get Pushed with a Man and Crushed by a Car)
    SceneTree[9].id = 9;
    SceneTree[9].TotalScene = 6;
    SceneTree[9].numChoices = 0;
    SceneTree[9].IdLeftSon = -1;
    SceneTree[9].IdRightSon = -1;
    SceneTree[9].choiceLeftSon = NULL;
    SceneTree[9].choiceRightSon = NULL;

    Scene scenes9[] = {
        {NULL, "Girl : i have to be careful....", "Assets/Endings/stealingdie1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The Man : WHAT ARE YOU DOING? ARE YOU TRYING TO STEAL FROM ME?", "Assets/Endings/stealingdie2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The Man : I'm not going to let you get away with this! Get out of here!", "Assets/Endings/stealingdie3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/acident.mp3", NULL, "Assets/Endings/stealingdie4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/panic.mp3", "The Man : What Am i doing, she died.....", "Assets/Endings/stealingdie5.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic3.mp3", NULL, "Assets/Endings/stealingdie6.png", NULL, {0}, {0}, CHAR_POS_NONE}

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
        {NULL, "Girl : Hello sir, would you like a match? You're seemingly shivering…", "Assets/BackgroundSprites/background34.png", "Assets/CharaSprites/chara14.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "The Man : Okay.. Sure", "Assets/BackgroundSprites/background50.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl: Thank you, sir", "Assets/BackgroundSprites/background51.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : The money is too a lot", "Assets/BackgroundSprites/background34.png", "Assets/CharaSprites/chara23.png", {0}, {0}, CHAR_POS_CENTER}
    };
    for (int i = 0; i < SceneTree[10].TotalScene; i++) {
        SceneTree[10].scenes[i] = scenes10[i];
    }

     // Node 11
    SceneTree[11].id = 11;
    SceneTree[11].TotalScene = 6;
    SceneTree[11].numChoices = 2;
    SceneTree[11].choiceLeftSon = "Alleway";
    SceneTree[11].choiceRightSon = "Stair";
    SceneTree[11].IdLeftSon = 15;
    SceneTree[11].IdRightSon = 16;

    Scene scenes11[] = {
        {"Assets/Music/running_snow.mp3", "Running....", "Assets/BackgroundSprites/background34.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/breath.mp3", "Girl : At least, They won't Hurt me anymore", "Assets/BackgroundSprites/background42.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assetes/Music/angry man.mp3", "Stranger Homeless : Give me all of your match", "Assets/BackgroundSprites/background61.png", "Assets/CharaSprites/chara25.png", {0}, {0}, CHAR_POS_CENTER},
        {NULL, "Girl : No, I won't", "Assets/BackgroundSprites/background67.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/BackgroundSprites/background65.png", NULL , {0}, {0}, CHAR_POS_NONE},
        {NULL, "Stranger Homeless : Where Are you Wanna Go?", "Assets/BackgroundSprites/background66.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[11].TotalScene; i++) {
        SceneTree[11].scenes[i] = scenes11[i];
    }

      // Node 12 Ending Eat with homeless man
    SceneTree[12].id = 12;
    SceneTree[12].TotalScene = 5;
    SceneTree[12].numChoices = 0;
    SceneTree[12].choiceLeftSon = NULL;
    SceneTree[12].choiceRightSon = NULL;
    SceneTree[12].IdLeftSon = -1;
    SceneTree[12].IdRightSon = -1;

    Scene scenes12[] = {
        {NULL, "Homeless Man : So?", "Assets/Endings/eatwithhomeless1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Okay, i want it", "Assets/Endings/eatwithhomeless1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Homeless Man: How was it?", "Assets/Endings/eatwithhomeless2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl :It's delicious... Thanks", "Assets/Endings/eatwithhomeless2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingGoodMusic1.mp3", NULL, "Assets/Endings/eatwithhomeless2.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[12].TotalScene; i++) {
        SceneTree[12].scenes[i] = scenes12[i];
    }

    // Node 13 (Ending: Get Pushed with a Man and Crushed by a Car)
    SceneTree[13].id = 13;
    SceneTree[13].TotalScene = 6;
    SceneTree[13].numChoices = 0;
    SceneTree[13].IdLeftSon = -1;
    SceneTree[13].IdRightSon = -1;
    SceneTree[13].choiceLeftSon = NULL;
    SceneTree[13].choiceRightSon = NULL;

    Scene scenes13[] = {
        {NULL, "The man : You stealing from me, right?", "Assets/Endings/stealingdie1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I didn't mean to, i'm sorry", "Assets/Endings/stealingdie2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The Man : I'm not going to let you get away with this! Get out of here!", "Assets/Endings/stealingdie3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/acident.mp3", NULL, "Assets/Endings/stealingdie4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/panic.mp3", "The Man : What Am i doing, she died.....", "Assets/Endings/stealingdie5.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic3.mp3", NULL, "Assets/Endings/stealingdie6.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[13].TotalScene; i++) {
        SceneTree[13].scenes[i] = scenes13[i];
    }
    
    // Node 14 belum di isi
    SceneTree[14].id = 14;
    SceneTree[14].TotalScene = 9;
    SceneTree[14].numChoices = 2;
    SceneTree[14].choiceLeftSon = "Sell to a cop";
    SceneTree[14].choiceRightSon = "Sell to a homeless";
    SceneTree[14].IdLeftSon = 17;
    SceneTree[14].IdRightSon = 18;

    Scene scenes14[] = {
        {NULL, "Girl : Well, i think i could keep the money", "Assets/BackgroundSprites/background34.png", "Assets/CharaSprites/chara23.png", {0}, {0}, CHAR_POS_CENTER},
        {"Assets/Music/footsteps.mp3", NULL, "Assets/BackgroundSprites/background65.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Stranger : Alright girl, you better hand over all your stuffs", "Assets/BackgroundSprites/background75.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl: Wait, no please, this is all I have", "Assets/BackgroundSprites/background63.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {"Assets/Music/girlcry.mp3", "Girl: It's cold", "Assets/BackgroundSprites/background63.png", "Assets/CharaSprites/chara5.png", {0}, {0}, CHAR_POS_LEFT},
        {"Assets/Music/girlcry.mp3", "Girl: What's that light", "Assets/BackgroundSprites/background77.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/snowstorm.mp3", "Girl : Oh they serve warm soups", "Assets/BackgroundSprites/background78.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/snowstorm.mp3", "Girl : I need to sell a few matches more to afford it, just a few more", "Assets/BackgroundSprites/background78.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/snowstorm.mp3", "Girl : Where i have to sell this match?", "Assets/BackgroundSprites/background79.png", NULL, {0}, {0}, CHAR_POS_NONE},
    };
    for (int i = 0; i < SceneTree[14].TotalScene; i++) {
        SceneTree[14].scenes[i] = scenes14[i];
    }

    // Node 15
    SceneTree[15].id = 15;
    SceneTree[15].TotalScene = 3;
    SceneTree[15].numChoices = 2;
    SceneTree[15].choiceLeftSon = "Light on a few match";
    SceneTree[15].choiceRightSon = "Keep Walking";
    SceneTree[15].IdLeftSon = 19;
    SceneTree[15].IdRightSon = 20;

    Scene scenes15[] = {
        {"Assets/Music/snowstorm.mp3", "Girl : Finally I'm Safe Now...", "Assets/BackgroundSprites/background46.png", NULL, {0}, {0}, CHAR_POS_CENTER},
        {"Assets/Music/snowstorm.mp3", "Girl : It's Cold", "Assets/BackgroundSprites/background48.png", NULL, {0}, {0}, CHAR_POS_CENTER},
        {"Assets/Music/snowstorm.mp3", "Girl : I think i can light on a few match", "Assets/BackgroundSprites/background45.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[15].TotalScene; i++) {
        SceneTree[15].scenes[i] = scenes15[i];
    }

    //Node 16 Ending Die in Stairs
    SceneTree[16].id = 16;
    SceneTree[16].TotalScene = 5;
    SceneTree[16].numChoices = 0;
    SceneTree[16].choiceLeftSon = NULL;
    SceneTree[16].choiceRightSon = NULL;
    SceneTree[16].IdLeftSon = -1;
    SceneTree[16].IdRightSon = -1;

    Scene scenes16[] = {
        {NULL, "Stranger Homeless : You can't escape from me", "Assets/Endings/diestair1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Stranger Homeless : I said Give me all of your match", "Assets/Endings/diestair2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Stranger Homeless : Fuck you", "Assets/Endings/diestair3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/fall.mp3", NULL, "Assets/Endings/diestair4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic2.mp3", NULL, "Assets/Endings/diestair5.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[16].TotalScene; i++) {
        SceneTree[16].scenes[i] = scenes16[i];
    }

      // Node 17 Ending Eat soup
    SceneTree[17].id = 17;
    SceneTree[17].TotalScene = 6;
    SceneTree[17].numChoices = 0;
    SceneTree[17].choiceLeftSon = NULL;
    SceneTree[17].choiceRightSon = NULL;
    SceneTree[17].IdLeftSon = -1;
    SceneTree[17].IdRightSon = -1;

    Scene scenes17[] = {
        {NULL, "Girl : Excuse me sir, would you like to buy some matches?", "Assets/Endings/endingeatsoup1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Officer : Oh…That would be nice", "Assets/Endings/endingeatsoup2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Officer : Here the money for the match", "Assets/Endings/endingeatsoup3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Thank you officer", "Assets/Endings/endingeatsoup3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/endingeatsoup4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingGoodMusic1.mp3", NULL, "Assets/Endings/endingeatsoup5.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[17].TotalScene; i++) {
        SceneTree[17].scenes[i] = scenes17[i];
    }

    // Node 18
    SceneTree[18].id = 18;
    SceneTree[18].TotalScene = 4;
    SceneTree[18].numChoices = 2;
    SceneTree[18].choiceLeftSon = "Give all the match";
    SceneTree[18].choiceRightSon = "Escape";
    SceneTree[18].IdLeftSon = 21;
    SceneTree[18].IdRightSon = 22;

    Scene scenes18[] = {
        {NULL, "Girl : Execuse me, sir do you wanna buy my match?", "Assets/BackgroundSprites/background52.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Homeless : I think you have to..", "Assets/BackgroundSprites/background80.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Homeless : Give me all your match", "Assets/BackgroundSprites/background81.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : What should i do?", "Assets/BackgroundSprites/background67.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[18].TotalScene; i++) {
        SceneTree[18].scenes[i] = scenes18[i];
    }
    

    // Node 19
    SceneTree[19].id = 19;
    SceneTree[19].TotalScene = 7;
    SceneTree[19].numChoices = 2;
    SceneTree[19].choiceLeftSon = "Escape Again";
    SceneTree[19].choiceRightSon = "Accept the offer";
    SceneTree[19].IdLeftSon = 23;
    SceneTree[19].IdRightSon = 24;

    Scene scenes19[] = {
        {"Assets/Music/ignitematch.mp3", NULL, "Assets/BackgroundSprites/background39.png", NULL, {0}, {0}, CHAR_POS_CENTER},
        {NULL, NULL, "Assets/BackgroundSprites/background68.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The Man : What are you doing here?", "Assets/BackgroundSprites/background55.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : ?!", "Assets/BackgroundSprites/background68.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The Man : Where's your parent?", "Assets/BackgroundSprites/background68.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I dont have parent", "Assets/BackgroundSprites/background68.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "The man : Come with me i have a hot soup for you", "Assets/BackgroundSprites/background69.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[19].TotalScene; i++) {
        SceneTree[19].scenes[i] = scenes19[i];
    }
    
    // Node 20 (Ending: Die Alone)
    SceneTree[20].id = 20;
    SceneTree[20].TotalScene = 4;
    SceneTree[20].numChoices = 1;
    SceneTree[20].IdLeftSon = -1;
    SceneTree[20].IdRightSon = -1;
    SceneTree[20].choiceLeftSon = NULL;
    SceneTree[20].choiceRightSon = NULL;

    Scene scenes20[] = {
        {NULL, "Girl : No, i can handle it", "Assets/Endings/diecoldalone1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm still can handle it", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm Just need a rest for a moment......", "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic1.mp3", NULL, "Assets/Endings/diecoldalone2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[20].TotalScene; i++) {
        SceneTree[20].scenes[i] = scenes20[i];
    }

    // Node 21 (Ending: Die Stanbbing Alone)
    SceneTree[21].id = 21;
    SceneTree[21].TotalScene = 4;
    SceneTree[21].numChoices = 0;
    SceneTree[21].IdLeftSon = -1;
    SceneTree[21].IdRightSon = -1;
    SceneTree[21].choiceLeftSon = NULL;
    SceneTree[21].choiceRightSon = NULL;

    Scene scenes21[] = {
        {NULL, "Girl : Okey... take it all, but dont take my live", "Assets/Endings/endingstabing1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Homeless : Sorry kid, i can't trust you", "Assets/Endings/endingstabing2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/stabingKnife.mp3", NULL, "Assets/Endings/endingstabing3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic2.mp3", NULL, "Assets/Endings/endingstabing4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[21].TotalScene; i++) {
        SceneTree[21].scenes[i] = scenes21[i];
    }


    // Node 22
    SceneTree[22].id = 22;
    SceneTree[22].TotalScene = 5;
    SceneTree[22].numChoices = 2;
    SceneTree[22].choiceLeftSon = "Fight with the Teenager";
    SceneTree[22].choiceRightSon = "Just Defend";
    SceneTree[22].IdLeftSon = 25;
    SceneTree[22].IdRightSon = 26;

    Scene scenes22[] = {
        {"Assets/Music/running_snow.mp3", NULL, "Assets/BackgroundSprites/background65.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/breath.mp3", "Girl : I think i can sit down there a while", "Assets/BackgroundSprites/background30.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Stranger : Well, well well. Look there the match girl", "Assets/BackgroundSprites/background30.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : ????", "Assets/BackgroundSprites/background84.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Teenager: Let's give him a lesson again guys...", "Assets/BackgroundSprites/background84.png", NULL, {0}, {0}, CHAR_POS_NONE}


    };
    for (int i = 0; i < SceneTree[22].TotalScene; i++) {
        SceneTree[22].scenes[i] = scenes22[i];
    }


    // Node 25 (Ending: Die BECAUSE BULLY)
    SceneTree[25].id = 25;
    SceneTree[25].TotalScene = 6;
    SceneTree[25].numChoices = 0;
    SceneTree[25].IdLeftSon = -1;
    SceneTree[25].IdRightSon = -1;
    SceneTree[25].choiceLeftSon = NULL;
    SceneTree[25].choiceRightSon = NULL;

    Scene scenes25[] = {
        {"Assets/Music/Fighting.mp3", NULL, "Assets/Endings/diebecausebully1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/Fighting.mp3", NULL, "Assets/Endings/diebecausebully1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/Fighting.mp3", NULL, "Assets/Endings/diebecausebully1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/diebecausebully2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/diebecausebully3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic1.mp3", NULL, "Assets/Endings/diebecausebully4.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[25].TotalScene; i++) {
        SceneTree[25].scenes[i] = scenes25[i];
    }



    // Node 26 ( BELUM SELESAI)
    SceneTree[26].id = 26;
    SceneTree[26].TotalScene = 7;
    SceneTree[26].numChoices = 2;
    SceneTree[26].choiceLeftSon = "Police";
    SceneTree[26].choiceRightSon = "Business man";
    SceneTree[26].IdLeftSon = 27;
    SceneTree[26].IdRightSon = 28;

    Scene scenes26[] = {
        {"Assets/Music/Fighting.mp3", NULL, "Assets/BackgroundSprites/background84.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/breath.mp3", "Teenager : Forget it guys, let's just bounce", "Assets/BackgroundSprites/background55.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/breath.mp3", NULL, "Assets/BackgroundSprites/background68.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/footsteps.mp3", NULL, "Assets/BackgroundSprites/background67.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/footsteps.mp3", "Girl : I'm should sell these matches", "Assets/BackgroundSprites/background57.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "To the police man", "Assets/BackgroundSprites/background83.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Or to the business man", "Assets/BackgroundSprites/background82.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[26].TotalScene; i++) {
        SceneTree[26].scenes[i] = scenes26[i];
    }


     // Node 27 Ending Eat with homeless man
    SceneTree[27].id = 27;
    SceneTree[27].TotalScene = 6;
    SceneTree[27].numChoices = 0;
    SceneTree[27].choiceLeftSon = NULL;
    SceneTree[27].choiceRightSon = NULL;
    SceneTree[27].IdLeftSon = -1;
    SceneTree[27].IdRightSon = -1;

    Scene scenes27[] = {
        {NULL, "Girl : Excuse me sir, would you like to buy some matches?", "Assets/Endings/endingeatsoup1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Officer : Oh…That would be nice", "Assets/Endings/endingeatsoup2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Officer : Here the money for the match", "Assets/Endings/endingeatsoup3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Thank you officer", "Assets/Endings/endingeatsoup3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, NULL, "Assets/Endings/endingeatsoup4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingGoodMusic1.mp3", NULL, "Assets/Endings/endingeatsoup5.png", NULL, {0}, {0}, CHAR_POS_NONE}

    };
    for (int i = 0; i < SceneTree[27].TotalScene; i++) {
        SceneTree[27].scenes[i] = scenes27[i];
    }

    // Node 23
    SceneTree[23].id = 23;
    SceneTree[23].TotalScene = 3;
    SceneTree[23].numChoices = 0;
    SceneTree[23].choiceLeftSon = NULL;
    SceneTree[23].choiceRightSon = NULL;
    SceneTree[23].IdLeftSon = -1;
    SceneTree[23].IdRightSon = -1;

    Scene scenes23[] = {
        {NULL, "Girl : Now where i have to go? ", "Assets/Endings/walkwithoutdirection1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : I'm just walking without any direction", "Assets/Endings/walkwithoutdirection2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic2.mp3", NULL, "Assets/Endings/walkwithoutdirection2.png", NULL, {0}, {0}, CHAR_POS_NONE}
    };
    for (int i = 0; i < SceneTree[23].TotalScene; i++) {
        SceneTree[23].scenes[i] = scenes23[i];
    }

    // Node 24 Happy got a new family
    SceneTree[24].id = 24;
    SceneTree[24].TotalScene = 4;
    SceneTree[24].numChoices = 0;
    SceneTree[24].IdLeftSon = -1;
    SceneTree[24].IdRightSon = -1;
    SceneTree[24].choiceLeftSon = NULL;
    SceneTree[24].choiceRightSon = NULL;

    Scene scenes24[] = {
        {NULL, "The Man : Follow me", "Assets/Endings/endingfamily1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/family.mp3", "The Man : This is my home", "Assets/Endings/endingfamily2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/family.mp3", "The Man : And this is your new family", "Assets/Endings/endingfamily3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingGoodMusic1.mp3", NULL, "Assets/Endings/endingfamily4.png", NULL, {0}, {0}, CHAR_POS_NONE},

    };
    for (int i = 0; i < SceneTree[24].TotalScene; i++) {
        SceneTree[24].scenes[i] = scenes24[i];
    }

     // Node 28 Ending Business
    SceneTree[28].id = 28;
    SceneTree[28].TotalScene = 5;
    SceneTree[28].numChoices = 0;
    SceneTree[28].choiceLeftSon = NULL;
    SceneTree[28].choiceRightSon = NULL;
    SceneTree[28].IdLeftSon = -1;
    SceneTree[28].IdRightSon = -1;

    Scene scenes28[] = {
        {NULL, "Girl : Excuse me sir, would you like to buy some matches?", "Assets/Endings/endingbusinessman1.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/angry no.mp3", "Business man : The fuck you want, Chicago sox just lost and that means i lose that 100 dollars debt, now GET OUT!", "Assets/Endings/endingbusinessman2.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/slap.mp3", ".......", "Assets/Endings/endingbusinessman3.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {NULL, "Girl : Why my life become like this", "Assets/Endings/endingbusinessman4.png", NULL, {0}, {0}, CHAR_POS_NONE},
        {"Assets/Music/endingSadMusic2.mp3", NULL, "Assets/Endings/endingbusinessman4.png", NULL, {0}, {0}, CHAR_POS_NONE},


    };
    for (int i = 0; i < SceneTree[28].TotalScene; i++) {
        SceneTree[28].scenes[i] = scenes28[i];
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
}

// IS: Layar node cerita belum digambar
// FS: Layar node cerita berhasil digambar sesuai scene
void DrawCurrentNodeScreen(TreeStory SceneTree[]) {
    if (storyCurrentNode < 0 || storyCurrentNode >= MAX_NODE_TREE) return;

    TreeStory *node = &SceneTree[storyCurrentNode];
    Scene *current = &node->scenes[storyCurrentScene];


    
    // Draw current scene assets
    if (current->backgroundTex.id != 0) {
        DrawTexture(current->backgroundTex, 0, 0, WHITE);
    }
    if (current->characterTex.id != 0) {
        DrawCharacterAtPosition(current->characterTex, current->CharPosition);
    }

    // Draw dialogue
    if (current->dialogue != NULL) {
        DrawRectangle(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, Fade(BLACK, 1.0f));
        DrawRectangleLines(50, SCREEN_HEIGHT - 200, SCREEN_WIDTH - 100, 250, WHITE);
        DrawText(current->dialogue, 70, SCREEN_HEIGHT - 180, 30, WHITE);
    }

    // Draw choice buttons if this is the final scene and there are choices
    if (storyCurrentScene == node->TotalScene - 1 && node->numChoices == 2) {
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
        Rectangle choiceRectRight = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
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
    char debugText[100];
    sprintf(debugText, "Scene: %d/%d | Node ID: %d", 
            storyCurrentScene + 1, 
            node->TotalScene, 
            node->id
            );
    
    // Calculate position to center the text
    int textWidth = MeasureText(debugText, 20);
    int textX = (SCREEN_WIDTH - textWidth) / 2;
    int textY = SCREEN_HEIGHT / 2;
    
    // Draw the debug text in red
    DrawText(debugText, textX, textY, 20, RED);
}

// IS: Cerita belum diperbarui
// FS: Cerita berhasil diperbarui sesuai input user
void UpdateCerita(TreeStory SceneTree[], GameState *gameState) {
    if (storyCurrentNode < 0 || storyCurrentNode >= MAX_NODE_TREE) {
        return;
    }

    TreeStory *node = &SceneTree[storyCurrentNode];
    Scene *current = &node->scenes[storyCurrentScene];

    static char* lastSoundPath = NULL;

    // Handle audio transitions
    if (current->soundPath != NULL) {
        // Only load new music if it's different from the last one
        if (lastSoundPath == NULL || strcmp(lastSoundPath, current->soundPath) != 0) {
            // Stop and unload previous music if playing
            if (isMusicPlaying) {
                StopMusicStream(currentSceneMusic);
                UnloadMusicStream(currentSceneMusic);
                isMusicPlaying = false;
            }
            // Load and play new music
            currentSceneMusic = LoadMusicStream(current->soundPath);
            SetMusicVolume(currentSceneMusic, 0.5f);
            PlayMusicStream(currentSceneMusic);
            isMusicPlaying = true;
            currentSceneMusic.looping = false;
            lastSoundPath = current->soundPath;
        }
        // Update music if it's playing
        if (isMusicPlaying) {
            UpdateMusicStream(currentSceneMusic);
        }
    } else {
        // If current scene has no audio, stop and unload any playing music
        if (isMusicPlaying) {
            StopMusicStream(currentSceneMusic);
            UnloadMusicStream(currentSceneMusic);
            isMusicPlaying = false;
            lastSoundPath = NULL;
        }
    }

    Vector2 mouse = GetMousePosition();

    if (IsKeyPressed(KEY_F1)) {
        *gameState = GAME_STATE_PAUSE;
        InitButtonRects(*gameState);
        return;
    }

    if (node->id == 4 || node->id == 5 || node->id == 12 || node->id == 9 || node->id == 8 ||
     node->id == 13 || node->id == 16 || node->id == 17 || node->id == 20 || node->id == 21 ||
     node->id == 24 || node->id == 25 || node->id == 23 || node->id == 27 || node->id == 28) {
        
        // Check for space key press first, independent of frame timer
        if (storyCurrentScene == node->TotalScene - 1 && IsKeyPressed(KEY_SPACE)) {
            UnloadNodeAssets(SceneTree, storyCurrentNode);
            *gameState = GAME_STATE_MAIN_MENU;
            storyCurrentNode = 0;
            storyCurrentScene = 0;
            return;
        }

        frameTimer += GetFrameTime();
        if (frameTimer >= frameDelay) {
            frameTimer = 0.0f;
            if (storyCurrentScene < node->TotalScene - 1) {
                storyCurrentScene++;
            } else {
                // For ending scenes, wait until music finishes
                if (isMusicPlaying) {
                    UpdateMusicStream(currentSceneMusic);
                    if (!IsMusicStreamPlaying(currentSceneMusic)) {
                        UnloadNodeAssets(SceneTree, storyCurrentNode);
                        *gameState = GAME_STATE_MAIN_MENU;
                        storyCurrentNode = 0;
                        storyCurrentScene = 0;  
                    }
                }
            }
        }
    } else {
        if (storyCurrentScene == node->TotalScene - 1 && node->numChoices == 2) {
            int choiceButtonWidth = 400;
            int choiceButtonHeight = 60;
            int choiceStartY = SCREEN_HEIGHT / 2 + 275;

            Rectangle choiceRectLeft = {
                SCREEN_WIDTH / 2 - choiceButtonWidth / 2 - 715,
                choiceStartY,
                (float)choiceButtonWidth,
                (float)choiceButtonHeight
            };
            Rectangle choiceRectRight = {
                    SCREEN_WIDTH / 2 - choiceButtonWidth / 2 + 715,
                    choiceStartY,
                    (float)choiceButtonWidth,
                    (float)choiceButtonHeight
                };
            if (CheckCollisionPointRec(mouse, choiceRectLeft) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ProsesChoice(SceneTree, 0);
                return;
            }
            
                if (CheckCollisionPointRec(mouse, choiceRectRight) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ProsesChoice(SceneTree, 1);
                    return;
                }
           
        } else {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (storyCurrentScene < node->TotalScene - 1) {
                    storyCurrentScene++;
                }
            }
        }
    
    }
}

// IS: Pilihan cerita belum diproses
// FS: Pilihan cerita berhasil diproses dan state berpindah
void ProsesChoice(TreeStory SceneTree[], int choice) {
    int nextNodeIndex = (choice == 0) ? SceneTree[storyCurrentNode].IdLeftSon : SceneTree[storyCurrentNode].IdRightSon;

    if (nextNodeIndex < 0 || nextNodeIndex >= MAX_NODE_TREE || SceneTree[nextNodeIndex].TotalScene == 0) {
        return;
    }
    
    UnloadNodeAssets(SceneTree, storyCurrentNode);
    storyCurrentNode = nextNodeIndex;
    storyCurrentScene = 0;
    LoadNodeAssets(SceneTree, storyCurrentNode);
    printf("Berpindah ke scene %d\n", storyCurrentNode);
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