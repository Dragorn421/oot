/*
 * File: z_bg_ddan_jd.c
 * Overlay: ovl_Bg_Ddan_Jd
 * Description: Rising stone platform (Dodongo's Cavern)
 */

#include "z_bg_ddan_jd.h"

#define FLAGS 0x00000030

#define THIS ((BgDdanJd*)thisx)

void BgDdanJd_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Update(Actor* thisx, GlobalContext* globalCtx);
void BgDdanJd_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgDdanJd_Idle(BgDdanJd* this, GlobalContext* globalCtx);
void func_80870D2C(BgDdanJd* this, GlobalContext* globalCtx);
void BgDdanJd_Move(BgDdanJd* this, GlobalContext* globalCtx);

extern Gfx D_060037B8[];
extern UNK_TYPE D_06003CE0;

const ActorInit Bg_Ddan_Jd_InitVars = {
    ACTOR_BG_DDAN_JD,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_DDAN_OBJECTS,
    sizeof(BgDdanJd),
    (ActorFunc)BgDdanJd_Init,
    (ActorFunc)BgDdanJd_Destroy,
    (ActorFunc)BgDdanJd_Update,
    (ActorFunc)BgDdanJd_Draw,
};

static InitChainEntry D_80871080[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgDdanJd_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* this = (BgDdanJd*)thisx;
    s32 pad;
    s32 sp24;

    sp24 = 0;
    Actor_ProcessInitChain(&this->dyna.actor, D_80871080);
    DynaPolyInfo_SetActorMove(&this->dyna, DPM_PLAYER);
    DynaPolyInfo_Alloc(&D_06003CE0, &sp24);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp24);
    this->timer = 100;
    this->state = 0;
    if (Flags_GetSwitch(globalCtx, this->dyna.actor.params)) {
        this->yVelocity = 5;
    } else {
        this->yVelocity = 1;
    }
    this->actionFunc = BgDdanJd_Idle;
}

void BgDdanJd_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* this = (BgDdanJd*)thisx;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
}

void BgDdanJd_Idle(BgDdanJd* this, GlobalContext* globalCtx) {
    if (this->timer != 0) {
        this->timer--;
    }
    if ((this->yVelocity == 1) && (this->dyna.actor.params < 0x40) &&
        Flags_GetSwitch(globalCtx, this->dyna.actor.params)) {
        this->yVelocity = 5;
        this->state = 1;
        this->timer = 0;
        this->dyna.actor.posRot.pos.y = this->dyna.actor.initPosRot.pos.y + 140.0f;
        func_800800F8(globalCtx, 0xBF4, -0x63, &this->dyna.actor, 0);
    }
    if (this->timer == 0) {
        this->timer = 100;
        if (this->state == 0) {
            this->state = 1;
            this->targetPosY = this->dyna.actor.initPosRot.pos.y + 140.0f;
        } else if (this->state == 1) {
            if (this->yVelocity != 1) {
                this->state = 3;
                this->targetPosY = this->dyna.actor.initPosRot.pos.y + 700.0f;
            } else {
                this->state = 0;
                this->targetPosY = this->dyna.actor.initPosRot.pos.y;
            }
        } else if (this->state == 2) {
            if (this->yVelocity != 1) {
                this->state = 3;
                this->targetPosY = this->dyna.actor.initPosRot.pos.y + 700.0f;
            } else {
                this->state = 0;
                this->targetPosY = this->dyna.actor.initPosRot.pos.y;
            }
        } else if (this->state == 3) {
            this->state = 2;
            this->targetPosY = this->dyna.actor.initPosRot.pos.y + 140.0f;
        }
        this->actionFunc = BgDdanJd_Move;
    }
}

void func_80870D2C(BgDdanJd* this, GlobalContext* globalCtx) {
    Vec3f sp34;

    sp34.y = this->dyna.actor.initPosRot.pos.y;
    if (globalCtx->gameplayFrames & 1) {
        sp34.x = this->dyna.actor.posRot.pos.x + 65.0f;
        sp34.z = Rand_CenteredFloat(110.0f) + this->dyna.actor.posRot.pos.z;
        func_80033480(globalCtx, &sp34, 5.0f, 1, 0x14, 0x3C, 1U);
        sp34.x = this->dyna.actor.posRot.pos.x - 65.0f;
        sp34.z = Rand_CenteredFloat(110.0f) + this->dyna.actor.posRot.pos.z;
        func_80033480(globalCtx, &sp34, 5.0f, 1, 0x14, 0x3C, 1U);
    } else {
        sp34.x = Rand_CenteredFloat(110.0f) + this->dyna.actor.posRot.pos.x;
        sp34.z = this->dyna.actor.posRot.pos.z + 65.0f;
        func_80033480(globalCtx, &sp34, 5.0f, 1, 0x14, 0x3C, 1U);
        sp34.x = Rand_CenteredFloat(110.0f) + this->dyna.actor.posRot.pos.x;
        sp34.z = this->dyna.actor.posRot.pos.z - 65.0f;
        func_80033480(globalCtx, &sp34, 5.0f, 1, 0x14, 0x3C, 1U);
    }
    if (this->yVelocity == 5) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
    }
}

void BgDdanJd_Move(BgDdanJd* this, GlobalContext* globalCtx) {
    if ((this->yVelocity == 1) && (this->dyna.actor.params < 0x40) &&
        Flags_GetSwitch(globalCtx, this->dyna.actor.params)) {
        this->yVelocity = 5;
        this->state = 1;
        this->dyna.actor.posRot.pos.y = this->dyna.actor.initPosRot.pos.y + 140.0f;
        this->timer = 0;
        this->actionFunc = BgDdanJd_Idle;
        func_800800F8(globalCtx, 0xBF4, -0x63, &this->dyna.actor, 0);
    } else {
        if (Math_StepToF(&this->dyna.actor.posRot.pos.y, this->targetPosY, this->yVelocity)) {
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_PILLAR_MOVE_STOP);
            this->actionFunc = BgDdanJd_Idle;
        }
    }
    func_80870D2C(this, globalCtx);
}

void BgDdanJd_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* this = (BgDdanJd*)thisx;

    this->actionFunc(this, globalCtx);
}

void BgDdanJd_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgDdanJd* this = (BgDdanJd*)thisx;

    Gfx_DrawDListOpa(globalCtx, D_060037B8);
}
