/*
 * File: z_en_wood02.c
 * Overlay: ovl_En_Wood02
 * Description: Trees & Bushes
 */

#include "z_en_wood02.h"

#define FLAGS 0x00000000

#define THIS ((EnWood02*)thisx)

void EnWood02_Init(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Update(Actor* thisx, GlobalContext* globalCtx);
void EnWood02_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 EnWood02_IsInUncullZone(EnWood02* this, GlobalContext* globalCtx, Vec3f* arg2);
void EnWood02_SpawnUnculledChildren(EnWood02* this, GlobalContext* globalCtx);

extern Gfx D_6000090[];
extern Gfx D_6000340[];
extern Gfx D_6000440[];
extern Gfx D_6000700[];
extern Gfx D_6007E20[];

const ActorInit En_Wood02_InitVars = {
    ACTOR_EN_WOOD02,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_WOOD02,
    sizeof(EnWood02),
    (ActorFunc)EnWood02_Init,
    (ActorFunc)EnWood02_Destroy,
    (ActorFunc)EnWood02_Update,
    (ActorFunc)EnWood02_Draw,
};
static ColliderCylinderInit D_80B3BF00 = {
    { 0xD, 0, 0xD, 0x39, 0x10, 1 },
    { 5, { 0, 0, 0 }, { 0x0FC0074A, 0, 0 }, 0, 1, 1 },
    { 0x12, 0x3C, 0, { 0, 0, 0 } },
};
static f32 sSpawnDistances[] = { 707.0f, 525.0f, 510.0f, 500.0f, 566.0f, 141.0f };
static s16 sSpawnAngles[] = { 0x1FFF, 0x4C9E, 0x77F5, 0xA5C9, -0x293D, 0xA000 };
static InitChainEntry sInitChain[] = {
    ICHAIN_F32(unk_4C, 5600, ICHAIN_STOP),
};
static Gfx* D_80B3BF54[7] = {
    (Gfx*)0x060078D0, (Gfx*)0x06007CA0, (Gfx*)0x060080D0, D_6000090, D_6000340, D_6000340, D_6000700,
};
static Gfx* D_80B3BF70[0xC] = {
    (Gfx*)0x06007968, (Gfx*)0x06007D38, (Gfx*)0x060081A8, NULL,      NULL,      NULL, (Gfx*)0x06007AD0,
    D_6007E20,        (Gfx*)0x06008350, (Gfx*)0x06000160, D_6000440, D_6000700,
};
static f32 sCos;
static f32 sSin;

s32 EnWood02_IsInUncullZone(EnWood02* this, GlobalContext* globalCtx, Vec3f* pos) {
    f32 invW;

    SkinMatrix_Vec3fMtxFMultXYZW(&globalCtx->mf_11D60, pos, &this->actor.projectedPos, &this->actor.projectedW);
    if (this->actor.projectedW == 0.0f) {
        invW = 1000.0f;
    } else {
        invW = fabsf(1.0f / this->actor.projectedW);
    }
    if ((-this->actor.uncullZoneScale < this->actor.projectedPos.z) &&
        (this->actor.projectedPos.z < (this->actor.uncullZoneForward + this->actor.uncullZoneScale)) &&
        (((fabsf(this->actor.projectedPos.x) - this->actor.uncullZoneScale) * invW) < 1.0f)) {
        if ((((this->actor.projectedPos.y + this->actor.uncullZoneDownward) * invW) > -1.0f) &&
            (((this->actor.projectedPos.y - this->actor.uncullZoneScale) * invW) < 1.0f)) {
            return true;
        }
    }
    return false;
}

void EnWood02_SpawnUnculledChildren(EnWood02* this, GlobalContext* globalCtx) {
    s32 pad[2];
    Vec3f newEnWood02Pos;
    EnWood02* newEnWood02;
    s16 spawnAngleModifier;
    s16 newEnWood02Params;
    s32 i;

    for (i = 4; i >= 0; i--) {
        if ((this->unk14E[i] & 0x7F) == 0) {
            spawnAngleModifier = 0;
            if (this->actor.params == EN_WOOD_02_TYPE_15) {
                spawnAngleModifier = 0x4000;
            }
            sCos = Math_CosS(sSpawnAngles[i] + this->actor.posRot.rot.y + spawnAngleModifier);
            sSin = Math_SinS(sSpawnAngles[i] + this->actor.posRot.rot.y + spawnAngleModifier);
            newEnWood02Pos.x = (sSpawnDistances[i] * sSin) + this->actor.initPosRot.pos.x;
            newEnWood02Pos.y = this->actor.initPosRot.pos.y;
            newEnWood02Pos.z = (sSpawnDistances[i] * sCos) + this->actor.initPosRot.pos.z;
            if (EnWood02_IsInUncullZone(this, globalCtx, &newEnWood02Pos)) {
                if (this->unk14E[i] & 0x80) {
                    newEnWood02Params = ((this->actor.params + 1) | 0xFF00);
                } else {
                    newEnWood02Params = (((this->unk154 & 0xF0) * 0x10) | (this->actor.params + 1));
                }
                newEnWood02 = (EnWood02*)Actor_SpawnAsChild(
                    &globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_WOOD02, newEnWood02Pos.x, newEnWood02Pos.y,
                    newEnWood02Pos.z, this->actor.posRot.rot.x, sSpawnAngles[i], 0, newEnWood02Params);
                if (newEnWood02 != NULL) {
                    newEnWood02->unk14E[0] = i;
                    this->unk14E[i] |= 1;
                    newEnWood02->actor.projectedPos = this->actor.projectedPos;
                } else {
                    this->unk14E[i] &= 0x80;
                }
            }
        }
    }
}

void EnWood02_Init(Actor* thisx, GlobalContext* globalCtx) {
    s16 var_t0_sp4E;
    f32 scale;
    EnWood02* this = (EnWood02*)thisx;
    f32 floorY;
    CollisionPoly* floorPoly;
    s32 floorBgId;
    s32 pad;
    s16 spawnAngleModifier;

    scale = 1.0f;
    var_t0_sp4E = 0;
    this->unk14C = (this->actor.params >> 8) & 0xFF;
    if (this->actor.initPosRot.rot.z != 0) {
        this->actor.shape.rot.z = 0;
        this->actor.initPosRot.rot.z = (this->actor.initPosRot.rot.z << 8) | this->unk14C;
        this->unk14C = -1;
        this->actor.posRot.rot.z = this->actor.shape.rot.z;
    } else if (this->unk14C & 0x80) {
        this->unk14C = -1;
    }
    this->actor.params = this->actor.params & 0xFF;
    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (this->actor.params < EN_WOOD_02_TYPE_11) {
        Collider_InitCylinder(globalCtx, &this->unk158);
        Collider_SetCylinder(globalCtx, &this->unk158, &this->actor, &D_80B3BF00);
        var_t0_sp4E = 0;
    }
    switch (this->actor.params) {
        case EN_WOOD_02_TYPE_15:
        case EN_WOOD_02_TYPE_21:
            var_t0_sp4E = 1;
        case EN_WOOD_02_TYPE_16:
        case EN_WOOD_02_TYPE_22:
            var_t0_sp4E += 1;
        case EN_WOOD_02_TYPE_0:
        case EN_WOOD_02_TYPE_12:
        case EN_WOOD_02_TYPE_18:
            scale = 1.5f;
            this->actor.uncullZoneForward = 4000.0f;
            this->actor.uncullZoneScale = 2000.0f;
            this->actor.uncullZoneDownward = 2400.0f;
            break;

        case EN_WOOD_02_TYPE_3:
        case EN_WOOD_02_TYPE_6:
        case EN_WOOD_02_TYPE_8:
        case EN_WOOD_02_TYPE_13:
        case EN_WOOD_02_TYPE_19:
            var_t0_sp4E = 1;
        case EN_WOOD_02_TYPE_4:
        case EN_WOOD_02_TYPE_7:
        case EN_WOOD_02_TYPE_9:
        case EN_WOOD_02_TYPE_14:
        case EN_WOOD_02_TYPE_20:
            var_t0_sp4E += 1;
        case EN_WOOD_02_TYPE_1:
        case EN_WOOD_02_TYPE_5:
        case EN_WOOD_02_TYPE_10:
        case EN_WOOD_02_TYPE_11:
        case EN_WOOD_02_TYPE_17:
            this->actor.uncullZoneForward = 4000.0f;
            this->actor.uncullZoneScale = 800.0f;
            this->actor.uncullZoneDownward = 1800.0f;
            break;

        case EN_WOOD_02_TYPE_2:
            scale = 0.6f;
            this->actor.uncullZoneForward = 4000.0f;
            this->actor.uncullZoneScale = 400.0f;
            this->actor.uncullZoneDownward = 1000.0f;
            break;

        case EN_WOOD_02_TYPE_23:
        case EN_WOOD_02_TYPE_24:
            this->unk14E[0] = 0x4B;
            scale = 0.02f;
            this->actor.velocity.x = Rand_CenteredFloat(6.0f);
            this->actor.velocity.z = Rand_CenteredFloat(6.0f);
            var_t0_sp4E = 0;
            this->actor.velocity.y = (Rand_ZeroOne() * 1.25f) + -3.1f;
            break;
    }
    if (this->actor.params <= EN_WOOD_02_TYPE_4) {
        this->unk154 = 0;
    } else if (this->actor.params <= EN_WOOD_02_TYPE_9) {
        this->unk154 = 1;
    } else if (this->actor.params <= EN_WOOD_02_TYPE_10) {
        this->unk154 = 2;
    } else if (this->actor.params <= EN_WOOD_02_TYPE_16) {
        this->unk154 = 3;
    } else if (this->actor.params <= EN_WOOD_02_TYPE_23) {
        this->unk154 = 4;
    } else {
        this->unk154 = 5;
    }
    Actor_SetScale(&this->actor, scale);
    this->unk153 = var_t0_sp4E;
    if (var_t0_sp4E != 0) {
        spawnAngleModifier = 0;
        if (this->actor.params == EN_WOOD_02_TYPE_15) {
            spawnAngleModifier = 0x4000;
        }
        if (var_t0_sp4E == 2) {
            this->unk154 |= this->unk14C * 0x10;
            EnWood02_SpawnUnculledChildren(this, globalCtx);
            sCos = Math_CosS(sSpawnAngles[5] + this->actor.posRot.rot.y + spawnAngleModifier);
            sSin = Math_SinS(sSpawnAngles[5] + this->actor.posRot.rot.y + spawnAngleModifier);
            this->actor.posRot.pos.x += sSin * sSpawnDistances[5];
            this->actor.posRot.pos.z += sCos * sSpawnDistances[5];
        } else {
            this->actor.flags |= 0x10;
        }
        this->actor.posRot.pos.y += 200.0f;
        floorY = BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &floorPoly, &floorBgId, &this->actor,
                                             &this->actor.posRot.pos);
        if (floorY > BGCHECK_Y_MIN) {
            this->actor.posRot.pos.y = floorY;
        } else {
            Actor_Kill(&this->actor);
            return;
        }
    }
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
    this->actor.initPosRot.rot.y = 0;
    this->actor.colChkInfo.mass = 0xFF;
}

void EnWood02_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnWood02* this = (EnWood02*)thisx;

    if (this->actor.params < EN_WOOD_02_TYPE_11) {
        Collider_DestroyCylinder(globalCtx, &this->unk158);
    }
}

void EnWood02_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad2;
    EnWood02* this = (EnWood02*)thisx;
    f32 sp6C;
    Player* player;
    s32 var_s0;
    Vec3f sp58;
    s32 pad;
    s32 var_v1_sp44_or_sp50;
    u8 v;
    u8 var_v0;

    if ((this->unk153 == 1) && (this->actor.parent != NULL)) {
        if (!(this->actor.flags & 0x40)) {
            v = this->unk14E[0];
            var_v0 = 0;
            if (this->unk14C < 0) {
                var_v0 = 0x80;
            }
            ((EnWood02*)this->actor.parent)->unk14E[v] = var_v0;
            Actor_Kill(&this->actor);
            return;
        }
    } else if (this->unk153 == 2) {
        EnWood02_SpawnUnculledChildren(this, globalCtx);
    }
    if (thisx->params < EN_WOOD_02_TYPE_11) {
        if (this->unk158.base.acFlags & 2) {
            this->unk158.base.acFlags &= ~2;
            Audio_PlayActorSound2(&this->actor, NA_SE_IT_REFLECTION_WOOD);
        }
        if (this->actor.initPosRot.rot.y != 0) {
            sp58 = this->actor.posRot.pos;
            sp58.y += 200.0f;
            if ((this->unk14C >= 0) && (this->unk14C < 0x64)) {
                Item_DropCollectibleRandom(globalCtx, &this->actor, &sp58, this->unk14C * 0x10);
            } else if (this->actor.initPosRot.rot.z != 0) {
                this->actor.initPosRot.rot.z &= 0x1FFF;
                this->actor.initPosRot.rot.z |= 0xE000;
                Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_SW, sp58.x, sp58.y, sp58.z, 0,
                            this->actor.posRot.rot.y, 0, this->actor.initPosRot.rot.z);
                this->actor.initPosRot.rot.z = 0;
            }
            var_v1_sp44_or_sp50 = EN_WOOD_02_TYPE_23;
            if (this->unk14C >= -1) {
                if ((this->actor.params == EN_WOOD_02_TYPE_6) || (this->actor.params == EN_WOOD_02_TYPE_7)) {
                    var_v1_sp44_or_sp50 = EN_WOOD_02_TYPE_24;
                }
                Audio_PlayActorSound2(&this->actor, NA_SE_EV_TREE_SWING);
                for (var_s0 = 3; var_s0 >= 0; var_s0--) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_WOOD02, sp58.x, sp58.y, sp58.z, 0,
                                (s16)(s32)Rand_CenteredFloat(65535.0f), 0, var_v1_sp44_or_sp50);
                }
            }
            this->unk14C = -21;
            this->actor.initPosRot.rot.y = 0;
        }
        if (this->actor.xzDistToLink < 600.0f) {
            Collider_CylinderUpdate(&this->actor, &this->unk158);
            CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk158.base);
            CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk158.base);
        }
    } else if (this->actor.params < EN_WOOD_02_TYPE_23) {
        player = PLAYER;
        if ((this->unk14C >= (-1)) && (((((player->rideActor == NULL)) && (sqrt(this->actor.xyzDistToLinkSq) < 20.0)) &&
                                        (player->linearVelocity != 0.0f)) ||
                                       (((player->rideActor != NULL) && (sqrt(this->actor.xyzDistToLinkSq) < 60.0)) &&
                                        (player->rideActor->speedXZ != 0.0f)))) {
            if ((this->unk14C >= 0) && (this->unk14C < 0x64)) {
                Item_DropCollectibleRandom(globalCtx, &this->actor, &this->actor.posRot.pos,
                                           (this->unk14C * 0x10) | 0x8000);
            }
            this->unk14C = -21;
            Audio_PlayActorSound2(&this->actor, NA_SE_EV_TREE_SWING);
        }
    } else {
        this->unk14C += 1;
        Math_ApproachF(&this->actor.velocity.x, 0.0f, 1.0f, 0.049999997f);
        Math_ApproachF(&this->actor.velocity.z, 0.0f, 1.0f, 0.049999997f);
        func_8002D7EC(&this->actor);
        this->actor.shape.rot.z = (s16)(s32)(Math_SinS((s16)(this->unk14C * 0xBB8)) * 16384.0f);
        this->unk14E[0] -= 1;
        if (this->unk14E[0] == 0) {
            Actor_Kill(&this->actor);
        }
    }
    if (this->unk14C < -1) {
        this->unk14C += 1;
        sp6C = Math_SinS((this->unk14C ^ 0xFFFF) * 0x3332) * 250.0f;
        this->actor.shape.rot.x = Math_CosS(this->actor.yawTowardsLink - this->actor.shape.rot.y) * sp6C;
        this->actor.shape.rot.z = Math_SinS(this->actor.yawTowardsLink - this->actor.shape.rot.y) * sp6C;
    }
}

void EnWood02_Draw(Actor* thisx, GlobalContext* globalCtx) {
    GraphicsContext* gfxCtx;
    s16 type;
    EnWood02* this = (EnWood02*)thisx;
    u8 r;
    u8 g;
    u8 b;

    gfxCtx = globalCtx->state.gfxCtx;
    OPEN_DISPS(gfxCtx, "../z_en_wood02.c", 775);
    type = this->actor.params;
    if ((type == EN_WOOD_02_TYPE_8) || (type == EN_WOOD_02_TYPE_9) || (type == EN_WOOD_02_TYPE_5) ||
        (type == EN_WOOD_02_TYPE_23)) {
        r = 50;
        g = 170;
        b = 70;
    } else if ((type == EN_WOOD_02_TYPE_6) || (type == EN_WOOD_02_TYPE_7) || ((type == EN_WOOD_02_TYPE_24))) {
        r = 180;
        g = 155;
        b = 0;
    } else {
        r = g = b = 255;
    }
    func_80093D84(gfxCtx);
    if ((this->actor.params == EN_WOOD_02_TYPE_23) || (this->actor.params == EN_WOOD_02_TYPE_24)) {
        func_80093D18(gfxCtx);
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x00, r, g, b, 127);
        Gfx_DrawDListOpa(globalCtx, D_6000700);
    } else {
        if (D_80B3BF70[this->unk154 & 0xF] != NULL) {
            Gfx_DrawDListOpa(globalCtx, D_80B3BF54[this->unk154 & 0xF]);
            gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, 0);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_wood02.c", 808),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, D_80B3BF70[this->unk154 & 0xF]);
        } else {
            func_80093D84(gfxCtx);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_en_wood02.c", 814),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, D_80B3BF54[this->unk154 & 0xF]);
        }
    }
    CLOSE_DISPS(gfxCtx, "../z_en_wood02.c", 840);
}
