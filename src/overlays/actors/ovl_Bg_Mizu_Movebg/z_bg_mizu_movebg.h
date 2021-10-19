#ifndef Z_BG_MIZU_MOVEBG_H
#define Z_BG_MIZU_MOVEBG_H

#include "ultra64.h"
#include "global.h"

typedef enum {
    /* 0 */ BGMIZUMOVEBG_TYPE_0_PLATFORM_SLANTED,
    /* 1 */ BGMIZUMOVEBG_TYPE_1_PLATFORM,
    /* 2 */ BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER,
    /* 3 */ BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL,
    /* 4 */ BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG,
    /* 5 */ BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG,
    /* 6 */ BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG,
    /* 7 */ BGMIZUMOVEBG_TYPE_7_MOVING_PLATFORM
} BgMizuMovebgType;

typedef enum {
    /* 0 */ BGMIZUMOVEBG_421PART_BODY,
    /* 1 */ BGMIZUMOVEBG_421PART_HEAD,
    /* 2 */ BGMIZUMOVEBG_421PART_LEG_FL,
    /* 3 */ BGMIZUMOVEBG_421PART_LEG_FR,
    /* 4 */ BGMIZUMOVEBG_421PART_LEG_BL,
    /* 5 */ BGMIZUMOVEBG_421PART_LEG_BR
} BgMizuMovebg421Part;

struct BgMizuMovebg;

typedef void (*BgMizuMovebgActionFunc)(struct BgMizuMovebg*, GlobalContext*);

typedef struct BgMizuMovebg {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ BgMizuMovebgActionFunc actionFunc;
    /* 0x0168 */ f32 homeY;
    /* 0x016C */ s32 scrollAlpha1;
    /* 0x0170 */ s32 scrollAlpha2;
    /* 0x0174 */ s32 scrollAlpha3;
    /* 0x0178 */ s32 scrollAlpha4;
    /* 0x017C */ u8 sfxFlags;
    /* 0x0180 */ Gfx* dlist;
    /* 0x0184 */ s32 waypointId;
    s32 mode421;
    s32 part;
    Vec3s rotJitter;
    s32 timer;
    s32 targetYrot;
} BgMizuMovebg; // size = 0x0188

#endif
