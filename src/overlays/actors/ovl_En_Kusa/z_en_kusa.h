#ifndef _Z_EN_KUSA_H_
#define _Z_EN_KUSA_H_

#include "ultra64.h"
#include "global.h"

struct EnKusa;

typedef void (*EnKusaActionFunc)(struct EnKusa*, GlobalContext*);

typedef struct EnKusa {
    /* 0x000 */ Actor actor;
    /* 0x14C */ EnKusaActionFunc actionFunc;
    /* 0x150 */ ColliderCylinder unk150;
    /* 0x19C */ s16 unk19C;
    /* 0x19E */ s8 unk19E;
    /* 0x19F */ char pad19F[1];
} EnKusa; // size = 0x1A0

extern const ActorInit En_Kusa_InitVars;

#endif
