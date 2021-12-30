#include "global.h"

void Setup_InitImpl(SetupState* this) {
    PRINTF(T("ゼルダ共通データ初期化\n", "Zelda common data initalization\n"));
    SaveContext_Init();

    /*
    // vanilla
    this->state.running = false;
    SET_NEXT_GAMESTATE(&this->state, ConsoleLogo_Init, ConsoleLogoState);
    */

    // Load debug save
    Sram_InitDebugSave();

    // Set age, time and entrance
    gSaveContext.save.linkAge = LINK_AGE_ADULT;
    gSaveContext.save.dayTime = CLOCK_TIME(12, 0);
    gSaveContext.save.entranceIndex = ENTR_HYRULE_FIELD_0;

    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = ENTR_LOAD_OPENING;

    // Have the magic meter load correctly
    gSaveContext.magicFillTarget = gSaveContext.save.info.playerData.magic;
    gSaveContext.magicCapacity = 0;
    gSaveContext.save.info.playerData.magic = 0;
    gSaveContext.save.info.playerData.magicLevel = 0;

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
