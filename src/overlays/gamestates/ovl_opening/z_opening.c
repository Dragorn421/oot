/*
 * File: z_opening.c
 * Overlay: ovl_opening
 * Description: Initializes the game into the title screen
 */

#include "global.h"

void Opening_SetupTitleScreen(OpeningContext* this) {
    gSaveContext.gameMode = 0;
    this->state.running = false;
    Sram_InitDebugSave();
    gSaveContext.magicAcquired = false;
    gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KOKIRI;
    gSaveContext.equips.buttonItems[1] = ITEM_NONE;
    gSaveContext.equips.buttonItems[2] = ITEM_NONE;
    gSaveContext.equips.buttonItems[3] = ITEM_NONE;
    gSaveContext.equips.equipment = 0x1111;
    gSaveContext.linkAge = 1;
    gSaveContext.health = gSaveContext.healthCapacity = 3*16;
    gSaveContext.cutsceneIndex = 0;
    gSaveContext.sceneSetupIndex = 0;
    gSaveContext.dayTime = 0x8000;
    SET_NEXT_GAMESTATE(&this->state, Gameplay_Init, GlobalContext);
}

void func_80803C5C(OpeningContext* this) {
}

void Opening_Main(GameState* thisx) {
    OpeningContext* this = (OpeningContext*)thisx;

    func_80095248(this->state.gfxCtx, 0, 0, 0);
    Opening_SetupTitleScreen(this);
    func_80803C5C(this);
}

void Opening_Destroy(GameState* thisx) {
}

void Opening_Init(GameState* thisx) {
    OpeningContext* this = (OpeningContext*)thisx;

    R_UPDATE_RATE = 1;
    Matrix_Init(&this->state);
    View_Init(&this->view, this->state.gfxCtx);
    this->state.main = Opening_Main;
    this->state.destroy = Opening_Destroy;
}
