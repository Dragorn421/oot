#ifndef _Z_BG_DDAN_JD_H_
#define _Z_BG_DDAN_JD_H_

#include <ultra64.h>
#include <global.h>

struct BgDdanJd;

typedef void (*BgDdanJdActionFunc)(struct BgDdanJd*, GlobalContext*);

typedef struct BgDdanJd {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ BgDdanJdActionFunc actionFunc;
    /* 0x168 */ u8 state;
    /* 0x169 */ u8 yVelocity;
    /* 0x16A */ s16 timer;
    /* 0x16C */ f32 targetPosY;
} BgDdanJd; // size = 0x170

extern const ActorInit Bg_Ddan_Jd_InitVars;

#endif
