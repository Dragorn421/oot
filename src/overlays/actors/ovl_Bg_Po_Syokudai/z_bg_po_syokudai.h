#ifndef _Z_BG_PO_SYOKUDAI_H_
#define _Z_BG_PO_SYOKUDAI_H_

#include <ultra64.h>
#include <global.h>

struct BgPoSyokudai;

typedef struct BgPoSyokudai {
    /* 0x000 */ Actor actor;
    /* 0x14C */ u8 unk14C;
    /* 0x14E */ s16 unk14E;
    /* 0x150 */ LightNode* unk150;
    /* 0x154 */ LightInfo unk154;
    /* 0x164 */ ColliderCylinder unk164;
} BgPoSyokudai; // size = 0x01B0

extern const ActorInit Bg_Po_Syokudai_InitVars;

#endif
