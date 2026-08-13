#ifndef PADMGR_H
#define PADMGR_H

#include "ultra64.h"
#include "libu64/pad.h"

typedef enum ControllerPakType {
    CONT_PAK_NONE,
    CONT_PAK_RUMBLE,
    CONT_PAK_OTHER
} ControllerPakType;

typedef struct PadMgr {
    /* 0x0230 */ Input inputs[MAXCONTROLLERS];
} PadMgr; // size = 0x468

// Initialization

void PadMgr_Init(PadMgr* padMgr);

// Fetching inputs

// This function cannot be prototyped here in all configurations because it is called incorrectly in fault_gc.c
// (see bug in `Fault_PadCallback`)
#if PLATFORM_N64 || defined(AVOID_UB)
void PadMgr_RequestPadData(PadMgr* padmgr, Input* inputs, s32 gameRequest);
#endif

// Rumble

void PadMgr_RumbleStop(PadMgr* padMgr);
void PadMgr_RumbleReset(PadMgr* padMgr);
void PadMgr_RumbleSetSingle(PadMgr* padMgr, u32 port, u32 rumble);
void PadMgr_RumbleSet(PadMgr* padMgr, u8* enable);

extern PadMgr gPadMgr;

#endif
