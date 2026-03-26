#ifndef _Z_EN_DNS_H_
#define _Z_EN_DNS_H_

#include "ultra64.h"
#include "global.h"

typedef enum EnDnsType {
    /*  0 */ EN_DNS_TYPE_DEKU_NUTS,
    /*  1 */ EN_DNS_TYPE_DEKU_STICKS,
    /*  2 */ EN_DNS_TYPE_HEART_PIECE,
    /*  3 */ EN_DNS_TYPE_DEKU_SEEDS,
    /*  4 */ EN_DNS_TYPE_DEKU_SHIELD,
    /*  5 */ EN_DNS_TYPE_BOMBS,
    /*  6 */ EN_DNS_TYPE_ARROWS,
    /*  7 */ EN_DNS_TYPE_RED_POTION,
    /*  8 */ EN_DNS_TYPE_GREEN_POTION,
    /*  9 */ EN_DNS_TYPE_DEKU_STICKS_CAPACITY,
    /* 10 */ EN_DNS_TYPE_DEKU_NUTS_CAPACITY
} EnDnsType;

struct EnDns;

typedef struct EnDns {
    /* 0x0000 */ Actor actor;
    /* 0x14C */ SkelAnime unk14C;
    /* 0x190 */ Vec3s unk190[0x12];
    /* 0x1FC */ Vec3s unk1FC[0x12];
    /* 0x268 */ void (*unk268)(struct EnDns*, GlobalContext*);
    /* 0x26C */ ColliderCylinder unk26C;
    /* 0x2B8 */ s16 unk2B8;
    /* 0x2BA */ u8 unk2BA;
    /* 0x2BB */ u8 unk2BB;
    /* 0x2BC */ u8 unk2BC;
    /* 0x2BD */ u8 unk2BD;
    /* 0x2C0 */ struct EnDnsPurchaseInfo* unk2C0;
    /* 0x2C4 */ f32 unk2C4;
} EnDns; // size = 0x02C8

extern const ActorInit En_Dns_InitVars;

#endif
