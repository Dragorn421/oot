#ifndef Z_DOOR_TOKI_H
#define Z_DOOR_TOKI_H

#include "ultra64.h"
#include "global.h"

struct DoorToki;

typedef struct DoorToki {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ s32 bgId;
    /* 0x0150 */ u8 unk_150[0x18];
} DoorToki; // size = 0x0168

#endif
