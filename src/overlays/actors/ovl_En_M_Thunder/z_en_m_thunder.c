#include "z_en_m_thunder.h"

#define FLAGS 0x00000000

#define THIS ((EnMThunder*)thisx)

void EnMThunder_Init(Actor* thisx, GlobalContext* globalCtx);
void EnMThunder_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnMThunder_Update(Actor* thisx, GlobalContext* globalCtx);
void EnMThunder_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80A9F314(GlobalContext* globalCtx, f32 arg1);
void func_80A9F408(EnMThunder* this, GlobalContext* globalCtx);
void func_80A9F9B4(EnMThunder* this, GlobalContext* globalCtx);

extern Gfx D_4012570[];
extern Gfx D_4012690[];
extern Gfx D_4012AF0[];
extern Gfx D_4012C10[];
extern Gfx D_4013610[];

const ActorInit En_M_Thunder_InitVars = {
    ACTOR_EN_M_THUNDER,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnMThunder),
    (ActorFunc)EnMThunder_Init,
    (ActorFunc)EnMThunder_Destroy,
    (ActorFunc)EnMThunder_Update,
    (ActorFunc)EnMThunder_Draw,
};

static ColliderCylinderInit D_80AA0420 = {
    {
        COLTYPE_NONE,
        AT_ON | AT_TYPE_PLAYER,
        AC_NONE,
        OC1_NONE,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000001, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_ON | TOUCH_SFX_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 200, 200, 0, { 0, 0, 0 } },
};

static u32 D_80AA044C[3] = { 0x01000000, 0x00400000, 0x00800000 };
static u32 D_80AA0458[3] = { 0x08000000, 0x02000000, 0x04000000 };
static u16 D_80AA0464[4] = {
    NA_SE_IT_ROLLING_CUT_LV2,
    NA_SE_IT_ROLLING_CUT_LV1,
    NA_SE_IT_ROLLING_CUT_LV2,
    NA_SE_IT_ROLLING_CUT_LV1,
};
static f32 D_80AA046C[9] = { 0.1f, 0.15f, 0.2f, 0.25f, 0.3f, 0.25f, 0.2f, 0.15f, 0.0f };

void EnMThunder_SetupAction(EnMThunder* this, EnMThunderActionFunc actionFunc) {
    this->actionFunc = actionFunc;
}

void EnMThunder_Init(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnMThunder* this = (EnMThunder*)thisx;
    Player* player;

    player = PLAYER;
    Collider_InitCylinder(globalCtx, &this->unk14C);
    Collider_SetCylinder(globalCtx, &this->unk14C, &this->actor, &D_80AA0420);
    this->unk1C7 = (this->actor.params & 0xFF) - 1;
    Lights_PointNoGlowSetInfo(&this->unk19C, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                              255, 255, 255, 0);
    this->unk198 = LightContext_InsertLight(globalCtx, &globalCtx->lightCtx, &this->unk19C);
    this->unk14C.dim.radius = 0;
    this->unk14C.dim.height = 40;
    this->unk14C.dim.yShift = -20;
    this->unk1C4 = 8;
    this->unk1B4 = 0.0f;
    this->actor.world.pos = player->bodyPartsPos[0];
    this->unk1AC = 0.0f;
    this->unk1BC = 0.0f;
    this->actor.shape.rot.y = player->actor.shape.rot.y + 0x8000;
    this->actor.room = -1;
    Actor_SetScale(&this->actor, 0.1f);
    this->unk1CA = 0;
    if (player->stateFlags2 & 0x20000) {
        if (((gSaveContext.magicAcquired) == 0) || (gSaveContext.unk_13F0 != 0) ||
            (((((this->actor.params & 0xFF00) >> 8) != 0)) &&
             (func_80087708(globalCtx, (s16)((this->actor.params & 0xFF00) >> 8), 0) == 0))) {
            Audio_PlaySoundGeneral(NA_SE_IT_ROLLING_CUT, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_SWING_HARD, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            Actor_Kill(&this->actor);
            return;
        }
        player->stateFlags2 &= ~0x20000;
        this->unk1CA = 1;
        this->unk14C.info.toucher.dmgFlags = D_80AA044C[this->unk1C7];
        this->unk1C6 = 1;
        if (this->unk1C7 == 1) {
            this->unk1C9 = 2;
        } else {
            this->unk1C9 = 4;
        }
        EnMThunder_SetupAction(this, func_80A9F9B4);
        this->unk1C4 = 8;
        Audio_PlaySoundGeneral(NA_SE_IT_ROLLING_CUT_LV1, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        this->unk1AC = 1.0f;
    } else {
        EnMThunder_SetupAction(this, func_80A9F408);
    }
    this->actor.child = NULL;
}

void EnMThunder_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnMThunder* this = (EnMThunder*)thisx;

    if (this->unk1CA != 0) {
        func_800876C8(globalCtx);
    }
    Collider_DestroyCylinder(globalCtx, &this->unk14C);
    func_80A9F314(globalCtx, 0.0f);
    LightContext_RemoveLight(globalCtx, &globalCtx->lightCtx, this->unk198);
}

void func_80A9F314(GlobalContext* globalCtx, f32 arg1) {
    func_800773A8(globalCtx, arg1, 850.0f, 0.2f, 0.0f);
}

void func_80A9F350(EnMThunder* this, GlobalContext* globalCtx) {
    Player* player;

    player = PLAYER;
    if (player->stateFlags2 & 0x20000) {
        if (player->swordAnimation >= 0x18) {
            Audio_PlaySoundGeneral(NA_SE_IT_ROLLING_CUT, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_SWING_HARD, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                   &D_801333E8);
        }
        Actor_Kill(&this->actor);
        return;
    }
    if (!(player->stateFlags1 & 0x1000)) {
        Actor_Kill(&this->actor);
    }
}

void func_80A9F408(EnMThunder* this, GlobalContext* globalCtx) {
    Player* player;
    Actor* child;

    player = PLAYER;
    child = this->actor.child;
    this->unk1B8 = player->unk_858;
    this->actor.world.pos = player->bodyPartsPos[0];
    this->actor.shape.rot.y = player->actor.shape.rot.y + 0x8000;
    if ((this->unk1CA == 0) && (player->unk_858 >= 0.10f)) {
        if ((gSaveContext.unk_13F0 != 0) || ((((this->actor.params & 0xFF00) >> 8) != 0) &&
                                             (func_80087708(globalCtx, (this->actor.params & 0xFF00) >> 8, 4) == 0))) {
            func_80A9F350(this, globalCtx);
            EnMThunder_SetupAction(this, func_80A9F350);
            this->unk1C8 = 0;
            this->unk1BC = 0;
            this->unk1AC = 0.0f;
            return;
        }
        this->unk1CA = 1;
    }
    if (player->unk_858 >= 0.10f) {
        func_800AA000(0.0f, (u8)(s32)(player->unk_858 * 150.0f), 2, (u8)(s32)(player->unk_858 * 150.0f));
    }
    if (player->stateFlags2 & 0x20000) {
        if ((child != NULL) && (child->update != NULL)) {
            child->parent = NULL;
        }
        if (player->unk_858 <= 0.15f) {
            if ((player->unk_858 >= 0.10f) && (player->swordAnimation >= 0x18)) {
                Audio_PlaySoundGeneral(NA_SE_IT_ROLLING_CUT, &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                                       &D_801333E8);
                Audio_PlaySoundGeneral(NA_SE_IT_SWORD_SWING_HARD, &player->actor.projectedPos, 4U, &D_801333E0,
                                       &D_801333E0, &D_801333E8);
            }
            Actor_Kill(&this->actor);
            return;
        }
        player->stateFlags2 &= ~0x20000;
        if (((this->actor.params & 0xFF00) >> 8) != 0) {
            gSaveContext.unk_13F0 = 1;
        }
        if (player->unk_858 < 0.85f) {
            this->unk14C.info.toucher.dmgFlags = D_80AA044C[this->unk1C7];
            this->unk1C6 = 1;
            if (this->unk1C7 == 1) {
                this->unk1C9 = 2;
            } else {
                this->unk1C9 = 4;
            }
        } else {
            this->unk14C.info.toucher.dmgFlags = D_80AA0458[this->unk1C7];
            this->unk1C6 = 0;
            if (this->unk1C7 == 1) {
                this->unk1C9 = 4;
            } else {
                this->unk1C9 = 8;
            }
        }
        EnMThunder_SetupAction(this, func_80A9F9B4);
        this->unk1C4 = 8;
        Audio_PlaySoundGeneral(D_80AA0464[this->unk1C6], &player->actor.projectedPos, 4U, &D_801333E0, &D_801333E0,
                               &D_801333E8);
        this->unk1AC = 1.0f;
        return;
    }
    if (!(player->stateFlags1 & 0x1000)) {
        if (this->actor.child != NULL) {
            this->actor.child->parent = NULL;
        }
        Actor_Kill(&this->actor);
        return;
    }
    if (player->unk_858 > 0.15f) {
        this->unk1C8 = 0xFF;
        if (this->actor.child == NULL) {
            Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EFF_DUST, this->actor.world.pos.x,
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
    if (Gameplay_InCsMode(globalCtx)) {
        Actor_Kill(&this->actor);
    }
}

void func_80A9F938(EnMThunder* this, GlobalContext* globalCtx) {
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

void func_80A9F9B4(EnMThunder* this, GlobalContext* globalCtx) {
    Player* player;

    player = PLAYER;
    if (Math_StepToF(&this->unk1AC, 0.0f, 0.0625f)) {
        Actor_Kill(&this->actor);
    } else {
        Math_SmoothStepToF(&this->actor.scale.x, (s32)this->unk1C9, 0.6f, 0.8f, 0.0f);
        Actor_SetScale(&this->actor, this->actor.scale.x);
        this->unk14C.dim.radius = this->actor.scale.x * 25.0f;
        Collider_UpdateCylinder(&this->actor, &this->unk14C);
        CollisionCheck_SetAT(globalCtx, &globalCtx->colChkCtx, &this->unk14C.base);
    }
    if (this->unk1C4 > 0) {
        this->actor.world.pos.x = player->bodyPartsPos[0].x;
        this->actor.world.pos.z = player->bodyPartsPos[0].z;
        this->unk1C4 = this->unk1C4 - 1;
    }
    if (this->unk1AC > 0.6f) {
        this->unk1B0 = 1.0f;
    } else {
        this->unk1B0 = this->unk1AC * 1.6666666f;
    }
    func_80A9F938(this, globalCtx);
    if (Gameplay_InCsMode(globalCtx)) {
        Actor_Kill(&this->actor);
    }
}

void EnMThunder_Update(Actor* thisx, GlobalContext* globalCtx) {
    f32 temp_fv0;
    s32 temp_v0;
    EnMThunder* this = (EnMThunder*)thisx;

    this->actionFunc(this, globalCtx);
    func_80A9F314(globalCtx, this->unk1BC);
    temp_fv0 = this->unk1AC;
    temp_v0 = (u32)(temp_fv0 * 255.0f) & 0xFF;
    Lights_PointNoGlowSetInfo(&this->unk19C, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                              temp_v0, temp_v0, temp_fv0 * 100.0f, temp_fv0 * 800.0f);
}

void EnMThunder_Draw(Actor* thisx, GlobalContext* globalCtx2) {
    f32 var_fa1;
    GlobalContext* globalCtx = globalCtx2;
    Player* player;
    EnMThunder* this = (EnMThunder*)thisx;
    s32 var_t1;

    player = PLAYER;
    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_m_thunder.c", 0x34C);
    func_80093D84(globalCtx->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_m_thunder.c", 0x355),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    switch (this->unk1C6) {
        case 0:
        case 1:
            gSPSegment(POLY_XLU_DISP++, 8,
                       Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, 0xFF - ((u8)(s32)(this->unk1B4 * 30.0f) & 0xFF), 0,
                                        64, 32, 1, 0xFF - ((u8)(s32)(this->unk1B4 * 20.0f) & 0xFF), 0, 8, 8));
            break;
    }
    switch (this->unk1C6) {
        case 0:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 0xAA, (u8)(this->unk1B0 * 255.0f));
            gSPDisplayList(POLY_XLU_DISP++, D_4012AF0);
            gSPDisplayList(POLY_XLU_DISP++, D_4012C10);
            break;
        case 1:
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 0xAA, 255, 255, (u8)(this->unk1B0 * 255.0f));
            gSPDisplayList(POLY_XLU_DISP++, D_4012570);
            gSPDisplayList(POLY_XLU_DISP++, D_4012690);
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
        var_fa1 = (D_80AA046C[globalCtx->gameplayFrames & 7] * 6.0f) + 1.0f;
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 255, 255, 0xAA, this->unk1C8);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 100, 0, 128);
        var_t1 = 0x28;
    } else {
        var_fa1 = (2.0f * D_80AA046C[globalCtx->gameplayFrames & 7]) + 1.0f;
        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x80, 0xAA, 255, 255, this->unk1C8);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 100, 255, 128);
        var_t1 = 0x14;
    }
    Matrix_Scale(1.0f, var_fa1, var_fa1, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_en_m_thunder.c", 0x3C0),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(POLY_XLU_DISP++, 9,
               Gfx_TwoTexScroll(globalCtx->state.gfxCtx, 0, (globalCtx->gameplayFrames * 5) & 0xFF, 0U, 32, 32, 1,
                                (globalCtx->gameplayFrames * 0x14) & 0xFF, (globalCtx->gameplayFrames * var_t1) & 0xFF,
                                8, 8));
    gSPDisplayList(POLY_XLU_DISP++, D_4013610);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_m_thunder.c", 0x407);
}
