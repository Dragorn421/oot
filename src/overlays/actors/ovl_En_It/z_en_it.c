/*
 * File: z_en_it.c
 * Overlay: ovl_En_It
 * Description: Dampe's Minigame digging spot hitboxes
 */

#include "z_en_it.h"

#define FLAGS 0x00000000

void EnIt_Init(EnIt* this, GlobalContext* globalCtx);
void EnIt_Destroy(EnIt* this, GlobalContext* globalCtx);
void EnIt_Update(EnIt* this, GlobalContext* globalCtx);

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

static u8 damageTblInitData[] = {
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

void EnIt_Init(EnIt* this, GlobalContext* globalCtx) {
    EnIt* it = this;

    it->actor.params = 0x0D05;
    Collider_InitCylinder(globalCtx, &it->collider);
    Collider_LoadCylinder(globalCtx, &it->collider, &it->actor, &cylinderInitData);
    func_80061EFC(&it->actor.collideData, 0, &damageTblInitData); // Init Damage Chart
}

void EnIt_Destroy(EnIt* this, GlobalContext* globalCtx) {
    EnIt* it = this;

    Collider_DestroyCylinder(globalCtx, &it->collider);
}

void EnIt_Update(EnIt* this, GlobalContext* globalCtx) {
    s32 pad;
    EnIt* it = this;

    Collider_UpdateCylinderShape(&it->actor, &it->collider);
    Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &it->collider);
}
