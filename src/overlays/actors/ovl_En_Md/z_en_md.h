#ifndef Z_EN_MD_H
#define Z_EN_MD_H

#include "ultra64.h"
#include "global.h"

struct EnMd;

typedef enum {
    ENMD_LIMB_NONE,
    ENMD_LIMB_ROOT,
    ENMD_LIMB_WAIST,
    ENMD_LIMB_LEFT_THIGH,
    ENMD_LIMB_LEFT_LEG,
    ENMD_LIMB_LEFT_FOOT,
    ENMD_LIMB_RIGHT_THIGH,
    ENMD_LIMB_RIGHT_LEG,
    ENMD_LIMB_RIGHT_FOOT,
    ENMD_LIMB_TORSO,
    ENMD_LIMB_LEFT_UPPER_ARM,
    ENMD_LIMB_LEFT_FOREARM,
    ENMD_LIMB_LEFT_HAND,
    ENMD_LIMB_RIGHT_UPPER_ARM,
    ENMD_LIMB_RIGHT_FOREARM,
    ENMD_LIMB_RIGHT_HAND,
    ENMD_LIMB_HEAD,
    ENMD_LIMB_MAX
} EnMdLimb;

typedef void (*EnMdActionFunc)(struct EnMd*, PlayState*);

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
    /* 0x214 */ s16 unk214[ENMD_LIMB_MAX];
    /* 0x236 */ s16 unk236[ENMD_LIMB_MAX];
    /* 0x258 */ Vec3s unk258[ENMD_LIMB_MAX];
    /* 0x2BE */ Vec3s unk2BE[ENMD_LIMB_MAX];
} EnMd; // size = 0x0324

#endif
