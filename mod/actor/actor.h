#ifndef _MOD_ACTOR_H_
#define _MOD_ACTOR_H_

#include "ultra64.h"
#include "global.h"

/*
LinkedFace#neighbours is indexed by edge, where
edge 0 goes from verts[0] to verts[1]
edge 1 goes from verts[1] to verts[2]
edge 2 goes from verts[2] to verts[0]
*/
typedef struct LinkedFace {
    Vec3f verts[3];
    Vec3f norm;
    s32 neighbours[3]; // indexes of neighbouring faces (-1 if none)
} LinkedFace;

typedef struct ModActor {
    Actor actor;
    MtxF transform;
    Vec3f positionModel;
    LinkedFace* curFace;
    s32 rotating;
    LinkedFace* rotatingTo;
    Vec3f rotatingAxis;
    f32 rotatingAngle;
    f32 rotatingAngleTarget;
} ModActor;

#endif
