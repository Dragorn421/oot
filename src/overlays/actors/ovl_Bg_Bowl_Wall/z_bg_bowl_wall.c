/*
 * File: z_bg_bowl_wall.c
 * Overlay: Bg_Bowl_Wall
 * Description: Bombchu Bowling Alley Wall
 */

#include "z_bg_bowl_wall.h"
#include "overlays/actors/ovl_En_Bom_Bowl_Man/z_en_bom_bowl_man.h"
#include "overlays/actors/ovl_En_Wall_Tubo/z_en_wall_tubo.h"
#include "objects/object_bowl/object_bowl.h"

#define FLAGS 0x00000030

#define THIS ((BgBowlWall*)thisx)

void BgBowlWall_Init(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Update(Actor* thisx, GlobalContext* globalCtx);
void BgBowlWall_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_8086F260(BgBowlWall* this, GlobalContext* globalCtx);
void func_8086F440(BgBowlWall* this, GlobalContext* globalCtx);
void func_8086F464(BgBowlWall* this, GlobalContext* globalCtx);
void func_8086F718(BgBowlWall* this, GlobalContext* globalCtx);
void func_8086F7F8(BgBowlWall* this, GlobalContext* globalCtx);

const ActorInit Bg_Bowl_Wall_InitVars = {
    ACTOR_BG_BOWL_WALL,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_BOWL,
    sizeof(BgBowlWall),
    (ActorFunc)BgBowlWall_Init,
    (ActorFunc)BgBowlWall_Destroy,
    (ActorFunc)BgBowlWall_Update,
    (ActorFunc)BgBowlWall_Draw,
};
Vec3f D_8086FA40[4] = {
    { 0.0f, 210.0f, -20.0f },
    { 0.0f, 170.0f, -20.0f },
    { -170.0f, 0.0f, -20.0f },
    { 170.0f, 0.0f, -20.0f },
};
s16 D_8086FA70[4] = { 0, 0, 0x3FFF, -0x3FFF };
Vec3f D_8086FA78 = { 0.0f, 0.1f, 0.0f };
Vec3f D_8086FA84 = { 0.0f, 0.0f, 0.0f };

void BgBowlWall_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* this = (BgBowlWall*)thisx;
    s32 pad1[2];
    CollisionHeader* sp28;
    s32 pad2[2];

    sp28 = NULL;
    DynaPolyActor_Init(&this->dyna, DPM_UNK);
    if (this->dyna.actor.params == 0) {
        CollisionHeader_GetVirtual(&gBowlingFirstAndFinalRoundCol, &sp28);
    } else {
        CollisionHeader_GetVirtual(&gBowlingSecondRoundCol, &sp28);
    }
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp28);
    this->unk168 = this->dyna.actor.world.pos;
    osSyncPrintf("\n\n");
    osSyncPrintf("\x1b[32m ☆☆☆☆☆ ボーリングおじゃま壁発生 ☆☆☆☆☆ %d\n\x1b[m", this->dyna.actor.params);
    this->actionFunc = func_8086F260;
    this->dyna.actor.scale.z = 1.0f;
    this->dyna.actor.scale.y = 1.0f;
    this->dyna.actor.scale.x = 1.0f;
}

void BgBowlWall_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* this = (BgBowlWall*)thisx;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, (s32)this->dyna.bgId);
}

void func_8086F260(BgBowlWall* this, GlobalContext* globalCtx) {
    Actor* var_v0_2;
    EnWallTubo* temp_v0_2;
    s32 pad;
    s16 params = this->dyna.actor.params;

    if (params != 0) {
        params += (s16)Rand_ZeroFloat(2.99f);
        this->dyna.actor.shape.rot.z = this->dyna.actor.world.rot.z = D_8086FA70[params];
        osSyncPrintf("\n\n");
    }
    this->unk174.x = D_8086FA40[params].x + this->dyna.actor.world.pos.x;
    this->unk174.y = D_8086FA40[params].y + this->dyna.actor.world.pos.y;
    this->unk174.z = D_8086FA40[params].z + this->dyna.actor.world.pos.z;
    if (0) {}
    temp_v0_2 = (EnWallTubo*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->dyna.actor, globalCtx, ACTOR_EN_WALL_TUBO,
                                                this->unk174.x, this->unk174.y, this->unk174.z, 0, 0, 0,
                                                this->dyna.actor.params);
    if (temp_v0_2 != NULL) {
        temp_v0_2->unk154 = this->unk174;
        if (params != 0) {
            temp_v0_2->unk154 = this->unk174 = this->dyna.actor.world.pos;
        }
        if (this->unk184 == NULL) {
            var_v0_2 = globalCtx->actorCtx.actorLists[4].head;
            while (var_v0_2 != NULL) {
                if (var_v0_2->id != ACTOR_EN_BOM_BOWL_MAN) {
                    var_v0_2 = var_v0_2->next;
                    continue;
                }
                this->unk184 = (EnBomBowlMan*)var_v0_2;
                break;
            }
        }
        this->actionFunc = func_8086F440;
    }
}

void func_8086F440(BgBowlWall* this, GlobalContext* globalCtx) {
    if (this->unk180 != 0) {
        this->actionFunc = func_8086F464;
    }
}

void func_8086F464(BgBowlWall* this, GlobalContext* globalCtx) {
    s32 var_s0;
    Vec3f spA0 = D_8086FA78;
    Vec3f sp94 = D_8086FA84;
    Vec3f sp88;
    s16 temp_v0;
    s32 var_s0_2;

    var_s0 = false;
    if (this->dyna.actor.params == 0) {
        Math_SmoothStepToS(&this->dyna.actor.shape.rot.x, -0x3E80, 3, 0x1F4, 0);
        this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x;
        if (this->dyna.actor.shape.rot.x < -0x3C1E) {
            var_s0 = true;
        }
    } else {
        Math_ApproachF(&this->dyna.actor.world.pos.y, this->unk168.y - 450.0f, 0.3f, 10.0f);
        if (this->dyna.actor.world.pos.y < (this->unk168.y - 400.0f)) {
            var_s0 = true;
        }
    }
    if (var_s0) {
        for (var_s0_2 = 0; var_s0_2 < 15; var_s0_2++) {
            sp88.x = Rand_CenteredFloat(300.0f) + this->unk174.x;
            sp88.y = -100.0f;
            sp88.z = Rand_CenteredFloat(400.0f) + this->unk174.z;
            EffectSsBomb2_SpawnLayered(globalCtx, &sp88, &sp94, &spA0, 100, 30);
            sp88.y = -50.0f;
            EffectSsHahen_SpawnBurst(globalCtx, &sp88, 10.0f, 0, 50, 15, 3, -1, 10, NULL);
            Audio_PlayActorSound2(&this->dyna.actor, NA_SE_IT_BOMB_EXPLOSION);
        }
        temp_v0 = Quake_Add(GET_ACTIVE_CAM(globalCtx), 1U);
        Quake_SetSpeed(temp_v0, 0x7FFF);
        Quake_SetQuakeValues(temp_v0, 0x12C, 0, 0, 0);
        Quake_SetCountdown(temp_v0, 0x1E);
        this->unk182 = 0x14;
        this->actionFunc = func_8086F718;
    }
}

void func_8086F718(BgBowlWall* this, GlobalContext* globalCtx) {
    if (this->unk182 >= 2) {
        if (this->dyna.actor.params == 0) {
            Math_SmoothStepToS(&this->dyna.actor.shape.rot.x, -0x3E80, 1, 0xC8, 0);
        } else {
            Math_ApproachF(&this->dyna.actor.world.pos.y, this->unk168.y - 450.0f, 0.3f, 10.0f);
        }
    } else if (this->unk182 == 1) {
        this->dyna.actor.world.pos.y = this->unk168.y - 450.0f;
        this->dyna.actor.world.rot.x = this->dyna.actor.shape.rot.x = 0;
        this->unk184->unk23E_arr[this->dyna.actor.params] = 2;
        this->actionFunc = func_8086F7F8;
    }
}

void func_8086F7F8(BgBowlWall* this, GlobalContext* globalCtx) {
    if (this->unk184->unk23E_arr[this->dyna.actor.params] != 2) {
        Math_ApproachF(&this->dyna.actor.world.pos.y, this->unk168.y, 0.3f, 50.0f);
        if (fabsf(this->dyna.actor.world.pos.y - this->unk168.y) <= 10.0f) {
            this->dyna.actor.world.pos.y = this->unk168.y;
            this->unk180 = 0;
            this->actionFunc = func_8086F260;
        }
    }
}

void BgBowlWall_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgBowlWall* this = (BgBowlWall*)thisx;

    if (this->unk182 != 0) {
        this->unk182--;
    }
    this->actionFunc(this, globalCtx);
}

void BgBowlWall_Draw(Actor* thisx, GlobalContext* globalCtx) {
    u32 new_var2;
    BgBowlWall* this = (BgBowlWall*)thisx;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 441);
    func_80093D84(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x8,
               Gfx_TexScroll(globalCtx->state.gfxCtx, 0, (new_var2 = globalCtx->state.frames) * -2, 16, 16));
    gDPPipeSync(POLY_OPA_DISP++);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 453),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (this->dyna.actor.params == 0) {
        gSPDisplayList(POLY_OPA_DISP++, gBowlingRound1WallDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gBowlingRound2WallDL);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_bowl_wall.c", 464);
}
