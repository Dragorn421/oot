/*
 * File: z_bg_jya_haheniron
 * Overlay: ovl_Bg_Jya_Haheniron
 * Description: Chunks of Iron Knucle Chair and Pillar
 */

#include "z_bg_jya_haheniron.h"

#define FLAGS 0x00000010

#define THIS ((BgJyaHaheniron*)thisx)

void BgJyaHaheniron_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaHaheniron_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaHaheniron_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaHaheniron_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808980C0(BgJyaHaheniron* this, GlobalContext* globalCtx);
void func_80898114(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2);
void func_8089843C(BgJyaHaheniron* this);
void func_8089844C(BgJyaHaheniron* this, GlobalContext* globalCtx);
void func_80898588(BgJyaHaheniron* this);
void func_80898598(BgJyaHaheniron* this, GlobalContext* globalCtx);
void func_8089861C(BgJyaHaheniron* this);
void func_8089862C(BgJyaHaheniron* this, GlobalContext* globalCtx);

extern Gfx D_6000600[];
extern Gfx D_6000880[];

const ActorInit Bg_Jya_Haheniron_InitVars = {
    ACTOR_BG_JYA_HAHENIRON,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_JYA_IRON,
    sizeof(BgJyaHaheniron),
    (ActorFunc)BgJyaHaheniron_Init,
    (ActorFunc)BgJyaHaheniron_Destroy,
    (ActorFunc)BgJyaHaheniron_Update,
    (ActorFunc)BgJyaHaheniron_Draw,
};
static ColliderSpheresElementSrc D_80898740[1] = {
    {
        { 0, { 0xFFCFFFFF, 0, 4 }, { 0, 0, 0 }, 1, 0, 0 },
        { 0, { { 0, 0, 0 }, 0xA }, 0x64 },
    },
};
static ColliderSpheresSrc D_80898764 = { { 0xA, 0x11, 1, 0, 0, 0 }, 1, D_80898740 };
static s16 D_80898774[5] = { 5, 8, 0xB, 0xE, 0x11 };
static InitChainEntry D_80898780[5] = {
    ICHAIN_F32_DIV1000(gravity, -2000, ICHAIN_CONTINUE),  ICHAIN_F32_DIV1000(minVelocityY, -15000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE), ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};
static f32 D_80898794[3] = { 0.13f, 0.1f, 0.1f };
static Vec3f D_808987A0 = { 0.0f, 14.0f, 0.0f };
static Vec3f D_808987AC = { 0.0f, 8.0f, 0.0f };
static Gfx* D_808987B8[6] = { D_6000880, (Gfx*)0x06000AE0, D_6000600, NULL, NULL, NULL };

void func_808980C0(BgJyaHaheniron* this, GlobalContext* globalCtx) {
    s32 pad;

    Collider_InitSpheres(globalCtx, &this->unk150);
    Collider_LoadSpheres(globalCtx, &this->unk150, &this->actor, &D_80898764, this->unk170);
}

void func_80898114(GlobalContext* globalCtx, Vec3f* arg1, Vec3f* arg2) {
    Vec3f spB4;
    Vec3f spA8;
    f32 temp_fv0;
    s32 var_s1;
    s32 var_v0;

    for (var_s1 = 0; var_s1 < ARRAY_COUNT(D_80898774); var_s1++) {
        temp_fv0 = Math_Rand_ZeroOne() * 10.0f;
        spB4.x = (Math_Sins(var_s1 * 0x3333) * temp_fv0) + arg2->x;
        spB4.y = (Math_Rand_ZeroOne() * 10.0f) + arg2->y;
        spB4.z = (Math_Coss(var_s1 * 0x3333) * temp_fv0) + arg2->z;
        temp_fv0 = Math_Rand_ZeroOne();
        if (temp_fv0 < 0.2f) {
            var_v0 = 0x60;
        } else if (temp_fv0 < 0.8f) {
            var_v0 = 0x40;
        } else {
            var_v0 = 0x20;
        }
        EffectSsKakera_Spawn(globalCtx, arg1, &spB4, arg1, -350, (s16)var_v0, 0x28, 4, 0, D_80898774[var_s1], 0, 0x14,
                             40, -1, OBJECT_JYA_IRON, D_6000880);
    }
    spA8.x = arg1->x + (arg2->x * 5.0f);
    spA8.y = arg1->y + (arg2->y * 5.0f);
    spA8.z = arg1->z + (arg2->z * 5.0f);
    func_80033480(globalCtx, &spA8, 100.0f, 4, 0x64, 0xA0, 1U);
}

void BgJyaHaheniron_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaHaheniron* this = (BgJyaHaheniron*)thisx;

    Actor_ProcessInitChain(&this->actor, D_80898780);
    Actor_SetScale(&this->actor, D_80898794[this->actor.params]);
    if (this->actor.params == 0) {
        func_808980C0(this, globalCtx);
        this->actor.shape.rot.z = (s16)(s32)(Math_Rand_ZeroOne() * 65535.0f);
        func_8089843C(this);
    } else if (this->actor.params == 1) {
        func_80898588(this);
    } else if (this->actor.params == 2) {
        func_8089861C(this);
    }
}

void BgJyaHaheniron_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaHaheniron* this = (BgJyaHaheniron*)thisx;

    if (this->actor.params == 0) {
        Collider_DestroySpheres(globalCtx, &this->unk150);
    }
}

void func_8089843C(BgJyaHaheniron* this) {
    this->unk14C = func_8089844C;
}

void func_8089844C(BgJyaHaheniron* this, GlobalContext* globalCtx) {
    Vec3f sp2C;

    Actor_MoveForward(&this->actor);
    func_8002E4B4(globalCtx, &this->actor, 5.0f, 8.0f, 0.0f, 0x85);
    if ((this->actor.bgCheckFlags & 9) ||
        ((this->unk150.base.atFlags & 2) && (this->unk150.base.at != NULL) && (this->unk150.base.at->type == 2))) {
        sp2C.x = -Math_Rand_ZeroOne() * this->actor.velocity.x;
        sp2C.y = -Math_Rand_ZeroOne() * this->actor.velocity.y;
        sp2C.z = -Math_Rand_ZeroOne() * this->actor.velocity.z;
        func_80898114(globalCtx, &this->actor.posRot.pos, &sp2C);
        Actor_Kill(&this->actor);
    } else if (this->unk1B0 >= 0x3D) {
        Actor_Kill(&this->actor);
    } else {
        Collider_AddAT(globalCtx, &globalCtx->colliderCtx, &this->unk150.base);
    }
    this->actor.shape.rot.y += 0x4B0;
    this->actor.shape.rot.x += 0xFA0;
}

void func_80898588(BgJyaHaheniron* this) {
    this->unk14C = func_80898598;
}

void func_80898598(BgJyaHaheniron* this, GlobalContext* globalCtx) {
    if (this->unk1B0 >= 8) {
        Actor_MoveForward(&this->actor);
    } else if (this->unk1B0 >= 0x11) {
        func_80898114(globalCtx, &this->actor.posRot.pos, &D_808987A0);
        Actor_Kill(&this->actor);
    }
    this->actor.shape.rot.y += 0x258;
    this->actor.shape.rot.x += 0x3E8;
}

void func_8089861C(BgJyaHaheniron* this) {
    this->unk14C = func_8089862C;
}

void func_8089862C(BgJyaHaheniron* this, GlobalContext* globalCtx) {
    if (this->unk1B0 >= 0x11) {
        func_80898114(globalCtx, &this->actor.posRot.pos, &D_808987AC);
        Audio_PlaySoundAtPosition(globalCtx, &this->actor.posRot.pos, 80, NA_SE_EN_IRONNACK_BREAK_PILLAR2);
        Actor_Kill(&this->actor);
    }
}

void BgJyaHaheniron_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaHaheniron* this = (BgJyaHaheniron*)thisx;

    this->unk1B0 += 1;
    this->unk14C(this, globalCtx);
}

void BgJyaHaheniron_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaHaheniron* this = (BgJyaHaheniron*)thisx;

    if (this->actor.params == 0) {
        func__800628A4_Type0(0, &this->unk150);
    }
    Gfx_DrawDListOpa(globalCtx, D_808987B8[this->actor.params]);
}
