/*
 * File: z_en_tp.c
 * Overlay: ovl_En_Tp
 * Description: Electric Tailpasaran
 */

#include "z_en_tp.h"

#define FLAGS 0x00000000

#define THIS ((EnTp*)thisx)

void EnTp_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B20DE0(EnTp* this, void (*arg1)(EnTp*, GlobalContext*));
void func_80B21084(EnTp* this);
void func_80B210B0(EnTp* this, GlobalContext* globalCtx);
void func_80B2128C(EnTp* this);
void func_80B212C0(EnTp* this, GlobalContext* globalCtx);
void func_80B21454(EnTp* this);
void func_80B214CC(EnTp* this, GlobalContext* globalCtx);
void func_80B217FC(EnTp* this);
void func_80B21900(EnTp* this, GlobalContext* globalCtx);
void func_80B2194C(EnTp* this);
void func_80B219A8(EnTp* this, GlobalContext* globalCtx);
void func_80B21B90(EnTp* this);
void func_80B21BDC(EnTp* this, GlobalContext* globalCtx);
void func_80B21EE8(EnTp* this);
void func_80B21F18(EnTp* this, GlobalContext* globalCtx);
void func_80B221E8(EnTp* this, GlobalContext* globalCtx);

extern Gfx D_6000000[];
extern Gfx D_60008D0[];
extern UNK_TYPE D_6000C68;

const ActorInit En_Tp_InitVars = {
    ACTOR_EN_TP,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_TP,
    sizeof(EnTp),
    (ActorFunc)EnTp_Init,
    (ActorFunc)EnTp_Destroy,
    (ActorFunc)EnTp_Update,
    (ActorFunc)EnTp_Draw,
};

static ColliderJntSphElementInit D_80B22A90[1] = {
    {
        {
            ELEMTYPE_UNK0,
            { 0xFFCFFFFF, 0x03, 0x08 },
            { 0xFFCFFFFF, 0x01, 0x00 },
            TOUCH_ON | TOUCH_SFX_NORMAL,
            BUMP_ON,
            OCELEM_NONE,
        },
        { 0, { { 0, 0, 0 }, 4 }, 100 },
    },
};

static ColliderJntSphInit D_80B22AB4 = {
    {
        COLTYPE_HIT1,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    1,
    D_80B22A90,
};

static DamageTable D_80B22AC4 = {
    {
        0x10, 0xF2, 0, 0, 0xF1, 0, 0xE2, 0,    0xE1, 0xE2, 0xE4, 0,    0, 0, 0,    0,
        0,    0,    0, 0, 0,    0, 0xE1, 0xE4, 0xE2, 0xE2, 0xE8, 0xE4, 0, 0, 0xE4, 0,
    },
};
static InitChainEntry D_80B22AE4[] = {
    ICHAIN_F32(targetArrowOffset, 10, ICHAIN_STOP),
};

void func_80B20DE0(EnTp* this, void (*arg1)(EnTp*, GlobalContext*)) {
    this->unk154 = arg1;
}

void EnTp_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnTp* temp_v0_2;
    EnTp* var_s5;
    s32 var_s1;
    EnTp* this = (EnTp*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s64 new_var;

    Actor_ProcessInitChain(&this->actor, D_80B22AE4);
    this->actor.targetMode = 3;
    this->actor.colChkInfo.damageTable = &D_80B22AC4;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 0.14f);
    this->unk150 = 0;
    this->actor.colChkInfo.health = 1;
    var_s5 = this;
    this->unk15E = 0xFF;
    Collider_InitJntSph(globalCtx, &this->unk174);
    Collider_SetJntSph(globalCtx, &this->unk174, &this->actor, &D_80B22AB4, &this->unk194);
    if (this->actor.params < 0) {
        this->actor.naviEnemyId = 6;
        this->unk15A = 0;
        this->unk174.base.acFlags |= 4;
        this->unk174.elements->dim.worldSphere.radius = 8;
        this->unk174.elements->dim.modelSphere.radius = this->unk174.elements->dim.worldSphere.radius;
        func_80B21B90(this);
        this->actor.focus.pos = this->actor.world.pos;
        this->actor.flags |= 0x15;
        Actor_SetScale(&this->actor, 1.5f);
        for (var_s1 = 0; var_s1 < 7; var_s1++) {
            if (((0 * var_s1) != 0) && ((0 * var_s1) != 0)) {}
            temp_v0_2 = (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, this->actor.world.pos.x,
                                           this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0 * var_s1);
            if (temp_v0_2 != NULL) {
                var_s5->actor.child = &temp_v0_2->actor;
                temp_v0_2->actor.parent = &var_s5->actor;
                temp_v0_2->unk162 = (s16)(var_s1 + 1);
                temp_v0_2->unk1D4 = this;
                Actor_SetScale(&temp_v0_2->actor, 0.3f);
                if (var_s1 == 2) {
                    temp_v0_2->actor.flags |= 0x15;
                    temp_v0_2->unk150 = 1;
                }
                temp_v0_2->unk15C = var_s1 * -5;
                temp_v0_2->unk15A = var_s1 * -5;
                var_s5 = (EnTp*)temp_v0_2;
                temp_v0_2->unk16C = (f32)(6.0f - ((f32)var_s1 * 0.75f));
            }
        }
    } else if (this->actor.params == 0) {
        func_80B21084(this);
    } else {
        func_80B217FC(this);
    }
}

void EnTp_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnTp* this = (EnTp*)thisx;

    Collider_DestroyJntSph(globalCtx, &this->unk174);
}

void func_80B21084(EnTp* this) {
    this->unk14C = 2;
    func_80B20DE0(this, func_80B210B0);
}

void func_80B210B0(EnTp* this, GlobalContext* globalCtx) {
    s16 sp36;
    s16 temp_a0_sp34;

    if (this->actor.params == 0xB) {
        this->unk14C = 1;
        if (this->actor.parent == NULL) {
            func_80B21454(this);
        }
    } else {
        if (this->unk150 != 0) {
            this->actor.flags |= 1;
        }
        if (this->unk1D4->unk150 != 0) {
            this->actor.speedXZ = this->unk160 = this->actor.velocity.y = this->unk168 = 0.0f;
            if (this->actor.world.pos.y < this->unk1D4->actor.home.pos.y) {
                this->actor.flags &= ~1;
            }
            this->actor.world.pos = this->actor.parent->prevPos;
        } else {
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.parent->world.pos.y - 4.0f, 1.0f, 1.0f, 0.0f);
            sp36 = this->unk1D4->actor.shape.rot.y + 0x4000;
            temp_a0_sp34 = (this->unk1D4->unk15C + this->unk15A) * 0x7D0;
            this->actor.world.pos.x =
                (Math_SinS(temp_a0_sp34) * (Math_SinS(sp36) * this->unk16C)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(temp_a0_sp34) * (Math_CosS(sp36) * this->unk16C)) + this->actor.home.pos.z;
        }
    }
}

void func_80B2128C(EnTp* this) {
    this->unk14C = 7;
    this->unk15A = 0xC8;
    func_80B20DE0(this, func_80B212C0);
}

void func_80B212C0(EnTp* this, GlobalContext* globalCtx) {
    Player* player;

    player = PLAYER;
    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 30.0f, 1.0f, 0.5f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                           &D_801333E8);
    if (this->unk174.base.atFlags & AT_HIT) {
        this->unk174.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->unk174.base.at) {
            this->unk15A = 1;
        }
    }
    if (this->unk160 < 0xFF) {
        this->unk160 += 0xF;
    }
    if (Math_CosF(this->unk168) == 0.0f) {
        this->unk170 = 2.0f * Rand_ZeroOne();
    }
    this->actor.world.pos.y += Math_CosF(this->unk168) * (2.0f + this->unk170);
    this->unk168 += 0.2f;
    Math_SmoothStepToF(&this->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    this->unk15A -= 1;
    if (this->unk15A != 0) {
        Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x2EE, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
    } else {
        func_80B21EE8(this);
    }
}

void func_80B21454(EnTp* this) {
    Actor* var_v0;

    this->unk15A = 2;
    if (this->actor.params < 0) {
        var_v0 = this->actor.child;
        while (var_v0 != NULL) {
            var_v0->params = 0xB;
            var_v0->colChkInfo.health = 0;
            var_v0 = var_v0->child;
        }
        this->unk15A = 0xD;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_TAIL_DEAD);
    }
    this->unk14C = 1;
    func_80B20DE0(this, func_80B214CC);
}

void func_80B214CC(EnTp* this, GlobalContext* globalCtx) {
    s16 var_s1;
    EnTp* temp_v0;
    s32 pad;
    Vec3f sp78 = { 0.0f, 0.5f, 0.0f };
    Vec3f sp6C = { 0.0f, 0.0f, 0.0f };

    this->unk15A -= 1;
    if (this->unk15A <= 0) {
        if (this->actor.params == 0xC) {
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, (Vec3f*)&sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                 1);
            sp6C.x = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.x;
            sp6C.z = ((Rand_ZeroOne() - 0.5f) * 15.0f) + this->actor.world.pos.z;
            sp6C.y = ((Rand_ZeroOne() - 0.5f) * 5.0f) + this->actor.world.pos.y;
            EffectSsDeadDb_Spawn(globalCtx, (Vec3f*)&sp6C, &sp78, &sp78, 100, 0, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                 1);
            Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.world.pos, 0x50);
        } else {
            for (var_s1 = 0; var_s1 < 1; var_s1++) {
                temp_v0 = (EnTp*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_TP, this->actor.world.pos.x,
                                             this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 0xA);
                if (temp_v0 != NULL) {
                    Actor_SetScale(&temp_v0->actor, this->actor.scale.z * 0.5f);
                    temp_v0->unk160 = (s16)this->unk160;
                }
            }
        }
        if (this->actor.child != NULL) {
            this->actor.child->parent = NULL;
            this->actor.child->params = 0xB;
            this->actor.child->colChkInfo.health = 0;
        }
        this->unk150 = 2;
        Actor_Kill(&this->actor);
    }
}

void func_80B217FC(EnTp* this) {
    this->unk14C = 0;
    this->actor.world.pos.x += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.world.pos.y += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.world.pos.z += (Rand_ZeroOne() - 0.5f) * 5.0f;
    this->actor.velocity.x = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.y = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.velocity.z = (Rand_ZeroOne() - 0.5f) * 1.5f;
    this->actor.flags &= ~1;
    func_80B20DE0(this, func_80B21900);
}

void func_80B21900(EnTp* this, GlobalContext* globalCtx) {
    func_8002D7EC(&this->actor);
    this->unk15E -= 0x14;
    if (this->unk15E < 0x14) {
        this->unk15E = 0;
        Actor_Kill(&this->actor);
    }
}

void func_80B2194C(EnTp* this) {
    this->unk15A = (Rand_ZeroOne() * 15.0f) + 40.0f;
    this->unk14C = 8;
    func_80B20DE0(this, func_80B219A8);
}

void func_80B219A8(EnTp* this, GlobalContext* globalCtx) {
    s32 pad;
    Player* player;

    player = PLAYER;
    Math_SmoothStepToF(&this->actor.speedXZ, 2.5f, 0.1f, 0.2f, 0.0f);
    Math_SmoothStepToF(&this->actor.world.pos.y, player->actor.world.pos.y + 85.0f + this->unk16C, 1.0f,
                       this->actor.speedXZ * 0.25f, 0.0f);
    Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                           &D_801333E8);
    if (this->unk174.base.atFlags & AT_HIT) {
        this->unk174.base.atFlags &= ~AT_HIT;
        if (&player->actor == this->unk174.base.at) {
            this->unk15C = 1;
        }
    }
    if (this->unk160 != 0) {
        this->unk160 -= 0xF;
    }
    if (Math_CosF(this->unk168) == 0.0f) {
        this->unk170 = Rand_ZeroOne() * 4.0f;
    }
    this->actor.world.pos.y += Math_CosF(this->unk168) * ((this->actor.speedXZ * 0.25f) + this->unk170);
    this->actor.world.rot.y += this->unk164;
    this->unk168 += 0.2f;
    if (this->unk15A != 0) {
        this->unk15A -= 1;
    }
    Math_SmoothStepToS(&this->actor.world.rot.y, Math_Vec3f_Yaw(&this->actor.world.pos, &this->actor.home.pos), 1,
                       0x2EE, 0);
    if (this->unk15A == 0) {
        func_80B2128C(this);
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void func_80B21B90(EnTp* this) {
    this->unk14C = 4;
    this->unk150 = 0;
    this->actor.shape.rot.x = -0x4000;
    this->unk15A = 0x3C;
    this->unk15C = 0;
    this->actor.speedXZ = 0.0f;
    func_80B20DE0(this, func_80B21BDC);
}

void func_80B21BDC(EnTp* this, GlobalContext* globalCtx) {
    Player* player;
    s16 sp32;

    player = PLAYER;
    this->unk15C -= 1;
    if (this->actor.xzDistToPlayer < 200.0f) {
        if (this->unk174.base.atFlags & AT_HIT) {
            this->unk174.base.atFlags &= ~AT_HIT;
            if (&player->actor == this->unk174.base.at) {
                this->unk15A = 0;
            }
        }
        if (this->unk15A != 0) {
            this->unk15A -= 1;
            Math_SmoothStepToS(&this->actor.shape.rot.x, 0, 1, 0x1F4, 0);
            Math_SmoothStepToS(&this->actor.world.rot.y, this->actor.yawTowardsPlayer, 1, 0x5DC, 0);
            sp32 = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos) + 0x4000;
            Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 30.0f, 0.3f, 1.0f, 0.3f);
            this->actor.world.pos.x =
                (Math_SinS(this->unk15C * 0x7D0) * (Math_SinS(sp32) * this->unk16C)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(this->unk15C * 0x7D0) * (Math_CosS(sp32) * this->unk16C)) + this->actor.home.pos.z;
        } else {
            this->actor.shape.rot.x = 0;
            this->unk150 = 1;
            func_80B2194C(this);
        }
    } else {
        Math_SmoothStepToS(&this->actor.shape.rot.x, -0x4000, 1, 0x1F4, 0);
        if (Math_SmoothStepToF(&this->actor.world.pos.y, this->actor.home.pos.y, 0.3f, 1.5f, 0.3f) == 0.0f) {
            this->unk15A = 0x3C;
        } else {
            sp32 = Math_Vec3f_Yaw(&this->actor.home.pos, &player->actor.world.pos);
            this->actor.world.pos.x =
                (Math_SinS(this->unk15C * 0x7D0) * (Math_SinS(sp32) * 6.0f)) + this->actor.home.pos.x;
            this->actor.world.pos.z =
                (Math_SinS(this->unk15C * 0x7D0) * (Math_CosS(sp32) * 6.0f)) + this->actor.home.pos.z;
        }
    }
    this->actor.shape.rot.y = this->actor.world.rot.y;
    if (this->actor.world.pos.y != this->actor.home.pos.y) {
        Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                               &D_801333E8);
    }
}

void func_80B21EE8(EnTp* this) {
    this->unk14C = 9;
    this->unk15A = 0;
    func_80B20DE0(this, func_80B21F18);
}

void func_80B21F18(EnTp* this, GlobalContext* globalCtx) {
    static Vec3f D_80B22B00 = { 0.0f, -0.5f, 0.0f };
    static Color_RGBA8 D_80B22B0C = { 0xFF, 0xFF, 0xFF, 0xFF };
    static Color_RGBA8 D_80B22B10 = { 0x96, 0x96, 0x96, 0 };
    Vec3f sp54;
    Vec3f sp48;
    s32 sp44;
    EnTp* var_v0;
    s16 new_var;

    sp44 = 0;
    this->unk15C -= 1;
    if ((this->unk15A != 0) || ((this->actor.home.pos.y - this->actor.world.pos.y) > 60.0f)) {
        if (1) {
            this->unk15A -= 1;
        }
        if ((new_var = this->unk15A) == 0) {
            func_80B21B90(this);
            var_v0 = (EnTp*)this->actor.child;
            while (var_v0 != NULL) {
                var_v0->unk15C = var_v0->unk15A;
                var_v0 = (EnTp*)var_v0->actor.child;
            }
        } else {
            if (this->actor.shape.rot.x != -0x4000) {
                this->unk15A = 0x50;
                this->actor.velocity.y = 0.0f;
                this->actor.speedXZ = 0.0f;
                this->actor.world.pos = this->actor.home.pos;
                this->actor.shape.rot.x = -0x4000;
                var_v0 = (EnTp*)this->actor.child;
                while (var_v0 != NULL) {
                    var_v0->actor.velocity.y = 0.0f;
                    var_v0->actor.speedXZ = 0.0f;
                    var_v0->actor.world.pos = this->actor.home.pos;
                    var_v0->actor.world.pos.y = this->actor.home.pos.y - 80.0f;
                    var_v0 = (EnTp*)var_v0->actor.child;
                }
            }
            this->actor.world.pos.y = this->actor.home.pos.y - (f32)this->unk15A;
        }
    } else {
        if (this->actor.shape.rot.x != 0x4000) {
            this->actor.shape.rot.x -= 0x400;
        }
        if (this->unk160 != 0) {
            this->unk160 -= 0xF;
        }
        this->actor.speedXZ = 2.0f * Math_CosS(this->actor.shape.rot.x);
        this->actor.velocity.y = Math_SinS(this->actor.shape.rot.x) * -2.0f;
        if ((this->actor.world.pos.y - this->actor.floorHeight) < 20.0f) {
            sp44 = 1;
        }
        if (this->actor.world.pos.y != this->actor.home.pos.y) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_FLY - SFX_FLAG, &this->actor.projectedPos, 4U, &D_801333E0,
                                   &D_801333E0, &D_801333E8);
        }
        if ((sp44 != 0) && (globalCtx->gameplayFrames & 1)) {
            sp48 = this->actor.world.pos;
            sp48.y = this->actor.floorHeight;
            sp54.x = Rand_CenteredFloat(5.0f);
            sp54.y = (Rand_ZeroOne() * 3.5f) + 1.5f;
            sp54.z = Rand_CenteredFloat(5.0f);
            EffectSsDtBubble_SpawnCustomColor(globalCtx, &sp48, &sp54, &D_80B22B00, &D_80B22B0C, &D_80B22B10,
                                              Rand_S16Offset(100, 50), 20, 0);
        }
    }
}

void func_80B221E8(EnTp* this, GlobalContext* globalCtx) {
    EnTp* var_s0;
    EnTp* var_s0_2;
    s32 var_s2;
    s32 var_s4;
    EnTp* new_var;

    if ((this->unk174.base.acFlags & AC_HIT) && (this->unk14C >= 2)) {
        var_s2 = 0;
        var_s4 = 0;
        if (this->actor.params < 0) {
            var_s2 = 1;
        }
        this->unk174.base.acFlags &= ~2;
        func_8003573C(&this->actor, &this->unk174, 1);
        this->unk158 = this->actor.colChkInfo.damageEffect;
        if (this->actor.colChkInfo.damageEffect != 0) {
            if (this->actor.colChkInfo.damageEffect == 1) {
                var_s4 = 1;
            }
            if (var_s2 == 0) {
                Actor_ApplyDamage(&this->actor);
            }
            if (this->actor.colChkInfo.health == 0) {
                this->actor.flags &= ~1;
                new_var = this->unk1D4;
                if (new_var->actor.params < 0) {
                    func_80B21454(new_var);
                    new_var->unk158 = this->actor.colChkInfo.damageEffect;
                    new_var->actor.params = 0xC;
                }
            } else {
                if (var_s4 != 0) {
                    this->actor.freezeTimer = 80;
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                    if (var_s2 != 0) {
                        Actor_SetColorFilter(&this->actor, 0, 0xFF, 0, 80);
                    } else {
                        Actor_SetColorFilter(&this->actor, 0, 0xFF, 0x2000, 80);
                    }
                }
                var_s0 = (EnTp*)this->actor.parent;
                while (var_s0 != NULL) {
                    var_s0->unk174.base.acFlags &= ~AC_HIT;
                    if (var_s4 != 0) {
                        var_s0->actor.freezeTimer = 80;
                        Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
                        if (var_s2 != 0) {
                            Actor_SetColorFilter(&var_s0->actor, 0, 0xFF, 0, 80);
                        } else {
                            Actor_SetColorFilter(&var_s0->actor, 0, 0xFF, 0x2000, 80);
                        }
                    }
                    var_s0 = (EnTp*)var_s0->actor.parent;
                }
                var_s0_2 = (EnTp*)this->actor.child;
                while (var_s0_2 != NULL) {
                    var_s0_2->unk174.base.acFlags &= ~AC_HIT;
                    if (var_s4 != 0) {
                        var_s0_2->actor.freezeTimer = 80;
                        if (var_s2 != 0) {
                            Actor_SetColorFilter(&var_s0_2->actor, 0, 0xFF, 0, 80);
                        } else {
                            Actor_SetColorFilter(&var_s0_2->actor, 0, 0xFF, 0x2000, 80);
                        }
                    }
                    var_s0_2 = (EnTp*)var_s0_2->actor.child;
                }
            }
        }
    }
}

void EnTp_Update(Actor* thisx, GlobalContext* globalCtx) {
    Player* player;
    s32 pad;
    Vec3f sp5C = { 0.0f, 0.0f, 0.0f };
    Vec3f sp50 = { 0.0f, -0.6f, 0.0f };
    Vec3f sp44;
    Color_RGBA8 sp40 = { 0, 0, 255, 255 };
    Color_RGBA8 sp3C = { 0, 0, 0, 0 };
    EnTp* this = (EnTp*)thisx;
    s16 v;

    player = PLAYER;

    if (player->stateFlags1 & 0x04000000) {
        this->unk158 = 0;
    }
    if (this->actor.colChkInfo.health != 0) {
        func_80B221E8(this, globalCtx);
    }
    this->unk154(this, globalCtx);
    if (this->actor.params < 0) {
        Actor_MoveForward(&this->actor);
        if (this->unk14C != 9) {
            Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 15.0f, 10.0f, 5);
        }
        if ((this->actor.speedXZ != 0.0f) && (this->actor.bgCheckFlags & 8)) {
            v = this->actor.wallYaw - this->actor.world.rot.y;
            if (ABS(v) > 0x4000) {
                if (v >= 0) {
                    this->actor.world.rot.y -= 0x1F4;
                } else {
                    this->actor.world.rot.y += 0x1F4;
                }
                this->actor.shape.rot.y = this->actor.world.rot.y;
            }
        }
        this->actor.shape.rot.z += 0x800;
        if (this->actor.shape.rot.z == 0) {
            Audio_PlaySoundGeneral(NA_SE_EN_TAIL_CRY, &this->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
        if (this->unk14C >= 2) {
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk174.base);
        }
    }
    if (this->actor.params != 0xB) {
        this->unk162 -= 1;
        this->unk162 &= 7;
    }
    this->actor.focus.pos = this->actor.world.pos;
    if (this->unk158 == 0xE) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk174.base);
    }
    if ((this->unk162 & 7) == 0) {
        sp40.r = this->unk160;
        sp50.x = -this->actor.velocity.x * 0.25f;
        sp50.y = -this->actor.velocity.y * 0.25f;
        sp50.z = -this->actor.velocity.z * 0.25f;
        sp44.x = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.x;
        sp44.y = ((Rand_ZeroOne() - 0.5f) * 20.0f) + this->actor.world.pos.y;
        sp44.z = ((Rand_ZeroOne() - 0.5f) * 25.0f) + this->actor.world.pos.z;
        EffectSsKiraKira_SpawnSmall(globalCtx, &sp44, &sp5C, &sp50, &sp40, &sp3C);
    }
    if ((this->unk14C >= 2) && (this->actor.colChkInfo.health != 0)) {
        CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk174.base);
    }
}

void EnTp_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s16 temp_v0;
    EnTp* this = (EnTp*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 0x5AB);
    if (this->unk150 != 2) {
        temp_v0 = this->actor.params;
        if ((temp_v0 < 0) || (temp_v0 == 0xC)) {
            func_80093D18(globalCtx->state.gfxCtx);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 0x5B3),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_OPA_DISP++, D_60008D0);
            Matrix_Translate(0.0f, 0.0f, 8.0f, 1U);
        } else {
            func_80093D84(globalCtx->state.gfxCtx);
            func_800D1FD4(&globalCtx->mf_11DA0);
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, this->unk160, 0, 255, this->unk15E);
            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetCombineLERP(POLY_XLU_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT,
                              TEXEL0, ENVIRONMENT);
            gDPPipeSync(POLY_XLU_DISP++);
            gSPSegment(POLY_XLU_DISP++, 8, SEGMENTED_TO_VIRTUAL(&D_6000C68));
            gDPPipeSync(POLY_XLU_DISP++);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_tp.c", 0x5C8),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, D_6000000);
        }
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tp.c", 0x5D7);
    if ((this->actor.params <= 0) || (this->actor.params == 0xB)) {
        Collider_UpdateSpheres(0, &this->unk174);
    }
}
