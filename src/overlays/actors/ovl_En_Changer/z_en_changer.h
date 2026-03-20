#ifndef _Z_EN_CHANGER_H_
#define _Z_EN_CHANGER_H_

#include "ultra64.h"
#include "global.h"

struct EnChanger;

// EnBox
typedef struct ActorA {
    /* 0x000 */ Actor actor;
    /* 0x14C */ char pad14C[0xA8];
    /* 0x1F4 */ s16 unk1F4;
} ActorA;

typedef void (*EnChangerActionFunc)(struct EnChanger*, GlobalContext*);

typedef struct EnChanger {
    /* 0x000 */ Actor actor;
    /* 0x14C */ EnChangerActionFunc unk14C;
    /* 0x150 */ ActorA* unk150;
    /* 0x154 */ ActorA* unk154;
    /* 0x158 */ ActorA* unk158;
    /* 0x15C */ s16 unk15C_l;
    /* 0x15E */ s16 unk15E_r;
    /* 0x160 */ s16 unk160;
    /* 0x162 */ s16 unk162;
    /* 0x164 */ s16 unk164;
    /* 0x166 */ s16 unk166;
    /* 0x168 */ s16 unk168;
    /* 0x16A */ char pad16A[2];
} EnChanger; // size = 0x016C

extern const ActorInit En_Changer_InitVars;

#endif
