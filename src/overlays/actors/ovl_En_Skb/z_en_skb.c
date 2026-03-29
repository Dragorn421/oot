#include "z_en_skb.h"

#define FLAGS 0x00000015

#define THIS ((EnSkb*)thisx)

void EnSkb_Init(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Update(Actor* thisx, GlobalContext* globalCtx);
void EnSkb_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AFCDF8(EnSkb* this);
void func_80AFCE5C(EnSkb* this, GlobalContext* globalCtx);
void func_80AFCF48(EnSkb* this);
void func_80AFCFF0(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD0A4(EnSkb* this);
void func_80AFD13C(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD33C(EnSkb* this);
void func_80AFD3D4(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD47C(EnSkb* this);
void func_80AFD508(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD59C(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD6CC(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD7B4(EnSkb* this, GlobalContext* globalCtx);
void func_80AFD880(EnSkb* this, GlobalContext* globalCtx);

extern AnimationHeader D_6000460;
extern AnimationHeader D_60009DC;
extern AnimationHeader D_6000D98;
extern AnimationHeader D_6001854;
extern SkeletonHeader D_60041F8;
extern AnimationHeader D_60047E0;

static ColliderJntSphElementInit D_80AFE020[2] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0x00000000, 0x00, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_NONE,
            OCELEM_NONE,
        },
        { 15, { { 0, 0, 0 }, 10 }, 100 },
    },
    {
        {
            ELEMTYPE_UNK0,
            { 0x00000000, 0x00, 0x00 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            TOUCH_NONE,
            BUMP_ON | BUMP_HOOKABLE,
            OCELEM_ON,
        },
        { 1, { { 0, 0, 0 }, 20 }, 100 },
    },
};

static ColliderJntSphInit D_80AFE068 = {
    {
        COLTYPE_HIT6,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    2,
    D_80AFE020,
};

static DamageTable D_80AFE078 = {
    {
        0x10, 0xF2, 0xF1, 0xF2, 0x10, 0xF2, 0xF2, 0x10, 0xE1, 0xF2, 0xF4, 0x74, 0xF2, 0xF2, 0xF2, 0,
        0,    0x74, 0x60, 0xD3, 0,    0,    0xD1, 0xF4, 0xF2, 0xF2, 0xF8, 0xF4, 0,    0,    0xF4, 0,
    },
};

const ActorInit En_Skb_InitVars = {
    ACTOR_EN_SKB,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_SKB,
    sizeof(EnSkb),
    (ActorFunc)EnSkb_Init,
    (ActorFunc)EnSkb_Destroy,
    (ActorFunc)EnSkb_Update,
    (ActorFunc)EnSkb_Draw,
};

static Vec3f D_80AFE0B8 = { 0.0f, 8.0f, 0.0f };
static Vec3f D_80AFE0C4 = { 0.0f, -1.5f, 0.0f };
static InitChainEntry D_80AFE0D0[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_STOP),
};

void func_80AFC9A0(EnSkb* this, void (*arg1)(EnSkb*, GlobalContext*)) {
    this->unk284 = arg1;
}

void func_80AFC9A8(GlobalContext* globalCtx, EnSkb* this, Vec3f* arg2) {
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    f32 temp_fa0;
    s32 pad;

    sp50 = D_80AFE0B8;
    sp44 = D_80AFE0C4;
    temp_fa0 = (Rand_ZeroOne() - 0.5f) * 6.28f;
    sp5C.y = this->actor.floorHeight;
    sp5C.x = (Math_SinF(temp_fa0) * 15.0f) + arg2->x;
    sp5C.z = (Math_CosF(temp_fa0) * 15.0f) + arg2->z;
    sp44.x = Rand_CenteredFloat(1.0f);
    sp44.z = Rand_CenteredFloat(1.0f);
    sp50.y += (Rand_ZeroOne() - 0.5f) * 4.0f;
    EffectSsHahen_Spawn(globalCtx, &sp5C, &sp50, &sp44, 2, ((Rand_ZeroOne() * 5.0f) + 12.0f) * 0.8f, -1, 10, NULL);
    func_80033480(globalCtx, &sp5C, 10.0f, 1, 0x96, 0, 1U);
}

void EnSkb_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* this = (EnSkb*)thisx;

    Actor_ProcessInitChain(&this->actor, D_80AFE0D0);
    this->actor.colChkInfo.damageTable = &D_80AFE078;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.colChkInfo.mass = MASS_HEAVY;
    this->actor.colChkInfo.health = 2;
    this->actor.shape.yOffset = -8000.0f;
    SkelAnime_Init(globalCtx, &this->unk14C, &D_60041F8, &D_6001854, this->unk190, this->unk208, 20);
    this->actor.naviEnemyId = 0x55;
    Collider_InitJntSph(globalCtx, &this->unk2A4);
    Collider_SetJntSph(globalCtx, &this->unk2A4, &this->actor, &D_80AFE068, this->unk2C4);
    Actor_SetScale(&this->actor, ((this->actor.params * 0.1f) + 1.0f) * 0.01f);
    this->unk2A4.elements[0].dim.modelSphere.radius = this->unk2A4.elements[0].dim.worldSphere.radius =
        this->actor.params + 10;
    this->unk2A4.elements[1].dim.modelSphere.radius = this->unk2A4.elements[1].dim.worldSphere.radius =
        (this->actor.params + 10) * 2;
    this->actor.home.pos = this->actor.world.pos;
    this->actor.floorHeight = this->actor.world.pos.y;
    func_80AFCDF8(this);
}

typedef struct UnkActor {
    Actor actor;
    char pad14C[6];
    s16 unk152;
} UnkActor;

void EnSkb_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    UnkActor* temp_v1;
    EnSkb* this = (EnSkb*)thisx;

    if ((this->actor.parent != NULL)) {
        temp_v1 = (UnkActor*)this->actor.parent;
        if ((temp_v1->actor.update != NULL)) {
            if (temp_v1->unk152 > 0) {
                temp_v1->unk152 -= 1;
            }
        }
    }
    Collider_DestroyJntSph(globalCtx, &this->unk2A4);
}

void func_80AFCD60(EnSkb* this) {
    if (gSaveContext.nightFlag == 0) {
        func_80AFCF48(this);
    } else if ((func_8002E084(&this->actor, 0x11C7) != 0) &&
               (this->actor.xzDistToPlayer < (60.0f + (this->actor.params * 6.0f)))) {
        func_80AFD33C(this);
    } else {
        func_80AFD0A4(this);
    }
}

void func_80AFCDF8(EnSkb* this) {
    Animation_PlayOnceSetSpeed(&this->unk14C, &D_6001854, 1.0f);
    this->unk280 = 0;
    this->actor.flags &= ~1;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_RIVA_APPEAR);
    func_80AFC9A0(this, func_80AFCE5C);
}

void func_80AFCE5C(EnSkb* this, GlobalContext* globalCtx) {
    if (this->unk14C.curFrame < 4.0f) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    } else {
        this->actor.flags |= 1;
    }
    Math_SmoothStepToF(&this->actor.shape.yOffset, 0.0f, 1.0f, 800.0f, 0.0f);
    Math_SmoothStepToF(&this->actor.shape.shadowScale, 25.0f, 1.0f, 2.5f, 0.0f);
    if (globalCtx->gameplayFrames & 1) {
        func_80AFC9A8(globalCtx, this, &this->actor.world.pos);
    }
    if (SkelAnime_Update(&this->unk14C) && (this->actor.shape.yOffset == 0.0f)) {
        func_80AFCD60(this);
    }
}

void func_80AFCF48(EnSkb* this) {
    Animation_Change(&this->unk14C, &D_6001854, -1.0f, Animation_GetLastFrame(&D_6001854), 0.0f, ANIMMODE_ONCE, -4.0f);
    this->unk280 = 0;
    this->unk281 = 0;
    this->actor.flags &= ~1;
    this->actor.speedXZ = 0.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_AKINDONUTS_HIDE);
    func_80AFC9A0(this, func_80AFCFF0);
}

void func_80AFCFF0(EnSkb* this, GlobalContext* globalCtx) {
    if ((Math_SmoothStepToF(&this->actor.shape.yOffset, -8000.0f, 1.0f, 500.0f, 0.0f) != 0.0f) &&
        (globalCtx->gameplayFrames & 1)) {
        func_80AFC9A8(globalCtx, this, &this->actor.world.pos);
    }
    Math_SmoothStepToF(&this->actor.shape.shadowScale, 0.0f, 1.0f, 2.5f, 0.0f);
    if (SkelAnime_Update(&this->unk14C) != 0) {
        Actor_Kill(&this->actor);
    }
}

void func_80AFD0A4(EnSkb* this) {
    Animation_Change(&this->unk14C, &D_60047E0, 0.96000004f, 0.0f, Animation_GetLastFrame(&D_60047E0), ANIMMODE_LOOP,
                     -4.0f);
    this->unk280 = 4;
    this->unk288 = 0;
    this->actor.speedXZ = this->actor.scale.y * 160.0f;
    func_80AFC9A0(this, func_80AFD13C);
}

void func_80AFD13C(EnSkb* this, GlobalContext* globalCtx) {
    s32 temp_ft2_sp34;
    f32 var_fv1_2;
    s32 temp_ft4;
    Actor* sp28;

    sp28 = globalCtx->actorCtx.actorLists[2].head;
    if ((this->unk283 != 0) && !(globalCtx->gameplayFrames & 0xF)) {
        this->unk288 = (s16)(s32)Rand_CenteredFloat(50000.0f);
    }
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer + this->unk288, 1, 0x2EE, 0);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    temp_ft2_sp34 = (s32)this->unk14C.curFrame;
    SkelAnime_Update(&this->unk14C);
    var_fv1_2 = ABS(this->unk14C.playSpeed);
    temp_ft4 = (s32)(this->unk14C.curFrame - var_fv1_2);
    var_fv1_2 = ABS(this->unk14C.playSpeed);
    if ((temp_ft2_sp34 != (s32)this->unk14C.curFrame) &&
        (((temp_ft4 < 9) && (((s32)var_fv1_2 + temp_ft2_sp34) >= 8)) ||
         ((temp_ft4 < 0x10) && (((s32)var_fv1_2 + temp_ft2_sp34) >= 0xF)))) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STALKID_WALK);
    }
    if ((Math_Vec3f_DistXZ(&this->actor.home.pos, &sp28->world.pos) > 800.0f) || (gSaveContext.nightFlag == 0)) {
        func_80AFCF48(this);
    } else if ((func_8002E084(&this->actor, 0x11C7) != 0) &&
               (this->actor.xzDistToPlayer < (60.0f + (this->actor.params * 6.0f)))) {
        func_80AFD33C(this);
    }
}

void func_80AFD33C(EnSkb* this) {
    Animation_Change(&this->unk14C, &D_6000460, 0.6f, 0.0f, Animation_GetLastFrame(&D_6000460), ANIMMODE_ONCE_INTERP,
                     4.0f);
    this->unk2A4.base.atFlags &= ~AT_BOUNCED;
    this->unk280 = 3;
    this->actor.speedXZ = 0.0f;
    func_80AFC9A0(this, func_80AFD3D4);
}

void func_80AFD3D4(EnSkb* this, GlobalContext* globalCtx) {
    s32 temp_ft1;
    u8 temp_v0;

    temp_ft1 = (s32)this->unk14C.curFrame;
    if (temp_ft1 == 3) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_STALKID_ATTACK);
        this->unk281 = 1;
    } else if (temp_ft1 == 6) {
        this->unk281 = 0;
    }
    if (this->unk2A4.base.atFlags & AT_BOUNCED) {
        this->unk2A4.base.atFlags &= ~(AT_HIT | AT_BOUNCED);
        func_80AFD47C(this);
    } else if (SkelAnime_Update(&this->unk14C)) {
        func_80AFCD60(this);
    }
}

void func_80AFD47C(EnSkb* this) {
    Animation_Change(&this->unk14C, &D_6000460, -0.4f, this->unk14C.curFrame - 1.0f, 0.0f, ANIMMODE_ONCE_INTERP, 0.0f);
    this->unk2A4.base.atFlags &= ~AT_BOUNCED;
    this->unk280 = 5;
    this->unk281 = 0;
    func_80AFC9A0(this, func_80AFD508);
}

void func_80AFD508(EnSkb* this, GlobalContext* globalCtx) {
    if (SkelAnime_Update(&this->unk14C)) {
        func_80AFCD60(this);
    }
}

void func_80AFD540(EnSkb* this) {
    if (this->actor.bgCheckFlags & 1) {
        this->actor.speedXZ = 0.0f;
    }
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->unk281 = 0;
    this->unk280 = 6;
    func_80AFC9A0(this, func_80AFD59C);
}

void func_80AFD59C(EnSkb* this, GlobalContext* globalCtx) {
    f32 temp_fv0;

    if (this->actor.bgCheckFlags & 2) {
        this->actor.speedXZ = 0.0f;
    }
    if (this->actor.bgCheckFlags & 1) {
        if (this->actor.speedXZ < 0.0f) {
            this->actor.speedXZ += 0.05f;
        }
    }
    if ((this->actor.colorFilterTimer == 0) && (this->actor.bgCheckFlags & 1)) {
        if (this->actor.colChkInfo.health == 0) {
            func_80AFD7B4(this, globalCtx);
        } else {
            func_80AFCD60(this);
        }
    }
}

void func_80AFD644(EnSkb* this) {
    Animation_MorphToPlayOnce(&this->unk14C, &D_6000D98, -4.0f);
    if (this->actor.bgCheckFlags & 1) {
        this->actor.speedXZ = -4.0f;
    }
    this->actor.world.rot.y = this->actor.yawTowardsPlayer;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_STALKID_DAMAGE);
    this->unk280 = 2;
    func_80AFC9A0(this, func_80AFD6CC);
}

void func_80AFD6CC(EnSkb* this, GlobalContext* globalCtx) {
    u8* new_var;

    new_var = &this->unk283;
    if ((this->unk283 != 1) || (func_8003305C(&this->actor, &this->unk28C, globalCtx, 1) != 0)) {
        if (*new_var != 0) {
            this->unk283 = *new_var | 2;
        }
        if (this->actor.bgCheckFlags & 2) {
            this->actor.speedXZ = 0.0f;
        }
        if (this->actor.bgCheckFlags & 1) {
            if (this->actor.speedXZ < 0.0f) {
                this->actor.speedXZ += 0.05f;
            }
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 1, 0x1194, 0);
        if (SkelAnime_Update(&this->unk14C) && (this->actor.bgCheckFlags & 1)) {
            func_80AFCD60(this);
        }
    }
}

void func_80AFD7B4(EnSkb* this, GlobalContext* globalCtx) {
    Animation_MorphToPlayOnce(&this->unk14C, &D_60009DC, -4.0f);
    this->actor.world.rot.y = this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
    if (this->actor.bgCheckFlags & 1) {
        this->actor.speedXZ = -6.0f;
    }
    this->unk280 = 1;
    this->actor.flags &= ~1;
    func_80032E24(&this->unk28C, 0x12, globalCtx);
    this->unk283 |= 4;
    EffectSsDeadSound_SpawnStationary(globalCtx, &this->actor.projectedPos, NA_SE_EN_STALKID_DEAD, 1, 1, 40);
    func_80AFC9A0(this, func_80AFD880);
}

void func_80AFD880(EnSkb* this, GlobalContext* globalCtx) {

    if (func_8003305C(&this->actor, &this->unk28C, globalCtx, 1) != 0) {
        if (this->actor.scale.x == 0.01f) {
            Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.world.pos, 0x10);
        } else {
            if (this->actor.scale.x <= 0.015f) {
                Item_DropCollectible(globalCtx, &this->actor.world.pos, 1);
            } else {
                Item_DropCollectible(globalCtx, &this->actor.world.pos, 2);
                Item_DropCollectible(globalCtx, &this->actor.world.pos, 2);
                Item_DropCollectible(globalCtx, &this->actor.world.pos, 2);
            }
        }
        this->unk283 |= 8;
        Actor_Kill(&this->actor);
    }
}

void func_80AFD968(EnSkb* this, GlobalContext* globalCtx) {
    Player* player;
    s32 var_v1;
    Vec3f sp54;
    s16 sp52;
    s32 var_s0;

    if ((this->unk280 != 1) && (this->actor.bgCheckFlags & 0x60) && (this->actor.yDistToWater >= 40.0f)) {
        this->actor.colChkInfo.health = 0;
        this->unk281 = 0;
        func_80AFD7B4(this, globalCtx);
    } else if ((this->unk280 >= 3) && (this->unk2A4.base.acFlags & AC_HIT)) {
        this->unk2A4.base.acFlags &= ~AC_HIT;
        if (((this->actor.colChkInfo.damageEffect != 6))) {
            this->unk282 = this->actor.colChkInfo.damageEffect;
            func_80035650(&this->actor, &this->unk2A4.elements[1].info, 1);
            this->unk281 = 0;
            if (this->actor.colChkInfo.damageEffect == 1) {
                if (this->unk280 != 6) {
                    Actor_SetColorFilter(&this->actor, 0, 0x78, 0, 80);
                    Actor_ApplyDamage(&this->actor);
                    func_80AFD540(this);
                }
            } else {
                var_v1 = 8;
                if (this->actor.colChkInfo.damageEffect == 7) {
                    sp52 = this->actor.scale.y * 7500.0f;
                    for (var_s0 = 4; var_s0 >= 0; var_s0--) {
                        sp54 = this->actor.world.pos;
                        sp54.x += Rand_CenteredFloat(20.0f);
                        sp54.z += Rand_CenteredFloat(20.0f);
                        sp54.y += Rand_ZeroOne() * 25.0f;
                        EffectSsEnFire_SpawnVec3f(globalCtx, &this->actor, &sp54, sp52, 0, 0, -1);
                    }
                    var_v1 = 25;
                }
                Actor_SetColorFilter(&this->actor, 0x4000, 0xFF, 0, var_v1);
                if (Actor_ApplyDamage(&this->actor) == 0) {
                    func_80AFD7B4(this, globalCtx);
                } else {
                    player = PLAYER;
                    if ((this->unk283 == 0) &&
                        (((this->actor.colChkInfo.damageEffect == 0xD)) ||
                         ((this->actor.colChkInfo.damageEffect == 0xE) &&
                          (((((player->swordAnimation >= 4))) && (player->swordAnimation < 0xC)) ||
                           (player->swordAnimation == 0x14) || (player->swordAnimation == 0x15))))) {
                        func_80032E24(&this->unk28C, 2, globalCtx);
                        this->unk283 = 1;
                    }
                    func_80AFD644(this);
                }
            }
        }
    }
}

void EnSkb_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* this = (EnSkb*)thisx;
    s32 pad;

    func_80AFD968(this, globalCtx);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 15.0f, 30.0f, 60.0f, 0x1D);
    this->unk284(this, globalCtx);
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 3000.0f * this->actor.scale.y;
    if (this->unk281 != 0) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk2A4.base);
    }
    if ((this->unk280 >= 3) && ((this->actor.colorFilterTimer == 0) || !(this->actor.colorFilterParams & 0x4000))) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk2A4.base);
    }
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk2A4.base);
}

s32 func_80AFDD30(GlobalContext* arg0, s32 arg1, Gfx** arg2, Vec3f* arg3, Vec3s* arg5, void* thisx) {
    EnSkb* this = thisx;
    s32 pad;
    s32 temp_v0;

    if (arg1 == 11) {
        if (!(this->unk283 & 2)) {
            OPEN_DISPS(arg0->state.gfxCtx, "../z_en_skb.c", 972);
            temp_v0 = (s16)(ABS((s16)(Math_SinS(arg0->gameplayFrames * 0x1770) * 95.0f)) + 0xA0) & 0xFF;
            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, temp_v0, temp_v0, temp_v0, 255);
            CLOSE_DISPS(arg0->state.gfxCtx, "../z_en_skb.c", 978);
        } else {
            *arg2 = NULL;
        }
    } else if ((arg1 == 12) && (this->unk283 & 2)) {
        *arg2 = NULL;
    }
    return 0;
}

void func_80AFDF24(GlobalContext* arg0, s32 arg1, Gfx** arg2, Vec3s* arg3, void* thisx) {
    EnSkb* this = thisx;

    Collider_UpdateSpheres(arg1, &this->unk2A4);
    if ((this->unk283 ^ 1) == 0) {
        func_80032F54(&this->unk28C, arg1, 0xB, 0xC, 0x12U, arg2, -1);
    } else if ((this->unk283 ^ (this->unk283 | 4)) == 0) {
        func_80032F54(&this->unk28C, arg1, 0, 0x12, 0x12U, arg2, -1);
    }
}

void EnSkb_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnSkb* this = (EnSkb*)thisx;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawOpa(globalCtx, this->unk14C.skeleton, this->unk14C.jointTable, func_80AFDD30, func_80AFDF24, this);
}
