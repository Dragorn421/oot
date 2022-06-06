/*
 * File: z_door_toki.c
 * Overlay: ovl_Door_Toki
 * Description: Door of Time Collision
 */

#include "z_door_toki.h"
#include "assets/objects/object_toki_objects/object_toki_objects.h"

#define FLAGS 0

void DoorToki_Init(Actor* thisx, PlayState* play);
void DoorToki_Destroy(Actor* thisx, PlayState* play);
void DoorToki_Update(Actor* thisx, PlayState* play);

const ActorInit Door_Toki_InitVars = {
    ACTOR_DOOR_TOKI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(DoorToki),
    (ActorFunc)DoorToki_Init,
    (ActorFunc)DoorToki_Destroy,
    (ActorFunc)DoorToki_Update,
    NULL,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void DoorToki_Init(Actor* thisx, PlayState* play) {
    DoorToki* this = (DoorToki*)thisx;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    DynaPolyActor_Init(&this->actor, DPM_UNK);
    CollisionHeader_GetVirtual(&gDoorTokiCol, &colHeader);
    this->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->actor, colHeader);
}

void DoorToki_Destroy(Actor* thisx, PlayState* play) {
    DoorToki* this = (DoorToki*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->bgId);
}

void DoorToki_Update(Actor* thisx, PlayState* play) {
    DoorToki* this = (DoorToki*)thisx;

    if (GET_EVENTCHKINF(EVENTCHKINF_4B)) {
        func_8003EBF8(play, &play->colCtx.dyna, this->bgId);
    } else {
        func_8003EC50(play, &play->colCtx.dyna, this->bgId);
    }
}
