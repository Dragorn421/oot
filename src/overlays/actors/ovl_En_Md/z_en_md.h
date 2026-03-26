#ifndef _Z_EN_MD_H_
#define _Z_EN_MD_H_

#include "z64.h"
#include <ultra64.h>
#include <global.h>

struct EnMd;

typedef void (*EnMdActionFunc)(struct EnMd*, GlobalContext*);

typedef struct EnMd {
    /* 0x000 */ Actor actor;
    /* 0x14C */ SkelAnime unk14C;
    /* 0x190 */ EnMdActionFunc unk190;
    /* 0x194 */ ColliderCylinder unk194;
    /* 0x1E0 */ struct_80034A14_arg1 unk1E0;
    /* 0x208 */ u8 unk208;
    /* 0x209 */ u8 unk209;
    /* 0x20A */ u8 unk20A;
    /* 0x20B */ u8 unk20B;
    /* 0x20C */ s16 unk20C;
    /* 0x20E */ s16 unk20E;
    /* 0x210 */ s16 unk210;
    /* 0x212 */ s16 unk212;
    /* 0x214 */ s16 unk214[17];
    /* 0x236 */ s16 unk236[17];
    /* 0x258 */ Vec3s unk258[17];
    /* 0x2BE */ Vec3s unk2BE[17];
} EnMd; // size = 0x0324

extern const ActorInit En_Md_InitVars;

#endif
