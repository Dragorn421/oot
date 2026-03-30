#include "z_bg_toki_hikari.h"

#define FLAGS 0x00000020

#define THIS ((BgTokiHikari*)thisx)

void BgTokiHikari_Init(Actor* thisx, GlobalContext* globalCtx);
void BgTokiHikari_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgTokiHikari_Update(Actor* thisx, GlobalContext* globalCtx);
void BgTokiHikari_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808B9F98(BgTokiHikari* this, GlobalContext* globalCtx);
void func_808BA018(BgTokiHikari* this, GlobalContext* globalCtx);
void func_808BA204(BgTokiHikari* this, GlobalContext* globalCtx);
void func_808BA22C(BgTokiHikari* this, GlobalContext* globalCtx);
void func_808BA274(BgTokiHikari* this, GlobalContext* globalCtx);
void func_808BA2CC(BgTokiHikari* this, GlobalContext* globalCtx);

extern Gfx D_6000880[];
extern Gfx D_60009C0[];
extern Gfx D_6000A10[];
extern Gfx D_6007E20[];
extern Gfx D_6007EE0[];
extern Gfx D_6008190[];

const ActorInit Bg_Toki_Hikari_InitVars = {
    ACTOR_BG_TOKI_HIKARI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_TOKI_OBJECTS,
    sizeof(BgTokiHikari),
    (ActorFunc)BgTokiHikari_Init,
    (ActorFunc)BgTokiHikari_Destroy,
    (ActorFunc)BgTokiHikari_Update,
    (ActorFunc)BgTokiHikari_Draw,
};

static InitChainEntry D_808BAC70[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgTokiHikari_Init(Actor* thisx, GlobalContext* globalCtx) {
    s16 temp_v0;
    BgTokiHikari* this = (BgTokiHikari*)thisx;

    temp_v0 = this->actor.params;
    switch (temp_v0) { /* irregular */
        case 0:
            Actor_ProcessInitChain(&this->actor, D_808BAC70);
            this->unk150 = func_808B9F98;
            return;
        case 1:
            if (!(gSaveContext.eventChkInf[4] & 0x800)) {
                this->unk150 = func_808BA204;
                this->unk14C = 0.0f;
                return;
            }
            Actor_Kill(&this->actor);
            return;
    }
}

void BgTokiHikari_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgTokiHikari* this = (BgTokiHikari*)thisx;
}

void func_808B9F98(BgTokiHikari* this, GlobalContext* globalCtx) {
}

void BgTokiHikari_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgTokiHikari* this = (BgTokiHikari*)thisx;
    this->unk150(this, globalCtx);
}

void BgTokiHikari_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s16 temp_v0;
    BgTokiHikari* this = (BgTokiHikari*)thisx;

    temp_v0 = this->actor.params;
    switch (temp_v0) { /* irregular */
        case 0:
            func_808BA018(this, globalCtx);
            return;
        case 1:
            func_808BA2CC(this, globalCtx);
            return;
    }
}

void func_808BA018(BgTokiHikari* this, GlobalContext* globalCtx) {
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0xF6);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0xFC),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (gSaveContext.linkAge == 0) {
        gSPDisplayList(POLY_OPA_DISP++, D_6008190);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, D_6007E20);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(POLY_XLU_DISP++, 8,
                   Gfx_TexScroll(globalCtx->state.gfxCtx, 0U, globalCtx->gameplayFrames & 0x7F, 0x40, 0x20));
        gSPSegment(POLY_XLU_DISP++, 9,
                   Gfx_TexScroll(globalCtx->state.gfxCtx, 0U, globalCtx->gameplayFrames & 0x7F, 0x40, 0x20));
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x116),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, D_6007EE0);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x11C);
}

void func_808BA204(BgTokiHikari* this, GlobalContext* globalCtx) {
    if (globalCtx->unk_11D30[1] != 0) {
        this->unk150 = func_808BA22C;
    }
}

void func_808BA22C(BgTokiHikari* this, GlobalContext* globalCtx) {
    f32 temp_fv0;

    temp_fv0 = this->unk14C;
    if (temp_fv0 < 1.0f) {
        this->unk14C = temp_fv0 + 0.05f;
        return;
    }
    this->unk14C = 1.0f;
    this->unk150 = func_808BA274;
}

void func_808BA274(BgTokiHikari* this, GlobalContext* globalCtx) {
    if (this->unk14C > 0.2f) {
        this->unk14C -= 0.025f;
        return;
    }
    this->unk14C = 0.0f;
    Actor_Kill(&this->actor);
}

void func_808BA2CC(BgTokiHikari* this, GlobalContext* globalCtx) {
    s32 pad[2];

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x15E);
    Matrix_Translate(0.0f, 276.0f, 1122.0f, 0U);
    Matrix_Scale(0.32f, 0.32f, this->unk14C * 7.0f, 1U);
    Matrix_RotateZ(3.1415927f, 1U);
    func_80093D18(globalCtx->state.gfxCtx);
    Matrix_Push();
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, (u8)(this->unk14C * 255.0f), (u8)(155.0f * this->unk14C) + 100,
                    (u8)(this->unk14C * 255.0f), (u8)(this->unk14C * 255.0f));
    gDPSetEnvColor(POLY_XLU_DISP++, (u8)(this->unk14C * 155.0f) + 100, (u8)(255.0f * this->unk14C), 0, 128);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x17E),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 8,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (globalCtx->gameplayFrames & 0x7F) * -2, 0U, 0x20, 0x40, 1,
                                (globalCtx->gameplayFrames & 0x7F) * 4, 0U, 0x20, 0x40));
    gSPDisplayList(POLY_XLU_DISP++, D_6000880);
    Matrix_Pop();
    Matrix_Push();
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 255, 255, 255, (u8)(this->unk14C * 200.0f));
    gDPSetEnvColor(POLY_XLU_DISP++, (u8)(this->unk14C * 255.0f), (u8)(this->unk14C * 255.0f),
                   (u8)(this->unk14C * 255.0f), (u8)(200.0f * this->unk14C));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x19F),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, D_60009C0);
    Matrix_Pop();
    Matrix_Push();
    gDPPipeSync(POLY_XLU_DISP++);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 255, 255, 255, (u8)(this->unk14C * 200.0f));
    gDPSetEnvColor(POLY_XLU_DISP++, (u8)(this->unk14C * 255.0f), (u8)(this->unk14C * 255.0f),
                   (u8)(this->unk14C * 255.0f), (u8)(200.0f * this->unk14C));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x1B5),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, D_6000A10);
    Matrix_Pop();
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_toki_hikari.c", 0x1BB);
}
