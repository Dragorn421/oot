/*
 * File: z_obj_tsubo.c
 * Overlay: ovl_Obj_Tsubo
 * Description: Breakable pot
 */

#include "z_obj_tsubo.h"

#define FLAGS 0x00800010

#define THIS ((ObjTsubo*)thisx)

void ObjTsubo_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjTsubo_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80BA152C(ObjTsubo* this);
void func_80BA153C(ObjTsubo* this, GlobalContext* globalCtx);
void func_80BA15AC(ObjTsubo* this);
void func_80BA15BC(ObjTsubo* this, GlobalContext* globalCtx);
void func_80BA17C4(ObjTsubo* this);
void func_80BA180C(ObjTsubo* this, GlobalContext* globalCtx);
void func_80BA188C(ObjTsubo* this);
void func_80BA1958(ObjTsubo* this, GlobalContext* globalCtx);
void func_80BA1B0C(Actor* thisx, GlobalContext* globalCtx);

extern Gfx D_5017870[];
extern Gfx D_5017A60[];
extern Gfx D_60017C0[];
extern Gfx D_6001960[];

static s16 D_80BA1B50 = 0;
static s16 D_80BA1B54 = 0;
static s16 D_80BA1B58 = 0;
static s16 D_80BA1B5C = 0;

const ActorInit Obj_Tsubo_InitVars = {
    ACTOR_OBJ_TSUBO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(ObjTsubo),
    (ActorFunc)ObjTsubo_Init,
    (ActorFunc)ObjTsubo_Destroy,
    (ActorFunc)ObjTsubo_Update,
    NULL,
};

static s16 D_80BA1B80[2] = { OBJECT_GAMEPLAY_DANGEON_KEEP, OBJECT_TSUBO };
static Gfx* D_80BA1B84[2] = { D_5017870, D_60017C0 };
static Gfx* D_80BA1B8C[2] = { D_5017A60, D_6001960 };

static ColliderCylinderInit D_80BA1B94 = {
    {
        COLTYPE_HARD,
        AT_ON | AT_TYPE_PLAYER,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000002, 0x00, 0x01 },
        { 0x4FC1FFFE, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 9, 26, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit D_80BA1BC0 = { 0, 0xC, 0x3C, MASS_IMMOVABLE };
static InitChainEntry D_80BA1BC8[] = {
    ICHAIN_F32_DIV1000(gravity, -1200, ICHAIN_CONTINUE), ICHAIN_F32_DIV1000(minVelocityY, -20000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 150, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneForward, 900, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 100, ICHAIN_CONTINUE),   ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

void func_80BA0D60(ObjTsubo* this, GlobalContext* globalCtx) {
    s16 temp_v0;

    temp_v0 = this->actor.params & 0x1F;
    if ((temp_v0 >= 0) && (temp_v0 < 0x1A)) {
        Item_DropCollectible(globalCtx, &this->actor.world.pos,
                             (s16)(temp_v0 | (((this->actor.params >> 9) & 0x3F) << 8)));
    }
}

void func_80BA0DC0(ObjTsubo* this) {
    this->actor.velocity.y += this->actor.gravity;
    if (this->actor.velocity.y < this->actor.minVelocityY) {
        this->actor.velocity.y = this->actor.minVelocityY;
    }
}

s32 func_80BA0DF4(ObjTsubo* this, GlobalContext* globalCtx) {
    CollisionPoly* sp34;
    Vec3f sp28;
    s32 sp24;
    f32 temp_fv0;

    sp28.x = this->actor.world.pos.x;
    sp28.y = this->actor.world.pos.y + 20.0f;
    sp28.z = this->actor.world.pos.z;
    temp_fv0 = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &sp34, &sp24, &this->actor, (Vec3f*)&sp28);
    if (temp_fv0 > BGCHECK_Y_MIN) {
        this->actor.world.pos.y = temp_fv0;
        Math_Vec3f_Copy(&this->actor.home.pos, &this->actor.world.pos);
        return 1;
    } else {
        osSyncPrintf("地面に付着失敗\n");
        return 0;
    }
}

void func_80BA0E98(ObjTsubo* this2, GlobalContext* globalCtx) {
    ObjTsubo* this = this2;

    Collider_InitCylinder(globalCtx, &this->unk150);
    Collider_SetCylinder(globalCtx, &this->unk150, &this->actor, &D_80BA1B94);
    Collider_UpdateCylinder(&this->actor, &this->unk150);
}

void ObjTsubo_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjTsubo* this = (ObjTsubo*)thisx;

    Actor_ProcessInitChain(&this->actor, D_80BA1BC8);
    func_80BA0E98(this, globalCtx);
    CollisionCheck_SetInfo(&this->actor.colChkInfo, NULL, &D_80BA1BC0);
    if (func_80BA0DF4(this, globalCtx) == 0) {
        Actor_Kill(&this->actor);
    } else {
        this->unk19C = Object_GetIndex(&globalCtx->objectCtx, D_80BA1B80[(this->actor.params >> 8) & 1]);
        if (this->unk19C < 0) {
            osSyncPrintf("Error : バンク危険！ (arg_data 0x%04x)(%s %d)\n", this->actor.params, "../z_obj_tsubo.c",
                         410);
            Actor_Kill(&this->actor);
        } else {
            func_80BA152C(this);
            osSyncPrintf("(dungeon keep 壷)(arg_data 0x%04x)\n", this->actor.params);
        }
    }
}

void ObjTsubo_Destroy(Actor* thisx, GlobalContext* globalCtx2) {
    GlobalContext* globalCtx = globalCtx2;
    ObjTsubo* this = (ObjTsubo*)thisx;

    Collider_DestroyCylinder(globalCtx, &this->unk150);
}

void func_80BA100C(ObjTsubo* this, GlobalContext* globalCtx) {
    s32 pad;
    f32 sin;
    f32 cos;
    Vec3f spC8;
    Vec3f spBC;
    f32 temp_fv0;
    s32 var_s0;
    s32 var_s2;
    s16 var_s1;

    for (var_s2 = 0, var_s1 = 0; var_s2 < 15; var_s2++, var_s1 += 0x4E20) {
        sin = Math_SinS(var_s1);
        cos = Math_CosS(var_s1);
        spC8.x = sin * 8.0f;
        spC8.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        spC8.z = cos * 8.0f;
        spBC.x = spC8.x * 0.23f;
        spBC.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        spBC.z = spC8.z * 0.23f;
        Math_Vec3f_Sum(&spC8, &this->actor.world.pos, &spC8);
        temp_fv0 = Rand_ZeroOne();
        if (temp_fv0 < 0.2f) {
            var_s0 = 0x60;
        } else if (temp_fv0 < 0.6f) {
            var_s0 = 0x40;
        } else {
            var_s0 = 0x20;
        }
        EffectSsKakera_Spawn(globalCtx, &spC8, &spBC, &this->actor.world.pos, -240, var_s0, 0xA, 0xA, 0,
                             (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 0x20, 60, -1,
                             D_80BA1B80[(this->actor.params >> 8) & 1], D_80BA1B8C[(this->actor.params >> 8) & 1]);
    }
    func_80033480(globalCtx, &this->actor.world.pos, 30.0f, 4, 0x14, 0x32, 1U);
}

void func_80BA1294(ObjTsubo* this, GlobalContext* globalCtx) {
    f32 temp_fs0;
    f32 temp_fs1;
    s16 var_s1;
    Vec3f spC8;
    Vec3f spBC;
    s32 var_s0;
    s32 var_s2;
    s32 pad[2];

    spC8 = this->actor.world.pos;
    spC8.y += this->actor.yDistToWater;
    EffectSsGSplash_Spawn(globalCtx, &spC8, NULL, NULL, 0, 0x190);

    for (var_s2 = 0, var_s1 = 0; var_s2 < 15; var_s2++, var_s1 += 0x4E20) {
        temp_fs0 = Math_SinS(var_s1);
        temp_fs1 = Math_CosS(var_s1);
        spC8.x = temp_fs0 * 8.0f;
        spC8.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        spC8.z = temp_fs1 * 8.0f;
        spBC.x = spC8.x * 0.20f;
        spBC.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        spBC.z = spC8.z * 0.20f;
        Math_Vec3f_Sum(&spC8, &this->actor.world.pos, &spC8);
        if (Rand_ZeroOne() < 0.2f) {
            var_s0 = 0x40;
        } else {
            var_s0 = 0x20;
        }
        EffectSsKakera_Spawn(globalCtx, &spC8, &spBC, &this->actor.world.pos, -180, var_s0, 0x1E, 0x1E, 0,
                             (Rand_ZeroOne() * 95.0f) + 15.0f, 0, 0x20, 70, -1,
                             D_80BA1B80[(this->actor.params >> 8) & 1], D_80BA1B8C[(this->actor.params >> 8) & 1]);
    }
}

void func_80BA152C(ObjTsubo* this) {
    this->actionFunc = func_80BA153C;
}

void func_80BA153C(ObjTsubo* this, GlobalContext* globalCtx) {
    if (Object_IsLoaded(&globalCtx->objectCtx, this->unk19C)) {
        this->actor.draw = func_80BA1B0C;
        this->actor.objBankIndex = this->unk19C;
        func_80BA15AC(this);
        this->actor.flags &= ~0x10;
    }
}

void func_80BA15AC(ObjTsubo* this) {
    this->actionFunc = func_80BA15BC;
}

void func_80BA15BC(ObjTsubo* this, GlobalContext* globalCtx) {
    s16 temp_v0;
    s16 var_v1;
    s32 pad;

    if (Actor_HasParent(&this->actor, globalCtx)) {
        func_80BA17C4(this);
    } else if ((this->actor.bgCheckFlags & 0x20) && (this->actor.yDistToWater > 15.0f)) {
        func_80BA1294(this, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        func_80BA0D60(this, globalCtx);
        Actor_Kill(&this->actor);
    } else if ((this->unk150.base.acFlags & AC_HIT) && (this->unk150.info.acHitInfo->toucher.dmgFlags & 0x4FC1FFFC)) {
        func_80BA100C(this, globalCtx);
        func_80BA0D60(this, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    } else {
        if (this->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&this->actor, &this->unk150);
            this->unk150.base.acFlags &= ~AC_HIT;
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk150.base);
            if (this->actor.xzDistToPlayer < 150.0f) {
                CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk150.base);
            }
        }
        if (this->actor.xzDistToPlayer < 100.0f) {
            temp_v0 = this->actor.yawTowardsPlayer - PLAYER->actor.world.rot.y;
            if (ABS(temp_v0) > 0x5555) {
                func_8002F434(&this->actor, globalCtx, GI_NONE, 30.0f, 30.0f);
            }
        }
    }
}

void func_80BA17C4(ObjTsubo* this) {
    this->actionFunc = func_80BA180C;
    this->actor.room = -1;
    func_8002F7DC(&this->actor, NA_SE_PL_PULL_UP_POT);
    this->actor.flags |= 0x10;
}

void func_80BA180C(ObjTsubo* this, GlobalContext* globalCtx) {
    if (Actor_HasNoParent(&this->actor, globalCtx)) {
        this->actor.room = globalCtx->roomCtx.curRoom.num;
        func_80BA188C(this);
        func_80BA0DC0(this);
        func_8002D7EC(&this->actor);
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 5.0f, 15.0f, 0.0f, 0x85);
    }
}

void func_80BA188C(ObjTsubo* this) {
    this->actor.velocity.x = Math_SinS(this->actor.world.rot.y) * this->actor.speedXZ;
    this->actor.velocity.z = Math_CosS(this->actor.world.rot.y) * this->actor.speedXZ;
    this->actor.colChkInfo.mass = 0xF0;
    D_80BA1B50 = (Rand_ZeroOne() - 0.7f) * 2800.0f;
    D_80BA1B58 = (Rand_ZeroOne() - 0.5f) * 2000.0f;
    D_80BA1B54 = 0;
    D_80BA1B5C = 0;
    this->actionFunc = func_80BA1958;
}

void func_80BA1958(ObjTsubo* this, GlobalContext* globalCtx) {
    s32 pad[2];

    if ((this->actor.bgCheckFlags & 0xB) || (this->unk150.base.atFlags & AT_HIT)) {
        func_80BA100C(this, globalCtx);
        func_80BA0D60(this, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    } else if (this->actor.bgCheckFlags & 0x40) {
        func_80BA1294(this, globalCtx);
        func_80BA0D60(this, globalCtx);
        Audio_PlaySoundAtPosition(globalCtx, &this->actor.world.pos, 20, NA_SE_EV_POT_BROKEN);
        Actor_Kill(&this->actor);
    } else {
        func_80BA0DC0(this);
        func_8002D7EC(&this->actor);
        Math_StepToS(&D_80BA1B54, D_80BA1B50, 0x64);
        Math_StepToS(&D_80BA1B5C, D_80BA1B58, 0x64);
        this->actor.shape.rot.x += D_80BA1B54;
        this->actor.shape.rot.y += D_80BA1B5C;
        Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 5.0f, 15.0f, 0.0f, 0x85);
        Collider_UpdateCylinder(&this->actor, &this->unk150);
        CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk150.base);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk150.base);
    }
}

void ObjTsubo_Update(Actor* thisx, GlobalContext* globalCtx) {
    ObjTsubo* this = (ObjTsubo*)thisx;

    this->actionFunc(this, globalCtx);
}

void func_80BA1B0C(Actor* thisx, GlobalContext* globalCtx) {
    Gfx_DrawDListOpa(globalCtx, D_80BA1B84[(thisx->params >> 8) & 1]);
}
