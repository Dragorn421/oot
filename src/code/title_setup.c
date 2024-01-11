#include "global.h"

#if USE_8MiB_RAM
void Setup_DrawNoExpansionPakMessage(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;
    GfxPrint printer;
    Gfx* gfx;

    // Print a basic message on screen

    OPEN_DISPS(this->state.gfxCtx, __FILE__, __LINE__);

    Gfx_SetupFrame(this->state.gfxCtx, 45, 52, 54);

    gfx = POLY_OPA_DISP + 1;
    gSPDisplayList(OVERLAY_DISP++, gfx);

    printer.flags |= GFXP_FLAG_ENLARGE;

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);

    GfxPrint_SetColor(&printer, 116, 185, 255, 255);
    GfxPrint_SetPos(&printer, 10, 10);
    GfxPrint_Printf(&printer, "NO EXPANSION PAK");

    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);

    gSPEndDisplayList(gfx++);
    gSPBranchList(POLY_OPA_DISP, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS(this->state.gfxCtx, __FILE__, __LINE__);
}
#endif

void Setup_InitImpl(SetupState* this) {
    PRINTF(T("ゼルダ共通データ初期化\n", "Zelda common data initalization\n"));
    SaveContext_Init();

#if USE_8MiB_RAM
    // If less than 8MiB ram (no expansion pak)
    if (osMemSize < 8 * 1024 * 1024) {
        this->state.main = Setup_DrawNoExpansionPakMessage;
        return; // Stay in this game state
    }
#endif

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
