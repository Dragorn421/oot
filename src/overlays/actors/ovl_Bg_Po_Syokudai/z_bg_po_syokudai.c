#include "z_bg_po_syokudai.h"

#define FLAGS 0x00000000

#define THIS ((BgPoSyokudai*)thisx)

void BgPoSyokudai_Init(Actor* thisx, GlobalContext* globalCtx);
void BgPoSyokudai_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgPoSyokudai_Update(Actor* thisx, GlobalContext* globalCtx);
void BgPoSyokudai_Draw(Actor* thisx, GlobalContext* globalCtx);

extern Gfx D_404D4E0[];
extern Gfx D_60003A0[];

static ColliderCylinderSrc D_808A8960 = {
    { 9, 0, 0xD, 0x39, 0x20, 1 },
    { 0, { 0, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 0xC, 0x3C, 0, { 0, 0, 0 } },
};
static Color_RGBA8 D_808A898C[4] = {
    { 0xFF, 0xAA, 0xFF, 0xFF },
    { 0xFF, 0xC8, 0x00, 0xFF },
    { 0x00, 0xAA, 0xFF, 0xFF },
    { 0xAA, 0xFF, 0x00, 0xFF },
};
static Color_RGBA8 D_808A899C[4] = {
    { 0x64, 0x00, 0xFF, 0xFF },
    { 0xFF, 0x00, 0x00, 0xFF },
    { 0x00, 0x00, 0xFF, 0xFF },
    { 0x00, 0x96, 0x00, 0xFF },
};

const ActorInit Bg_Po_Syokudai_InitVars = {
    ACTOR_BG_PO_SYOKUDAI,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_SYOKUDAI,
    sizeof(BgPoSyokudai),
    (ActorFunc)BgPoSyokudai_Init,
    (ActorFunc)BgPoSyokudai_Destroy,
    (ActorFunc)BgPoSyokudai_Update,
    (ActorFunc)BgPoSyokudai_Draw,
};

static InitChainEntry D_808A89CC[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_STOP),
};

void BgPoSyokudai_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgPoSyokudai* this = (BgPoSyokudai*)thisx;
    s32 pad;

    Actor_ProcessInitChain(&this->actor, D_808A89CC);
    this->unk14C = (thisx->params >> 8) & 0xFF;
    this->actor.params &= 0x3F;
    this->actor.collideData.mass = MASS_IMMOVABLE;
    this->unk150 = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &this->unk154);
    Lights_PointGlowSetInfo(&this->unk154, this->actor.posRot.pos.x, (s16)this->actor.posRot.pos.y + 65,
                            this->actor.posRot.pos.z, 0, 0, 0, 0);
    Collider_InitCylinder(globalCtx, &this->unk164);
    Collider_LoadCylinder(globalCtx, &this->unk164, &this->actor, &D_808A8960);
    this->unk164.shape.pos.x = this->actor.posRot.pos.x;
    this->unk164.shape.pos.y = this->actor.posRot.pos.y;
    this->unk164.shape.pos.z = this->actor.posRot.pos.z;
    if ((this->unk14C == 0) && Flags_GetSwitch(globalCtx, 0x1F) && Flags_GetSwitch(globalCtx, 0x1E) &&
        Flags_GetSwitch(globalCtx, 0x1D) && !Flags_GetSwitch(globalCtx, this->actor.params)) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_PO_SISTERS, 119.0f, 225.0f, -1566.0f, 0, 0, 0,
                    this->actor.params);
        globalCtx->envCtx.unk_BF = 4;
    } else if (!Flags_GetSwitch(globalCtx, 0x1C) && !Flags_GetSwitch(globalCtx, 0x1B)) {
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_PO_SISTERS, this->actor.posRot.pos.x,
                    this->actor.posRot.pos.y + 52.0f, this->actor.posRot.pos.z, 0, 0, 0,
                    (this->unk14C << 8) + this->actor.params + 0x1000);
    } else if (!Flags_GetSwitch(globalCtx, this->actor.params) && (globalCtx->envCtx.unk_BF == 0xFF)) {
        globalCtx->envCtx.unk_BF = 4;
    }
    this->unk14E = Math_Rand_ZeroOne() * 20.0f;
}

void BgPoSyokudai_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgPoSyokudai* this = (BgPoSyokudai*)thisx;

    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, this->unk150);
    Collider_DestroyCylinder(globalCtx, &this->unk164);
    if (globalCtx->envCtx.unk_BF != 0xFF) {
        globalCtx->envCtx.unk_BF = 0xFF;
    }
}

void BgPoSyokudai_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgPoSyokudai* this = (BgPoSyokudai*)thisx;
    s32 pad;

    Collider_AddAC(globalCtx, &globalCtx->colliderCtx, &this->unk164.base);
    Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &this->unk164.base);
    if (Flags_GetSwitch(globalCtx, this->actor.params)) {
        func_8002F974(&this->actor, NA_SE_EV_TORCH - SFX_FLAG);
    }
    this->unk14E += 1;
}

void BgPoSyokudai_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgPoSyokudai* this = (BgPoSyokudai*)thisx;
    f32 temp_fv1;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_po_syokudai.c", 315);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPMatrix(oGfxCtx->polyOpa.p++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_po_syokudai.c", 319),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(oGfxCtx->polyOpa.p++, D_60003A0);
    if (Flags_GetSwitch(globalCtx, this->actor.params)) {
        Color_RGBA8* sp64;
        Color_RGBA8* sp60;

        sp64 = &D_808A898C[this->unk14C];
        sp60 = &D_808A899C[this->unk14C];
        temp_fv1 = (Math_Rand_ZeroOne() * 0.3f) + 0.7f;
        Lights_PointSetColorAndRadius(&this->unk154, sp64->r * temp_fv1, sp64->g * temp_fv1, sp64->b * temp_fv1, 200);
        func_80093D84(globalCtx->state.gfxCtx);
        gSPSegment(
            oGfxCtx->polyXlu.p++, 0x08,
            Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0, 0, 32, 64, 1, 0, (this->unk14E * -0x14) & 0x1FF, 32, 128));
        gDPSetPrimColor(oGfxCtx->polyXlu.p++, 0x80, 0x80, sp64->r, sp64->g, sp64->b, 255);
        gDPSetEnvColor(oGfxCtx->polyXlu.p++, sp60->r, sp60->g, sp60->b, 255);
        Matrix_Translate(0.0f, 52.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY((s16)((func_8005A9F4(ACTIVE_CAM) - this->actor.shape.rot.y) + 0x8000) * 0.0000958738f,
                       MTXMODE_APPLY);
        Matrix_Scale(0.0027f, 0.0027f, 0.0027f, MTXMODE_APPLY);
        gSPMatrix(oGfxCtx->polyXlu.p++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_po_syokudai.c", 368),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(oGfxCtx->polyXlu.p++, D_404D4E0);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_po_syokudai.c", 373);
}
