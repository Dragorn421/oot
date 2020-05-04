/*
 * File: z_en_it.c
 * Overlay: ovl_En_It
 * Description: Dampe's Minigame digging spot hitboxes
 */

#include "z_en_it.h"

#define FLAGS 0x00000000

#define THIS ((EnIt*)thisx)

void EnIt_Init(Actor* thisx, GlobalContext* globalCtx);
void EnIt_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnIt_Update(Actor* thisx, GlobalContext* globalCtx);

static ColliderCylinderSrc cylinderInitData = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_NO_PUSH,
        OC2_TYPE_1,
        COLTYPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        {
            0x00000000,
            HIT_SPECIAL_EFFECT_NONE,
            0,
        },
        {
            0x00000000,
            HIT_BACKLASH_NONE,
            0,
        },
        ATELEM_NONE,
        ACELEM_NONE,
        OCELEM_ON,
    },
    {
        40,
        10,
        0,
        {
            0,
            0,
            0,
        },
    },
};

static u8 collideDataInit[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
};

const ActorInit En_It_InitVars = {
    ACTOR_EN_IT,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnIt),
    (ActorFunc)EnIt_Init,
    (ActorFunc)EnIt_Destroy,
    (ActorFunc)EnIt_Update,
    (ActorFunc)NULL,
};

void EnIt_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnIt* this = THIS;

    this->actor.params = 0x0D05;
    Collider_InitCylinder(globalCtx, &this->collider);
    Collider_LoadCylinder(globalCtx, &this->collider, &this->actor, &cylinderInitData);
    func_80061EFC(&this->actor.collideData, 0, &collideDataInit); // Init Damage Chart
}

void EnIt_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnIt* this = THIS;

    Collider_DestroyCylinder(globalCtx, &this->collider);
}

void EnIt_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnIt* this = THIS;
    s32 pad;

    Collider_UpdateCylinderShape(&this->actor, &this->collider);
    Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &this->collider);
}
