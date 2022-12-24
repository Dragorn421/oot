/*
 * File: z_bg_pushbox.c
 * Overlay: ovl_Bg_Pushbox
 * Description: Unused (and non functional) pushable block
 */

#include "z_bg_pushbox.h"
#include "assets/objects/object_pu_box/object_pu_box.h"

#define FLAGS 0

void BgPushbox_Init(Actor* thisx, PlayState* play);
void BgPushbox_Destroy(Actor* thisx, PlayState* play);
void BgPushbox_Update(Actor* thisx, PlayState* play);
void BgPushbox_Draw(Actor* thisx, PlayState* play);

void BgPushbox_UpdateImpl(BgPushbox* this, PlayState* play);

ActorInit Bg_Pushbox_InitVars = {
    ACTOR_BG_PUSHBOX,
    ACTORCAT_BG,
    FLAGS,
    //! @bug fixing this actor would involve using OBJECT_PU_BOX
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(BgPushbox),
    (ActorFunc)BgPushbox_Init,
    (ActorFunc)BgPushbox_Destroy,
    (ActorFunc)BgPushbox_Update,
    (ActorFunc)BgPushbox_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_STOP),
};

void BgPushbox_SetupAction(BgPushbox* this, BgPushboxActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void BgPushbox_Init(Actor* thisx, PlayState* play) {
    BgPushbox* this = (BgPushbox*)thisx;
    s32 pad;
    CollisionHeader* colHeader = NULL;
    s32 pad2;

    Actor_ProcessInitChain(thisx, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gBlockSmallCol, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    BgPushbox_SetupAction(this, BgPushbox_UpdateImpl);
}

void BgPushbox_Destroy(Actor* thisx, PlayState* play) {
    BgPushbox* this = (BgPushbox*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void BgPushbox_UpdateImpl(BgPushbox* this, PlayState* play) {
    Actor* thisx = &this->dyna.actor;

    thisx->speed += this->dyna.unk_150 * 0.2f;
    thisx->speed = (thisx->speed < -1.0f) ? -1.0f : ((thisx->speed > 1.0f) ? 1.0f : thisx->speed);
    Math_StepToF(&thisx->speed, 0.0f, 0.2f);
    thisx->world.rot.y = this->dyna.unk_158;
    Actor_MoveForward(thisx);
    Actor_UpdateBgCheckInfo(play, thisx, 20.0f, 40.0f, 40.0f,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_3 |
                                UPDBGCHECKINFO_FLAG_4);
}

void BgPushbox_Update(Actor* thisx, PlayState* play) {
    BgPushbox* this = (BgPushbox*)thisx;

    this->actionFunc(this, play);
    func_8002DF90(this);
}

void BgPushbox_Draw(Actor* thisx, PlayState* play) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx, "../z_bg_pushbox.c", 263);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_bg_pushbox.c", 269),
              G_MTX_NOPUSH | G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(POLY_OPA_DISP++, gBlockSmallDL);

    CLOSE_DISPS(play->state.gfxCtx, "../z_bg_pushbox.c", 272);
}
