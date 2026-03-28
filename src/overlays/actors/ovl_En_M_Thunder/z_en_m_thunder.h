#ifndef _Z_EN_M_THUNDER_H_
#define _Z_EN_M_THUNDER_H_

#include "ultra64.h"
#include "global.h"

struct EnMThunder;

typedef void (*EnMThunderActionFunc)(struct EnMThunder*, GlobalContext*);

typedef struct EnMThunder {
    /* 0x000 */ Actor actor;
    /* 0x14C */ ColliderCylinder unk14C;
    /* 0x198 */ LightNode* unk198;
    /* 0x19C */ LightInfo unk19C;
    /* 0x1AA */ char pad1AA[2];
    /* 0x1AC */ f32 unk1AC;
    /* 0x1B0 */ f32 unk1B0;
    /* 0x1B4 */ f32 unk1B4;
    /* 0x1B8 */ f32 unk1B8;
    /* 0x1BC */ f32 unk1BC;
    /* 0x1C0 */ EnMThunderActionFunc actionFunc;
    /* 0x1C4 */ u16 unk1C4;
    /* 0x1C6 */ u8 unk1C6;
    /* 0x1C7 */ u8 unk1C7;
    /* 0x1C8 */ u8 unk1C8;
    /* 0x1C9 */ u8 unk1C9;
    /* 0x1CA */ u8 unk1CA;
    /* 0x1CB */ char pad1CB[1];
} EnMThunder; // size = 0x1CC

extern const ActorInit En_M_Thunder_InitVars;

#endif
