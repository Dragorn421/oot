#ifndef _MOD_ACTOR_H_
#define _MOD_ACTOR_H_

#include "ultra64.h"
#include "global.h"

typedef enum {
    /* 0 */ ROT_AXIS_NONE,
    /* 1 */ ROT_AXIS_X_POSI,
    /* 2 */ ROT_AXIS_X_NEG,
    /* 3 */ ROT_AXIS_Z_POSI,
    /* 4 */ ROT_AXIS_Z_NEG
} RotAxis;

typedef struct ModActor {
    Actor actor;
    Mtx transform;
    Vec3f curTranslation;
    RotAxis curRotationAxis;
    f32 curRotationRad;
} ModActor;

#endif
