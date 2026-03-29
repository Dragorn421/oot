/*
 * File: z_en_ge3.c
 * Overlay: ovl_En_Ge3
 * Description: Gerudo giving you membership card
 */

#include "z_en_ge3.h"

#define FLAGS 0x00000019

#define THIS ((EnGe3*)thisx)

void EnGe3_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGe3_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A34A80(EnGe3* this, GlobalContext* globalCtx);
void func_80A34B90(EnGe3* this, GlobalContext* globalCtx);
void func_80A34D68(Actor* thisx, GlobalContext* globalCtx);

extern FlexSkeletonHeader D_600A458;
extern AnimationHeader D_600B07C;

const ActorInit En_Ge3_InitVars = {
    ACTOR_EN_GE3,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GELDB,
    sizeof(EnGe3),
    (ActorFunc)EnGe3_Init,
    (ActorFunc)EnGe3_Destroy,
    (ActorFunc)EnGe3_Update,
    (ActorFunc)EnGe3_Draw,
};

static ColliderCylinderInit D_80A35190 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000722, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

static void (*D_80A351BC[1])(EnGe3*, GlobalContext*) = { func_80A34A80 };
static AnimationHeader* D_80A351C0[1] = { &D_600B07C };
static u8 D_80A351C4[1] = { ANIMMODE_LOOP };
static Vec3f D_80A351C8 = { 600.0f, 700.0f, 0.0f };
static s32 D_80A351D4[3] = { 0x06005FE8, 0x060065A8, 0x06006D28 };

void func_80A34620(EnGe3* this, s32 arg1) {
    this->unk310 = D_80A351BC[arg1];
    Animation_Change(&this->unk198, D_80A351C0[arg1], 1.0f, 0.0f, Animation_GetLastFrame(D_80A351C0[arg1]),
                     D_80A351C4[arg1], -8.0f);
    this->unk30C &= ~2;
}

void EnGe3_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* this = (EnGe3*)thisx;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(globalCtx, &this->unk198, &D_600A458, NULL, this->unk1DC, this->unk26C, 24);
    Animation_PlayLoop(&this->unk198, &D_600B07C);
    Collider_InitCylinder(globalCtx, &this->unk14C);
    Collider_SetCylinder(globalCtx, &this->unk14C, &this->actor, &D_80A35190);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.world.rot.z = 0;
    this->actor.shape.rot.z = 0;
    func_80A34620(this, 0);
    this->unk310 = func_80A34B90;
    this->unk30C = 0;
    this->actor.targetMode = 6;
    this->actor.minVelocityY = -4.0f;
    this->actor.gravity = -1.0f;
}

void EnGe3_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* this = (EnGe3*)thisx;

    Collider_DestroyCylinder(globalCtx, &this->unk14C);
}

void func_80A347F4(EnGe3* this, GlobalContext* globalCtx) {
    s16 temp_v0;
    s32 pad;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if (ABS(temp_v0) < 0x4001) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 6, 0xFA0, 0x64);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        func_80038290(globalCtx, &this->actor, &this->unk300, &this->unk306, this->actor.focus.pos);
    } else {
        if (temp_v0 < 0) {
            Math_SmoothStepToS(&this->unk300.y, -0x2000, 6, 0x1838, 0x100);
        } else {
            Math_SmoothStepToS(&this->unk300.y, 0x2000, 6, 0x1838, 0x100);
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xC, 0x3E8, 0x64);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }
}

void func_80A3490C(EnGe3* this, GlobalContext* globalCtx) {
    s16 temp_v0;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if ((ABS(temp_v0) < 0x2301) && (this->actor.xzDistToPlayer < 100.0f)) {
        func_80038290(globalCtx, &this->actor, &this->unk300, &this->unk306, this->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&this->unk300.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->unk300.y, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->unk306.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->unk306.y, 0, 6, 0x1838, 0x64);
    }
}

void func_80A34A20(EnGe3* this, GlobalContext* globalCtx) {
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->unk310 = func_80A34A80;
        this->actor.update = func_80A34D68;
        this->actor.flags &= ~0x10000;
    }
    func_80A347F4(this, globalCtx);
}

void func_80A34A80(EnGe3* this, GlobalContext* globalCtx) {
    func_80A3490C(this, globalCtx);
}

void func_80A34AA0(EnGe3* this, GlobalContext* globalCtx) {
    if (Actor_HasParent(&this->actor, globalCtx) != 0) {
        this->actor.parent = NULL;
        this->unk310 = func_80A34A20;
    } else {
        func_8002F434(&this->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void func_80A34B00(EnGe3* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->actor.flags &= ~0x10000;
        this->unk310 = func_80A34AA0;
        func_8002F434(&this->actor, globalCtx, GI_GERUDO_CARD, 10000.0f, 50.0f);
    }
}

void func_80A34B90(EnGe3* this, GlobalContext* globalCtx) {
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk310 = func_80A34B00;
    } else {
        if (!(this->unk30C & 4)) {
            func_8002DF54(globalCtx, &this->actor, 7U);
            this->unk30C |= 4;
        }
        this->actor.textId = 0x6004;
        this->actor.flags |= 0x10000;
        func_8002F1C4(&this->actor, globalCtx, 300.0f, 300.0f, 0U);
    }
    func_80A3490C(this, globalCtx);
}

void func_80A34C40(EnGe3* this, GlobalContext* globalCtx) {
    s32 pad[2];

    Collider_UpdateCylinder(&this->actor, &this->unk14C);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk14C.base);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 40.0f, 25.0f, 40.0f, 5);
    if (!(this->unk30C & 2) && SkelAnime_Update(&this->unk198)) {
        this->unk30C |= 2;
    }
}

void func_80A34CE4(EnGe3* this, GlobalContext* globalCtx) {
    Actor_MoveForward(&this->actor);
    if (DECR(this->unk2FE) == 0) {
        this->unk2FE = Rand_S16Offset(0x3C, 0x3C);
    }
    this->unk2FC = this->unk2FE;
    if (this->unk2FC >= 3) {
        this->unk2FC = 0;
    }
}

void func_80A34D68(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* this = (EnGe3*)thisx;

    func_80A34C40(this, globalCtx);
    this->unk310(this, globalCtx);
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk310 = func_80A34A20;
        this->actor.update = EnGe3_Update;
    } else {
        this->actor.textId = 0x6005;
        if (this->actor.xzDistToPlayer < 100.0f) {
            func_8002F2CC(&this->actor, globalCtx, 100.0f);
        }
    }
    func_80A34CE4(this, globalCtx);
}

void EnGe3_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnGe3* this = (EnGe3*)thisx;

    func_80A34C40(this, globalCtx);
    this->unk310(this, globalCtx);
    func_80A34CE4(this, globalCtx);
}

s32 func_80A34E58(GlobalContext* globalCtx, s32 arg1, Gfx** arg2, Vec3f* arg3, Vec3s* arg4, void* thisx) {
    EnGe3* this = thisx;

    switch (arg1) {
        case 5:
        case 0xB:
        case 0x10:
            *arg2 = NULL;
            return 0;

        case 6:
            arg4->x += this->unk300.y;
        default:
            OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 547);
            switch (arg1) {
                case 3:
                    break;

                case 6:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 80, 60, 10, 255);
                    break;

                case 11:
                case 16:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 140, 170, 230, 255);
                    gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x00, 255, 255, 255, 255);
                    break;

                default:
                    gDPPipeSync(POLY_OPA_DISP++);
                    gDPSetEnvColor(POLY_OPA_DISP++, 140, 0, 0, 255);
                    break;
            }
            CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 566);
            break;
    }
    return 0;
}

void func_80A35004(GlobalContext* globalCtx, s32 arg1, Gfx** arg2, Vec3s* arg3, void* thisx) {
    EnGe3* this = thisx;
    Vec3f sp18 = D_80A351C8;

    if (arg1 == 6) {
        Matrix_MultVec3f(&sp18, &this->actor.focus.pos);
    }
}

void EnGe3_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 temp_a0;
    EnGe3* this = (EnGe3*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 614);
    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80A351D4[this->unk2FC]));
    func_8002EBCC(&this->actor, globalCtx, 0);
    SkelAnime_DrawFlexOpa(globalCtx, this->unk198.skeleton, this->unk198.jointTable, this->unk198.dListCount,
                          func_80A34E58, func_80A35004, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge3.c", 631);
}
