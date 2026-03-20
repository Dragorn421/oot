#ifndef _Z_BG_BOWL_WALL_H_
#define _Z_BG_BOWL_WALL_H_

#include "ultra64.h"
#include "global.h"

struct BgBowlWall;

typedef void (*BgBowlWallActionFunc)(struct BgBowlWall*, GlobalContext*);

typedef struct BgBowlWall {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ BgBowlWallActionFunc actionFunc;
    /* 0x168 */ Vec3f unk168;
    /* 0x174 */ Vec3f unk174;
    /* 0x180 */ s16 unk180;
    /* 0x182 */ s16 unk182;
    /* 0x184 */ struct EnBomBowlMan* unk184;
} BgBowlWall; // size = 0x188

extern const ActorInit Bg_Bowl_Wall_InitVars;

#endif
