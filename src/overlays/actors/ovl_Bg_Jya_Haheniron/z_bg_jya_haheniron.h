#ifndef _Z_BG_JYA_HAHENIRON_H_
#define _Z_BG_JYA_HAHENIRON_H_

#include "ultra64.h"
#include "global.h"

struct BgJyaHaheniron;

typedef void (*BgJyaHahenironActionFunc)(struct BgJyaHaheniron*, GlobalContext*);

typedef struct BgJyaHaheniron {
    /* 0x000 */ Actor actor;
    /* 0x14C */ BgJyaHahenironActionFunc unk14C;
    /* 0x150 */ ColliderSpheres unk150;
    /* 0x170 */ ColliderSpheresElement unk170[1];
    /* 0x1B0 */ s16 unk1B0;
    /* 0x1B2 */ char pad1B2[2];
} BgJyaHaheniron; // size = 0x01B4

extern const ActorInit Bg_Jya_Haheniron_InitVars;

#endif
