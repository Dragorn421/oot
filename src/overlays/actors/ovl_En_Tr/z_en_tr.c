/*
 * File: z_en_tr.c
 * Overlay: ovl_En_Tr
 * Description: Koume and Kotake
 */

#include "z_en_tr.h"

#define FLAGS 0x00000010

#define THIS ((EnTr*)thisx)

void EnTr_Init(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Update(Actor* thisx, GlobalContext* globalCtx);
void EnTr_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80B22F1C(EnTr* this, GlobalContext* globalCtx);
void func_80B234D4(EnTr* this, GlobalContext* globalCtx);
void func_80B23820(EnTr* this, GlobalContext* globalCtx);
void func_80B23A88(EnTr* this, GlobalContext* globalCtx);
void func_80B24038(EnTr* this, GlobalContext* globalCtx, s32 arg2);
void func_80B24230(EnTr* this, GlobalContext* globalCtx, s32 arg2);
void func_80B242B4(EnTr* this, GlobalContext* globalCtx, s32 arg2);

extern AnimationHeader D_6000BFC;
extern AnimationHeader D_60013CC;
extern AnimationHeader D_6001CDC;
extern AnimationHeader D_60035CC;
extern AnimationHeader D_6003FC8;
extern AnimationHeader D_60049C8;
extern FlexSkeletonHeader D_600C530;
extern FlexSkeletonHeader D_6011688;

const ActorInit En_Tr_InitVars = {
    ACTOR_EN_TR,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_TR,
    sizeof(EnTr),
    (ActorFunc)EnTr_Init,
    (ActorFunc)EnTr_Destroy,
    (ActorFunc)EnTr_Update,
    (ActorFunc)EnTr_Draw,
};
static AnimationHeader* D_80B24360[2] = { &D_6003FC8, &D_6001CDC };
static AnimationHeader* D_80B24368[2] = { (AnimationHeader*)0x06002BC4, &D_6000BFC };
static AnimationHeader* D_80B24370[2] = { &D_60035CC, &D_60013CC };
static AnimationHeader* D_80B24378[2] = { &D_60049C8, &D_60049C8 };
static AnimationHeader* D_80B24380[2] = { (AnimationHeader*)0x06012E1C, (AnimationHeader*)0x06012E1C };
static f32 D_80B24388[7] = { 0.0f, 20.0f, -30.0f, 20.0f, -20.0f, -20.0f, 30.0f };
static f32 D_80B243A4[7] = { 0.0f, 30.0f, 0.0f, -30.0f, 30.0f, -30.0f, 0.0f };
static Color_RGBA8 D_80B243C0[4] = {
    { 0xFF, 0xC8, 0, 0xFF },
    { 0xFF, 0, 0, 0xFF },
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0, 0, 0xFF, 0xFF },
};
static s32 D_80B243D0[3] = { 0x060086D8, 0x060094D8, 0x060098D8 };
static Vec3f D_80B243DC = { 2300.0f, 0.0f, -600.0f };
static Vec3f D_80B243E8 = { 0.0f, 0.0f, 0.0f };

void EnTr_SetupAction(EnTr* this, EnTrActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnTr_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnTr* this = (EnTr*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    EnTr_SetupAction(this, func_80B22F1C);
    this->unk2D4 = 0;
    this->actor.child = NULL;
    Actor_SetScale(&this->actor, 0.01f);
    switch (this->actor.params) {
        case 0:
            SkelAnime_InitFlex(globalCtx, &this->unk14C, &D_6011688, &D_6003FC8, this->unk190, this->unk232, 27);
            Animation_PlayOnce(&this->unk14C, &D_6003FC8);
            this->unk2E4 = NULL;
            EnTr_SetupAction(this, func_80B23A88);
            this->unk2D8 = 3;
            break;

        case 1:
            SkelAnime_InitFlex(globalCtx, &this->unk14C, &D_600C530, &D_6001CDC, this->unk190, this->unk232, 27);
            Animation_PlayOnce(&this->unk14C, &D_6001CDC);
            this->unk2E4 = NULL;
            EnTr_SetupAction(this, func_80B23A88);
            this->unk2D8 = 2;
            break;

        default:
            __assert("0", "../z_en_tr.c", 0x115);
            break;
    }
}

void EnTr_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_80B22E6C(EnTr* this, GlobalContext* globalCtx) {
    if ((this->unk2D6 == 0xB) && ((this->actor.params != 0) || (gSaveContext.sceneSetupIndex == 6))) {
        Audio_PlaySoundGeneral(NA_SE_EN_TWINROBA_SHOOT_VOICE, &D_801333D4, 4U, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    if (this->unk2D6 > 0) {
        this->unk2D6--;
    } else {
        if (this->actor.child != NULL) {
            this->actor.child = NULL;
        }
    }
    func_8002F974(&this->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
}

void func_80B22F1C(EnTr* this, GlobalContext* globalCtx) {
}

void func_80B22F28(EnTr* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v0 = globalCtx->csCtx.npcActions[this->unk2D8];
        if (temp_v0 != NULL) {
            switch (temp_v0->action) {
                case 4:
                    Actor_SetScale(&this->actor, 0.01f);
                    EnTr_SetupAction(this, func_80B234D4);
                    this->unk2D6 = 0x18;
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_PO_DEAD2);
                    break;

                case 6:
                    Animation_Change(&this->unk14C, D_80B24380[this->actor.params], 1.0f, 0.0f,
                                     Animation_GetLastFrame(D_80B24380[this->actor.params]), ANIMMODE_ONCE, -5.0f);
                    EnTr_SetupAction(this, func_80B22E6C);
                    this->unk2E4 = D_80B24378[this->actor.params];
                    this->unk2D6 = 0x27;
                    Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_DEMO_6K,
                                       this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0, 0,
                                       0, this->actor.params + 9);
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_FANTOM_MASIC1);
                    break;

                default:
                    func_80B24038(this, globalCtx, this->unk2D8);
                    func_80B24230(this, globalCtx, this->unk2D8);
                    break;
            }
            func_8002F974(&this->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
        }
    }
}

void func_80B230D8(EnTr* this, GlobalContext* globalCtx) {
    Vec3f sp34;
    CsCmdActorAction* temp_v0;

    sp34 = this->actor.world.pos;
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v0 = globalCtx->csCtx.npcActions[this->unk2D8];
        if (temp_v0 != NULL) {
            if (temp_v0->action == 8) {
                func_80B24038(this, globalCtx, this->unk2D8);
                this->actor.world.rot.y = Math_Atan2S(this->actor.velocity.z, this->actor.velocity.x);
                Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.world.rot.y, 0xA, 0x400, 0x100);
                this->actor.world.rot.y = this->actor.shape.rot.y;
            } else {
                func_80B242B4(this, globalCtx, this->unk2D8);
                this->actor.world.pos.x += Math_SinS(this->unk2D6) * 150.0f;
                this->actor.world.pos.y += -100.0f;
                this->actor.world.pos.z += Math_CosS(this->unk2D6) * 150.0f;
                this->actor.shape.rot.y = (s16)this->unk2D6 + 0x4000;
                this->unk2D6 += 0x400;
                this->actor.velocity.x = this->actor.world.pos.x - sp34.x;
                this->actor.velocity.y = this->actor.world.pos.y - sp34.y;
                this->actor.velocity.z = this->actor.world.pos.z - sp34.z;
            }
            if (globalCtx->csCtx.frames < 670) {
                func_8002F974(&this->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
            }
        }
    }
}

void func_80B23254(EnTr* this, GlobalContext* globalCtx, s32 arg2, f32 arg3, f32 arg4) {
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;
    Vec3f sp58;
    Color_RGBA8* sp54;
    Color_RGBA8* sp50;
    Vec3f sp44;
    s16 temp_s0;
    s16 sp40;

    sp44 = ACTIVE_CAM->eye;
    temp_s0 = Math_Vec3f_Yaw(&sp44, &this->actor.world.pos);
    sp40 = -Math_Vec3f_Pitch(&sp44, &this->actor.world.pos);
    sp64.z = 0.0f;
    sp64.x = 0.0f;
    sp70.x = arg3 * Math_SinS(temp_s0) * Math_CosS(sp40);
    sp70.y = arg3 * Math_SinS(sp40);
    sp70.z = arg3 * Math_CosS(temp_s0) * Math_CosS(sp40);
    sp64.y = 0.5f;
    sp54 = &D_80B243C0[(this->actor.params * 2) + 0];
    sp50 = &D_80B243C0[(this->actor.params * 2) + 1];
    sp58 = this->actor.world.pos;
    sp58.x -= sp70.x * 10.0f;
    sp58.y -= sp70.y * 10.0f;
    sp58.z -= sp70.z * 10.0f;
    sp7C.x = sp58.x + (D_80B24388[arg2] * arg4 * Math_CosS(temp_s0));
    sp7C.y = sp58.y + (D_80B243A4[arg2] * arg4);
    sp7C.z = sp58.z - (D_80B24388[arg2] * arg4 * Math_SinS(temp_s0));
    func_8002829C(globalCtx, &sp7C, &sp70, &sp64, sp54, sp50, 800.0f * arg4, 80.0f * arg4);
}

void func_80B234D4(EnTr* this, GlobalContext* globalCtx) {
    s32 temp_hi;

    if (this->unk2D6 >= 0x11) {
        this->actor.shape.rot.y = (this->actor.shape.rot.y - (this->unk2D6 * 0x28F)) + 0x3D68;
    } else if (this->unk2D6 >= 5) {
        Actor_SetScale(&this->actor, this->actor.scale.x * 0.9f);
        this->actor.shape.rot.y = (this->actor.shape.rot.y - (this->unk2D6 * 0x28F)) + 0x3D68;
    } else if (this->unk2D6 > 0) {
        temp_hi = (this->unk2D6 * 2) % 7;
        func_80B23254(this, globalCtx, temp_hi, 5.0f, 0.2f);
        func_80B23254(this, globalCtx, (temp_hi + 1) % 7, 5.0f, 0.2f);
        Actor_SetScale(&this->actor, this->actor.scale.x * 0.9f);
        this->actor.shape.rot.y = (this->actor.shape.rot.y - (this->unk2D6 * 0x28F)) + 0x3D68;
    } else {
        EnTr_SetupAction(this, func_80B23820);
        this->actor.draw = NULL;
    }
    if (this->unk2D6 == 4) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_BUBLE_DOWN);
    }
    if (this->unk2D6 > 0) {
        this->unk2D6--;
    }
}

void func_80B23690(EnTr* this, GlobalContext* globalCtx) {
    s32 temp_hi;

    if (this->unk2D6 >= 0x1F) {
        temp_hi = (this->unk2D6 * 2) % 7;
        func_80B23254(this, globalCtx, temp_hi, 5.0f, 1.0f);
        func_80B23254(this, globalCtx, (temp_hi + 1) % 7, 5.0f, 1.0f);
    } else if (this->unk2D6 == 0x1E) {
        this->actor.draw = EnTr_Draw;
        this->actor.shape.rot.y += this->unk2D6 * 0x1A6;
    } else if (this->unk2D6 > 0) {
        this->actor.shape.rot.y += this->unk2D6 * 0x1A6;
        Actor_SetScale(&this->actor, (this->actor.scale.x * 0.8f) + 0.002f);
    } else {
        EnTr_SetupAction(this, func_80B22F28);
        Actor_SetScale(&this->actor, 0.01f);
    }
    if (this->unk2D6 > 0) {
        this->unk2D6--;
    }
    func_8002F974(&this->actor, NA_SE_EN_TWINROBA_FLY_DEMO - SFX_FLAG);
}

void func_80B23820(EnTr* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;

    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v0 = globalCtx->csCtx.npcActions[this->unk2D8];
        if ((temp_v0 != NULL) && (((temp_v0->action == 3)) || (temp_v0->action == 5))) {
            Audio_PlayActorSound2(&this->actor, 0x390DU);
            this->unk2D6 = 0x22;
            func_80B242B4(this, globalCtx, this->unk2D8);
            EnTr_SetupAction(this, func_80B23690);
            Animation_PlayLoop(&this->unk14C, &D_60049C8);
            this->unk2E4 = NULL;
            Actor_SetScale(&this->actor, 0.003f);
        }
    }
}

void func_80B238E0(EnTr* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;
    f32 temp_fv0;

    temp_fv0 = Animation_GetLastFrame(D_80B24378[this->actor.params]);
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v0 = globalCtx->csCtx.npcActions[this->unk2D8];
        if ((temp_v0 != NULL) && (temp_v0->action == 3)) {
            Animation_Change(&this->unk14C, D_80B24378[this->actor.params], 1.0f, 0.0f, temp_fv0, 0U, -10.0f);
            this->unk2E4 = NULL;
            EnTr_SetupAction(this, func_80B22F28);
        }
    }
}

void func_80B239A8(EnTr* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;
    f32 temp_fv0;

    temp_fv0 = Animation_GetLastFrame(D_80B24368[this->actor.params]);
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v0 = globalCtx->csCtx.npcActions[this->unk2D8];
        if ((temp_v0 != NULL) && (temp_v0->action == 2)) {
            Animation_Change(&this->unk14C, D_80B24368[this->actor.params], 1.0f, 0.0f, temp_fv0, 2U, -4.0f);
            this->unk2E4 = D_80B24370[this->actor.params];
            EnTr_SetupAction(this, func_80B238E0);
        }
    }
}

void func_80B23A88(EnTr* this, GlobalContext* globalCtx) {
    u32 temp_a3;
    CsCmdActorAction* temp_v1;

    temp_a3 = globalCtx->gameplayFrames;
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        temp_v1 = globalCtx->csCtx.npcActions[this->unk2D8];
        if (temp_v1 != NULL) {
            switch (temp_v1->action) {
                case 1:
                    func_80B242B4(this, globalCtx, this->unk2D8);
                    EnTr_SetupAction(this, func_80B239A8);
                    break;

                case 3:
                    func_80B242B4(this, globalCtx, this->unk2D8);
                    EnTr_SetupAction(this, func_80B22F28);
                    Animation_PlayLoop(&this->unk14C, &D_60049C8);
                    this->unk2E4 = NULL;
                    break;

                case 4:
                    EnTr_SetupAction(this, func_80B23820);
                    this->actor.draw = NULL;
                    break;

                case 7:
                    EnTr_SetupAction(this, func_80B230D8);
                    Animation_PlayLoop(&this->unk14C, &D_60049C8);
                    this->unk2E4 = NULL;
                    if (this->actor.params != 0) {
                        this->unk2D6 = ((u8)temp_a3 * 0x400) + 0x8000;
                    } else {
                        this->unk2D6 = (u8)temp_a3 * 0x400;
                    }
                    break;
            }
        }
    }
}

void EnTr_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnTr* this = (EnTr*)thisx;
    s32 pad;

    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 0.0f, 0.0f, 0.0f, 5);
    this->actionFunc(this, globalCtx);
    if (SkelAnime_Update(&this->unk14C)) {
        if (this->unk2E4 != NULL) {
            if ((this->unk2E4 == &D_60035CC) || (this->unk2E4 == &D_60013CC)) {
                if (this->actor.params != 0) {
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_TWINROBA_LAUGH2);
                } else {
                    Audio_PlayActorSound2(&this->actor, NA_SE_EN_TWINROBA_LAUGH);
                }
                Animation_PlayLoop(&this->unk14C, this->unk2E4);
            } else if (this->unk2E4 == &D_60049C8) {
                EnTr_SetupAction(this, func_80B22F28);
                Animation_Change(&this->unk14C, &D_60049C8, 1.0f, 0.0f, Animation_GetLastFrame(&D_60049C8),
                                 ANIMMODE_LOOP, -5.0f);
            } else {
                Animation_PlayLoop(&this->unk14C, this->unk2E4);
            }
            this->unk2E4 = NULL;
        } else {
            this->unk14C.curFrame = 0.0f;
        }
    }
    Actor_SetFocus(&this->actor, 0.0f);
    if (DECR(this->unk2E2) == 0) {
        this->unk2E2 = Rand_S16Offset(0x3C, 0x3C);
    }
    this->unk2E0 = this->unk2E2;
    if (this->unk2E0 >= 3) {
        this->unk2E0 = 0;
    }
}

s32 EnTr_OverrideLimbDraw(GlobalContext* globalCtx, s32 arg1, Gfx** arg2, Vec3f* arg3, Vec3s* arg4, void* thisx) {
    Vec3f sp34;
    Vec3f sp28;
    EnTr* this = thisx;
    Actor* sp20;

    sp34 = D_80B243DC;
    sp28 = D_80B243E8;
    sp20 = this->actor.child;
    if ((sp20 != NULL) && (arg1 == 0x13)) {
        Matrix_MultVec3f(&sp34, &sp28);
        sp28.x -= 10.0f * Math_SinS(Camera_GetCamDirYaw(globalCtx->cameraPtrs[globalCtx->activeCamera]));
        sp28.z -= 10.0f * Math_CosS(Camera_GetCamDirYaw(globalCtx->cameraPtrs[globalCtx->activeCamera]));
        sp20->world.pos = sp28;
    }
    return 0;
}

void EnTr_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnTr* this = (EnTr*)thisx;
    s32 pad;

    if ((globalCtx->csCtx.state == CS_STATE_IDLE) || (globalCtx->csCtx.npcActions[this->unk2D8] == NULL)) {
        this->actor.shape.shadowDraw = NULL;
    } else {
        if (1) {}
        this->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_tr.c", 840);
        func_800943C8(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80B243D0[this->unk2E0]));
        func_8002EBCC(&this->actor, globalCtx, 0);
        SkelAnime_DrawFlexOpa(globalCtx, this->unk14C.skeleton, this->unk14C.jointTable, this->unk14C.dListCount,
                              EnTr_OverrideLimbDraw, NULL, this);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_tr.c", 854);
    }
}

f32 func_80B23FDC(GlobalContext* arg0, s32 arg1) {
    CsCmdActorAction* temp_v0;
    f32 temp_fv0;
    f32 var_fv1;

    temp_v0 = arg0->csCtx.npcActions[arg1];
    temp_fv0 = func_8006F93C(temp_v0->endFrame, temp_v0->startFrame, arg0->csCtx.frames);
    var_fv1 = temp_fv0;
    if (temp_fv0 > 1.0f) {
        var_fv1 = 1.0f;
    }
    return var_fv1;
}

void func_80B24038(EnTr* this, GlobalContext* globalCtx, s32 arg2) {
    Vec3f sp34;
    Vec3f sp28;
    f32 temp_fv0;
    f32 var_fa0;
    CsCmdActorAction* v;

    v = globalCtx->csCtx.npcActions[arg2];
    sp34.x = v->startPos.x;
    sp34.y = v->startPos.y;
    sp34.z = v->startPos.z;
    sp28.x = v->endPos.x;
    sp28.y = v->endPos.y;
    sp28.z = v->endPos.z;
    temp_fv0 = func_80B23FDC(globalCtx, (s32)arg2);
    sp34.x = ((sp28.x - sp34.x) * temp_fv0) + sp34.x;
    sp34.y = ((sp28.y - sp34.y) * temp_fv0) + sp34.y;
    sp34.z = ((sp28.z - sp34.z) * temp_fv0) + sp34.z;
    sp28.x = (sp34.x - this->actor.world.pos.x) * 0.1f;
    sp28.y = (sp34.y - this->actor.world.pos.y) * 0.1f;
    sp28.z = (sp34.z - this->actor.world.pos.z) * 0.1f;
    temp_fv0 = sqrtf(SQXYZ(sp28));
    var_fa0 = CLAMP(temp_fv0, 0.0f, 20.0f);
    if ((temp_fv0 != var_fa0) && (temp_fv0 != 0.0f)) {
        sp28.x *= var_fa0 / temp_fv0;
        sp28.y *= var_fa0 / temp_fv0;
        sp28.z *= var_fa0 / temp_fv0;
    }
    Math_StepToF(&this->actor.velocity.x, sp28.x, 1.0f);
    Math_StepToF(&this->actor.velocity.y, sp28.y, 1.0f);
    Math_StepToF(&this->actor.velocity.z, sp28.z, 1.0f);
    func_8002D7EC(&this->actor);
}

void func_80B24230(EnTr* this, GlobalContext* globalCtx, s32 arg2) {
    s32 var_v0;
    s16 new_var;
    s32 var_v1;

    new_var = globalCtx->csCtx.npcActions[arg2]->rot.y;
    var_v1 = this->actor.world.rot.y - new_var;
    if (var_v1 < 0) {
        var_v1 = -var_v1;
        var_v0 = 1;
    } else {
        var_v0 = -1;
    }
    if (var_v1 >= 0x8000) {
        var_v0 = -var_v0;
        var_v1 = 0x10000 - var_v1;
    }
    var_v1 *= 0.1f;
    this->actor.world.rot.y += var_v1 * var_v0;
    this->actor.shape.rot.y = this->actor.world.rot.y;
}

void func_80B242B4(EnTr* this, GlobalContext* globalCtx, s32 arg2) {
    Vec3f spC;
    s16 temp_v1;

    spC.x = globalCtx->csCtx.npcActions[arg2]->startPos.x;
    spC.y = globalCtx->csCtx.npcActions[arg2]->startPos.y;
    spC.z = globalCtx->csCtx.npcActions[arg2]->startPos.z;
    this->actor.world.pos = spC;
    temp_v1 = globalCtx->csCtx.npcActions[arg2]->rot.y;
    this->actor.shape.rot.y = temp_v1;
    this->actor.world.rot.y = temp_v1;
}
