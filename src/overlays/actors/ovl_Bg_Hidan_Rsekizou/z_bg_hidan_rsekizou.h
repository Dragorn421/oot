#ifndef _Z_BG_HIDAN_RSEKIZOU_H_
#define _Z_BG_HIDAN_RSEKIZOU_H_

#include <ultra64.h>
#include <global.h>

struct BgHidanRsekizou;

typedef struct BgHidanRsekizou {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ s16 unk164;
    /* 0x166 */ s16 unk166;
    /* 0x168 */ ColliderSpheres unk168;
    /* 0x188 */ ColliderSpheresElement unk188[6];
} BgHidanRsekizou; // size = 0x0308

extern const ActorInit Bg_Hidan_Rsekizou_InitVars;

#endif
