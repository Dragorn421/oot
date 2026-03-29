#ifndef _Z_OBJ_TSUBO_H_
#define _Z_OBJ_TSUBO_H_

#include "ultra64.h"
#include "global.h"

struct ObjTsubo;

typedef void (*ObjTsuboActionFunc)(struct ObjTsubo*, GlobalContext*);

typedef struct ObjTsubo {
    /* 0x000 */ Actor actor;
    /* 0x14C */ ObjTsuboActionFunc actionFunc;
    /* 0x150 */ ColliderCylinder unk150;
    /* 0x19C */ s8 unk19C;
    /* 0x19D */ char pad19D[3];
} ObjTsubo; // size = 0x1A0

extern const ActorInit Obj_Tsubo_InitVars;

#endif
