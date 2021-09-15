#ifndef Z_BG_SPOT00_BREAK_H
#define Z_BG_SPOT00_BREAK_H

#include "ultra64.h"
#include "global.h"


struct BgSpot00Break;

typedef struct BgSpot00Break {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ s32 bgId;
    /* 0x0150 */ char unk_150[0x14];
} BgSpot00Break; // size = 0x0164

#endif
