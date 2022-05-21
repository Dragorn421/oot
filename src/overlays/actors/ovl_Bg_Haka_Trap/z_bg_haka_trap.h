#ifndef Z_BG_HAKA_TRAP_H
#define Z_BG_HAKA_TRAP_H

#include "ultra64.h"
#include "global.h"
#include "z64collision_check.h"

typedef enum BgHakaTrapType {
    /* 0 */ BG_HAKA_TRAP_GUILLOTINE,
    /* 1 */ BG_HAKA_TRAP_SPIKED_CRUSHER,
    /* 2 */ BG_HAKA_TRAP_SPIKED_WOODEN_WALL_1,
    /* 3 */ BG_HAKA_TRAP_SPIKED_WOODEN_WALL_2,
    /* 4 */ BG_HAKA_TRAP_PROPELLER,
    /* 5 */ BG_HAKA_TRAP_GUILLOTINE_ALT
} BgHakaTrapType;

struct BgHakaTrap;

typedef void (*BgHakaTrapActionFunc)(struct BgHakaTrap*, PlayState*);

typedef struct BgHakaTrap {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ BgHakaTrapActionFunc actionFunc;
    /* 0x168 */ u8 unk168;
    /* 0x169 */ s8 unk169;
    /* 0x16A */ s16 unk16A;
    /* 0x16C */ Vec3f unk16C;
    /* 0x178 */ ColliderCylinder unk178;
    /* 0x1C4 */ ColliderTris unk1C4;
    /* 0x1E4 */ ColliderTrisElement unk1E4[2];
} BgHakaTrap; // size = 0x29C

#endif
