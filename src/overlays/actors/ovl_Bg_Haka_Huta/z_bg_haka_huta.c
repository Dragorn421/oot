#include "z_bg_haka_huta.h"

#define FLAGS 0x00000010

#define THIS ((BgHakaHuta*)thisx)

void BgHakaHuta_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaHuta_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8087D0AC(BgHakaHuta* this, GlobalContext* globalCtx);
void func_8087D2F0(BgHakaHuta* this, GlobalContext* globalCtx);
void func_8087D5B8(BgHakaHuta* this, GlobalContext* globalCtx);
void func_8087D66C(BgHakaHuta* this, GlobalContext* globalCtx);
void func_8087D720(BgHakaHuta* this, GlobalContext* globalCtx);
void func_8087D8C0(BgHakaHuta* this, GlobalContext* globalCtx);

extern Gfx D_60006B0[];
extern CollisionHeader D_6000870;

const ActorInit Bg_Haka_Huta_InitVars = {
    ACTOR_BG_HAKA_HUTA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HAKACH_OBJECTS,
    sizeof(BgHakaHuta),
    (ActorFunc)BgHakaHuta_Init,
    (ActorFunc)BgHakaHuta_Destroy,
    (ActorFunc)BgHakaHuta_Update,
    (ActorFunc)BgHakaHuta_Draw,
};
static InitChainEntry D_8087D940[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};
static Vec3f D_8087D944 = { 0.0f, 0.0f, 0.0f };
static Color_RGBA8 D_8087D950 = { 30, 20, 50, 255 };
static Color_RGBA8 D_8087D954 = { 0, 0, 0, 255 };

void BgHakaHuta_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* this = (BgHakaHuta*)thisx;
    s32 pad;
    CollisionHeader* sp24;

    sp24 = NULL;
    Actor_ProcessInitChain(&this->dyna.actor, D_8087D940);
    DynaPolyActor_Init(&this->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(&D_6000870, &sp24);
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp24);
    this->unk16A = (this->dyna.actor.params >> 8) & 0xFF;
    thisx->params &= 0xFF;
    if (Flags_GetSwitch(globalCtx, this->dyna.actor.params)) {
        this->unk168 = -1;
        this->actionFunc = func_8087D720;
    } else {
        this->actionFunc = func_8087D2F0;
    }
}

void BgHakaHuta_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* this = (BgHakaHuta*)thisx;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
}

void func_8087D0AC(BgHakaHuta* this, GlobalContext* globalCtx) {
    s32 var_s0;
    f32 temp_fs1;
    Vec3f sp94;
    Vec3f sp88;
    f32 var_fs0;
    f32 v;

    if (this->dyna.actor.world.rot.y == 0) {
        var_fs0 = 1.0f;
    } else {
        var_fs0 = -1.0f;
    }
    sp88.x = -0.5f * var_fs0;
    sp88.y = 0.0f;
    sp88.z = 0.0f;
    sp94.y = this->dyna.actor.world.pos.y;
    sp94.z = this->dyna.actor.world.pos.z;
    v = (50 - ((this->dyna.actor.world.pos.x - this->dyna.actor.home.pos.x) * var_fs0));
    temp_fs1 = v * var_fs0;
    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        if (var_s0 == 2) {
            sp94.z += 120.0f * var_fs0;
        }
        sp94.x = this->dyna.actor.home.pos.x - (Rand_ZeroOne() * temp_fs1);
        func_8002829C(globalCtx, &sp94, &sp88, &D_8087D944, &D_8087D950, &D_8087D954, (Rand_ZeroOne() * 10.0f) + 50.0f,
                      10);
    }
}

void func_8087D268(BgHakaHuta* this, GlobalContext* globalCtx, u16 arg2) {
    Vec3f sp24;

    if (this->dyna.actor.shape.rot.y == 0) {
        sp24.z = this->dyna.actor.world.pos.z + 120.0f;
    } else {
        sp24.z = this->dyna.actor.world.pos.z - 120.0f;
    }
    sp24.x = this->dyna.actor.world.pos.x;
    sp24.y = this->dyna.actor.world.pos.y;
    Audio_PlaySoundAtPosition(globalCtx, &sp24, 30, arg2);
}

void func_8087D2F0(BgHakaHuta* this, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, this->dyna.actor.params) && !Player_InCsMode(globalCtx)) {
        this->unk168 = 0x19;
        this->actionFunc = func_8087D5B8;
        func_800800F8(globalCtx, 0x1771, 0x3E7, &this->dyna.actor, 0);
        if (this->unk16A == 2) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FIREFLY,
                        this->dyna.actor.world.pos.x + (-25.0f * Math_CosS(this->dyna.actor.shape.rot.y)) +
                            (40.0f * Math_SinS(this->dyna.actor.shape.rot.y)),
                        this->dyna.actor.world.pos.y - 10.0f,

                        this->dyna.actor.world.pos.z - (-25.0f * Math_SinS(this->dyna.actor.shape.rot.y)) +
                            (40.0f * Math_CosS(this->dyna.actor.shape.rot.y)),
                        0, this->dyna.actor.shape.rot.y + 0x8000, 0, 2);
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_FIREFLY,
                        this->dyna.actor.world.pos.x + (-25.0f * Math_CosS(this->dyna.actor.shape.rot.y)) +
                            (80.0f * Math_SinS(this->dyna.actor.shape.rot.y)),
                        this->dyna.actor.world.pos.y - 10.0f,
                        this->dyna.actor.world.pos.z - (-25.0f * Math_SinS(this->dyna.actor.shape.rot.y)) +
                            (80.0f * Math_CosS(this->dyna.actor.shape.rot.y)),
                        0, this->dyna.actor.shape.rot.y, 0, 2);
        } else if (this->unk16A == 1) {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_RD,
                        this->dyna.actor.home.pos.x + (-25.0f * Math_CosS(this->dyna.actor.shape.rot.y)) +
                            (100.0f * Math_SinS(this->dyna.actor.shape.rot.y)),
                        this->dyna.actor.home.pos.y - 40.0f,
                        this->dyna.actor.home.pos.z - (-25.0f * Math_SinS(this->dyna.actor.shape.rot.y)) +
                            (100.0f * Math_CosS(this->dyna.actor.shape.rot.y)),
                        0, this->dyna.actor.shape.rot.y, 0, 0xFD);
        }
    }
}

void func_8087D5B8(BgHakaHuta* this, GlobalContext* globalCtx) {
    f32 var_fv0;
    s16 temp_v0;

    temp_v0 = this->unk168;
    if (temp_v0 != 0) {
        this->unk168 = temp_v0 - 1;
    }
    if (this->dyna.actor.world.rot.y == 0) {
        var_fv0 = 4.0f;
    } else {
        var_fv0 = -4.0f;
    }
    Math_StepToF(&this->dyna.actor.world.pos.x, this->dyna.actor.home.pos.x + var_fv0, 2.0f);
    if (this->unk168 == 0) {
        this->unk168 = 0x25;
        func_8087D268(this, globalCtx, NA_SE_EV_COFFIN_CAP_OPEN);
        this->actionFunc = func_8087D66C;
    }
}

void func_8087D66C(BgHakaHuta* this, GlobalContext* globalCtx) {
    f32 var_fv0;
    s16 temp_v0;

    temp_v0 = this->unk168;
    if (temp_v0 != 0) {
        this->unk168 = temp_v0 - 1;
    }
    if (this->dyna.actor.world.rot.y == 0) {
        var_fv0 = 24.0f;
    } else {
        var_fv0 = -24.0f;
    }
    if (!Math_StepToF(&this->dyna.actor.world.pos.x, this->dyna.actor.home.pos.x + var_fv0, 0.5f)) {
        func_8087D0AC(this, globalCtx);
    }
    if (this->unk168 == 0) {
        func_8087D268(this, globalCtx, NA_SE_EV_COFFIN_CAP_BOUND);
        this->actionFunc = func_8087D720;
    }
}

void func_8087D720(BgHakaHuta* this, GlobalContext* globalCtx) {
    static Vec3f D_8087D958 = { 30.0f, 0.0f, 0.0f };
    static Vec3f D_8087D964 = { 0.03258f, 0.3258f, -0.9449f };
    MtxF sp40;
    Vec3f sp34;
    s32 temp_v0;

    this->unk168 += 1;
    if (this->unk168 == 6) {
        this->actionFunc = func_8087D8C0;
        temp_v0 = Quake_Add(globalCtx->cameraPtrs[globalCtx->activeCamera], 3U);
        Quake_SetSpeed(temp_v0, 0x7530);
        Quake_SetQuakeValues(temp_v0, 4, 0, 0, 0);
        Quake_SetCountdown(temp_v0, 2);
    } else if (this->unk168 == 0) {
        this->unk168 = 6;
        this->actionFunc = func_8087D8C0;
    }
    D_8087D958.x = this->unk168 + 24.0f;
    if (D_8087D958.x > 30.0f) {
        D_8087D958.x = 30.0f;
    }
    Matrix_RotateY(this->dyna.actor.world.rot.y * 0.0000958738f, MTXMODE_NEW);
    func_800D23FC(this->unk168 * 0.16001178f, &D_8087D964, MTXMODE_APPLY);
    Matrix_MultVec3f(&D_8087D958, &sp34);
    this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + sp34.x;
    this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + sp34.y;
    this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z + sp34.z;
    Matrix_Get(&sp40);
    func_800D20CC(&sp40, &this->dyna.actor.shape.rot, 0);
}

void func_8087D8C0(BgHakaHuta* this, GlobalContext* globalCtx) {
}

void BgHakaHuta_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* this = (BgHakaHuta*)thisx;

    this->actionFunc(this, globalCtx);
}

void BgHakaHuta_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaHuta* this = (BgHakaHuta*)thisx;

    Gfx_DrawDListOpa(globalCtx, D_60006B0);
}
