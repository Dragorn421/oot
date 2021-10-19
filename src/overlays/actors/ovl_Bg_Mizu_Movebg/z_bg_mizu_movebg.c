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

    Actor_ProcessInitChain(thisx, sInitChain);
    THIS->homeY = thisx->world.pos.y;
    THIS->dlist = sDLists[MOVEBG_TYPE(thisx->params)];
    DynaPolyActor_Init(&THIS->dyna, DPM_PLAYER);
    CollisionHeader_GetVirtual(sColHeaders[MOVEBG_TYPE(thisx->params)], &colHeader);
    THIS->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);

    type = MOVEBG_TYPE(thisx->params);
    switch (type) {
        case BGMIZUMOVEBG_TYPE_0_PLATFORM_SLANTED:
            y = waterBoxes[2].ySurface + 15.0f;
            if (y < THIS->homeY - 700.0f) {
                thisx->world.pos.y = THIS->homeY - 700.0f;
            } else {
                thisx->world.pos.y = y;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_1_PLATFORM:
            y = waterBoxes[2].ySurface + 15.0f;
            if (y < THIS->homeY - 710.0f) {
                thisx->world.pos.y = THIS->homeY - 710.0f;
            } else {
                thisx->world.pos.y = y;
            }
            THIS->actionFunc = BgMizuMovebg_Move__;
            break;
        case BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER:
            y = waterBoxes[2].ySurface + 15.0f;
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

    type = MOVEBG_TYPE(this->dyna.actor.params);
    switch (type) {
        case BGMIZUMOVEBG_TYPE_0_PLATFORM_SLANTED:
        case BGMIZUMOVEBG_TYPE_2_PLATFORM_TALLER:
            y = waterBoxes[2].ySurface + 15.0f;
            if (y < this->homeY - 700.0f) {
                this->dyna.actor.world.pos.y = this->homeY - 700.0f;
            } else {
                this->dyna.actor.world.pos.y = y;
            }
            break;
        case BGMIZUMOVEBG_TYPE_1_PLATFORM:
            y = waterBoxes[2].ySurface + 15.0f;
            if (y < this->homeY - 710.0f) {
                this->dyna.actor.world.pos.y = this->homeY - 710.0f;
            } else {
                this->dyna.actor.world.pos.y = y;
            }
            break;
        case BGMIZUMOVEBG_TYPE_3_PILLAR_HIGHESTWATERLEVEL:
            y = this->homeY + sPillarYoffsetByWaterLevel_[BgMizuMovebg_GetWaterLevelIndex_(globalCtx)];
            if (!Math_StepToF(&this->dyna.actor.world.pos.y, y, 1.0f)) {
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
            if (!Math_StepToF(&this->dyna.actor.world.pos.y, y, 1.0f)) {
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
                Matrix_MultVec3f(&sHookshotTargetRelPos, &hookshotTargetRelPos);
                this->dyna.actor.child->world.pos.x = this->dyna.actor.world.pos.x + hookshotTargetRelPos.x;
                this->dyna.actor.child->world.pos.y = this->dyna.actor.world.pos.y + hookshotTargetRelPos.y;
                this->dyna.actor.child->world.pos.z = this->dyna.actor.world.pos.z + hookshotTargetRelPos.z;
                this->dyna.actor.child->flags &= ~1;
            }
            break;
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

void BgMizuMovebg_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgMizuMovebg* this = THIS;

    this->actionFunc(this, globalCtx);
}

void BgMizuMovebg_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    BgMizuMovebg* this = THIS;
    GlobalContext* globalCtx = globalCtx2;
    u32 frames;

    if (1) {}

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_mizu_movebg.c", 754);

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
