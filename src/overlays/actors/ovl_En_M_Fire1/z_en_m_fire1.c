/*
 * File: z_en_m_fire1.c
 * Overlay: ovl_En_M_Fire1
 * Description: The attack from a Deku Nut. Spawned by ovl_En_Arrow.
 */

#include "z_en_m_fire1.h"

#define FLAGS 0x00000000

void EnMFire1_Init(EnMFire1* this, GlobalContext* globalCtx);
void EnMFire1_Destroy(EnMFire1* this, GlobalContext* globalCtx);
void EnMFire1_Update(EnMFire1* this, GlobalContext* globalCtx);

const ActorInit En_M_Fire1_InitVars = {
    ACTOR_EN_M_FIRE1,
    ACTORTYPE_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnMFire1),
    (ActorFunc)EnMFire1_Init,
    (ActorFunc)EnMFire1_Destroy,
    (ActorFunc)EnMFire1_Update,
    NULL,
};

static ColliderCylinderSrc cylinderInitData = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_PLAYER,
        COLTYPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        {
            0x00000001,
            HIT_SPECIAL_EFFECT_NONE,
            0,
        },
        {
            0xFFCFFFFF,
            HIT_BACKLASH_NONE,
            0,
        },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_NONE,
        OCELEM_NONE,
    },
    {
        200,
        200,
        0,
        {
            0,
            0,
            0,
        },
    },
};

void EnMFire1_Init(EnMFire1* this, GlobalContext* globalCtx) {
    s32 pad;
    EnMFire1* thisLocal;

    thisLocal = this;

    if (this->actor.params < 0) {
        Actor_ChangeType(globalCtx, &globalCtx->actorCtx, &thisLocal->actor, ACTORTYPE_ITEMACTION);
    }

    Collider_InitCylinder(globalCtx, &thisLocal->collider);
    Collider_LoadCylinder(globalCtx, &thisLocal->collider, &thisLocal->actor, &cylinderInitData);
}

void EnMFire1_Destroy(EnMFire1* this, GlobalContext* globalCtx) {
    ColliderCylinder* collider = &this->collider;
    Collider_DestroyCylinder(globalCtx, collider);
}

void EnMFire1_Update(EnMFire1* this, GlobalContext* globalCtx) {
    s32 pad;
    EnMFire1* thisLocal = this;

    if (Math_ApproxF(&thisLocal->unk_198, 1.0f, 0.2f)) {
        Actor_Kill(&this->actor);
    } else {
        Collider_UpdateCylinderShape(&thisLocal->actor, &thisLocal->collider);
        Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &thisLocal->collider);
    }
}
