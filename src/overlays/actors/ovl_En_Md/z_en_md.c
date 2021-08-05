#include "global.h"
#include "z_en_md.h"
#include "objects/object_md/object_md.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "z64cutscene.h"

#define FLAGS 0x02000019

void EnMd_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMd_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80AAB874(EnMd* this, GlobalContext* globalCtx);
void func_80AAB8F8(EnMd* this, GlobalContext* globalCtx);
void func_80AAB948(EnMd* this, GlobalContext* globalCtx);
void func_80AABC10(EnMd* this, GlobalContext* globalCtx);
void func_80AABD0C(EnMd* this, GlobalContext* globalCtx);

extern AnimationHeader D_60002C8;
extern SkeletonHeader D_6007FB8;

const ActorInit En_Md_InitVars = {
    /**/ ACTOR_EN_MD,
    /**/ ACTORCAT_NPC,
    /**/ FLAGS,
    /**/ OBJECT_MD,
    /**/ sizeof(EnMd),
    /**/ EnMd_Init,
    /**/ EnMd_Destroy,
    /**/ EnMd_Update,
    /**/ EnMd_Draw,
};

ColliderCylinderInit D_80AAC310 = {
    { 0xA, 0, 0, 0x39, 0x20, 1 },
    { 0, { 0, 0, 0 }, { 0, 0, 0 }, 0, 0, 1 },
    { 0x24, 0x2E, 0, { 0, 0, 0 } },
};
CollisionCheckInfoInit2 D_80AAC33C = { 0, 0, 0, 0, 0xFF };
struct_80034EC0_Entry D_80AAC348[] = {
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, 0, 0.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, 0, -10.0f },
    { &gMidoRaiseHand1Anim, 1.0f, 0.0f, -1.0f, 2, -1.0f },
    { &gMidoHaltAnim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
    { &gMidoPutHandDownAnim, 1.0f, 0.0f, -1.0f, 2, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle1Anim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle2Anim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
    { &gMidoAnim_92B0, 1.0f, 0.0f, -1.0f, 2, -1.0f },
    { &gMidoWalkingAnim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
    { &gMidoHandsOnHipsTransitionAnim, 1.0f, 0.0f, -1.0f, 2, -1.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, 0, -8.0f },
    { &gMidoSlamAnim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
    { &gMidoRaiseHand2Anim, 1.0f, 0.0f, -1.0f, 2, -1.0f },
    { &gMidoAngryHeadTurnAnim, 1.0f, 0.0f, -1.0f, 0, -1.0f },
};
Vec3f D_80AAC498 = { 400.0f, 0.0f, 0.0f };
void* D_80AAC4A4[3] = {
    gMidoEyeOpenTex,
    gMidoEyeHalfTex,
    gMidoEyeClosedTex,
};

void func_80AAA250(EnMd* this) {
    f32 temp_fv0;
    f32 temp_fv1;

    temp_fv0 = this->unk14C.startFrame;
    temp_fv1 = this->unk14C.endFrame;
    this->unk14C.startFrame = temp_fv1;
    this->unk14C.curFrame = temp_fv1;
    this->unk14C.endFrame = temp_fv0;
    this->unk14C.playSpeed = -1.0f;
}

void func_80AAA274(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 2);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 3);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA308(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 4);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 5);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA39C(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 2);
            func_80AAA250(this);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 7);
                this->unk20A++;
            } else {
                break;
            }
        case 2:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 8);
                this->unk20A++;
            }
    }
}

void func_80AAA474(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 7);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 0xA);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA508(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 2);
            func_80AAA250(this);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 0xA);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA5A4(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 9);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA638(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 9);
            func_80AAA250(this);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 0xA);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA6D4(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 0xB);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA768(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 0xC);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 3);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA7FC(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 0xD);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA890(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            func_80034EC0(&this->unk14C, D_80AAC348, 7);
            func_80AAA250(this);
            this->unk20A++;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                func_80034EC0(&this->unk14C, D_80AAC348, 0xA);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA92C(EnMd* this, u8 arg1) {
    this->unk20B = arg1;
    this->unk20A = 0;
}

void func_80AAA93C(EnMd* this) {
    switch (this->unk20B) {
        case 1:
            func_80AAA274(this);
            break;
        case 2:
            func_80AAA308(this);
            break;
        case 3:
            func_80AAA39C(this);
            break;
        case 4:
            func_80AAA474(this);
            break;
        case 5:
            func_80AAA508(this);
            break;
        case 6:
            func_80AAA5A4(this);
            break;
        case 7:
            func_80AAA638(this);
            break;
        case 8:
            func_80AAA6D4(this);
            break;
        case 9:
            func_80AAA768(this);
            break;
        case 10:
            func_80AAA7FC(this);
            break;
        case 11:
            func_80AAA890(this);
            break;
        default:
            break;
    }
}

void func_80AAAA24(EnMd* this) {
    if (this->unk1E0.unk_00 != 0) {
        switch (this->actor.textId) {
            case 0x102F:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                if ((this->unk208 == 2) && (this->unk20B != 2)) {
                    func_80AAA92C(this, 2);
                }
                if ((this->unk208 == 5) && (this->unk20B != 8)) {
                    func_80AAA92C(this, 8);
                }
                if ((this->unk208 == 0xB) && (this->unk20B != 9)) {
                    func_80AAA92C(this, 9);
                }
                break;
            case 0x1033:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                if ((this->unk208 == 1) && (this->unk20B != 2)) {
                    func_80AAA92C(this, 2);
                }
                if ((this->unk208 == 5) && (this->unk20B != 0xA)) {
                    func_80AAA92C(this, 0xA);
                }
                if ((this->unk208 == 7) && (this->unk20B != 9)) {
                    func_80AAA92C(this, 9);
                }
                break;
            case 0x1030:
            case 0x1034:
            case 0x1045:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                break;
            case 0x1046:
                if ((this->unk208 == 0) && (this->unk20B != 6)) {
                    func_80AAA92C(this, 6);
                }
                break;
        }
    } else if (this->unk14C.animation != &gMidoHandsOnHipsIdleAnim) {
        func_80034EC0(&this->unk14C, D_80AAC348, 0xA);
        func_80AAA92C(this, 0);
    }
    func_80AAA93C(this);
}

s16 func_80AAAC78(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* this = (EnMd*)thisx;
    s16 temp_v0;

    temp_v0 = func_8010BDBC(&globalCtx->msgCtx);
    if (((this->unk209 == 0xA) || (this->unk209 == 5) || (this->unk209 == 2) || (this->unk209 == 1)) &&
        (this->unk209 != temp_v0)) {
        this->unk208++;
    }
    this->unk209 = temp_v0;
    return temp_v0;
}

u16 func_80AAACF8(GlobalContext* globalCtx, EnMd* this) {
    u16 temp_v0;

    temp_v0 = Text_GetFaceReaction(globalCtx, 0x11);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    this->unk208 = 0;
    this->unk209 = 0;
    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        return 0x1045;
    }
    if (gSaveContext.eventChkInf[0] & 0x10) {
        return 0x1034;
    }
    if ((CUR_EQUIP_VALUE(EQUIP_SHIELD) == 1) && (CUR_EQUIP_VALUE(EQUIP_SWORD) == 1)) {
        return 0x1033;
    }
    if (gSaveContext.infTable[0] & 0x1000) {
        return 0x1030;
    }
    return 0x102F;
}

u16 func_80AAADE0(GlobalContext* globalCtx, EnMd* this) {
    this->unk208 = 0;
    this->unk209 = 0;
    if (gSaveContext.eventChkInf[4] & 1) {
        return 0x1028;
    }
    return 0x1046;
}

u16 func_80AAAE14(GlobalContext* globalCtx, EnMd* this) {
    this->unk208 = 0;
    this->unk209 = 0;
    if (gSaveContext.eventChkInf[4] & 0x100) {
        if (gSaveContext.infTable[1] & 0x200) {
            return 0x1071;
        } else {
            return 0x1070;
        }
    } else if (gSaveContext.eventChkInf[0] & 0x400) {
        return 0x1068;
    } else if (gSaveContext.infTable[1] & 0x20) {
        return 0x1061;
    } else {
        return 0x1060;
    }
}

u16 func_80AAAE94(GlobalContext* globalCtx, Actor* thisx) {
    EnMd* this = (EnMd*)thisx;

    switch (globalCtx->sceneNum) {
        case SCENE_SPOT04:
            return func_80AAACF8(globalCtx, this);
        case SCENE_KOKIRI_HOME4:
            return func_80AAADE0(globalCtx, this);
        case SCENE_SPOT10:
            return func_80AAAE14(globalCtx, this);
        default:
            return 0;
    }
}

s16 func_80AAAF04(GlobalContext* globalCtx, Actor* thisx) {
    switch (func_80AAAC78(thisx, globalCtx)) {
        case 0:
        case 1:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
            return 1;

        case 2:
            switch (thisx->textId) {
                case 0x1028:
                    gSaveContext.eventChkInf[0] |= 0x8000;
                    break;
                case 0x102F:
                    gSaveContext.eventChkInf[0] |= 4;
                    gSaveContext.infTable[0] |= 0x1000;
                    break;
                case 0x1060:
                    gSaveContext.infTable[1] |= 0x20;
                    break;
                case 0x1070:
                    gSaveContext.infTable[1] |= 0x200;
                    break;
                case 0x1033:
                case 0x1067:
                    return 2;
            }
            return 0;

        case 5:
            if (func_80106BC8(globalCtx) != 0) {
                return 2;
            }
            break;
    }
    return 1;
}

s32 func_80AAB03C(EnMd* this, GlobalContext* globalCtx) {
    if ((globalCtx->sceneNum == SCENE_SPOT04) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
        !(gSaveContext.eventChkInf[4] & 1)) {
        return 1;
    } else if ((globalCtx->sceneNum == SCENE_KOKIRI_HOME4) &&
               ((gSaveContext.eventChkInf[1] & 0x1000) || (gSaveContext.eventChkInf[4] & 1)) && LINK_IS_CHILD) {
        return 1;
    } else if (globalCtx->sceneNum == SCENE_SPOT10) {
        return 1;
    } else {
        return 0;
    }
}

void func_80AAB0E0(EnMd* arg0) {
    if (DECR(arg0->unk20C) == 0) {
        arg0->unk20E += 1;
        if (arg0->unk20E >= 3) {
            arg0->unk20C = Rand_S16Offset(0x1E, 0x1E);
            arg0->unk20E = 0;
        }
    }
}

void func_80AAB158(EnMd* this, GlobalContext* globalCtx) {
    Player* temp_a2;
    s16 temp_v1;
    s16 temp_ft1;
    s16 var_v1_real;
    s16 var_a3_real;

    temp_a2 = PLAYER;
    if (this->actor.xzDistToPlayer < 170.0f) {
        temp_ft1 = (f32)this->actor.yawTowardsPlayer - (f32)this->actor.shape.rot.y;
        temp_v1 = ABS(temp_ft1);
        if (temp_v1 <= func_800347E8(2)) {
            var_a3_real = 2;
            var_v1_real = 1;
        } else {
            var_a3_real = 1;
            var_v1_real = 1;
        }
    } else {
        var_a3_real = 1;
        var_v1_real = 0;
    }
    if (this->unk1E0.unk_00 != 0) {
        var_a3_real = 4;
    }
    if (this->unk190 == func_80AABD0C) {
        var_a3_real = 1;
        var_v1_real = 0;
    }
    if (this->unk190 == func_80AAB8F8) {
        var_a3_real = 4;
        var_v1_real = 1;
    }
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) || (gDbgCamEnabled != 0)) {
        this->unk1E0.unk_18 = globalCtx->view.eye;
        this->unk1E0.unk_14 = 40.0f;
        var_a3_real = 2;
    } else {
        this->unk1E0.unk_18 = temp_a2->actor.world.pos;
        if (gSaveContext.linkAge > 0) {
            this->unk1E0.unk_14 = 0.0f;
        } else {
            this->unk1E0.unk_14 = -18.0f;
        }
    }
    func_80034A14(&this->actor, &this->unk1E0, 2, var_a3_real);
    if ((this->unk190 != func_80AABC10) && (var_v1_real != 0)) {
        func_800343CC(globalCtx, &this->actor, &this->unk1E0.unk_00, this->unk194.dim.radius + 30.0f, func_80AAAE94,
                      func_80AAAF04);
    }
}

s32 func_80AAB370(EnMd* this, GlobalContext* globalCtx) {
    Path* temp_a0;
    s32 pad;
    f32 temp_fa0;
    f32 temp_fa1;
    Vec3s* temp_v0;
    s32 temp_a2;

    temp_a2 = this->actor.params & 0xFF00;
    if (temp_a2 == 0xFF00) {
        return 0;
    }
    temp_a0 = &globalCtx->setupPathList[temp_a2 >> 8];
    temp_v0 = SEGMENTED_TO_VIRTUAL(temp_a0->points);
    temp_v0 += this->unk212;
    temp_fa0 = (f32)temp_v0->x - this->actor.world.pos.x;
    temp_fa1 = (f32)temp_v0->z - this->actor.world.pos.z;
    Math_SmoothStepToS(&this->actor.world.rot.y, (Math_FAtan2F(temp_fa0, temp_fa1) * 10430.378f), 4, 0xFA0, 1);
    if (((temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1)) < 100.0f) {
        this->unk212++;
        if (this->unk212 >= temp_a0->count) {
            this->unk212 = 0;
        }
        return 1;
    }
    return 0;
}

s32 func_80AAB4DC(EnMd* this, GlobalContext* globalCtx) {
    Path* temp_a1;
    s32 temp_v1;
    Vec3s* temp_a3;

    temp_v1 = this->actor.params & 0xFF00;
    if (temp_v1 == 0xFF00) {
        return 0;
    }
    temp_a1 = &globalCtx->setupPathList[temp_v1 >> 8];
    temp_a3 = SEGMENTED_TO_VIRTUAL(temp_a1->points);
    temp_a3 += temp_a1->count - 1;
    this->actor.world.pos.x = temp_a3->x;
    this->actor.world.pos.y = temp_a3->y;
    this->actor.world.pos.z = temp_a3->z;
    return 1;
}

void func_80AAB5A4(EnMd* this, GlobalContext* globalCtx) {
    f32 var_fv0;

    if (globalCtx->sceneNum != SCENE_KOKIRI_HOME4) {
        if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
            (globalCtx->sceneNum == SCENE_SPOT04)) {
            var_fv0 = 100.0f;
        } else {
            var_fv0 = 400.0f;
        }
        this->unk210 = func_80034DD4(&this->actor, globalCtx, this->unk210, var_fv0);
        this->actor.shape.shadowAlpha = (u8)this->unk210;
    } else {
        this->unk210 = 255;
        this->actor.shape.shadowAlpha = (u8)this->unk210;
    }
}

void EnMd_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* this = (EnMd*)thisx;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(globalCtx, &this->unk14C, &gMidoSkel, NULL, this->unk258, this->unk2BE, 0x11);
    Collider_InitCylinder(globalCtx, &this->unk194);
    Collider_SetCylinder(globalCtx, &this->unk194, &this->actor, &D_80AAC310);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, NULL, &D_80AAC33C);
    if (func_80AAB03C(this, globalCtx) == 0) {
        Actor_Kill(&this->actor);
        return;
    }
    func_80034EC0(&this->unk14C, D_80AAC348, 0);
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = 6;
    this->unk210 = 255;
    Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_ELF, this->actor.world.pos.x,
                       this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, FAIRY_KOKIRI);
    if (((globalCtx->sceneNum == SCENE_SPOT04) && !(gSaveContext.eventChkInf[0] & 0x10)) ||
        ((globalCtx->sceneNum == SCENE_SPOT04) && (gSaveContext.eventChkInf[0] & 0x10) &&
         CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) ||
        ((globalCtx->sceneNum == SCENE_SPOT10) && !(gSaveContext.eventChkInf[0] & 0x400))) {
        this->actor.home.pos = this->actor.world.pos;
        this->unk190 = func_80AAB948;
    } else {
        if (globalCtx->sceneNum != SCENE_KOKIRI_HOME4) {
            func_80AAB4DC(this, globalCtx);
        }
        this->unk190 = func_80AAB874;
    }
}

void EnMd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* this = (EnMd*)thisx;

    Collider_DestroyCylinder(globalCtx, &this->unk194);
}

void func_80AAB874(EnMd* this, GlobalContext* globalCtx) {
    if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(globalCtx, this->unk214, this->unk236, 0x11);
    } else if ((this->unk1E0.unk_00 == 0) && (this->unk20B != 7)) {
        func_80AAA92C(this, 7);
    }
    func_80AAAA24(this);
}

void func_80AAB8F8(EnMd* this, GlobalContext* globalCtx) {
    if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(globalCtx, this->unk214, this->unk236, 0x11);
    }
    func_80AAA93C(this);
}

void func_80AAB948(EnMd* this, GlobalContext* globalCtx) {
    Player* sp2C;
    f32 temp_fv1;
    Player* sp24;
    s16 temp_v0_3;
    s32 temp_v0_4;

    sp2C = PLAYER;
    sp24 = PLAYER;
    func_80AAAA24(this);
    if (this->unk1E0.unk_00 == 0) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        temp_v0_3 = Math_Vec3f_Yaw(&this->actor.home.pos, &sp24->actor.world.pos);
        this->actor.world.pos.x = this->actor.home.pos.x;
        this->actor.world.pos.x += 60.0f * Math_SinS(temp_v0_3);
        this->actor.world.pos.z = this->actor.home.pos.z;
        this->actor.world.pos.z += 60.0f * Math_CosS(temp_v0_3);
        temp_fv1 = fabsf((f32)this->actor.yawTowardsPlayer - (f32)temp_v0_3) * 0.001f * 3.0f;
        this->unk14C.playSpeed = CLAMP(temp_fv1, 1.0f, 3.0f);
    }
    if (this->unk1E0.unk_00 == 2) {
        if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
            (globalCtx->sceneNum == SCENE_SPOT04)) {
            globalCtx->msgCtx.msgMode = 0x37;
        }
        if (globalCtx->sceneNum == SCENE_SPOT04) {
            gSaveContext.eventChkInf[0] |= 0x10;
        }
        if (globalCtx->sceneNum == SCENE_SPOT10) {
            gSaveContext.eventChkInf[0] |= 0x400;
        }
        func_80AAA92C(this, 3);
        func_80AAA93C(this);
        this->unk212 = 1;
        this->unk1E0.unk_00 = 0;
        this->unk190 = func_80AABD0C;
        this->actor.speedXZ = 1.5f;
    } else {
        if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
            func_80034F54(globalCtx, this->unk214, this->unk236, 0x11);
        }
        if ((this->unk1E0.unk_00 == 0) && (globalCtx->sceneNum == SCENE_SPOT10)) {
            if (sp2C->stateFlags2 & 0x01000000) {
                sp2C->stateFlags2 |= 0x02000000;
                sp2C->unk_6A8 = &this->actor;
                func_8010BD58(globalCtx, 0x22U);
                this->unk190 = func_80AABC10;
            } else if (this->actor.xzDistToPlayer < (30.0f + (f32)this->unk194.dim.radius)) {
                sp2C->stateFlags2 |= 0x800000;
            }
        }
    }
}

void func_80AABC10(EnMd* this, GlobalContext* globalCtx) {
    Player* temp_v0;

    temp_v0 = PLAYER;
    if (globalCtx->msgCtx.unk_E3EE >= 4) {
        this->unk190 = func_80AAB948;
        globalCtx->msgCtx.unk_E3EE = 4;
    } else if (globalCtx->msgCtx.unk_E3EE == 3) {
        Audio_PlaySoundGeneral(0x4802U, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
        this->actor.textId = 0x1067;
        func_8002F2CC(&this->actor, globalCtx, (f32)this->unk194.dim.radius + 30.0f);
        this->unk190 = func_80AAB948;
        globalCtx->msgCtx.unk_E3EE = 4;
    } else {
        temp_v0->stateFlags2 |= 0x800000;
    }
}

void func_80AABD0C(EnMd* this, GlobalContext* globalCtx) {
    func_80034F54(globalCtx, this->unk214, this->unk236, 0x11);
    func_80AAA93C(this);
    if ((func_80AAB370(this, globalCtx) == 0) || (this->unk212 != 0)) {
        this->actor.shape.rot = this->actor.world.rot;
    } else if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !(gSaveContext.eventChkInf[1] & 0x1000) &&
               (globalCtx->sceneNum == SCENE_SPOT04)) {
        func_80106CCC(globalCtx);
        gSaveContext.eventChkInf[1] |= 0x1000;
        Actor_Kill(&this->actor);
    } else {
        func_80AAA92C(this, 0xB);
        this->unk14C.playSpeed = 0.0f;
        this->actor.speedXZ = 0.0f;
        this->actor.home.pos = this->actor.world.pos;
        this->unk190 = func_80AAB8F8;
    }
}

void EnMd_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* this = (EnMd*)thisx;
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->unk194);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk194.base);
    SkelAnime_Update(&this->unk14C);
    func_80AAB0E0(this);
    func_80AAB5A4(this, globalCtx);
    Actor_MoveForward(&this->actor);
    func_80AAB158(this, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 0.0f, 0.0f, 4);
    this->unk190(this, globalCtx);
}

s32 func_80AABEF0(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx,
                  Gfx** gfx) {
    EnMd* this = (EnMd*)thisx;
    Vec3s sp2C;

    if (limbIndex == 0x10) {
        Matrix_Translate(1200.0f, 0.0f, 0.0f, 1U);
        sp2C = this->unk1E0.unk_08;
        Matrix_RotateX((sp2C.y / 32768.0f) * 3.1415927f, MTXMODE_APPLY);
        Matrix_RotateZ((sp2C.x / 32768.0f) * 3.1415927f, MTXMODE_APPLY);
        Matrix_Translate(-1200.0f, 0.0f, 0.0f, 1U);
    }
    if (limbIndex == 9) {
        sp2C = this->unk1E0.unk_0E;
        Matrix_RotateX((sp2C.x / 32768.0f) * 3.1415927f, MTXMODE_APPLY);
        Matrix_RotateY((sp2C.y / 32768.0f) * 3.1415927f, MTXMODE_APPLY);
    }
    if ((limbIndex == 9) || (limbIndex == 0xA) || (limbIndex == 0xD)) {
        rot->y += (Math_SinS(this->unk214[limbIndex]) * 200.0f);
        rot->z += (Math_CosS(this->unk236[limbIndex]) * 200.0f);
    }
    return 0;
}

void func_80AAC104(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnMd* this = (EnMd*)thisx;
    Vec3f sp18;

    sp18 = D_80AAC498;
    if (limbIndex == 0x10) {
        Matrix_MultVec3f(&sp18, &this->actor.focus.pos);
    }
}

void EnMd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnMd* this = (EnMd*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_md.c", 1280);
    if (this->unk210 == 255) {
        gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80AAC4A4[this->unk20E]));
        func_80034BA0(globalCtx, &this->unk14C, func_80AABEF0, func_80AAC104, &this->actor, this->unk210);
    } else if (this->unk210 != 0) {
        gSPSegment(POLY_XLU_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80AAC4A4[this->unk20E]));
        func_80034CC4(globalCtx, &this->unk14C, func_80AABEF0, func_80AAC104, &this->actor, this->unk210);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_md.c", 1317);
}
