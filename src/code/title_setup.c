#include "global.h"

void TitleSetup_InitImpl(GameState* gameState) {
    osSyncPrintf("ゼルダ共通データ初期化\n"); // "Zelda common data initalization"
    SaveContext_Init();

    /*
    // vanilla
    gameState->running = false;
    SET_NEXT_GAMESTATE(gameState, Title_Init, TitleContext);
    */

    gameState->running = false;
    gSaveContext.gameMode = 0;
    gSaveContext.linkAge = 0; // adult (1 for child)
    Sram_InitDebugSave();
    gSaveContext.dayTime = 0x8000;
    gSaveContext.entranceIndex = 0;
    while (!(gEntranceTable[gSaveContext.entranceIndex].scene == SCENE_SPOT00 &&
             gEntranceTable[gSaveContext.entranceIndex].spawn == 0)) {
        gSaveContext.entranceIndex++;
    }
    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = -1;
    SET_NEXT_GAMESTATE(gameState, Gameplay_Init, GlobalContext);
}

void TitleSetup_Destroy(GameState* gameState) {
}

void TitleSetup_Init(GameState* gameState) {
    gameState->destroy = TitleSetup_Destroy;
    TitleSetup_InitImpl(gameState);
}
