#ifndef Z_OBJ_BOMBIWA_H
#define Z_OBJ_BOMBIWA_H

#include "ultra64.h"
#include "global.h"

struct ObjBombiwa;

typedef struct ObjBombiwa {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderCylinder collider;
    bool isTalking;
} ObjBombiwa;

#endif
