#ifndef Z_BG_MENKURI_KAITEN_H
#define Z_BG_MENKURI_KAITEN_H

#include "ultra64.h"
#include "global.h"

struct BgMenkuriKaiten;

typedef struct BgMenkuriKaiten {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ s32 bgId;
    /* 0x0150 */ char unk_150[0x14];
} BgMenkuriKaiten; // size = 0x0164

#endif
