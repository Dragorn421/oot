#ifndef _Z_EN_GE3_H_
#define _Z_EN_GE3_H_

#include "ultra64.h"
#include "global.h"

struct EnGe3;

typedef struct EnGe3 {
    /* 0x000 */ Actor actor;
    /* 0x14C */ ColliderCylinder unk14C;
    /* 0x198 */ SkelAnime unk198;
    /* 0x1DC */ Vec3s unk1DC[24];
    /* 0x26C */ Vec3s unk26C[24];
    /* 0x2FC */ s16 unk2FC;
    /* 0x2FE */ s16 unk2FE;
    /* 0x300 */ Vec3s unk300;
    /* 0x306 */ Vec3s unk306;
    /* 0x30C */ u16 unk30C;
    /* 0x30E */ char pad30E[2];
    /* 0x310 */ void (*unk310)(struct EnGe3*, GlobalContext*);
} EnGe3; // size = 0x314

extern const ActorInit En_Ge3_InitVars;

#endif
