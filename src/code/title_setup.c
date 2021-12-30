#include "global.h"

void Setup_InitImpl(SetupState* this) {
    osSyncPrintf("ゼルダ共通データ初期化\n"); // "Zelda common data initalization"
    SaveContext_Init();

    /*
    // vanilla
    this->state.running = false;
    SET_NEXT_GAMESTATE(&this->state, ConsoleLogo_Init, ConsoleLogoState);
    */

    // Load debug save
    Sram_InitDebugSave();

    // Set age, time and entrance
    gSaveContext.linkAge = LINK_AGE_ADULT;
    gSaveContext.dayTime = CLOCK_TIME(12, 0);
    gSaveContext.entranceIndex = ENTR_HYRULE_FIELD_0;

    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = ENTR_LOAD_OPENING;

    // Have the magic meter load correctly
    gSaveContext.magicFillTarget = gSaveContext.magic;
    gSaveContext.magicCapacity = 0;
    gSaveContext.magic = 0;
    gSaveContext.magicLevel = 0;

    // Load Play state
    gSaveContext.gameMode = GAMEMODE_NORMAL;
    this->state.running = false;
    SET_NEXT_GAMESTATE(&this->state, Play_Init, PlayState);
}

void Setup_Destroy(GameState* thisx) {
}

void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    this->state.destroy = Setup_Destroy;
    Setup_InitImpl(this);
}
