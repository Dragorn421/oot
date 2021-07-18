#include "ultra64.h"
#include "z64.h"
#include "macros.h"

#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

extern s16 objectList[];
extern ActorEntry actorList[];
PolygonType0 meshHeader;

SCmdBase roomHeaders[] = {
    SCENE_CMD_ECHO_SETTINGS(0),
    SCENE_CMD_ROOM_BEHAVIOR(0x00, 0x00, false, false),
    SCENE_CMD_SKYBOX_DISABLES(false, false),
    SCENE_CMD_TIME_SETTINGS(255, 255, 10),
    SCENE_CMD_MESH(&meshHeader),
    SCENE_CMD_OBJECT_LIST(1, objectList),
    SCENE_CMD_ACTOR_LIST(1, actorList),
    SCENE_CMD_WIND_SETTINGS(15, 40, 109, 190),
    SCENE_CMD_END(),
};

s16 objectList[] = {
    OBJECT_MOD_CUBE,
};

ActorEntry actorList[] = {
    { ACTOR_MOD_ACTOR, { 0, 0, 0 }, { 0, 0, 0 }, 0x0000 },
};

PolygonType0 meshHeader = { 0, 0, NULL, NULL };
