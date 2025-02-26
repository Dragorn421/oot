#ifndef Z64_H
#define Z64_H

#include "ultra64.h"
#include "ultra64/gs2dex.h"
#include "attributes.h"
#include "versions.h"
#include "z64player.h"
#include "z64audio.h"
#include "z64ocarina.h"
#include "z64effect.h"
#include "z64animation.h"
#include "z64animation_legacy.h"
#include "z64play.h"
#include "z64skin.h"
#include "z64skin_matrix.h"
#include "alignment.h"
#include "audiothread_cmd.h"
#include "sfx.h"
#include "color.h"
#include "sys_matrix.h" // in room assets, gIdentityMtx

// TODO: include all files listed above into the right place, and then delete this file.

// Vis...
typedef struct struct_801664F0 {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 color;
    /* 0x0C */ Color_RGBA8_u32 envColor;
} struct_801664F0; // size = 0x10

typedef struct VisMono {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 primColor;
    /* 0x0C */ Color_RGBA8_u32 envColor;
    /* 0x10 */ u16* tlut;
    /* 0x14 */ Gfx* dList;
} VisMono; // size = 0x18

// Vis...
typedef struct struct_80166500 {
    /* 0x00 */ u32 useRgba;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 primColor;
    /* 0x08 */ Color_RGBA8_u32 envColor;
} struct_80166500; // size = 0x10

#endif
