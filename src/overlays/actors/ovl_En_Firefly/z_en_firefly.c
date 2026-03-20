#include "z_en_firefly.h"

#define FLAGS 0x00005005

#define THIS ((EnFirefly*)thisx)

void EnFirefly_Init(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_Update(Actor* thisx, GlobalContext* globalCtx);
void EnFirefly_DrawOpa(Actor* thisx, GlobalContext* globalCtx);

void func_80A13A08_Action(EnFirefly*, GlobalContext*);
void func_80A13C98_Action(EnFirefly*, GlobalContext*);
void func_80A13D68_Action(EnFirefly*, GlobalContext*);
void func_80A13DE4_Action(EnFirefly*, GlobalContext*);
void func_80A13FF4_Action(EnFirefly*, GlobalContext*);
void func_80A14088_Action(EnFirefly*, GlobalContext*);
void func_80A141F0_Action(EnFirefly*, GlobalContext*);
void func_80A14294_Action(EnFirefly*, GlobalContext*);
void func_80A142F4_Action_Perched_(EnFirefly*, GlobalContext*);
void func_80A143B4_Action_SwoopOnPlayer(EnFirefly*, GlobalContext*);
void EnFirefly_DrawXlu(Actor*, GlobalContext*);

extern AnimationHeader D_600017C;
extern Gfx D_6001678[];
extern SkeletonHeader D_60018B8;

const ActorInit En_Firefly_InitVars = {
    ACTOR_EN_FIREFLY,
    ACTORTYPE_ENEMY,
    FLAGS,
    OBJECT_FIREFLY,
    sizeof(EnFirefly),
    (ActorFunc)EnFirefly_Init,
    (ActorFunc)EnFirefly_Destroy,
    (ActorFunc)EnFirefly_Update,
    (ActorFunc)EnFirefly_DrawOpa,
};
static ColliderSpheresElementSrc D_80A14F30 = {
    { 0, { 0xFFCFFFFF, 1, 8 }, { 0xFFCFFFFF, 0, 0 }, 9, 1, 1 },
    { 1, { { 0, 0x3E8, 0 }, 0xF }, 0x64 },
};
static ColliderSpheresSrc D_80A14F54 = { { 3, 0x11, 9, 0x39, 0x10, 0 }, 1, &D_80A14F30 };
static CollideDataInit D_80A14F64 = { 1, 0xA, 0xA, 0x1E };
static ActorDamageChart D_80A14F6C = {
    {
        { 0x10 }, { 2 },    { 1 }, { 2 }, { 1 }, { 2 }, { 2 },    { 2 },    { 1 }, { 2 }, { 4 },
        { 0xF2 }, { 0x34 }, { 2 }, { 2 }, { 2 }, { 2 }, { 0x20 }, { 0x34 }, { 0 }, { 0 }, { 0 },
        { 1 },    { 4 },    { 2 }, { 2 }, { 8 }, { 4 }, { 0 },    { 0 },    { 4 }, { 0 },
    },
};
static InitChainEntry D_80A14F8C[5] = {
    ICHAIN_VEC3F_DIV1000(scale, 5, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -4, ICHAIN_CONTINUE),   ICHAIN_U8(unk_1F, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_4C, 4000, ICHAIN_STOP),
};
static Color_RGBA8 D_80A14FA0 = { 0xFF, 0xFF, 0x64, 0xFF };
static Color_RGBA8 D_80A14FA4 = { 0xFF, 0x32, 0, 0 };
static Color_RGBA8 D_80A14FA8 = { 0x64, 0xC8, 0xFF, 0xFF };
static Color_RGBA8 D_80A14FAC = { 0, 0, 0xFF, 0 };
static Vec3f D_80A14FB0 = { 0.0f, 0.5f, 0.0f };
static Vec3f D_80A14FBC = { 0.0f, 0.5f, 0.0f };
static Vec3f D_80A14FC8 = { 0.0f, 0.0f, 0.0f };

void func_80A13070_Unignite_(EnFirefly* this) {
    this->actor.params += 2;
    this->collider.elements->base.atDmgInfo.hitSpecialEffect = HIT_SPECIAL_EFFECT_NONE;
    this->unk1B8 = 0;
    this->unk1B9 = 0;
    this->actor.naviEnemyId = 0x12;
}

void func_80A13098_Ignite_(EnFirefly* this) {
    if (this->actor.params == 4) {
        this->actor.params = 0;
    } else {
        this->actor.params -= 2;
    }
    this->collider.elements->base.atDmgInfo.hitSpecialEffect = HIT_SPECIAL_EFFECT_FIRE;
    this->unk1B8 = 1;
    this->unk1B9 = 1;
    this->actor.naviEnemyId = 0x11;
}

void EnFirefly_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* this = (EnFirefly*)thisx;

    Actor_ProcessInitChain(&this->actor, D_80A14F8C);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFunc_Circle, 25.0f);
    SkelAnime_Init(globalCtx, &this->skelAnime, &D_60018B8, &D_600017C, this->unk1BE, this->unk266, 0x1C);
    Collider_InitSpheres(globalCtx, &this->collider);
    Collider_LoadSpheres(globalCtx, &this->collider, &this->actor, &D_80A14F54, this->colliderElements);
    func_80061ED4(&this->actor.collideData, &D_80A14F6C, &D_80A14F64);
    if (this->actor.params & 0x8000) {
        this->actor.flags |= 0x80;
        thisx->draw = EnFirefly_DrawXlu;
        this->actor.params &= 0x7FFF;
    }
    if (this->actor.params < 2) {
        this->unk1B9 = 1;
    } else {
        this->unk1B9 = 0;
    }
    if (this->unk1B9 != 0) {
        this->actionFunc = func_80A13A08_Action;
        this->unk1BA_timer = Math_Rand_S16Offset(20, 60);
        this->actor.shape.rot.x = 0x1554;
        this->unk1B8 = 1;
        this->actor.naviEnemyId = 0x11;
        this->unk310_homeY = this->actor.initPosRot.pos.y;
    } else {
        if (this->actor.params == 3) {
            this->actionFunc = func_80A142F4_Action_Perched_;
        } else {
            this->actionFunc = func_80A13A08_Action;
        }
        if (this->actor.params == 4) {
            this->collider.elements->base.atDmgInfo.hitSpecialEffect = HIT_SPECIAL_EFFECT_ICE;
            this->actor.naviEnemyId = 0x56;
        } else {
            this->collider.elements->base.atDmgInfo.hitSpecialEffect = HIT_SPECIAL_EFFECT_NONE;
            this->actor.naviEnemyId = 0x12;
        }
        this->unk310_homeY = this->actor.initPosRot.pos.y + 100.0f;
        if (this->actor.params == 4) {
            this->unk1B8 = 2;
        } else {
            this->unk1B8 = 0;
        }
    }
    this->collider.elements->shape.world.radius = D_80A14F54.elements->shape.unk2.radius;
}

void EnFirefly_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* this = (EnFirefly*)thisx;

    Collider_DestroySpheres(globalCtx, &this->collider);
}

void func_80A132F4_Setup(EnFirefly* this) {
    s32 var_v0;

    this->unk1BA_timer = Math_Rand_S16Offset(0x46, 0x64);
    this->actor.speedXZ = (Math_Rand_ZeroOne() * 1.5f) + 1.5f;
    Math_ApproxUpdateScaledS(&this->actor.shape.rot.y, func_8002DAC0(&this->actor, &this->actor.initPosRot.pos), 0x300);
    if (this->unk310_homeY < this->actor.posRot.pos.y) {
        var_v0 = 0xC00;
    } else {
        var_v0 = -0xC00;
    }
    this->unk1BC_targetRotX = var_v0 + 0x1554;
    this->skelAnime.animPlaybackSpeed = 1.0f;
    this->actionFunc = func_80A13A08_Action;
}

void func_80A133A0_Setup(EnFirefly* this) {
    this->unk1BA_timer = 0x28;
    this->actor.velocity.y = 0.0f;
    SkelAnime_ChangeAnim(&this->skelAnime, &D_600017C, 0.5f, 0.0f, 0.0f, 1U, -3.0f);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_FFLY_DEAD);
    this->actor.flags |= 0x10;
    func_8003426C(&this->actor, 0x4000, 0xFF, 0, 0x28);
    this->actionFunc = func_80A13C98_Action;
}

void func_80A13444_Setup(EnFirefly* this) {
    this->unk1BA_timer = 0xF;
    this->actionFunc = func_80A13D68_Action;
    this->actor.speedXZ = 0.0f;
}

void func_80A13464_Setup(EnFirefly* this) {
    this->actor.posRot.rot.x = 0x7000;
    this->unk1BA_timer = 18;
    this->actionFunc = func_80A13FF4_Action;
    this->skelAnime.animPlaybackSpeed = 1.0f;
    this->actor.speedXZ = 2.5f;
}

void func_80A1349C_Setup(EnFirefly* this) {
    s32 var_v0;

    this->unk1BA_timer = Math_Rand_S16Offset(70, 100);
    this->skelAnime.animPlaybackSpeed = 1.0f;
    if (this->actor.yDistFromLink > 0.0f) {
        var_v0 = -0xC00;
    } else {
        var_v0 = 0xC00;
    }
    this->unk1BC_targetRotX = var_v0 + 0x1554;
    this->actionFunc = func_80A13DE4_Action;
}

void func_80A1350C_Setup(EnFirefly* this) {
    this->unk1BA_timer = 150;
    this->unk1BC_targetRotX = 0x954;
    this->actionFunc = func_80A14088_Action;
    this->skelAnime.animPlaybackSpeed = 1.0f;
}

void func_80A13538_Setup(EnFirefly* this) {
    this->unk1BA_timer = 80;
    func_8003426C(&this->actor, 0, 0xFF, 0, 0x50);
    this->unk1B8 = 0;
    this->actor.velocity.y = 0.0f;
    this->skelAnime.animPlaybackSpeed = 3.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_GOMA_JR_FREEZE);
    this->actionFunc = func_80A141F0_Action;
}

void func_80A135A8_Setup(EnFirefly* this, GlobalContext* globalCtx) {
    s32 var_s0;
    Vec3f sp78;

    this->actor.flags |= 0x10;
    this->unk1B8 = 0;
    this->actor.speedXZ = 0.0f;
    func_8003426C(&this->actor, 0, 0xFF, 0, 0xFF);
    Audio_PlayActorSound2(&this->actor, NA_SE_EN_FFLY_DEAD);
    for (var_s0 = 0; var_s0 < 8; var_s0++) {
        sp78.x = this->actor.posRot.pos.x + ((var_s0 & 1) ? 7.0f : -7.0f);
        sp78.y = this->actor.posRot.pos.y + ((var_s0 & 2) ? 7.0f : -7.0f);
        sp78.z = this->actor.posRot.pos.z + ((var_s0 & 4) ? 7.0f : -7.0f);
        EffectSsEnIce_SpawnFlyingVec3f(globalCtx, &this->actor, &sp78, 150, 150, 150, 250, 235, 245, 255,
                                       (Math_Rand_ZeroOne() * 0.15f) + 0.85f);
    }
    this->actionFunc = func_80A14294_Action;
}

void func_80A13744_Setup_Perched_(EnFirefly* this) {
    this->unk1BA_timer = 1;
    this->actionFunc = func_80A142F4_Action_Perched_;
    this->actor.speedXZ = 0.0f;
}

void func_80A13764_Setup_SwoopOnPlayer(EnFirefly* this) {
    this->actor.shape.rot.x = 0x1554;
    this->skelAnime.animPlaybackSpeed = 3.0f;
    this->actor.shape.rot.y = this->actor.yawTowardsLink;
    this->unk1BA_timer = 50;
    this->actor.speedXZ = 3.0f;
    this->actionFunc = func_80A143B4_Action_SwoopOnPlayer;
}

s32 func_80A1379C_HoneInOnHomeIfPlayerFar_(EnFirefly* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    f32 temp_fv0;
    f32 temp_fv1;

    if (this->actor.params != 3) {
        return false;
    }
    if (func_8002DBB0(&player->actor, &this->actor.initPosRot.pos) > 300.0f) {
        temp_fv0 = func_8002DB6C(&this->actor, &this->actor.initPosRot.pos);
        if (temp_fv0 < 5.0f) {
            func_80A13744_Setup_Perched_(this);
        } else {
            temp_fv1 = temp_fv0 * 0.05f;
            if (temp_fv1 < 1.0f) {
                this->actor.speedXZ *= temp_fv1;
            }
            Math_ApproxUpdateScaledS(&this->actor.shape.rot.y, func_8002DAC0(&this->actor, &this->actor.initPosRot.pos),
                                     0x300);
            Math_ApproxUpdateScaledS(&this->actor.shape.rot.x,
                                     func_8002DB28(&this->actor, &this->actor.initPosRot.pos) + 0x1554, 0x100);
        }
        return true;
    } else {
        return false;
    }
}

// ObjSyokudai
typedef struct struct_actor5E {
    char unk0[0x1E4];
    s16 unk1E4;
} struct_actor5E;

s32 func_80A138B8_HoneInOnLitTorch(EnFirefly* this, GlobalContext* globalCtx) {
    Actor* iter;
    f32 closestLitTorchDist;
    f32 dist;
    Actor* closestLitTorch;
    Vec3f torchFlamePos;

    iter = globalCtx->actorCtx.actorList[ACTORTYPE_PROP].first;
    closestLitTorch = NULL;
    closestLitTorchDist = 35000.0f;
    while (iter != NULL) {
        if ((iter->id == ACTOR_OBJ_SYOKUDAI) && (((struct_actor5E*)iter)->unk1E4 != 0)) {
            dist = func_8002DB48(&this->actor, iter);
            if (dist < closestLitTorchDist) {
                closestLitTorchDist = dist;
                closestLitTorch = iter;
            }
        }
        iter = iter->next;
    }
    if (closestLitTorch != NULL) {
        torchFlamePos.x = closestLitTorch->posRot.pos.x;
        torchFlamePos.y = closestLitTorch->posRot.pos.y + 52.0f + 15.0f;
        torchFlamePos.z = closestLitTorch->posRot.pos.z;
        if (func_8002DB6C(&this->actor, &torchFlamePos) < 15.0f) {
            func_80A13098_Ignite_(this);
            return true;
        } else {
            Math_ApproxUpdateScaledS(&this->actor.shape.rot.y, func_8002DA78(&this->actor, closestLitTorch), 0x300);
            Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, func_8002DB28(&this->actor, &torchFlamePos) + 0x1554,
                                     0x100);
            return true;
        }
    } else {
        return false;
    }
}

void func_80A13A08_Action(EnFirefly* this, GlobalContext* globalCtx) {
    s32 sp3C;
    f32 temp_fv0;

    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    sp3C = func_800A56C8(&this->skelAnime, 0.0f);
    this->actor.speedXZ = (Math_Rand_ZeroOne() * 1.5f) + 1.5f;
    if ((this->unk1B9 != 0) || (this->actor.params == 4) ||
        (!func_80A1379C_HoneInOnHomeIfPlayerFar_(this, globalCtx) &&
         !func_80A138B8_HoneInOnLitTorch(this, globalCtx))) {
        if (sp3C != 0) {
            temp_fv0 = Math_Rand_ZeroOne();
            if (temp_fv0 < 0.5f) {
                Math_ApproxUpdateScaledS(&this->actor.shape.rot.y,
                                         func_8002DAC0(&this->actor, &this->actor.initPosRot.pos), 0x300);
            } else if (temp_fv0 < 0.8f) {
                f32 new_var = Math_Rand_CenteredFloat(1536.0f);

                this->actor.shape.rot.y = (s16)(s32)(this->actor.shape.rot.y + new_var);
            }
            if (this->actor.posRot.pos.y < (this->actor.groundY + 20.0f)) {
                this->unk1BC_targetRotX = 0x954;
            } else if (this->unk310_homeY < this->actor.posRot.pos.y) {
                this->unk1BC_targetRotX = 0x2154;
            } else if (Math_Rand_ZeroOne() > 0.35f) {
                this->unk1BC_targetRotX = 0x954;
            } else {
                this->unk1BC_targetRotX = 0x2154;
            }
        } else {
            if (this->actor.bgCheckFlags & 1) {
                this->unk1BC_targetRotX = 0x954;
            } else if ((this->actor.bgCheckFlags & 0x10) || (this->unk310_homeY < this->actor.posRot.pos.y)) {
                this->unk1BC_targetRotX = 0x2154;
            }
        }
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, this->unk1BC_targetRotX, 0x100);
    }
    if (this->actor.bgCheckFlags & 8) {
        Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.wallPolyRot, 2, 0xC00, 0x300);
    }
    if ((this->unk1BA_timer == 0) && (this->actor.xzDistFromLink < 200.0f) && (Player_GetMask(globalCtx) != 2)) {
        func_80A1349C_Setup(this);
    }
}

void func_80A13C98_Action(EnFirefly* this, GlobalContext* globalCtx) {
    if (func_800A56C8(&this->skelAnime, 6.0f) != 0) {
        this->skelAnime.animPlaybackSpeed = 0.0f;
    }
    this->actor.dmgEffectTimer = 40;
    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    Math_ApproxF(&this->actor.speedXZ, 0.0f, 0.5f);
    if (this->actor.flags & 0x8000) {
        this->actor.dmgEffectTimer = 40;
    } else {
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, 0x6800, 0x200);
        this->actor.shape.rot.y -= 0x300;
        if (this->unk1BA_timer != 0) {
            this->unk1BA_timer--;
        }
        if ((this->actor.bgCheckFlags & 1) || (this->unk1BA_timer == 0)) {
            func_80A13444_Setup(this);
        }
    }
}

void func_80A13D68_Action(EnFirefly* this, GlobalContext* globalCtx) {
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    Math_ApproxF(&this->actor.scale.x, 0.0f, 0.00034f);
    this->actor.scale.y = this->actor.scale.z = this->actor.scale.x;
    if (this->unk1BA_timer == 0) {
        Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.posRot.pos, 0xE0);
        Actor_Kill(&this->actor);
    }
}

void func_80A13DE4_Action(EnFirefly* this, GlobalContext* globalCtx) {
    Actor* sp44;
    Vec3f sp38;

    sp44 = globalCtx->actorCtx.actorList[2].first;
    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    Math_ApproxF(&this->actor.speedXZ, 4.0f, 0.5f);
    if (this->actor.bgCheckFlags & 8) {
        Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.wallPolyRot, 2, 0xC00, 0x300);
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, this->unk1BC_targetRotX, 0x100);
    } else if (func_8002E084(&this->actor, 0x2800) != 0) {
        if (func_800A56C8(&this->skelAnime, 4.0f) != 0) {
            this->skelAnime.animPlaybackSpeed = 0.0f;
            this->skelAnime.animCurrentFrame = 4.0f;
        }
        Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.yawTowardsLink, 2, 0xC00, 0x300);
        sp38.x = sp44->posRot.pos.x;
        sp38.y = sp44->posRot.pos.y + 20.0f;
        sp38.z = sp44->posRot.pos.z;
        Math_SmoothScaleMaxMinS(&this->actor.shape.rot.x, (s16)(func_8002DB28(&this->actor, (Vec3f*)&sp38) + 0x1554), 2,
                                0x400, 0x100);
    } else {
        this->skelAnime.animPlaybackSpeed = 1.5f;
        if (this->actor.xzDistFromLink > 80.0f) {
            Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.yawTowardsLink, 2, 0xC00, 0x300);
        }
        if (this->actor.bgCheckFlags & 1) {
            this->unk1BC_targetRotX = 0x954;
        }
        if ((this->actor.bgCheckFlags & 0x10) || (this->unk310_homeY < this->actor.posRot.pos.y)) {
            this->unk1BC_targetRotX = 0x2154;
        } else {
            this->unk1BC_targetRotX = 0x954;
        }
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, this->unk1BC_targetRotX, 0x100);
    }
    if ((this->unk1BA_timer == 0) || (Player_GetMask(globalCtx) == 2)) {
        func_80A1350C_Setup(this);
    }
}

void func_80A13FF4_Action(EnFirefly* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, 0, 0x100);
    Math_ApproxF(&this->actor.velocity.y, 0.0f, 0.4f);
    if (Math_ApproxF(&this->actor.speedXZ, 0.0f, 0.15f) != 0) {
        if (this->unk1BA_timer != 0) {
            this->unk1BA_timer--;
        }
        if (this->unk1BA_timer == 0) {
            func_80A1350C_Setup(this);
        }
    }
}

void func_80A14088_Action(EnFirefly* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    if (((fabsf(this->actor.posRot.pos.y - this->unk310_homeY) < 10.0f) &&
         (Math_Vec3f_DistXZ(&this->actor.posRot.pos, &this->actor.initPosRot.pos) < 20.0f)) ||
        (this->unk1BA_timer == 0)) {
        func_80A132F4_Setup(this);
    } else {
        Math_ApproxF(&this->actor.speedXZ, 3.0f, 0.3f);
        if (this->actor.bgCheckFlags & 1) {
            this->unk1BC_targetRotX = 0x954;
        } else if ((this->actor.bgCheckFlags & 0x10) || (this->unk310_homeY < this->actor.posRot.pos.y)) {
            this->unk1BC_targetRotX = 0x2154;
        } else {
            this->unk1BC_targetRotX = 0x954;
        }
        if (this->actor.bgCheckFlags & 8) {
            Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.wallPolyRot, 2, 0xC00, 0x300);
        } else {
            Math_ApproxUpdateScaledS(&this->actor.shape.rot.y, func_8002DAC0(&this->actor, &this->actor.initPosRot.pos),
                                     0x300);
        }
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, this->unk1BC_targetRotX, 0x100);
    }
}

void func_80A141F0_Action(EnFirefly* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    Math_ApproxF(&this->actor.speedXZ, 0.0f, 0.5f);
    Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, 0x1554, 0x100);
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    if (this->unk1BA_timer == 0) {
        if (this->unk1B9 != 0) {
            this->unk1B8 = 1;
        } else if (this->actor.params == 4) {
            this->unk1B8 = 2;
        }
        func_80A132F4_Setup(this);
    }
}

void func_80A14294_Action(EnFirefly* this, GlobalContext* globalCtx) {
    if ((this->actor.bgCheckFlags & 1) || (this->actor.groundY == -32000.0f)) {
        this->actor.dmgEffectTimer = 0;
        func_80A13444_Setup(this);
    } else {
        this->actor.dmgEffectTimer = 0xFF;
    }
}

void func_80A142F4_Action_Perched_(EnFirefly* this, GlobalContext* globalCtx) {
    Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, 0, 0x100);
    if (this->unk1BA_timer != 0) {
        SkelAnime_FrameUpdateMatrix(&this->skelAnime);
        if (func_800A56C8(&this->skelAnime, 6.0f) != 0) {
            this->unk1BA_timer--;
        }
    } else if (Math_Rand_ZeroOne() < 0.02f) {
        this->unk1BA_timer = 1;
    }
    if (this->actor.xzDistFromLink < 120.0f) {
        func_80A13764_Setup_SwoopOnPlayer(this);
    }
}

void func_80A143B4_Action_SwoopOnPlayer(EnFirefly* this, GlobalContext* globalCtx) {
    Player* temp_a2 = PLAYER;
    Vec3f sp28;

    SkelAnime_FrameUpdateMatrix(&this->skelAnime);
    if (this->unk1BA_timer != 0) {
        this->unk1BA_timer--;
    }
    if (this->unk1BA_timer < 40) {
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, -0xAAC, 0x100);
    } else {
        sp28.x = temp_a2->actor.posRot.pos.x;
        sp28.y = temp_a2->actor.posRot.pos.y + 20.0f;
        sp28.z = temp_a2->actor.posRot.pos.z;
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.x, func_8002DB28(&this->actor, &sp28) + 0x1554, 0x100);
        Math_ApproxUpdateScaledS(&this->actor.shape.rot.y, this->actor.yawTowardsLink, 0x300);
    }
    if (this->unk1BA_timer == 0) {
        func_80A132F4_Setup(this);
    }
}

void func_80A1448C_SpawnFireEffects(EnFirefly* this, GlobalContext* globalCtx) {
    s32 bodyPart;

    for (bodyPart = 0; bodyPart < EN_FIREFLY_BODY_PART_MAX; bodyPart++) {
        EffectSsEnFire_SpawnVec3f(globalCtx, &this->actor, &this->actor.posRot.pos, 40, 0, 0, bodyPart);
    }
    this->unk1B8 = 0;
}

void func_80A1450C_ReactToAC(EnFirefly* this, GlobalContext* globalCtx) {
    if (this->collider.base.acFlags & 2) {
        this->collider.base.acFlags &= ~2;
        func_80035650(&this->actor, &this->collider.elements->base, 1);
        if ((this->actor.collideData.damageEffect != 0) || (this->actor.collideData.damage != 0)) {
            if (Actor_ApplyDamage(&this->actor) == 0) {
                func_80032C7C(globalCtx, &this->actor);
                this->actor.flags &= ~1;
            }
            if (this->actor.collideData.damageEffect == 2) {
                if (this->actor.params == 4) {
                    this->actor.collideData.health = 0;
                    func_80032C7C(globalCtx, &this->actor);
                    func_80A1448C_SpawnFireEffects(this, globalCtx);
                    func_80A133A0_Setup(this);
                } else if (this->unk1B9 == 0) {
                    func_80A13098_Ignite_(this);
                    if (this->actionFunc == func_80A142F4_Action_Perched_) {
                        func_80A132F4_Setup(this);
                    }
                }
            } else if (this->actor.collideData.damageEffect == 3) {
                if (this->actor.params == 4) {
                    func_80A133A0_Setup(this);
                } else {
                    func_80A135A8_Setup(this, globalCtx);
                }
            } else if (this->actor.collideData.damageEffect == 1) {
                if (this->actionFunc != func_80A141F0_Action) {
                    func_80A13538_Setup(this);
                }
            } else {
                if ((this->actor.collideData.damageEffect == 0xF) && (this->actor.params == 4)) {
                    func_80A1448C_SpawnFireEffects(this, globalCtx);
                }
                func_80A133A0_Setup(this);
            }
        }
    }
}

void EnFirefly_Update(Actor* thisx, GlobalContext* globalCtx2) {
    EnFirefly* this = (EnFirefly*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    f32 sp34;

    if (this->collider.base.atFlags & 2) {
        this->collider.base.atFlags &= ~2;
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_FFLY_ATTACK);
        if (this->unk1B9 != 0) {
            func_80A13070_Unignite_(this);
        }
        if (this->actionFunc != func_80A143B4_Action_SwoopOnPlayer) {
            func_80A13464_Setup(this);
        }
    }
    func_80A1450C_ReactToAC(this, globalCtx);
    this->actionFunc(this, globalCtx);
    if (!(this->actor.flags & 0x8000)) {
        if ((this->actor.collideData.health == 0) || ((this->actionFunc == func_80A141F0_Action))) {
            Actor_MoveForward(&this->actor);
        } else {
            if (this->actionFunc != func_80A13FF4_Action) {
                this->actor.posRot.rot.x = 0x1554 - this->actor.shape.rot.x;
            }
            func_8002D97C(&this->actor);
        }
    }
    func_8002E4B4(globalCtx, &this->actor, 10.0f, 10.0f, 15.0f, 7);
    this->collider.elements->shape.world.center.x = this->actor.posRot.pos.x;
    this->collider.elements->shape.world.center.y = this->actor.posRot.pos.y + 10.0f;
    this->collider.elements->shape.world.center.z = this->actor.posRot.pos.z;
    if ((this->actionFunc == func_80A13DE4_Action) || (this->actionFunc == func_80A143B4_Action_SwoopOnPlayer)) {
        Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &this->collider.base);
    }
    if (this->actor.collideData.health != 0) {
        Collider_AddAC(globalCtx, &globalCtx->colliderCtx, &this->collider.base);
        this->actor.posRot.rot.y = this->actor.shape.rot.y;
        if (func_800A56C8(&this->skelAnime, 5.0f) != 0) {
            Audio_PlayActorSound2(&this->actor, NA_SE_EN_FFLY_FLY);
        }
    }
    Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &this->collider.base);
    sp34 = Math_Sins(this->actor.shape.rot.x);
    this->actor.posRot2.pos.x = (Math_Sins(this->actor.shape.rot.y) * (10.0f * sp34)) + this->actor.posRot.pos.x;
    this->actor.posRot2.pos.y = (Math_Coss(this->actor.shape.rot.x) * 10.0f) + this->actor.posRot.pos.y;
    sp34 = Math_Sins(this->actor.shape.rot.x);
    this->actor.posRot2.pos.z = (Math_Coss(this->actor.shape.rot.y) * (10.0f * sp34)) + this->actor.posRot.pos.z;
}

s32 EnFirefly_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                               void* thisx, Gfx** gfx) {
    EnFirefly* this = thisx;
    if ((this->actor.draw == EnFirefly_DrawXlu) && (globalCtx->actorCtx.unk_03 == 0)) {
        *dList = NULL;
    } else if (limbIndex == 1) {
        pos->y += 2300.0f;
    }
    return 0;
}

void EnFirefly_PostLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    Vec3f sp94;
    Vec3f* var_a1;
    Color_RGBA8* var_v1;
    Color_RGBA8* var_v0;
    MtxF sp48;
    EnFirefly* this = thisx;
    s16 var_t0;
    s16 var_t1;

    if ((this->unk1B9 == 0) && (limbIndex == 0x1B)) {
        gSPDisplayList((*gfx)++, D_6001678);
    } else {
        if (((this->unk1B8 == 1) || (this->unk1B8 == 2)) && ((limbIndex == 0xF) || (limbIndex == 0x15))) {
            if (this->actionFunc != func_80A13D68_Action) {
                Matrix_Get(&sp48);
                sp94.x = (Math_Rand_ZeroOne() * 5.0f) + sp48.wx;
                sp94.y = (Math_Rand_ZeroOne() * 5.0f) + sp48.wy;
                sp94.z = (Math_Rand_ZeroOne() * 5.0f) + sp48.wz;
                var_t0 = -0x28;
                var_t1 = 3;
            } else {
                if (limbIndex == 0xF) {
                    sp94.x = (Math_Sins(this->unk1BA_timer * 0x238C) * this->unk1BA_timer) + this->actor.posRot.pos.x;
                    sp94.z = (Math_Coss(this->unk1BA_timer * 0x238C) * this->unk1BA_timer) + this->actor.posRot.pos.z;
                } else {
                    sp94.x = this->actor.posRot.pos.x - (Math_Sins(this->unk1BA_timer * 0x238C) * this->unk1BA_timer);
                    sp94.z = this->actor.posRot.pos.z - (Math_Coss(this->unk1BA_timer * 0x238C) * this->unk1BA_timer);
                }
                var_t0 = -5;
                var_t1 = 0xA;
                sp94.y = this->actor.posRot.pos.y + ((0xF - this->unk1BA_timer) * 1.5f);
            }
            if (this->unk1B8 == 1) {
                var_v0 = &D_80A14FA0;
                var_v1 = &D_80A14FA4;
            } else {
                var_v0 = &D_80A14FA8;
                var_v1 = &D_80A14FAC;
            }
            func_8002843C(globalCtx, &sp94, &D_80A14FB0, &D_80A14FBC, var_v0, var_v1, 250, var_t0, var_t1);
        }
    }
    if ((limbIndex == 0xF) || (limbIndex == 0x15) || (limbIndex == 0xA)) {
        if (limbIndex == 0xF) {
            var_a1 = &this->bodyPartsPos[EN_FIREFLY_BODY_PART_LIMB_15];
        } else if (limbIndex == 0x15) {
            var_a1 = &this->bodyPartsPos[EN_FIREFLY_BODY_PART_LIMB_21];
        } else {
            var_a1 = &this->bodyPartsPos[EN_FIREFLY_BODY_PART_LIMB_10];
        }
        Matrix_MultVec3f(&D_80A14FC8, var_a1);
        var_a1->y -= 5.0f;
    }
}

void EnFirefly_DrawOpa(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* this = (EnFirefly*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 0x6C5);
    func_80093D18(globalCtx->state.gfxCtx);
    if (this->unk1B9 != 0) {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    }
    POLY_OPA_DISP = SkelAnime_Draw(globalCtx, this->skelAnime.skeleton, this->skelAnime.limbDrawTbl,
                                   EnFirefly_OverrideLimbDraw, EnFirefly_PostLimbDraw, this, POLY_OPA_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1763);
}

void EnFirefly_DrawXlu(Actor* thisx, GlobalContext* globalCtx) {
    EnFirefly* this = (EnFirefly*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1775);
    func_80093D84(globalCtx->state.gfxCtx);
    if (this->unk1B9 != 0) {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 0);
    } else {
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, 255);
    }
    POLY_XLU_DISP = SkelAnime_Draw(globalCtx, this->skelAnime.skeleton, this->skelAnime.limbDrawTbl,
                                   EnFirefly_OverrideLimbDraw, EnFirefly_PostLimbDraw, thisx, POLY_XLU_DISP);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_firefly.c", 1805);
}
