#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "handpushplayground_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_HANDPUSHPLAYGROUND_ROOM_0_HEADER00_ACTORLIST 5
SceneCmd handpushplayground_room_0_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR(0x00, 0x00, false, false),
    SCENE_CMD_SKYBOX_DISABLES(false, false),
    SCENE_CMD_TIME_SETTINGS(0xFF, 0xFF, 10),
    SCENE_CMD_ROOM_SHAPE(&handpushplayground_room_0_shapeHeader),
    SCENE_CMD_ACTOR_LIST(LENGTH_HANDPUSHPLAYGROUND_ROOM_0_HEADER00_ACTORLIST, handpushplayground_room_0_header00_actorList),
    SCENE_CMD_END(),
};

ActorEntry handpushplayground_room_0_header00_actorList[LENGTH_HANDPUSHPLAYGROUND_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_ORI_PENSIVENEMY,
        /* Position   */ { 247, -965, -478 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ PENSIVEENEMY_PARAMS(0, 2)
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_ORI_PENSIVENEMY,
        /* Position   */ { 249, -965, -1061 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ PENSIVEENEMY_PARAMS(0, 2)
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_ORI_PENSIVENEMY,
        /* Position   */ { 249, -965, 53 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ PENSIVEENEMY_PARAMS(0, 2)
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_ORI_PENSIVENEMY,
        /* Position   */ { -2732, -889, 2401 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ PENSIVEENEMY_PARAMS(0, 2)
    },

    // Custom Actor
    {
        /* Actor ID   */ ACTOR_ORI_PENSIVENEMY,
        /* Position   */ { -287, -1111, 589 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ PENSIVEENEMY_PARAMS(0, 1)
    },
};

