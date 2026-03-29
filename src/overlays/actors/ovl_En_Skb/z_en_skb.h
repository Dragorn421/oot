#ifndef _Z_EN_SKB_H_
#define _Z_EN_SKB_H_

#include "ultra64.h"
#include "global.h"

struct EnSkb;

typedef struct EnSkb {
    /* 0x000 */ Actor actor;
    /* 0x14C */ SkelAnime unk14C;
    /* 0x190 */ Vec3s unk190[20];
    /* 0x208 */ Vec3s unk208[20];
    /* 0x280 */ u8 unk280;
    /* 0x281 */ u8 unk281;
    /* 0x282 */ u8 unk282;
    /* 0x283 */ u8 unk283;
    /* 0x284 */ void (*unk284)(struct EnSkb*, GlobalContext*);
    /* 0x288 */ s16 unk288;
    /* 0x28A */ char pad28A[2];
    /* 0x28C */ struct_80032E24 unk28C;
    /* 0x2A4 */ ColliderJntSph unk2A4;
    /* 0x2C4 */ ColliderJntSphElement unk2C4[2];
} EnSkb; // size = 0x344

extern const ActorInit En_Skb_InitVars;

#endif
