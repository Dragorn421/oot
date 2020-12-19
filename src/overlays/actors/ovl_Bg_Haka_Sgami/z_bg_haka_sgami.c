/*
 * File: z_bg_haka_sgami.c
 * Overlay: ovl_Bg_Haka_Sgami
 * Description: Spinning Scythe Trap
 */

#include "z_bg_haka_sgami.h"

#define FLAGS 0x00000011

#define THIS ((BgHakaSgami*)thisx)

typedef enum {
    /* 0 */ SCYTHE_TRAP_SHADOW_TEMPLE,
    /* 1 */ SCYTHE_TRAP_SHADOW_TEMPLE_INVISIBLE,
    /* 2 */ SCYTHE_TRAP_ICE_CAVERN
} SpinningScytheTrapMode;

#define SCYTHE_SPIN_TIME 32

void BgHakaSgami_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaSgami_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaSgami_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaSgami_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgHakaSgami_SetupSpin(BgHakaSgami* this, GlobalContext* globalCtx);
void BgHakaSgami_Spin(BgHakaSgami* this, GlobalContext* globalCtx);

extern Gfx D_0600BF20[];
extern Gfx D_060021F0[];

const ActorInit Bg_Haka_Sgami_InitVars = {
    ACTOR_BG_HAKA_SGAMI,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(BgHakaSgami),
    (ActorFunc)BgHakaSgami_Init,
    (ActorFunc)BgHakaSgami_Destroy,
    (ActorFunc)BgHakaSgami_Update,
    NULL,
};

static ColliderTrisElementSrc sTriItemsInit[4] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            {
                0x20000000,
                HIT_SPECIAL_EFFECT_NONE,
                4,
            },
            {
                0x00000000,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        {
            {
                365.0,
                45.0,
                27.0,
            }, // 0
            {
                130.0,
                45.0,
                150.0,
            }, // 1
            {
                290.0,
                45.0,
                145.0,
            }, // 2
        },
    }, // 0
    {
        {
            ELEM_MATERIAL_UNK2,
            {
                0x20000000,
                HIT_SPECIAL_EFFECT_NONE,
                4,
            },
            {
                0x00000000,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        {
            {
                250.0,
                45.0,
                90.0,
            }, // 0
            {
                50.0,
                45.0,
                80.0,
            }, // 1
            {
                160.0,
                45.0,
                160.0,
            }, // 2
        },
    }, // 1
    {
        {
            ELEM_MATERIAL_UNK2,
            {
                0x20000000,
                HIT_SPECIAL_EFFECT_NONE,
                4,
            },
            {
                0x00000000,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        {
            {
                -305.0,
                33.0,
                -7.0,
            }, // 0
            {
                -220.0,
                33.0,
                40.0,
            }, // 1
            {
                -130.0,
                33.0,
                -5.0,
            }, // 2
        },
    }, // 2
    {
        {
            ELEM_MATERIAL_UNK2,
            {
                0x20000000,
                HIT_SPECIAL_EFFECT_NONE,
                4,
            },
            {
                0x00000000,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_NONE,
            OCELEM_NONE,
        },
        {
            {
                -190.0,
                33.0,
                40.0,
            }, // 0
            {
                -30.0,
                33.0,
                15.0,
            }, // 1
            {
                -70.0,
                33.0,
                -30.0,
            }, // 2
        },
    }, // 3
};

static ColliderTrisSrc sTrisInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_2,
        COLTYPE_TRIANGLES,
    },
    4,
    sTriItemsInit,
};

static ColliderCylinderSrc sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
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
        80,
        130,
        0,
        {
            0,
            0,
            0,
        },
    },
};

static CollideDataInit sCollideDataInit = { 0, 80, 130, 0xFF };

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1000, ICHAIN_CONTINUE),
    ICHAIN_U8(unk_1F, 4, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaSgami_Init(Actor* thisx, GlobalContext* globalCtx) {
    static u8 sP1StartColor[] = { 250, 250, 250, 200 };
    static u8 sP2StartColor[] = { 200, 200, 200, 130 };
    static u8 sP1EndColor[] = { 200, 200, 200, 60 };
    static u8 sP2EndColor[] = { 150, 150, 150, 20 };
    BgHakaSgami* this = THIS;
    EffectBlureInit1 blureInit;
    s32 i;
    ColliderTris* colliderScythe = &this->colliderScythe;

    Actor_ProcessInitChain(thisx, sInitChain);

    this->unk_151 = thisx->params & 0xFF;
    thisx->params = (thisx->params >> 8) & 0xFF;

    if (this->unk_151 != 0) {
        thisx->flags |= 0x80;
    }

    Collider_InitTris(globalCtx, colliderScythe);
    Collider_LoadTris(globalCtx, colliderScythe, thisx, &sTrisInit, this->colliderScytheItems);
    Collider_InitCylinder(globalCtx, &this->colliderScytheCenter);
    Collider_LoadCylinder(globalCtx, &this->colliderScytheCenter, thisx, &sCylinderInit);

    this->colliderScytheCenter.shape.pos.x = thisx->posRot.pos.x;
    this->colliderScytheCenter.shape.pos.y = thisx->posRot.pos.y;
    this->colliderScytheCenter.shape.pos.z = thisx->posRot.pos.z;

    func_80061ED4(&thisx->collideData, NULL, &sCollideDataInit);

    for (i = 0; i < 4; i++) {
        blureInit.p1StartColor[i] = sP1StartColor[i];
        blureInit.p2StartColor[i] = sP2StartColor[i];
        blureInit.p1EndColor[i] = sP1EndColor[i];
        blureInit.p2EndColor[i] = sP2EndColor[i];
    }
    blureInit.elemDuration = 10;
    blureInit.unkFlag = false;
    blureInit.calcMode = 2;
    Effect_Add(globalCtx, &this->blureEffectIndex[0], EFFECT_BLURE1, 0, 0, &blureInit);
    Effect_Add(globalCtx, &this->blureEffectIndex[1], EFFECT_BLURE1, 0, 0, &blureInit);

    if (thisx->params == SCYTHE_TRAP_SHADOW_TEMPLE) {
        this->requiredObjBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_HAKA_OBJECTS);
        thisx->flags &= ~1;
    } else {
        this->requiredObjBankIndex = Object_GetIndex(&globalCtx->objectCtx, OBJECT_ICE_OBJECTS);
        this->colliderScytheCenter.shape.radius = 30;
        this->colliderScytheCenter.shape.height = 70;
        Actor_SetHeight(thisx, 40.0f);
    }

    if (this->requiredObjBankIndex < 0) {
        Actor_Kill(thisx);
        return;
    }

    this->actionFunc = BgHakaSgami_SetupSpin;
}

void BgHakaSgami_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaSgami* this = THIS;

    Effect_Delete(globalCtx, this->blureEffectIndex[0]);
    Effect_Delete(globalCtx, this->blureEffectIndex[1]);
    Collider_DestroyTris(globalCtx, &this->colliderScythe);
    Collider_DestroyCylinder(globalCtx, &this->colliderScytheCenter);
}

void BgHakaSgami_SetupSpin(BgHakaSgami* this, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, this->requiredObjBankIndex)) {
        this->actor.objBankIndex = this->requiredObjBankIndex;
        this->actor.draw = BgHakaSgami_Draw;
        this->timer = SCYTHE_SPIN_TIME;
        this->actor.flags &= ~0x10;
        this->actionFunc = BgHakaSgami_Spin;
    }
}

void BgHakaSgami_Spin(BgHakaSgami* this, GlobalContext* globalCtx) {
    static Vec3f blureEffectVertices2[] = {
        { -20.0f, 50.0f, 130.0f },
        { -50.0f, 33.0f, 20.0f },
    };
    static Vec3f blureEffectVertices1[] = {
        { 380.0f, 50.0f, 50.0f },
        { 310.0f, 33.0f, 0.0f },
    };
    s32 i;
    s32 j;
    Vec3f scytheVertices[3];
    f32 actorRotYSin;
    f32 actorRotYCos;
    s32 iterateCount;
    ColliderTrisElementSrc* colliderList;

    if (this->timer != 0) {
        this->timer--;
    }

    this->actor.shape.rot.y += ((s16)(512.0f * sinf(this->timer * (M_PI / 16.0f))) + 0x400) >> 1;

    if (this->timer == 0) {
        this->timer = SCYTHE_SPIN_TIME;
    }

    actorRotYSin = Math_Sins(this->actor.shape.rot.y);
    actorRotYCos = Math_Coss(this->actor.shape.rot.y);

    iterateCount = (this->actor.params != 0) ? 4 : 2;

    for (i = iterateCount - 2; i < iterateCount; i++) {
        colliderList = &sTrisInit.elements[i];

        for (j = 0; j < 3; j++) {
            scytheVertices[j].x = this->actor.posRot.pos.x + colliderList->vertices[j].z * actorRotYSin +
                                  colliderList->vertices[j].x * actorRotYCos;
            scytheVertices[j].y = this->actor.posRot.pos.y + colliderList->vertices[j].y;
            scytheVertices[j].z = this->actor.posRot.pos.z + colliderList->vertices[j].z * actorRotYCos -
                                  colliderList->vertices[j].x * actorRotYSin;
        }

        func_800627A0(&this->colliderScythe, i, &scytheVertices[0], &scytheVertices[1], &scytheVertices[2]);

        for (j = 0; j < 3; j++) {
            scytheVertices[j].x = (2 * this->actor.posRot.pos.x) - scytheVertices[j].x;
            scytheVertices[j].z = (2 * this->actor.posRot.pos.z) - scytheVertices[j].z;
        }

        func_800627A0(&this->colliderScythe, (i + 2) % 4, &scytheVertices[0], &scytheVertices[1], &scytheVertices[2]);
    }

    if ((this->unk_151 == 0) || (globalCtx->actorCtx.unk_03 != 0)) {
        scytheVertices[0].x = this->actor.posRot.pos.x + blureEffectVertices1[this->actor.params].z * actorRotYSin +
                              blureEffectVertices1[this->actor.params].x * actorRotYCos;
        scytheVertices[0].y = this->actor.posRot.pos.y + blureEffectVertices1[this->actor.params].y;
        scytheVertices[0].z = this->actor.posRot.pos.z + blureEffectVertices1[this->actor.params].z * actorRotYCos -
                              blureEffectVertices1[this->actor.params].x * actorRotYSin;
        scytheVertices[1].x = this->actor.posRot.pos.x + blureEffectVertices2[this->actor.params].z * actorRotYSin +
                              blureEffectVertices2[this->actor.params].x * actorRotYCos;
        scytheVertices[1].y = this->actor.posRot.pos.y + blureEffectVertices2[this->actor.params].y;
        scytheVertices[1].z = this->actor.posRot.pos.z + blureEffectVertices2[this->actor.params].z * actorRotYCos -
                              blureEffectVertices2[this->actor.params].x * actorRotYSin;
        EffectBlure_AddVertex(Effect_GetByIndex(this->blureEffectIndex[0]), &scytheVertices[0], &scytheVertices[1]);

        for (j = 0; j < 2; j++) {
            scytheVertices[j].x = (2 * this->actor.posRot.pos.x) - scytheVertices[j].x;
            scytheVertices[j].z = (2 * this->actor.posRot.pos.z) - scytheVertices[j].z;
        }

        EffectBlure_AddVertex(Effect_GetByIndex(this->blureEffectIndex[1]), &scytheVertices[0], &scytheVertices[1]);
    }

    Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &this->colliderScythe.base);
    Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &this->colliderScytheCenter.base);
    func_8002F974(&this->actor, NA_SE_EV_ROLLCUTTER_MOTOR - SFX_FLAG);
}

void BgHakaSgami_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaSgami* this = THIS;
    Player* player = PLAYER;

    if (!(player->stateFlags1 & 0x300000C0) || (this->actionFunc == BgHakaSgami_SetupSpin)) {
        this->actionFunc(this, globalCtx);
    }
}

void BgHakaSgami_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaSgami* this = THIS;

    if (this->unk_151 != 0) {
        Gfx_DrawDListXlu(globalCtx, D_0600BF20);
    } else if (this->actor.params == SCYTHE_TRAP_SHADOW_TEMPLE) {
        Gfx_DrawDListOpa(globalCtx, D_0600BF20);
    } else {
        Gfx_DrawDListOpa(globalCtx, D_060021F0);
    }
}
