/*
 * File: z_bg_mizu_movebg.c
 * Overlay: ovl_Bg_Mizu_Movebg
 * Description: Kakariko Village Well Water
 */

#include "z_bg_mizu_movebg.h"
#include "overlays/actors/ovl_Bg_Mizu_Water/z_bg_mizu_water.h"
#include "objects/object_mizu_objects/object_mizu_objects.h"

#define FLAGS 0x00000010

#define THIS ((BgMizuMovebg*)thisx)

#define MOVEBG_TYPE(params) (((u16)(params) >> 0xC) & 0xF)
#define MOVEBG_FLAGS(params) ((u16)(params)&0x3F)
#define MOVEBG_PATH_ID(params) (((u16)(params) >> 0x8) & 0xF)
#define MOVEBG_POINT_ID(params) ((u16)(params)&0xF)
#define MOVEBG_SPEED(params) (((u16)(params) >> 0x4) & 0xF)

void BgMizuMovebg_Init(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Update(Actor* thisx, GlobalContext* globalCtx);
void BgMizuMovebg_Draw(Actor* thisx, GlobalContext* globalCtx);

void BgMizuMovebg_Move__(BgMizuMovebg* this, GlobalContext* globalCtx);
void BgMizuMovebg_FollowPath_(BgMizuMovebg* this, GlobalContext* globalCtx);
s32 BgMizuMovebg_GetPathPointPos(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId);

const ActorInit Bg_Mizu_Movebg_InitVars = {
    ACTOR_BG_MIZU_MOVEBG,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuMovebg),
    (ActorFunc)BgMizuMovebg_Init,
    (ActorFunc)BgMizuMovebg_Destroy,
    (ActorFunc)BgMizuMovebg_Update,
    (ActorFunc)BgMizuMovebg_Draw,
};

static f32 sPillarYoffsetByWaterLevel_[] = { -115.200005f, -115.200005f, -115.200005f, 0.0f };

static Gfx* sDLists[] = {
    /* 0 */ gObjectMizuObjectsMovebgPlatformSlantedDL,
    /* 1 */ gObjectMizuObjectsMovebgPlatformDL,
    /* 2 */ gObjectMizuObjectsMovebgPlatformTallerDL,
    /* 3 */ gObjectMizuObjectsMovebgHookshotPillarDL,
    /* 4 */ gObjectMizuObjectsMovebgHookshotPillarDL,
    /* 5 */ gObjectMizuObjectsMovebgHookshotPillarDL,
    /* 6 */ gObjectMizuObjectsMovebgHookshotPillarDL,
    /* 7 */ gObjectMizuObjectsMovebgPlatformWithHookshotTargetDL,
};

static CollisionHeader* sColHeaders[] = {
    /* 0 */ &gObjectMizuObjectsMovebgCol_0003F0,
    /* 1 */ &gObjectMizuObjectsMovebgCol_000998,
    /* 2 */ &gObjectMizuObjectsMovebgCol_000ED0,
    /* 3 */ &gObjectMizuObjectsMovebgCol_003590,
    /* 4 */ &gObjectMizuObjectsMovebgCol_003590,
    /* 5 */ &gObjectMizuObjectsMovebgCol_003590,
    /* 6 */ &gObjectMizuObjectsMovebgCol_003590,
    /* 7 */ &gObjectMizuObjectsMovebgCol_0015F8,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

static Vec3f sHookshotTargetRelSpawnPos = { 0.0f, 80.0f, 23.0f };
static Vec3f sHookshotTargetRelPos = { 0.0f, 80.0f, 23.0f };

#define BGMIZU_SFXFLAG_0 1
#define BGMIZU_SFXFLAG_1 2
static u8 sActiveSfxFlags;

static BgMizuMovebg* sMode421Parts[6];

s32 BgMizuMovebg_GetWaterLevelIndex_(GlobalContext* globalCtx) {
    s32 result;

    if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F1_FLAG)) {
        result = 1;
    } else if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F2_FLAG)) {
        result = 2;
    } else if (Flags_GetSwitch(globalCtx, WATER_TEMPLE_WATER_F3_FLAG)) {
        result = 3;
    } else {
        result = 1;
    }
    return result;
}

void BgMizuMovebg_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 type;
    s32 waypointId;
    WaterBox* waterBoxes = globalCtx->colCtx.colHeader->waterBoxes;
    f32 y;
    CollisionHeader* colHeader = NULL;
    Vec3f hookshotTargetRelSpawnPos;

    if ((thisx->params & 0xFFF) == 0x421) {
        s16 partParams[] = {
            BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER << 12,
            BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG << 12,
            BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG << 12,
            BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG << 12,
            BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG << 12,
            BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG << 12,
        };

        THIS->mode421 = true;
        THIS->part = thisx->params >> 12;
        thisx->params = partParams[THIS->part];
        sMode421Parts[THIS->part] = THIS;
    }

    Actor_ProcessInitChain(thisx, sInitChain);
    THIS->homeY = thisx->world.pos.y;
    THIS->dlist = sDLists[MOVEBG_TYPE(thisx->params)];
    DynaPolyActor_Init(&THIS->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(sColHeaders[MOVEBG_TYPE(thisx->params)], &colHeader);
    THIS->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);

    if (globalCtx->colCtx.colHeader->numWaterBoxes >= 3) {
        y = waterBoxes[2].ySurface + 15.0f;
    } else {
        y = THIS->homeY;
    }

    type = MOVEBG_TYPE(thisx->params);
    switch (type) {
        case BGMIZUMOVEBG_TYPE_0_PLATFORM_SLANTED:
            if (y < THIS->homeY - 700.0f) {
                thisx->world.pos.y = THIS->homeY - 700.0f;
            } else {
                thisx->world.pos.y = y;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_1_PLATFORM:
            if (y < THIS->homeY - 710.0f) {
                thisx->world.pos.y = THIS->homeY - 710.0f;
            } else {
                thisx->world.pos.y = y;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER:
            if (y < THIS->homeY - 700.0f) {
                thisx->world.pos.y = THIS->homeY - 700.0f;
            } else {
                thisx->world.pos.y = y;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
            thisx->world.pos.y =
                THIS->homeY + sPillarYoffsetByWaterLevel_[BgMizuMovebg_GetWaterLevelIndex_(globalCtx)];
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG:
            if (Flags_GetSwitch(globalCtx, MOVEBG_FLAGS(thisx->params))) {
                thisx->world.pos.y = THIS->homeY + 115.19999999999999;
            } else {
                thisx->world.pos.y = THIS->homeY;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_7_MOVING_PLATFORM:
            THIS->scrollAlpha1 = 160;
            THIS->scrollAlpha2 = 160;
            THIS->scrollAlpha3 = 160;
            THIS->scrollAlpha4 = 160;
            waypointId = MOVEBG_POINT_ID(thisx->params);
            THIS->waypointId = waypointId;
            BgMizuMovebg_GetPathPointPos(globalCtx->setupPathList, &thisx->world.pos,
                                         MOVEBG_PATH_ID(thisx->params), waypointId);
            THIS->actionFunc = BgMizuMovebg_FollowPath_;
            break;
    }

    type = MOVEBG_TYPE(thisx->params);
    if (THIS->mode421 && THIS->part == BGMIZUMOVEBG_421PART_BODY) {
        type = BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL;
    }
    switch (type) {
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
        case BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG:
            Matrix_RotateY(thisx->world.rot.y * (M_PI / 32768), MTXMODE_NEW);
            Matrix_MultVec3f(&sHookshotTargetRelSpawnPos, &hookshotTargetRelSpawnPos);

            if (Actor_SpawnAsChild(&globalCtx->actorCtx, thisx, globalCtx, ACTOR_OBJ_HSBLOCK,
                                   thisx->world.pos.x + hookshotTargetRelSpawnPos.x,
                                   thisx->world.pos.y + hookshotTargetRelSpawnPos.y,
                                   thisx->world.pos.z + hookshotTargetRelSpawnPos.z,
                                   thisx->world.rot.x, thisx->world.rot.y,
                                   thisx->world.rot.z, 2) == NULL) {
                Actor_Kill(thisx);
            }
    }
}

void BgMizuMovebg_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuMovebg* this = THIS;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);
    switch (MOVEBG_TYPE(thisx->params)) {
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
        case BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG:
            if (this->sfxFlags & BGMIZU_SFXFLAG_1) {
                sActiveSfxFlags &= ~BGMIZU_SFXFLAG_1;
            }
            break;
        case BGMIZUMOVEBG_TYPE_7_MOVING_PLATFORM:
            if (this->sfxFlags & BGMIZU_SFXFLAG_0) {
                sActiveSfxFlags &= ~BGMIZU_SFXFLAG_0;
            }
            break;
    }
}

s32 BgMizuMovebg_GetPathPointPos(Path* pathList, Vec3f* pos, s32 pathId, s32 pointId) {
    Path* path = pathList;
    Vec3s* point;

    path += pathId;
    point = &((Vec3s*)SEGMENTED_TO_VIRTUAL(path->points))[pointId];

    pos->x = point->x;
    pos->y = point->y;
    pos->z = point->z;

    return 0;
}

void BgMizuMovebg_UpdateScrollAlpha___(BgMizuMovebg* this, GlobalContext* globalCtx) {
    f32 waterLevel = globalCtx->colCtx.colHeader->waterBoxes[2].ySurface;

    if (waterLevel < WATER_TEMPLE_WATER_F1_Y) {
        this->scrollAlpha1 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F2_Y) {
        this->scrollAlpha1 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_F1_Y) /
                                         (WATER_TEMPLE_WATER_F2_Y - WATER_TEMPLE_WATER_F1_Y) * (255 - 160));
    } else {
        this->scrollAlpha1 = 160;
    }

    if (waterLevel < WATER_TEMPLE_WATER_F2_Y) {
        this->scrollAlpha2 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F3_Y) {
        this->scrollAlpha2 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_F2_Y) /
                                         (WATER_TEMPLE_WATER_F3_Y - WATER_TEMPLE_WATER_F2_Y) * (255 - 160));
    } else {
        this->scrollAlpha2 = 160;
    }

    if (waterLevel < WATER_TEMPLE_WATER_B1_Y) {
        this->scrollAlpha3 = 255;
    } else if (waterLevel < WATER_TEMPLE_WATER_F1_Y) {
        this->scrollAlpha3 = 255 - (s32)((waterLevel - WATER_TEMPLE_WATER_B1_Y) /
                                         (WATER_TEMPLE_WATER_F1_Y - WATER_TEMPLE_WATER_B1_Y) * (255 - 160));
    } else {
        this->scrollAlpha3 = 160;
    }

    this->scrollAlpha4 = this->scrollAlpha3;
}

void BgMizuMovebg_Move__(BgMizuMovebg* this, GlobalContext* globalCtx) {
    WaterBox* waterBoxes = globalCtx->colCtx.colHeader->waterBoxes;
    f32 y;
    s32 type;
    Vec3f hookshotTargetRelPos;

    BgMizuMovebg_UpdateScrollAlpha___(this, globalCtx);

    if (globalCtx->colCtx.colHeader->numWaterBoxes >= 3) {
        y = waterBoxes[2].ySurface + 15.0f;
    } else {
        y = this->dyna.actor.world.pos.y;
    }

    type = MOVEBG_TYPE(this->dyna.actor.params);
    switch (type) {
        case BGMIZUMOVEBG_TYPE_0_PLATFORM_SLANTED:
        case BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER:
            if (y < this->homeY - 700.0f) {
                this->dyna.actor.world.pos.y = this->homeY - 700.0f;
            } else {
                this->dyna.actor.world.pos.y = y;
            }
            break;
        case BGMIZUMOVEBG_TYPE_1_PLATFORM:
            if (y < this->homeY - 710.0f) {
                this->dyna.actor.world.pos.y = this->homeY - 710.0f;
            } else {
                this->dyna.actor.world.pos.y = y;
            }
            break;
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
            y = this->homeY + sPillarYoffsetByWaterLevel_[BgMizuMovebg_GetWaterLevelIndex_(globalCtx)];
            if (this->mode421) {
                //func_8002F948(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
            } else if (!Math_StepToF(&this->dyna.actor.world.pos.y, y, 1.0f)) {
                if (!(sActiveSfxFlags & BGMIZU_SFXFLAG_1) && MOVEBG_SPEED(this->dyna.actor.params) != 0) {
                    sActiveSfxFlags |= BGMIZU_SFXFLAG_1;
                    this->sfxFlags |= BGMIZU_SFXFLAG_1;
                }
                if (this->sfxFlags & BGMIZU_SFXFLAG_1) {
                    if (this->dyna.actor.room == 0) {
                        func_8002F974(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                    } else {
                        func_8002F948(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                    }
                }
            }
            break;
        case BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG:
            if (Flags_GetSwitch(globalCtx, MOVEBG_FLAGS(this->dyna.actor.params))) {
                y = this->homeY + 115.200005f;
            } else {
                y = this->homeY;
            }
            if (this->mode421) {
                //func_8002F948(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
            } else if (!Math_StepToF(&this->dyna.actor.world.pos.y, y, 1.0f)) {
                if (!(sActiveSfxFlags & BGMIZU_SFXFLAG_1) && MOVEBG_SPEED(this->dyna.actor.params) != 0) {
                    sActiveSfxFlags |= BGMIZU_SFXFLAG_1;
                    this->sfxFlags |= BGMIZU_SFXFLAG_1;
                }
                if (this->sfxFlags & BGMIZU_SFXFLAG_1) {
                    func_8002F948(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
                }
            }
            break;
    }

    type = MOVEBG_TYPE(this->dyna.actor.params);
    switch (type) {
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
        case BGMIZUMOVEBG_TYPE_4_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_5_PILLAR_SWITCHFLAG:
        case BGMIZUMOVEBG_TYPE_6_PILLAR_SWITCHFLAG:
            if (globalCtx->roomCtx.curRoom.num == this->dyna.actor.room) {
                Matrix_RotateY(this->dyna.actor.world.rot.y * (M_PI / 32768), MTXMODE_NEW);
                Matrix_RotateX(this->dyna.actor.world.rot.x * (M_PI / 32768), MTXMODE_APPLY);
                Matrix_MultVec3f(&sHookshotTargetRelPos, &hookshotTargetRelPos);
                this->dyna.actor.child->world.pos.x = this->dyna.actor.world.pos.x + hookshotTargetRelPos.x;
                this->dyna.actor.child->world.pos.y = this->dyna.actor.world.pos.y + hookshotTargetRelPos.y;
                this->dyna.actor.child->world.pos.z = this->dyna.actor.world.pos.z + hookshotTargetRelPos.z;
                this->dyna.actor.child->flags &= ~1;
                this->dyna.actor.child->world.rot = this->dyna.actor.child->shape.rot = this->dyna.actor.world.rot;
            }
            break;
    }
    if (this->mode421 && this->part == BGMIZUMOVEBG_421PART_BODY) {
        BgMizuMovebg* head = sMode421Parts[BGMIZUMOVEBG_421PART_HEAD];
        Vec3f sHookshotTargetRelPos____ = sHookshotTargetRelPos;

        sHookshotTargetRelPos____.z *= -1.1f;

        Matrix_RotateY(this->dyna.actor.world.rot.y * (M_PI / 32768), MTXMODE_NEW);
        Matrix_RotateX(this->dyna.actor.world.rot.x * (M_PI / 32768), MTXMODE_APPLY);
        Matrix_MultVec3f(&sHookshotTargetRelPos____, &hookshotTargetRelPos);
        this->dyna.actor.child->world.pos.x = this->dyna.actor.world.pos.x + hookshotTargetRelPos.x;
        this->dyna.actor.child->world.pos.y = this->dyna.actor.world.pos.y + hookshotTargetRelPos.y;
        this->dyna.actor.child->world.pos.z = this->dyna.actor.world.pos.z + hookshotTargetRelPos.z;
        this->dyna.actor.child->flags &= ~1;
        this->dyna.actor.child->world.rot = this->dyna.actor.child->shape.rot = this->dyna.actor.world.rot;

        if (head != NULL) {
            Vec3s rot = head->dyna.actor.shape.rot;

            Math_Vec3f_Sum(&head->dyna.actor.world.pos, &hookshotTargetRelPos, &this->dyna.actor.child->world.pos);

            rot.x *= -1;
            rot.y += 0x8000;
            this->dyna.actor.child->world.rot = this->dyna.actor.child->shape.rot = rot;
        }
    }
}

void BgMizuMovebg_FollowPath_(BgMizuMovebg* this, GlobalContext* globalCtx) {
    Vec3f waypoint;
    f32 dist;
    f32 dx;
    f32 dy;
    f32 dz;

    this->dyna.actor.speedXZ = MOVEBG_SPEED(this->dyna.actor.params) * 0.1f;
    BgMizuMovebg_GetPathPointPos(globalCtx->setupPathList, &waypoint, MOVEBG_PATH_ID(this->dyna.actor.params),
                                 this->waypointId);
    dist = Actor_WorldDistXYZToPoint(&this->dyna.actor, &waypoint);
    if (dist < this->dyna.actor.speedXZ) {
        this->dyna.actor.speedXZ = dist;
    }
    func_80035844(&this->dyna.actor.world.pos, &waypoint, &this->dyna.actor.world.rot, 1);
    func_8002D97C(&this->dyna.actor);
    dx = waypoint.x - this->dyna.actor.world.pos.x;
    dy = waypoint.y - this->dyna.actor.world.pos.y;
    dz = waypoint.z - this->dyna.actor.world.pos.z;
    if (fabsf(dx) < 2.0f && fabsf(dy) < 2.0f && fabsf(dz) < 2.0f) {
        this->waypointId++;
        if (this->waypointId >= globalCtx->setupPathList[MOVEBG_PATH_ID(this->dyna.actor.params)].count) {
            this->waypointId = 0;
            BgMizuMovebg_GetPathPointPos(globalCtx->setupPathList, &this->dyna.actor.world.pos,
                                         MOVEBG_PATH_ID(this->dyna.actor.params), 0);
        }
    }
    if (!(sActiveSfxFlags & BGMIZU_SFXFLAG_0) && MOVEBG_SPEED(this->dyna.actor.params) != 0) {
        sActiveSfxFlags |= BGMIZU_SFXFLAG_0;
        this->sfxFlags |= BGMIZU_SFXFLAG_0;
    }
    if (this->sfxFlags & BGMIZU_SFXFLAG_0) {
        func_8002F948(&this->dyna.actor, NA_SE_EV_ROLL_STAND_2 - SFX_FLAG);
    }
}

void BgMizuMovebg_421Body(BgMizuMovebg* this, GlobalContext* globalCtx) {
    Vec3f targetPos;

    if (globalCtx->state.input[0].cur.button & BTN_DDOWN) {
        GET_PLAYER(globalCtx)->actor.world = this->dyna.actor.world;
        GET_PLAYER(globalCtx)->actor.world.pos.y += 100.0f;
        GET_PLAYER(globalCtx)->actor.home = GET_PLAYER(globalCtx)->actor.world;
        GET_PLAYER(globalCtx)->actor.prevPos = GET_PLAYER(globalCtx)->actor.world.pos;
    }

    Math_SmoothStepToF(&this->dyna.actor.scale.x, 0.25f, 0.1f, 0.01f, 0.001f);
    this->dyna.actor.scale.z = this->dyna.actor.scale.x;

    targetPos = this->dyna.actor.world.pos;
    targetPos.y = this->dyna.actor.home.pos.y + 260.0f;

    this->timer--;
    if (this->timer <= 0) {
        this->timer = 10 + Rand_ZeroFloat(10);
        if (Math3D_Vec3fMagnitude(&this->dyna.actor.world.pos) > 1000.0f) {
            this->targetYrot = Math_Atan2S(this->dyna.actor.world.pos.z, this->dyna.actor.world.pos.x) + 0x8000;
        } else {
            this->targetYrot = this->dyna.actor.world.rot.y;
            this->targetYrot += (2 * Rand_ZeroOne() - 1) * 0x2000;
        }
        this->dyna.actor.speedXZ = Rand_ZeroFloat(10) + 40;
    }

    //this->dyna.actor.world.pos.x += 5.0f;
    Math_SmoothStepToS(&this->dyna.actor.world.rot.y, this->targetYrot, 2,
        Math3D_Vec3fMagnitude(&this->dyna.actor.world.pos) > 800.0f ? 0x600 : 0x100,
        0x100);
    this->dyna.actor.shape.rot.y = this->dyna.actor.world.rot.y;

    targetPos.x += this->dyna.actor.speedXZ * Math_CosS(this->dyna.actor.shape.rot.y);
    targetPos.z -= this->dyna.actor.speedXZ * Math_SinS(this->dyna.actor.shape.rot.y);

    Math_SmoothStepToF(&this->dyna.actor.world.pos.x, targetPos.x, 0.1f, this->dyna.actor.speedXZ, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.y, targetPos.y, 0.1f, 3.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.z, targetPos.z, 0.1f, this->dyna.actor.speedXZ, 0.1f);

    func_8002F948(&this->dyna.actor, NA_SE_EV_ELEVATOR_MOVE - SFX_FLAG);
}

void BgMizuMovebg_421Head(BgMizuMovebg* this, GlobalContext* globalCtx) {
    BgMizuMovebg* body = sMode421Parts[BGMIZUMOVEBG_421PART_BODY];
    Vec3f targetPos;

    this->dyna.actor.shape.rot.x = 0x1000;
    this->dyna.actor.shape.rot.y = body->dyna.actor.shape.rot.y + 0x4000;

    this->dyna.actor.world.rot = this->dyna.actor.shape.rot;

    targetPos = body->dyna.actor.world.pos;
    targetPos.x += 80.0f * Math_CosS(body->dyna.actor.shape.rot.y);
    targetPos.z -= 80.0f * Math_SinS(body->dyna.actor.shape.rot.y);
    targetPos.y -= 10.0f;
    Math_SmoothStepToF(&this->dyna.actor.world.pos.x, targetPos.x, 0.1f, 40.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.y, targetPos.y, 0.1f, 40.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.z, targetPos.z, 0.1f, 40.0f, 0.1f);
}

void BgMizuMovebg_421Leg(BgMizuMovebg* this, GlobalContext* globalCtx) {
    BgMizuMovebg* body = sMode421Parts[BGMIZUMOVEBG_421PART_BODY];
    Vec3f targetPos;

    this->dyna.actor.shape.rot.x = 0x7000;
    this->dyna.actor.shape.rot.y = body->dyna.actor.shape.rot.y + 0x10000 + this->part * 0x4000;

    this->dyna.actor.world.rot = this->dyna.actor.shape.rot;

    targetPos = body->dyna.actor.world.pos;
    targetPos.x += 100.0f * Math_CosS(this->dyna.actor.shape.rot.y - 0x4000);
    targetPos.z -= 100.0f * Math_SinS(this->dyna.actor.shape.rot.y - 0x4000);
    targetPos.y -= 80.0f;
    Math_SmoothStepToF(&this->dyna.actor.world.pos.x, targetPos.x, 0.1f, 40.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.y, targetPos.y, 0.1f, 40.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.z, targetPos.z, 0.1f, 40.0f, 0.1f);
}

BgMizuMovebgActionFunc sMode421actionFuncs[] = {
    BgMizuMovebg_421Body,
    BgMizuMovebg_421Head,
    BgMizuMovebg_421Leg,
    BgMizuMovebg_421Leg,
    BgMizuMovebg_421Leg,
    BgMizuMovebg_421Leg
};

void BgMizuMovebg_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuMovebg* this = THIS;
    GfxPrint printer;
    Gfx* gfx = globalCtx->state.gfxCtx->polyOpa.p + 1;

    gSPDisplayList(globalCtx->state.gfxCtx->overlay.p++, gfx);

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);

    GfxPrint_SetColor(&printer, 255, 0, 0, 255);
    GfxPrint_SetPos(&printer, 1, 1);
    GfxPrint_Printf(&printer, "mode421 = %d", this->mode421);

    if (this->mode421) {
        GfxPrint_SetPos(&printer, 1, 5 + this->part);
        GfxPrint_Printf(&printer, "part = %d", this->part);
    }

    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);

    gSPEndDisplayList(gfx++);
    gSPBranchList(globalCtx->state.gfxCtx->polyOpa.p, gfx);
    globalCtx->state.gfxCtx->polyOpa.p = gfx;

    if (this->mode421) {
        s32 allPartsExist = true;
        s32 i;

        for (i = 0; i < 6; i++) {
            if (sMode421Parts[i] == NULL) {
                allPartsExist = false;
            }
        }

        this->rotJitter.x += (2 * Rand_ZeroOne() - 1) * 0x30;
        Math_SmoothStepToS(&this->rotJitter.x, 0, 2, 5, 1);

        this->rotJitter.y += (2 * Rand_ZeroOne() - 1) * 0x30;
        Math_SmoothStepToS(&this->rotJitter.y, 0, 2, 5, 1);

        this->rotJitter.z += (2 * Rand_ZeroOne() - 1) * 0x30;
        Math_SmoothStepToS(&this->rotJitter.z, 0, 2, 5, 1);

        if (allPartsExist) {
            gSaveContext.equips.buttonItems[1] = ITEM_LONGSHOT;

            sMode421actionFuncs[this->part](this, globalCtx);
        }
    }

    if (this->mode421 && this->actionFunc == BgMizuMovebg_FollowPath_) {

    } else {
        this->actionFunc(this, globalCtx);
    }
}

void BgMizuMovebg_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    BgMizuMovebg* this = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames;

    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 754);

    if (this->mode421) {
        Matrix_RotateRPY(this->rotJitter.x, this->rotJitter.y, this->rotJitter.z, MTXMODE_APPLY);
    }

    frames = globalCtx->gameplayFrames;
    func_80093D18(globalCtx->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        this->scrollAlpha1));

    gSPSegment(POLY_OPA_DISP++, 0x09,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        this->scrollAlpha2));

    gSPSegment(POLY_OPA_DISP++, 0x0A,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 1, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        this->scrollAlpha3));

    gSPSegment(POLY_OPA_DISP++, 0x0B,
               Gfx_TwoTexScrollEnvColor(globalCtx->state.gfxCtx, 0, frames * 3, 0, 32, 32, 1, 0, 0, 32, 32, 0, 0, 0,
                                        this->scrollAlpha4));

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 788),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    if (this->dlist != NULL) {
        gSPDisplayList(POLY_OPA_DISP++, this->dlist);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 795);
}
