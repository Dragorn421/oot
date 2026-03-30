#ifndef _Z_BG_TOKI_HIKARI_H_
#define _Z_BG_TOKI_HIKARI_H_

#include "ultra64.h"
#include "global.h"

struct BgTokiHikari;

typedef struct BgTokiHikari {
    /* 0x000 */ Actor actor;
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ void (*unk150)(struct BgTokiHikari*, GlobalContext*);
} BgTokiHikari; // size = 0x154

extern const ActorInit Bg_Toki_Hikari_InitVars;

#endif
