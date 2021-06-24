#ifndef _Z_TEST_ACTOR_H_
#define _Z_TEST_ACTOR_H_

#include "ultra64.h"
#include "global.h"

#define COLLISION 0x06000220
#define MODEL 0x06000B50

typedef struct TestActor {
    Actor actor;
    s32 timer;
} TestActor;

extern const ActorInit TestActor_InitVars;

#endif
