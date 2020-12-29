/*
 * File: z_bg_mizu_bwall.c
 * Overlay: ovl_Bg_Mizu_Bwall
 * Description: Water Temple bombable walls
 */

#include "z_bg_mizu_bwall.h"

#define FLAGS 0x00000010

#define THIS ((BgMizuBwall*)thisx)

void BgMizuBwall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMizuBwall_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMizuBwall_Idle(BgMizuBwall* this, GlobalContext* globalCtx);
void BgMizuBwall_Break(BgMizuBwall* this, GlobalContext* globalCtx);
void BgMizuBwall_DoNothing(BgMizuBwall* this, GlobalContext* globalCtx);

const ActorInit Bg_Mizu_Bwall_InitVars = {
    ACTOR_BG_MIZU_BWALL,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuBwall),
    (ActorFunc)BgMizuBwall_Init,
    (ActorFunc)BgMizuBwall_Destroy,
    (ActorFunc)BgMizuBwall_Update,
    (ActorFunc)BgMizuBwall_Draw,
};

static ColliderTrisElementSrc sTrisElementInitFloor[2] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            {
                0x00000000,
                HIT_SPECIAL_EFFECT_NONE,
                0,
            },
            {
                0x00000008,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        {
            {
                -40.0,
                0.0,
                -40.0,
            }, // 0
            {
                -40.0,
                0.0,
                40.0,
            }, // 1
            {
                40.0,
                0.0,
                40.0,
            }, // 2
        },
    }, // 0
    {
        {
            ELEM_MATERIAL_UNK0,
            {
                0x00000000,
                HIT_SPECIAL_EFFECT_NONE,
                0,
            },
            {
                0x00000008,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        {
            {
                -40.0,
                0.0,
                -40.0,
            }, // 0
            {
                40.0,
                0.0,
                40.0,
            }, // 1
            {
                40.0,
                0.0,
                -40.0,
            }, // 2
        },
    }, // 1
};

static ColliderTrisSrc sTrisInitFloor = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLTYPE_TRIANGLES,
    },
    2,
    sTrisElementInitFloor,
};

static ColliderTrisElementSrc sTrisElementInitRutoWall[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            {
                0x00000000,
                HIT_SPECIAL_EFFECT_NONE,
                0,
            },
            {
                0x00000008,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        {
            {
                0.0,
                116.0,
                0.0,
            }, // 0
            {
                0.0,
                0.0,
                70.0,
            }, // 1
            {
                0.0,
                0.0,
                -70.0,
            }, // 2
        },
    }, // 0
};

static ColliderTrisSrc sTrisInitRutoWall = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLTYPE_TRIANGLES,
    },
    1,
    sTrisElementInitRutoWall,
};

static ColliderTrisElementSrc sTrisElementInitWall[2] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            {
                0x00000000,
                HIT_SPECIAL_EFFECT_NONE,
                0,
            },
            {
                0x00000008,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        {
            {
                0.0,
                120.0,
                0.0,
            }, // 0
            {
                0.0,
                0.0,
                0.0,
            }, // 1
            {
                60.0,
                0.0,
                0.0,
            }, // 2
        },
    }, // 0
    {
        {
            ELEM_MATERIAL_UNK0,
            {
                0x00000000,
                HIT_SPECIAL_EFFECT_NONE,
                0,
            },
            {
                0x00000008,
                HIT_BACKLASH_NONE,
                0,
            },
            ATELEM_NONE,
            ACELEM_ON,
            OCELEM_NONE,
        },
        {
            {
                0.0,
                120.0,
                0.0,
            }, // 0
            {
                60.0,
                0.0,
                0.0,
            }, // 1
            {
                60.0,
                120.0,
                0.0,
            }, // 2
        },
    }, // 1
};

static ColliderTrisSrc sTrisInitUnusedWall = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLTYPE_TRIANGLES,
    },
    2,
    sTrisElementInitWall,
};

static ColliderTrisSrc sTrisInitStingerWall = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLTYPE_TRIANGLES,
    },
    2,
    sTrisElementInitWall,
};

static Gfx* sDLists[] = {
    0x06001A30, 0x06002390, 0x06001CD0, 0x06002090, 0x06001770,
};
static ColHeader* sColHeaders[] = {
    0x06001C58, 0x060025A4, 0x06001DE8, 0x06001DE8, 0x06001DE8,
};

static InitChainEntry D_8089D854[] = {
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void BgMizuBwall_RotateVec3f(Vec3f* out, Vec3f* in, f32 sin, f32 cos) {
    out->x = (in->z * sin) + (in->x * cos);
    out->y = in->y;
    out->z = (in->z * cos) - (in->x * sin);
}

void BgMizuBwall_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* this = THIS;
    ColHeader* colHeader = NULL;

    Actor_ProcessInitChain(&this->dyna.actor, D_8089D854);
    this->yRot = this->dyna.actor.posRot.pos.y;
    this->dList = sDLists[(u16)this->dyna.actor.params & 0xF];
    DynaPolyInfo_SetActorMove(&this->dyna, 1);
    DynaPolyInfo_Alloc(sColHeaders[(u16)this->dyna.actor.params & 0xF], &colHeader);
    this->dyna.dynaPolyId =
        DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);

    switch ((u16)this->dyna.actor.params & 0xF) {
        case MIZUBWALL_FLOOR:
            if (Flags_GetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
                this->dList = NULL;
                this->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &this->collider);
                if (!Collider_LoadTris(globalCtx, &this->collider, &this->dyna.actor, &sTrisInitFloor, this->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 484, this->dyna.actor.params);
                    Actor_Kill(&this->dyna.actor);
                } else {
                    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            offset.x = sTrisInitFloor.elements[i].vertices[j].x;
                            offset.y = sTrisInitFloor.elements[i].vertices[j].y;
                            offset.z = sTrisInitFloor.elements[i].vertices[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += this->dyna.actor.posRot.pos.x;
                            vtx[j].y += this->dyna.actor.posRot.pos.y;
                            vtx[j].z += this->dyna.actor.posRot.pos.z;
                        }
                        func_800627A0(&this->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    this->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_RUTO_ROOM:
            if (Flags_GetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
                this->dList = NULL;
                this->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &this->collider);
                if (!Collider_LoadTris(globalCtx, &this->collider, &this->dyna.actor, &sTrisInitRutoWall,
                                      this->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 558, this->dyna.actor.params);
                    Actor_Kill(&this->dyna.actor);
                } else {
                    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitRutoWall); i++) {
                        for (j = 0; j < 3; j++) {
                            offset.x = sTrisInitRutoWall.elements[i].vertices[j].x;
                            offset.y = sTrisInitRutoWall.elements[i].vertices[j].y;
                            offset.z = sTrisInitRutoWall.elements[i].vertices[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += this->dyna.actor.posRot.pos.x;
                            vtx[j].y += this->dyna.actor.posRot.pos.y;
                            vtx[j].z += this->dyna.actor.posRot.pos.z;
                        }
                        func_800627A0(&this->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    this->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_UNUSED:
            if (Flags_GetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
                this->dList = NULL;
                this->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &this->collider);
                if (!Collider_LoadTris(globalCtx, &this->collider, &this->dyna.actor, &sTrisInitUnusedWall,
                                      this->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 638, this->dyna.actor.params);
                    Actor_Kill(&this->dyna.actor);
                } else {
                    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitUnusedWall.
                            offset.x = sTrisInitFloor.elements[i].vertices[j].x;
                            offset.y = sTrisInitFloor.elements[i].vertices[j].y;
                            offset.z = sTrisInitFloor.elements[i].vertices[j].z;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += this->dyna.actor.posRot.pos.x;
                            vtx[j].y += this->dyna.actor.posRot.pos.y;
                            vtx[j].z += this->dyna.actor.posRot.pos.z;
                        }
                        func_800627A0(&this->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    this->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_STINGER_ROOM_1:
            if (Flags_GetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
                this->dList = NULL;
                this->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &this->collider);
                if (!Collider_LoadTris(globalCtx, &this->collider, &this->dyna.actor, &sTrisInitStingerWall,
                                      this->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 724, this->dyna.actor.params);
                    Actor_Kill(&this->dyna.actor);
                } else {
                    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitStingerWall.
                            offset.x = sTrisInitFloor.elements[i].vertices[j].x;
                            offset.y = sTrisInitFloor.elements[i].vertices[j].y;
                            offset.z = sTrisInitFloor.elements[i].vertices[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += this->dyna.actor.posRot.pos.x;
                            vtx[j].y += this->dyna.actor.posRot.pos.y;
                            vtx[j].z += this->dyna.actor.posRot.pos.z;
                        }
                        func_800627A0(&this->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    this->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
        case MIZUBWALL_STINGER_ROOM_2:
            if (Flags_GetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F)) {
                func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
                this->dList = NULL;
                this->actionFunc = BgMizuBwall_DoNothing;
            } else {
                Collider_InitTris(globalCtx, &this->collider);
                if (!Collider_LoadTris(globalCtx, &this->collider, &this->dyna.actor, &sTrisInitStingerWall,
                                      this->elements)) {
                    osSyncPrintf("Error : コリジョンデータセット失敗(%s %d)(arg_data 0x%04x)\n", "../z_bg_mizu_bwall.c",
                                 798, this->dyna.actor.params);
                    Actor_Kill(&this->dyna.actor);
                } else {
                    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
                    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
                    s32 i;
                    s32 j;
                    Vec3f offset;
                    Vec3f vtx[3];

                    for (i = 0; i < ARRAY_COUNT(sTrisElementInitFloor); i++) {
                        for (j = 0; j < 3; j++) {
                            //! @bug This uses the wrong set of collision triangles, causing the collider to be
                            //!      flat to the ground instead of vertical. It should use sTrisInitStingerWall.
                            offset.x = sTrisInitFloor.elements[i].vertices[j].x;
                            offset.y = sTrisInitFloor.elements[i].vertices[j].y;
                            offset.z = sTrisInitFloor.elements[i].vertices[j].z + 2.0f;
                            BgMizuBwall_RotateVec3f(&vtx[j], &offset, sin, cos);
                            vtx[j].x += this->dyna.actor.posRot.pos.x;
                            vtx[j].y += this->dyna.actor.posRot.pos.y;
                            vtx[j].z += this->dyna.actor.posRot.pos.z;
                        }
                        func_800627A0(&this->collider, i, &vtx[0], &vtx[1], &vtx[2]);
                    }
                    this->actionFunc = BgMizuBwall_Idle;
                }
            }
            break;
    }
}

void BgMizuBwall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* this = THIS;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
    Collider_DestroyTris(globalCtx, &this->collider);
}

void BgMizuBwall_SetAlpha(BgMizuBwall* this, GlobalContext* globalCtx) {
    f32 waterLevel = globalCtx->colCtx.stat.colHeader->waterBoxes[2].ySurface;
    s32 alphaMod;

    if (globalCtx->colCtx.stat.colHeader->waterBoxes) {}

    if (waterLevel < -15.0f) {
        this->scrollAlpha1 = 255;
    } else if (waterLevel < 445.0f) {
        alphaMod = ((waterLevel - -15.0f) / (445.0f - -15.0f)) * 95.0f;
        this->scrollAlpha1 = 255 - alphaMod;
    } else {
        this->scrollAlpha1 = 160;
    }
    if (waterLevel < 445.0f) {
        this->scrollAlpha2 = 255;
    } else if (waterLevel < 765.0f) {
        alphaMod = ((waterLevel - 445.0f) / (765.0f - 445.0f)) * 95.0f;
        this->scrollAlpha2 = 255 - alphaMod;
    } else {
        this->scrollAlpha2 = 160;
    }
    if (waterLevel < -835.0f) {
        this->scrollAlpha3 = 255;
    } else if (waterLevel < -15.0f) {
        alphaMod = ((waterLevel - -835.0f) / (-15.0f - -835.0f)) * 95.0f;
        this->scrollAlpha3 = 255 - alphaMod;
    } else {
        this->scrollAlpha3 = 160;
    }
    this->scrollAlpha4 = this->scrollAlpha3;
}

void BgMizuBwall_SpawnDebris(BgMizuBwall* this, GlobalContext* globalCtx) {
    s32 i;
    s32 pad;
    s16 rand1;
    s16 rand2;
    Vec3f* thisPos = &this->dyna.actor.posRot.pos;
    Vec3f debrisPos;
    f32 tempx;
    f32 tempz;
    f32 sin = Math_SinS(this->dyna.actor.shape.rot.y);
    f32 cos = Math_CosS(this->dyna.actor.shape.rot.y);
    Vec3f debrisOffsets[15];

    for (i = 0; i < ARRAY_COUNT(debrisOffsets); i++) {
        switch ((u16)this->dyna.actor.params & 0xF) {
            case MIZUBWALL_FLOOR:
                debrisOffsets[i].x = (Rand_ZeroOne() * 80.0f) - 40.0f;
                debrisOffsets[i].y = Rand_ZeroOne() * 0;
                debrisOffsets[i].z = (Rand_ZeroOne() * 80.0f) - 40.0f;
                break;
            case MIZUBWALL_RUTO_ROOM:
                debrisOffsets[i].x = Rand_ZeroOne() * 0;
                debrisOffsets[i].y = Rand_ZeroOne() * 100.0f;
                debrisOffsets[i].z = (Rand_ZeroOne() * 80.0f) - 40.0f;
                break;
            case MIZUBWALL_UNUSED:
            case MIZUBWALL_STINGER_ROOM_1:
            default:
                debrisOffsets[i].x = (Rand_ZeroOne() * 120) - 60.0f;
                debrisOffsets[i].y = Rand_ZeroOne() * 120;
                debrisOffsets[i].z = Rand_ZeroOne() * 0;
                break;
        }
    }

    for (i = 0; i < ARRAY_COUNT(debrisOffsets); i++) {
        tempx = debrisOffsets[i].x;
        tempz = debrisOffsets[i].z;

        debrisPos.x = thisPos->x + tempz * sin + tempx * cos;
        debrisPos.y = thisPos->y + debrisOffsets[i].y;
        debrisPos.z = thisPos->z + tempz * cos - tempx * sin;

        rand1 = (s16)(Rand_ZeroOne() * 120.0f) + 20;
        rand2 = (s16)(Rand_ZeroOne() * 240.0f) + 20;
        func_80033480(globalCtx, &debrisPos, 50.0f, 2, rand1, rand2, 0);
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_A_OBJ, debrisPos.x, debrisPos.y, debrisPos.z, 0, 0, 0,
                    0xB);
    }
}

void BgMizuBwall_Idle(BgMizuBwall* this, GlobalContext* globalCtx) {
    BgMizuBwall_SetAlpha(this, globalCtx);
    if (this->collider.base.acFlags & 2) {
        this->collider.base.acFlags &= ~2;
        Flags_SetSwitch(globalCtx, ((u16)this->dyna.actor.params >> 8) & 0x3F);
        this->breakTimer = 1;
        func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
        this->dList = NULL;
        BgMizuBwall_SpawnDebris(this, globalCtx);
        Audio_PlayActorSound2(&this->dyna.actor, NA_SE_EV_WALL_BROKEN);
        Audio_PlaySoundGeneral(NA_SE_SY_CORRECT_CHIME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
        this->actionFunc = BgMizuBwall_Break;
    } else if (this->dyna.actor.xzDistFromLink < 600.0f) {
        Collider_AddAC(globalCtx, &globalCtx->colliderCtx, &this->collider.base);
    }
}

void BgMizuBwall_Break(BgMizuBwall* this, GlobalContext* globalCtx) {
    if (this->breakTimer > 0) {
        this->breakTimer--;
    } else {
        this->actionFunc = BgMizuBwall_DoNothing;
    }
}

void BgMizuBwall_DoNothing(BgMizuBwall* this, GlobalContext* globalCtx) {
}

void BgMizuBwall_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    BgMizuBwall* this = THIS;

    this->actionFunc(this, globalCtx);
}

void BgMizuBwall_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    BgMizuBwall* this = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1095);
    if (1) {}
    frames = globalCtx->gameplayFrames;

    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, this->scrollAlpha1));
    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, this->scrollAlpha2));
    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 1 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, this->scrollAlpha3));
    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, 3 * frames, 0, 0x20, 0x20, 1, 0, 0, 0x20, 0x20, 0,
                                        0, 0, this->scrollAlpha4));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1129), 2);

    if (this->dList != NULL) {
        gSPDisplayList(POLY_OPA_DISP++, this->dList);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_bwall.c", 1136);
}
