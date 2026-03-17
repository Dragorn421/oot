/*
 * File: z_bg_ice_turara.c
 * Overlay: ovl_Bg_Ice_Turara
 * Description: Icicles
 */

#include "z_bg_ice_turara.h"

#define FLAGS 0x00000000

#define THIS ((BgIceTurara*)thisx)

void BgIceTurara_Init(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Update(Actor* thisx, GlobalContext* globalCtx);
void BgIceTurara_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80892220(BgIceTurara*, GlobalContext*);
void func_80892280(BgIceTurara*, GlobalContext*);
void func_808922B8(BgIceTurara*, GlobalContext*);
void func_80892424(BgIceTurara*, GlobalContext*);
void func_80892574(BgIceTurara*, GlobalContext*);

extern Gfx D_60023D0[];
extern UNK_TYPE D_6002594;

static ColliderCylinderSrc D_80892620 = {
    { 0xA, 0x11, 9, 0, 0x20, 1 },
    { 0, { 0xFFCFFFFF, 0, 4 }, { 0x4FC007CA, 0, 0 }, 1, 1, 0 },
    { 0xD, 0x78, 0, { 0, 0, 0 } },
};
const ActorInit Bg_Ice_Turara_InitVars = {
    ACTOR_BG_ICE_TURARA,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_ICE_OBJECTS,
    sizeof(BgIceTurara),
    (ActorFunc)BgIceTurara_Init,
    (ActorFunc)BgIceTurara_Destroy,
    (ActorFunc)BgIceTurara_Update,
    (ActorFunc)BgIceTurara_Draw,
};
static InitChainEntry D_8089266C[] = {
    ICHAIN_F32(uncullZoneScale, 600, ICHAIN_CONTINUE),
    ICHAIN_F32(gravity, -3, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -30, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};
static Vec3f D_8089267C = { 0.0f, -1.0f, 0.0f };
static Color_RGBA8 D_80892688 = { 0xAA, 0xFF, 0xFF, 0xFF };
static Color_RGBA8 D_8089268C = { 0, 0x32, 0x64, 0xFF };

void BgIceTurara_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* this = (BgIceTurara*)thisx;
    s32 pad;
    s32 sp24;

    sp24 = 0;
    Actor_ProcessInitChain(&this->dyna.actor, D_8089266C);
    DynaPolyInfo_SetActorMove(&this->dyna, DPM_UNK);
    DynaPolyInfo_Alloc(&D_6002594, &sp24);
    Collider_InitCylinder(globalCtx, &this->unk16C);
    Collider_LoadCylinder(globalCtx, &this->unk16C, &this->dyna.actor, &D_80892620);
    Collider_UpdateCylinderShape(&this->dyna.actor, &this->unk16C);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp24);
    if (this->dyna.actor.params == 0) {
        this->unk164 = func_80892220;
    } else {
        this->dyna.actor.shape.rot.x = -0x8000;
        this->dyna.actor.shape.unk_08 = 1200.0f;
        this->unk164 = func_80892280;
    }
}

void BgIceTurara_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* this = (BgIceTurara*)thisx;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
    Collider_DestroyCylinder(globalCtx, &this->unk16C);
}

void func_80892040(BgIceTurara* this, GlobalContext* globalCtx, f32 arg2) {
    Vec3f sp9C;
    Vec3f sp90;
    s32 var_s0;
    s32 sp88;

    Audio_PlaySoundAtPosition(globalCtx, &this->dyna.actor.posRot.pos, 30, NA_SE_EV_ICE_BROKEN);
    sp88 = 0;
    do {
        for (var_s0 = 0; var_s0 < 10; var_s0++) {
            sp90.x = Math_Rand_CenteredFloat(8.0f) + this->dyna.actor.posRot.pos.x;
            sp90.y = (Math_Rand_ZeroOne() * arg2) + this->dyna.actor.posRot.pos.y + (sp88 * arg2);
            sp90.z = Math_Rand_CenteredFloat(8.0f) + this->dyna.actor.posRot.pos.z;
            sp9C.x = Math_Rand_CenteredFloat(7.0f);
            sp9C.z = Math_Rand_CenteredFloat(7.0f);
            sp9C.y = (Math_Rand_ZeroOne() * 4.0f) + 8.0f;
            EffectSsEnIce_Spawn(globalCtx, &sp90, (Math_Rand_ZeroOne() * 0.2f) + 0.1f, &sp9C, &D_8089267C, &D_80892688,
                                &D_8089268C, 30);
        }
        sp88 = sp88 + 1;
    } while (sp88 != 2);
}

void func_80892220(BgIceTurara* this, GlobalContext* globalCtx) {
    if (this->unk16C.base.acFlags & 2) {
        func_80892040(this, globalCtx, 50.0f);
        Actor_Kill(&this->dyna.actor);
    } else {
        Collider_AddAC(globalCtx, &globalCtx->colliderCtx, &this->unk16C.base);
    }
}

void func_80892280(BgIceTurara* this, GlobalContext* globalCtx) {
    if (this->dyna.actor.xzDistFromLink < 60.0f) {
        this->unk168 = 0xA;
        this->unk164 = func_808922B8;
    }
}

void func_808922B8(BgIceTurara* this, GlobalContext* globalCtx) {
    s32 var_v0_2;
    f32 sp28;

    if (this->unk168 != 0) {
        this->unk168 -= 1;
    }
    if (!(this->unk168 % 4)) {
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_ICE_SWING);
    }
    if (this->unk168 == 0) {
        this->dyna.actor.posRot.pos.x = this->dyna.actor.initPosRot.pos.x;
        this->dyna.actor.posRot.pos.z = this->dyna.actor.initPosRot.pos.z;
        Collider_UpdateCylinderShape(&this->dyna.actor, &this->unk16C);
        Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &this->unk16C.base);
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
        this->unk164 = func_80892424;
    } else {
        sp28 = Math_Rand_ZeroOne();
        if (Math_Rand_ZeroOne() < 0.5f) {
            var_v0_2 = -1;
        } else {
            var_v0_2 = 1;
        }
        this->dyna.actor.posRot.pos.x = (var_v0_2 * ((0.5f * sp28) + 0.5f)) + this->dyna.actor.initPosRot.pos.x;
        sp28 = Math_Rand_ZeroOne();
        if (Math_Rand_ZeroOne() < 0.5f) {
            var_v0_2 = -1;
        } else {
            var_v0_2 = 1;
        }
        this->dyna.actor.posRot.pos.z = (var_v0_2 * ((0.5f * sp28) + 0.5f)) + this->dyna.actor.initPosRot.pos.z;
    }
}

void func_80892424(BgIceTurara* this, GlobalContext* globalCtx) {
    if ((this->unk16C.base.atFlags & 2) || (this->dyna.actor.bgCheckFlags & 1)) {
        this->unk16C.base.atFlags &= ~2;
        this->dyna.actor.bgCheckFlags &= ~1;
        if (this->dyna.actor.posRot.pos.y < this->dyna.actor.groundY) {
            this->dyna.actor.posRot.pos.y = this->dyna.actor.groundY;
        }
        func_80892040(this, globalCtx, 40.0f);
        if (this->dyna.actor.params == 2) {
            this->dyna.actor.posRot.pos.y = this->dyna.actor.initPosRot.pos.y + 120.0f;
            func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
            this->unk164 = func_80892574;
        } else {
            Actor_Kill(&this->dyna.actor);
        }
    } else {
        Actor_MoveForward(&this->dyna.actor);
        this->dyna.actor.posRot.pos.y += 40.0f;
        func_8002E4B4(globalCtx, &this->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
        this->dyna.actor.posRot.pos.y -= 40.0f;
        Collider_UpdateCylinderShape(&this->dyna.actor, &this->unk16C);
        Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &this->unk16C.base);
    }
}

void func_80892574(BgIceTurara* this, GlobalContext* globalCtx) {
    if (Math_ApproxF(&this->dyna.actor.posRot.pos.y, this->dyna.actor.initPosRot.pos.y, 1.0f)) {
        this->unk164 = func_80892280;
        this->dyna.actor.velocity.y = 0.0f;
    }
}

void BgIceTurara_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* this = (BgIceTurara*)thisx;

    this->unk164(this, globalCtx);
}

void BgIceTurara_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgIceTurara* this = (BgIceTurara*)thisx;

    Gfx_DrawDListOpa(globalCtx, D_60023D0);
}
