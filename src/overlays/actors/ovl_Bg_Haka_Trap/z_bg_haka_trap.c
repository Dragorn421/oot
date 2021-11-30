#include "functions.h"
#include "global.h"
#include "z64.h"
#include "z64collision_check.h"
#include "z_bg_haka_trap.h"
#include "objects/object_haka_objects/object_haka_objects.h"

#define FLAGS 0

void BgHakaTrap_Init(Actor* thisx, GlobalContext* globalCtx);
void BgHakaTrap_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgHakaTrap_Update(Actor* thisx, GlobalContext* globalCtx);
void BgHakaTrap_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808801B8(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808802D8(BgHakaTrap* this, GlobalContext* globalCtx);
void func_80880484(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808805C0(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808806BC(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808808F4(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808809B0(BgHakaTrap* this, GlobalContext* globalCtx);
void func_808809E4(BgHakaTrap* this, GlobalContext* globalCtx, s16);
void func_80880AE8(BgHakaTrap* this, GlobalContext* globalCtx);
void func_80880C0C(BgHakaTrap* this, GlobalContext* globalCtx);

s32 D_80880F30 = 0;

const ActorInit Bg_Haka_Trap_InitVars = {
    /**/ ACTOR_BG_HAKA_TRAP,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_HAKA_OBJECTS,
    /**/ sizeof(BgHakaTrap),
    /**/ BgHakaTrap_Init,
    /**/ BgHakaTrap_Destroy,
    /**/ BgHakaTrap_Update,
    /**/ BgHakaTrap_Draw,
};

ColliderCylinderInit D_80880F54 = {
    { 9, 0x11, 0xD, 9, 0x20, 1 },
    { 0, { 0xFFCFFFFF, 0, 4 }, { 0xFFCFFFFF, 0, 0 }, 1, 1, 1 },
    { 0x1E, 0x5A, 0, { 0, 0, 0 } },
};

ColliderTrisElementInit D_80880F80[2] = {
    {
        { 0, { 0, 0, 0 }, { 0x20000, 0, 0 }, 0, 1, 0 },
        {
            {
                { 1800.0f, 1200.0f, 0.0f },
                { -1800.0f, 1200.0f, 0.0f },
                { -1800.0f, 0.0f, 0.0f },
            },
        },
    },
    {
        { 0, { 0, 0, 0 }, { 0x20000, 0, 0 }, 0, 1, 0 },
        { {
            { 1800.0f, 1200.0f, 0.0f },
            { -1800.0f, 0.0f, 0.0f },
            { 1800.0f, 0.0f, 0.0f },
        } },
    },
};

ColliderTrisInit D_80880FF8 = { { 0xA, 0, 9, 0, 0x20, 2 }, 2, D_80880F80 };
CollisionCheckInfoInit D_80881008 = { 0, 0x50, 0x64, 0xFF };

InitChainEntry D_80881010[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgHakaTrap_Init(Actor* thisx, GlobalContext* globalCtx) {
    static s32 D_80881014 = 0;

    s32* new_var;
    BgHakaTrap* this = (BgHakaTrap*)thisx;
    CollisionHeader* sp2C;

    sp2C = NULL;
    Actor_ProcessInitChain(&this->dyna.actor, D_80881010);
    this->dyna.actor.params &= 0xFF;
    new_var = &D_80881014;
    if (this->dyna.actor.params != BG_HAKA_TRAP_PROPELLER) {
        Collider_InitCylinder(globalCtx, &this->unk178);
        Collider_SetCylinder(globalCtx, &this->unk178, &this->dyna.actor, &D_80880F54);
        if ((this->dyna.actor.params == BG_HAKA_TRAP_GUILLOTINE) ||
            (this->dyna.actor.params == BG_HAKA_TRAP_GUILLOTINE_ALT)) {
            this->unk168 = 0x14;
            do {
            } while (0);
            this->unk178.dim.yShift = 0xA;
            this->dyna.actor.velocity.y = 0.1f;
            if (this->dyna.actor.params == BG_HAKA_TRAP_GUILLOTINE_ALT) {
                this->dyna.actor.params = BG_HAKA_TRAP_GUILLOTINE;
                this->unk16A = 1;
            }
            this->actionFunc = &func_80880484;
        } else {
            DynaPolyActor_Init((DynaPolyActor*)this, DPM_PLAYER);
            this->dyna.actor.flags |= 0x10;
            if (this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_CRUSHER) {
                CollisionHeader_GetVirtual(&object_haka_objects_Col_009CD0, &sp2C);
                this->unk168 = 0x1E;
                if (*new_var != 0) {
                    this->actionFunc = &func_808808F4;
                    D_80881014 = 0;
                } else {
                    D_80881014 = 1;
                    this->actionFunc = &func_808806BC;
                    this->dyna.actor.velocity.y = 0.5f;
                }
                this->dyna.actor.floorHeight = this->dyna.actor.home.pos.y - 225.0f;
                this->unk16A = this->dyna.actor.floorHeight + 50.0f - 25.0f;
                this->unk178.dim.radius = 0xA;
                this->unk178.dim.height = 0x28;
            } else {
                if (this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_1) {
                    CollisionHeader_GetVirtual(&object_haka_objects_Col_0081D0, &sp2C);
                    this->dyna.actor.home.pos.x -= 200.0f;
                } else {
                    this->dyna.actor.home.pos.x += 200.0f;
                    CollisionHeader_GetVirtual(&object_haka_objects_Col_008D10, &sp2C);
                }
                Collider_InitTris(globalCtx, &this->unk1C4);
                Collider_SetTris(globalCtx, &this->unk1C4, &this->dyna.actor, &D_80880FF8, this->unk1E4);
                this->unk178.dim.radius = 0x12;
                this->unk178.dim.height = 0x73;
                this->unk178.info.toucherFlags |= 0;
                this->unk178.info.toucherFlags |= 0x10;
                this->actionFunc = &func_808801B8;
            }
            this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp2C);
        }
    } else {
        this->unk168 = 0x28;
        this->actionFunc = &func_808809B0;
        this->dyna.actor.uncullZoneScale = 500.0f;
    }
    CollisionCheck_SetInfo(&this->dyna.actor.colChkInfo, NULL, &D_80881008);
}

void BgHakaTrap_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaTrap* this = (BgHakaTrap*)thisx;

    if (this->dyna.actor.params != BG_HAKA_TRAP_PROPELLER) {
        if (this->dyna.actor.params != BG_HAKA_TRAP_GUILLOTINE) {
            DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
            if ((this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_1) ||
                (this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_2)) {
                Collider_DestroyTris(globalCtx, &this->unk1C4);
            }
        }
        Collider_DestroyCylinder(globalCtx, &this->unk178);
    }
    Audio_StopSfxByPos(&this->unk16C);
}

void func_8087FFC0(BgHakaTrap* this, GlobalContext* globalCtx) {
    f32 temp_fv0_cosShapeRotY;
    Vec3f sp28;
    f32 sp24_sinShapeRotY;
    Player* player = GET_PLAYER(globalCtx);

    func_8002DBD0(&this->dyna.actor, &sp28, &player->actor.world.pos);
    sp24_sinShapeRotY = Math_SinS(this->dyna.actor.shape.rot.y);
    temp_fv0_cosShapeRotY = Math_CosS(this->dyna.actor.shape.rot.y);
    if (this->dyna.actor.params == BG_HAKA_TRAP_GUILLOTINE) {
        sp28.x = CLAMP(sp28.x, -50.0f, 50.0f);
        sp28.z = ((sp28.z >= 0.0f) ? 1.0f : -1.0f) * -15.0f;
    } else {
        sp28.x = -CLAMP(sp28.x, -162.0f, 162.0f);
        sp28.z = ((sp28.z >= 0.0f) ? 1.0f : -1.0f) * 15.0f;
    }
    this->unk178.dim.pos.x =
        this->dyna.actor.world.pos.x + (sp28.x * temp_fv0_cosShapeRotY) + (sp28.z * sp24_sinShapeRotY);
    this->unk178.dim.pos.z =
        this->dyna.actor.world.pos.z + (sp28.x * sp24_sinShapeRotY) + (sp28.z * temp_fv0_cosShapeRotY);
}

void func_808801B8(BgHakaTrap* this, GlobalContext* globalCtx) {
    static s32 D_80881018 = 0;

    Player* player = GET_PLAYER(globalCtx);

    if ((D_80880F30 == 0) && (Player_InCsMode(globalCtx) == 0)) {
        if (Math_StepToF(&this->dyna.actor.world.pos.x, this->dyna.actor.home.pos.x, 0.5f) == 0) {
            func_8002F974(&this->dyna.actor, NA_SE_EV_TRAP_OBJ_SLIDE - SFX_FLAG);
        } else {
            if (this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_1) {
                D_80881018 |= 1;
            } else if (this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_2) {
                D_80881018 |= 2;
            }
        }
    }
    func_8087FFC0(this, globalCtx);
    if (this->unk1C4.base.acFlags & AC_HIT) {
        this->unk168 = 0x14;
        D_80880F30 = 1;
        this->actionFunc = func_808802D8;
    } else if (D_80881018 == 3) {
        D_80881018 = 4;
        player->actor.bgCheckFlags |= 0x100;
    }
}

Vec3f D_8088101C = { 0.0f, 0.0f, 0.0f };

void func_808802D8(BgHakaTrap* this, GlobalContext* globalCtx) {
    Vec3f sp94;
    s32 var_s0;

    if (this->unk168 != 0) {
        this->unk168--;
    }
    func_8002F974(&this->dyna.actor, NA_SE_EV_BURN_OUT - SFX_FLAG);
    for (var_s0 = 0; var_s0 < 2; var_s0++) {
        sp94.x = (Rand_ZeroOne() * ((this->dyna.actor.params == BG_HAKA_TRAP_SPIKED_WOODEN_WALL_1) ? -30.0f : 30.0f)) +
                 this->dyna.actor.world.pos.x;
        sp94.y = (Rand_ZeroOne() * 10.0f) + this->dyna.actor.world.pos.y + 30.0f;
        sp94.z = Rand_CenteredFloat(320.0f) + this->dyna.actor.world.pos.z;
        EffectSsDeadDb_Spawn(globalCtx, &sp94, &D_8088101C, &D_8088101C, 130, 20, 255, 255, 150, 170, 255, 0, 0, 1, 9,
                             false);
    }
    if (this->unk168 == 0) {
        D_80880F30 = 0;
        Actor_Kill(&this->dyna.actor);
    }
}

void func_80880484(BgHakaTrap* this, GlobalContext* globalCtx) {
    s32 sp24;
    s32 unk168;

    if (this->unk16A) {
        this->dyna.actor.velocity.y *= 3.0f;
    } else {
        this->dyna.actor.velocity.y *= 2.0f;
    }
    if (this->unk168 != 0) {
        this->unk168 -= 1;
    }
    sp24 =
        Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y - 185.0f, this->dyna.actor.velocity.y);
    unk168 = this->unk168;
    if (((unk168 == 0xA) && !this->unk16A) || ((unk168 == 0xD) && this->unk16A)) {
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_GUILLOTINE_BOUND);
    }
    if (this->unk168 == 0) {
        this->dyna.actor.velocity.y = 0.0f;
        if (this->unk16A) {
            this->unk168 = 0xA;
        } else {
            this->unk168 = 0x28;
        }
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_GUILLOTINE_UP);
        this->actionFunc = func_808805C0;
    }
    func_8087FFC0(this, globalCtx);
    if (!sp24) {
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk178.base);
    }
}

void func_808805C0(BgHakaTrap* this, GlobalContext* globalCtx) {
    if (this->unk168 != 0) {
        this->unk168 -= 1;
    }
    if (this->unk16A) {
        Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 27.0f);
    } else {
        if (this->unk168 >= 0x15) {
            Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y - 90.0f, 9.0f);
        } else {
            Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 4.5f);
        }
        if (this->unk168 == 0x14) {
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_GUILLOTINE_UP);
        }
    }
    if (this->unk168 == 0) {
        this->unk168 = 0x14;
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
        this->dyna.actor.velocity.y = 0.1f;
        this->actionFunc = func_80880484;
    }
    func_8087FFC0(this, globalCtx);
}

void func_808806BC(BgHakaTrap* this, GlobalContext* globalCtx) {
    Vec3f sp74;
    f32 temp_fv1;
    f32 var_fs0;
    s32 var_s0;
    s32 sp64;

    this->dyna.actor.velocity.y *= 1.6f;
    if (this->unk168 != 0) {
        this->unk168 -= 1;
    }
    sp74.x = this->dyna.actor.world.pos.x + 90.0f;
    sp74.y = this->dyna.actor.world.pos.y + 1.0f + 25.0f;
    sp74.z = this->dyna.actor.world.pos.z;
    var_fs0 = this->dyna.actor.floorHeight;
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        temp_fv1 = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &this->dyna.actor.floorPoly, &sp64,
                                               &this->dyna.actor, &sp74) -
                   25.0f;
        if (var_fs0 < temp_fv1) {
            var_fs0 = temp_fv1;
        }
        sp74.x -= 90.0f;
    }
    if (Math_StepToF(&this->dyna.actor.world.pos.y, var_fs0, this->dyna.actor.velocity.y) != 0) {
        if (this->dyna.actor.velocity.y > 0.01f) {
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_TRAP_BOUND);
        }
        this->dyna.actor.velocity.y = 0.0f;
    }
    if (this->dyna.actor.velocity.y >= 0.01f) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_CHINETRAP_DOWN - SFX_FLAG);
    }
    if (this->unk168 == 0) {
        this->dyna.actor.velocity.y = 0.0f;
        this->unk168 = 0x1E;
        this->unk16A = (s16)this->dyna.actor.world.pos.y + 50.0f;
        this->unk16A = MIN(this->dyna.actor.home.pos.y, this->unk16A);
        this->actionFunc = func_808808F4;
    }
}

void func_808808F4(BgHakaTrap* this, GlobalContext* globalCtx) {
    if (this->unk168 != 0) {
        this->unk168--;
    }
    if (this->unk168 >= 0x15) {
        this->unk169 = Math_StepToF(&this->dyna.actor.world.pos.y, this->unk16A, 15.0f);
    } else {
        this->unk169 = Math_StepToF(&this->dyna.actor.world.pos.y, this->dyna.actor.home.pos.y, 20.0f);
    }
    if (this->unk168 == 0) {
        this->unk168 = 0x1E;
        this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y;
        this->dyna.actor.velocity.y = 0.5f;
        this->actionFunc = func_808806BC;
    }
}

void func_808809B0(BgHakaTrap* this, GlobalContext* globalCtx) {
    if (this->unk168 != 0) {
        this->unk168 -= 1;
    }
    if (this->unk168 == 0) {
        this->actionFunc = func_80880AE8;
    }
}

void func_808809E4(BgHakaTrap* this, GlobalContext* globalCtx, s16 arg2) {
    Player* player = GET_PLAYER(globalCtx);
    Vec3f sp18;

    func_8002DBD0(&this->dyna.actor, &sp18, &player->actor.world.pos);
    if ((fabsf(sp18.x) < 70.0f) && (fabsf(sp18.y) < 100.0f) && (sp18.z < 500.0f) && (GET_PLAYER(globalCtx)->currentBoots != 1)) {
        player->windSpeed = (((500.0f - sp18.z) * 0.06f) + 5.0f) * arg2 * (1.0f / 14848.0f) * (2.0f / 3.0f);
        player->windDirection = this->dyna.actor.shape.rot.y;
    }
}

void func_80880AE8(BgHakaTrap* this, GlobalContext* globalCtx) {
    if (this->unk168 != 0) {
        if (Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0, (this->dyna.actor.world.rot.z * 0.03f) + 5.0f) != 0) {
            this->unk168 = 0x28;
            this->actionFunc = func_808809B0;
        }
    } else {
        if (Math_ScaledStepToS(&this->dyna.actor.world.rot.z, 0x3A00, (this->dyna.actor.world.rot.z * 0.03f) + 5.0f) !=
            0) {
            this->unk168 = 0x64;
            this->actionFunc = func_80880C0C;
        }
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    if (this->dyna.actor.world.rot.z > 0x1800) {
        func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
    }
    func_808809E4(this, globalCtx, this->dyna.actor.world.rot.z);
}

void func_80880C0C(BgHakaTrap* this, GlobalContext* globalCtx) {
    if (this->unk168 != 0) {
        this->unk168--;
    }
    func_8002F974(&this->dyna.actor, NA_SE_EV_WIND_TRAP - SFX_FLAG);
    if (this->unk168 == 0) {
        this->unk168 = 1;
        this->actionFunc = func_80880AE8;
    }
    this->dyna.actor.shape.rot.z += this->dyna.actor.world.rot.z;
    func_808809E4(this, globalCtx, this->dyna.actor.world.rot.z);
}

void BgHakaTrap_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaTrap* this = (BgHakaTrap*)thisx;
    s32 pad;

    this->actionFunc(this, globalCtx);

    if ((thisx->params != BG_HAKA_TRAP_PROPELLER) && (thisx->params != BG_HAKA_TRAP_SPIKED_CRUSHER)) {
        this->unk178.dim.pos.y = this->dyna.actor.world.pos.y;
        if ((thisx->params == BG_HAKA_TRAP_GUILLOTINE) || (thisx->params == BG_HAKA_TRAP_GUILLOTINE_ALT)) {
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk178.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk178.base);
        } else {
            if (this->actionFunc == func_808801B8) {
                CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk1C4.base);
            }
            CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk178.base);
        }
    }
}

void func_80880D68(BgHakaTrap* this) {
    Vec3f sp3C;
    Vec3f sp30;
    Vec3f sp24;

    Matrix_MultVec3f(&D_80880F80[0].dim.vtx[0], &sp24);
    Matrix_MultVec3f(&D_80880F80[0].dim.vtx[1], &sp30);
    Matrix_MultVec3f(&D_80880F80[0].dim.vtx[2], &sp3C);
    Collider_SetTrisVertices(&this->unk1C4, 0, &sp24, &sp30, &sp3C);
    Matrix_MultVec3f(&D_80880F80[1].dim.vtx[2], &sp30);
    Collider_SetTrisVertices(&this->unk1C4, 1, &sp24, &sp3C, &sp30);
}

Gfx* D_80881028[5] = {
    (Gfx*)0x06007610, // guillotine
    (Gfx*)0x06009860, // spiked crusher
    (Gfx*)0x06007EF0, // spiked wooden wall with holes
    (Gfx*)0x06008A20, // spiked wooden wall with less holes
    (Gfx*)0x060072C0, // propeller
};
Color_RGBA8 D_8088103C = { 0, 0, 0, 0 };

void BgHakaTrap_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgHakaTrap* this = (BgHakaTrap*)thisx;
    s32 pad;
    Vec3f sp2C;

    if (this->actionFunc == func_808802D8) {
        func_80026230(globalCtx, &D_8088103C, this->unk168 + 0x14, 0x28);
    }
    Gfx_DrawDListOpa(globalCtx, D_80881028[this->dyna.actor.params]);
    if (this->actionFunc == func_808801B8) {
        func_80880D68(this);
    }
    if (this->actionFunc == func_808802D8) {
        func_80026608(globalCtx);
    }
    if ((this->actionFunc == func_808808F4) && ((u8)this->unk169 == 0)) {
        sp2C.x = this->dyna.actor.world.pos.x;
        sp2C.z = this->dyna.actor.world.pos.z;
        sp2C.y = this->dyna.actor.world.pos.y + 110.0f;
        SkinMatrix_Vec3fMtxFMultXYZ(&globalCtx->viewProjectionMtxF, &sp2C, &this->unk16C);
        func_80078914(&this->unk16C, NA_SE_EV_BRIDGE_CLOSE - SFX_FLAG);
    }
}
