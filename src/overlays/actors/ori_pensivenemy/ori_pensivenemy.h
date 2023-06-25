// SPDX-License-Identifier: CC0-1.0

#ifndef MYCUSTOMACTOR_H
#define MYCUSTOMACTOR_H

#include "ultra64.h"
#include "global.h"

struct Pensivenemy;

typedef void (*PensivenemyActionFunc)(struct Pensivenemy*, PlayState*);
#define PENSIVENEMY_ACTIONFUNC(name) void name(struct Pensivenemy* this, PlayState* play)

enum PensivenemyInstType { PENSIVENEMY_FACE, PENSIVENEMY_HAND_PROJ };

#define PENSIVEENEMY_INSTTYPE(params) _SHIFTR(params, 0, 3)
#define PENSIVEENEMY_PROFILEID(params) _SHIFTR(params, 3, 5)

#define PENSIVEENEMY_PARAMS(instType, profileId) (_SHIFTL(instType, 0, 3) | _SHIFTL(profileId, 3, 5))

typedef struct Pensivenemy {
    Actor actor;
    enum PensivenemyInstType instType;
    int width_in_units;
    int height_in_units;
    f32 displaceActiveRadiusSq;
    f32 newHandProjsSpeed;
    f32 newHandProjsCooldownSeconds;
    PensivenemyActionFunc func_action;
    PensivenemyActionFunc func_draw;
    void (*func_gfxprint)(struct Pensivenemy*, PlayState*, GfxPrint*);
    int next_attack_cooldown;
} Pensivenemy;

#endif
