#ifndef _Z_TEST_ACTOR_H_
#define _Z_TEST_ACTOR_H_

#include "ultra64.h"
#include "global.h"

#define WAIT_DUP_TO_START false

#define LENGTH 300.0f
#define WIDTH 300.0f
#define MOVE_SPEED 30.0f
#define N_CORRIDORS 10
#define N_FAKE_CORRIDORS (N_CORRIDORS - 2)
#define N_OBSTACLES_MAX 30

typedef enum {
    ENV_KOKIRI_FOREST,
    TRANSI_KOKIRI_TO_DEKU,
    ENV_DEKU_TREE,
    CORRIDOR_SECTION_TYPE_COUNT
} CorridorSectionType;

typedef struct CorridorSectionInfo {
    CorridorSectionType idx;
    s16 objectId;
    u32 dlistOpa;
    u32 dlistXlu;
    s32 duration;
    s8 isEnv;
    s8 isTransi;
} CorridorSectionInfo;

typedef struct RunnerObstacle {
    Actor* actor;
} RunnerObstacle;

typedef struct TestActor {
    DynaPolyActor dyna;
    f32 offset;
    s32 enabled;
    CorridorSectionInfo* corridorSections[N_CORRIDORS];
    u32 corridorSectionsRand[N_CORRIDORS];
    CorridorSectionInfo* nextCorridorSection;
    s32 curCorridorSectionIdx;
    s32 timeBeforeNewCorridorType;
    s32 curCorridorSectionTotal;
    RunnerObstacle obstacles[N_OBSTACLES_MAX];
    s32 timeBeforeNewObstacle;
    PlayerRunner playerRunner;
} TestActor;

extern const ActorInit TestActor_InitVars;

#endif
