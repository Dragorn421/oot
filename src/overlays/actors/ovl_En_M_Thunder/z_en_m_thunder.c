#include "z_en_m_thunder.h"

#include "gfx.h"
#include "gfx_setupdl.h"
#include "rumble.h"
#include "sfx.h"
#include "sys_matrix.h"
#include "z_lib.h"
#include "audio.h"
#include "light.h"
#include "play_state.h"
#include "player.h"
#include "save.h"

#include "assets/objects/gameplay_keep/spin_attack.h"

#define FLAGS 0

void EnMThunder_Init(Actor* thisx, PlayState* play);
void EnMThunder_Destroy(Actor* thisx, PlayState* play);
void EnMThunder_Update(Actor* thisx, PlayState* play);
void EnMThunder_Draw(Actor* thisx, PlayState* play2);

void func_80A9F314(PlayState* play, f32 arg1);
void func_80A9F408(EnMThunder* this, PlayState* play);
void func_80A9F9B4(EnMThunder* this, PlayState* play);

ActorProfile En_M_Thunder_Profile = {
    /**/ ACTOR_EN_M_THUNDER,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(EnMThunder),
    /**/ EnMThunder_Init,
    /**/ EnMThunder_Destroy,
    /**/ EnMThunder_Update,
    /**/ EnMThunder_Draw,
};

static ColliderCylinderInit D_80AA0420 = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0x00000001, HIT_SPECIAL_EFFECT_NONE, 0x00 },
        { 0xFFCFFFFF, HIT_BACKLASH_NONE, 0x00 },
        ATELEM_ON | ATELEM_SFX_NONE,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 200, 200, 0, { 0, 0, 0 } },
};

static u32 D_80AA044C[3] = { DMG_SPIN_MASTER, DMG_SPIN_KOKIRI, DMG_SPIN_GIANT };
static u32 D_80AA0458[3] = { DMG_JUMP_MASTER, DMG_JUMP_KOKIRI, DMG_JUMP_GIANT };

void EnMThunder_SetupAction(EnMThunder* this, EnMThunderActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnMThunder_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnMThunder* this = (EnMThunder*)thisx;
    Player* player;

    player = GET_PLAYER(play);
    Collider_InitCylinder(play, &this->unk14C);
    Collider_SetCylinder(play, &this->unk14C, &this->actor, &D_80AA0420);
    this->unk1C7 = PARAMS_GET_U(this->actor.params, 0, 8) - 1;
    Lights_PointNoGlowSetInfo(&this->unk19C, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                              255, 255, 255, 0);
    this->unk198 = LightContext_InsertLight(play, &play->lightCtx, &this->unk19C);
    this->unk14C.dim.radius = 0;
    this->unk14C.dim.height = 40;
    this->unk14C.dim.yShift = -20;
    this->unk1C4 = 8;
    this->unk1B4 = 0.0f;
    this->actor.world.pos = player->bodyPartsPos[PLAYER_BODYPART_WAIST];
    this->unk1AC = 0.0f;
    this->unk1BC = 0.0f;
    this->actor.shape.rot.y = player->actor.shape.rot.y + 0x8000;
    this->actor.room = -1;
    Actor_SetScale(&this->actor, 0.1f);
    this->unk1CA = 0;
    if (player->stateFlags2 & PLAYER_STATE2_17) {
        if (((gSaveContext.save.info.playerData.isMagicAcquired) == 0) ||
            (gSaveContext.magicState != MAGIC_STATE_IDLE) ||
            ((PARAMS_GET_S(this->actor.params, 8, 8) != 0) &&
             (Magic_RequestChange(play, PARAMS_GET_S(this->actor.params, 8, 8), MAGIC_CONSUME_NOW) == 0))) {
            SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_ROLLING_CUT);
            SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_SWORD_SWING_HARD);
            Actor_Kill(&this->actor);
            return;
        }
        player->stateFlags2 &= ~PLAYER_STATE2_17;
        this->unk1CA = 1;
        this->unk14C.elem.atDmgInfo.dmgFlags = D_80AA044C[this->unk1C7];
        this->unk1C6 = 1;
        if (this->unk1C7 == 1) {
            this->unk1C9 = 2;
        } else {
            this->unk1C9 = 4;
        }
        EnMThunder_SetupAction(this, func_80A9F9B4);
        this->unk1C4 = 8;
        SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_ROLLING_CUT_LV1);
        this->unk1AC = 1.0f;
    } else {
        EnMThunder_SetupAction(this, func_80A9F408);
    }
    this->actor.child = NULL;
}

void EnMThunder_Destroy(Actor* thisx, PlayState* play) {
    EnMThunder* this = (EnMThunder*)thisx;

    if (this->unk1CA != 0) {
        Magic_Reset(play);
    }
    Collider_DestroyCylinder(play, &this->unk14C);
    func_80A9F314(play, 0.0f);
    LightContext_RemoveLight(play, &play->lightCtx, this->unk198);
}

void func_80A9F314(PlayState* play, f32 arg1) {
    Environment_AdjustLights(play, arg1, 850.0f, 0.2f, 0.0f);
}

void func_80A9F350(EnMThunder* this, PlayState* play) {
    Player* player;

    player = GET_PLAYER(play);
    if (player->stateFlags2 & PLAYER_STATE2_17) {
        if (player->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H) {
            SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_ROLLING_CUT);
            SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_SWORD_SWING_HARD);
        }
        Actor_Kill(&this->actor);
        return;
    }
    if (!(player->stateFlags1 & PLAYER_STATE1_CHARGING_SPIN_ATTACK)) {
        Actor_Kill(&this->actor);
    }
}

void func_80A9F408(EnMThunder* this, PlayState* play) {
    Player* player;
    Actor* child;

    player = GET_PLAYER(play);
    child = this->actor.child;
    this->unk1B8 = player->unk_858;
    this->actor.world.pos = player->bodyPartsPos[PLAYER_BODYPART_WAIST];
    this->actor.shape.rot.y = player->actor.shape.rot.y + 0x8000;
    if ((this->unk1CA == 0) && (player->unk_858 >= 0.10f)) {
        if ((gSaveContext.magicState != MAGIC_STATE_IDLE) ||
            ((PARAMS_GET_S(this->actor.params, 8, 8) != 0) &&
             (Magic_RequestChange(play, PARAMS_GET_S(this->actor.params, 8, 8), MAGIC_CONSUME_WAIT_PREVIEW) == 0))) {
            func_80A9F350(this, play);
            EnMThunder_SetupAction(this, func_80A9F350);
            this->unk1C8 = 0;
            this->unk1BC = 0;
            this->unk1AC = 0.0f;
            return;
        }
        this->unk1CA = 1;
    }
    if (player->unk_858 >= 0.10f) {
        Rumble_Request(0.0f, (u8)(s32)(player->unk_858 * 150.0f), 2, (u8)(s32)(player->unk_858 * 150.0f));
    }
    if (player->stateFlags2 & PLAYER_STATE2_17) {
        if ((child != NULL) && (child->update != NULL)) {
            child->parent = NULL;
        }
        if (player->unk_858 <= 0.15f) {
            if ((player->unk_858 >= 0.10f) && (player->meleeWeaponAnimation >= PLAYER_MWA_SPIN_ATTACK_1H)) {
                SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_ROLLING_CUT);
                SFX_PLAY_AT_POS(&player->actor.projectedPos, NA_SE_IT_SWORD_SWING_HARD);
            }
            Actor_Kill(&this->actor);
            return;
        }
        player->stateFlags2 &= ~PLAYER_STATE2_17;
        if (PARAMS_GET_S(this->actor.params, 8, 8) != 0) {
            gSaveContext.magicState = MAGIC_STATE_CONSUME_SETUP;
        }
        if (player->unk_858 < 0.85f) {
            this->unk14C.elem.atDmgInfo.dmgFlags = D_80AA044C[this->unk1C7];
            this->unk1C6 = 1;
            if (this->unk1C7 == 1) {
                this->unk1C9 = 2;
            } else {
                this->unk1C9 = 4;
            }
        } else {
            this->unk14C.elem.atDmgInfo.dmgFlags = D_80AA0458[this->unk1C7];
            this->unk1C6 = 0;
            if (this->unk1C7 == 1) {
                this->unk1C9 = 4;
            } else {
                this->unk1C9 = 8;
            }
        }
        EnMThunder_SetupAction(this, func_80A9F9B4);
        this->unk1C4 = 8;

        {
            static u16 D_80AA0464[4] = {
                NA_SE_IT_ROLLING_CUT_LV2,
                NA_SE_IT_ROLLING_CUT_LV1,
                NA_SE_IT_ROLLING_CUT_LV2,
                NA_SE_IT_ROLLING_CUT_LV1,
            };

            SFX_PLAY_AT_POS(&player->actor.projectedPos, D_80AA0464[this->unk1C6]);
        }

        this->unk1AC = 1.0f;
        return;
    }
    if (!(player->stateFlags1 & PLAYER_STATE1_CHARGING_SPIN_ATTACK)) {
        if (this->actor.child != NULL) {
            this->actor.child->parent = NULL;
        }
        Actor_Kill(&this->actor);
        return;
    }
    if (player->unk_858 > 0.15f) {
        this->unk1C8 = 0xFF;
        if (this->actor.child == NULL) {
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EFF_DUST, this->actor.world.pos.x,
                               this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                               this->unk1C7 + 2);
        }
        this->unk1BC = this->unk1BC + ((((player->unk_858 - 0.15f) * 1.5f) - this->unk1BC) * 0.5f);
    } else if (player->unk_858 > 0.1f) {
        this->unk1C8 = (u8)(s32)((player->unk_858 - 0.1f) * 255.0f * 20.0f);
        this->unk1AC = (player->unk_858 - 0.1f) * 10.0f;
    } else {
        this->unk1C8 = 0;
    }
    if (player->unk_858 > 0.85f) {
        func_800F4254(&player->actor.projectedPos, 2);
    } else if (player->unk_858 > 0.15f) {
        func_800F4254(&player->actor.projectedPos, 1);
    } else if (player->unk_858 > 0.10f) {
        func_800F4254(&player->actor.projectedPos, 0);
    }
    if (Play_InCsMode(play)) {
        Actor_Kill(&this->actor);
    }
}

void func_80A9F938(EnMThunder* this, PlayState* play) {
    if (this->unk1C4 < 2) {
        if (this->unk1C8 < 0x28) {
            this->unk1C8 = 0;
        } else {
            this->unk1C8 = this->unk1C8 - 0x28;
        }
    }
    this->unk1B4 += 2.0f * this->unk1B0;
    if (this->unk1BC < this->unk1AC) {
        this->unk1BC = this->unk1BC + ((this->unk1AC - this->unk1BC) * 0.1f);
    } else {
        this->unk1BC = this->unk1AC;
    }
}

void func_80A9F9B4(EnMThunder* this, PlayState* play) {
    Player* player;

    player = GET_PLAYER(play);
    if (Math_StepToF(&this->unk1AC, 0.0f, 1 / 16.0f)) {
        Actor_Kill(&this->actor);
    } else {
        Math_SmoothStepToF(&this->actor.scale.x, (s32)this->unk1C9, 0.6f, 0.8f, 0.0f);
        Actor_SetScale(&this->actor, this->actor.scale.x);
        this->unk14C.dim.radius = this->actor.scale.x * 25.0f;
        Collider_UpdateCylinder(&this->actor, &this->unk14C);
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->unk14C.base);
    }
    if (this->unk1C4 > 0) {
        this->actor.world.pos.x = player->bodyPartsPos[PLAYER_BODYPART_WAIST].x;
        this->actor.world.pos.z = player->bodyPartsPos[PLAYER_BODYPART_WAIST].z;
        this->unk1C4 = this->unk1C4 - 1;
    }
    if (this->unk1AC > 0.6f) {
        this->unk1B0 = 1.0f;
    } else {
        this->unk1B0 = this->unk1AC * (5.0f / 3.0f);
    }
    func_80A9F938(this, play);
    if (Play_InCsMode(play)) {
        Actor_Kill(&this->actor);
    }
}

void EnMThunder_Update(Actor* thisx, PlayState* play) {
    f32 temp_fv0;
    s32 temp_v0;
    EnMThunder* this = (EnMThunder*)thisx;

    this->actionFunc(this, play);
    func_80A9F314(play, this->unk1BC);
    temp_fv0 = this->unk1AC;
    temp_v0 = (u32)(temp_fv0 * 255.0f) & 0xFF;
    Lights_PointNoGlowSetInfo(&this->unk19C, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                              temp_v0, temp_v0, temp_fv0 * 100.0f, temp_fv0 * 800.0f);
}

static f32 D_80AA046C[9] = { 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.25f, 0.2f, 0.15f, 0.0f };

void EnMThunder_Draw(Actor* thisx, PlayState* play2) {
    f32 var_fa1;
    PlayState* play = play2;
    Player* player;
    EnMThunder* this = (EnMThunder*)thisx;
    s32 var_t1;

    player = GET_PLAYER(play);
    OPEN_DISPS(play->state.gfxCtx, "../z_en_m_thunder.c", 0x34C);
    Gfx_SetupDL_25Xlu(play->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx, "../z_en_m_thunder.c", 853);
    switch (this->unk1C6) {
        case 0:
        case 1:
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE,
                                        0xFF - ((u8)(s32)(this->unk1B4 * 30.0f) & 0xFF), 0, 64, 32, 1,
                                        0xFF - ((u8)(s32)(this->unk1B4 * 20.0f) & 0xFF), 0, 8, 8));
            break;
    }
    switch (this->unk1C6) {
        case 0:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 0xAA, (u8)(this->unk1B0 * 255.0f));
            gSPDisplayList(POLY_XLU_DISP++, gSpinAttack3DL);
            gSPDisplayList(POLY_XLU_DISP++, gSpinAttack4DL);
            break;
        case 1:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 0xAA, 255, 255, (u8)(this->unk1B0 * 255.0f));
            gSPDisplayList(POLY_XLU_DISP++, gSpinAttack1DL);
            gSPDisplayList(POLY_XLU_DISP++, gSpinAttack2DL);
            break;
    }
    Matrix_Mult(&player->mf_9E0, MTXMODE_NEW);
    switch (this->unk1C7) {
        case 1:
            Matrix_Translate(0.0f, 220.0f, 0.0f, MTXMODE_APPLY);
            Matrix_Scale(-0.7f, -0.6f, -0.4f, MTXMODE_APPLY);
            Matrix_RotateX(16384.0f, MTXMODE_APPLY);
            break;
        case 0:
            Matrix_Translate(0.0f, 300.0f, -100.0f, MTXMODE_APPLY);
            Matrix_Scale(-1.2f, -1.0f, -0.7f, MTXMODE_APPLY);
            Matrix_RotateX(16384.0f, MTXMODE_APPLY);
            break;
        case 2:
            Matrix_Translate(200.0f, 350.0f, 0.0f, MTXMODE_APPLY);
            Matrix_Scale(-1.8f, -1.4f, -0.7f, MTXMODE_APPLY);
            Matrix_RotateX(16384.0f, MTXMODE_APPLY);
            break;
    }
    if (this->unk1B8 >= 0.85f) {
        var_fa1 = (D_80AA046C[play->gameplayFrames & 7] * 6.0f) + 1.0f;
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 0xAA, this->unk1C8);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 100, 0, 128);
        var_t1 = 0x28;
    } else {
        var_fa1 = (2.0f * D_80AA046C[play->gameplayFrames & 7]) + 1.0f;
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 0xAA, 255, 255, this->unk1C8);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 128);
        var_t1 = 0x14;
    }
    Matrix_Scale(1.0f, var_fa1, var_fa1, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx, "../z_en_m_thunder.c", 960);
    gSPSegment(POLY_XLU_DISP++, 9,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, (play->gameplayFrames * 5) & 0xFF, 0U, 32, 32, 1,
                                (play->gameplayFrames * 0x14) & 0xFF, (play->gameplayFrames * var_t1) & 0xFF, 8, 8));
    gSPDisplayList(POLY_XLU_DISP++, gSpinAttackChargingDL);
    CLOSE_DISPS(play->state.gfxCtx, "../z_en_m_thunder.c", 0x407);
}
