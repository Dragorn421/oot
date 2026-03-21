#include "z_bg_jya_bigmirror.h"

#define FLAGS 0x00000030

#define THIS ((BgJyaBigmirror*)thisx)

void BgJyaBigmirror_Init(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Update(Actor* thisx, GlobalContext* globalCtx);
void BgJyaBigmirror_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_808936E0(BgJyaBigmirror* this, GlobalContext* globalCtx);
void func_80893750(BgJyaBigmirror* this, GlobalContext* globalCtx);
void func_8089394C(BgJyaBigmirror* this, GlobalContext* globalCtx);
void func_8089399C(BgJyaBigmirror* this, GlobalContext* globalCtx);
void func_80893C68(BgJyaBigmirror* this, GlobalContext* globalCtx);

extern Gfx D_600BC70[];
extern Gfx D_600BD80[];
extern Gfx D_600E1B0[];
extern Gfx D_600E2D0[];

static u8 D_80893ED0 = 0;

const ActorInit Bg_Jya_Bigmirror_InitVars = {
    ACTOR_BG_JYA_BIGMIRROR,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaBigmirror),
    (ActorFunc)BgJyaBigmirror_Init,
    (ActorFunc)BgJyaBigmirror_Destroy,
    (ActorFunc)BgJyaBigmirror_Update,
    (ActorFunc)BgJyaBigmirror_Draw,
};

typedef struct struct_80893EF4 {
    /* 0x00 */ Vec3f unk0;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ char pad12[2];
} struct_80893EF4;
static struct_80893EF4 D_80893EF4[2] = {
    { { -560.0f, 1743.0f, -310.0f }, -0xFF, 0x4000, 0x8000 },
    { { 60.0f, 1743.0f, -310.0f }, -0xFE, 0x8000, 0xA000 },
};
static u8 D_80893F1C[4] = { 1, 2, 0, 0 };
static s16 D_80893F20[4] = { 5, 7, 8, 0 };
static Vec3f D_80893F28[3] = {
    { 60.0f, 1802.0f, -1102.0f },
    { -560.0f, 1800.0f, -310.0f },
    { 60.0f, 1800.0f, -310.0f },
};
static Vec3s D_80893F4C = { 0, 0, 0 };

void func_808936E0(BgJyaBigmirror* this, GlobalContext* globalCtx) {
    this->unk15C &= ~0x38;
    if (globalCtx->roomCtx.curRoom.num == 5) {
        this->unk15C |= 8;
    } else if (globalCtx->roomCtx.curRoom.num == 0x19) {
        this->unk15C |= 0x10;
    } else if (globalCtx->roomCtx.curRoom.num == 0x1A) {
        this->unk15C |= 0x20;
    }
}

void func_80893750(BgJyaBigmirror* this2, GlobalContext* globalCtx) {
    s32 i;
    BgJyaBigmirror_sub14C* new_var;
    struct_80893EF4* new_var2;
    BgJyaBigmirror* this = this2;

    if (this->unk15C & 0x30) {
        for (i = 0; i < 2; i++) {
            new_var2 = &D_80893EF4[i];
            new_var = &this->unk14C[i];
            if (new_var->unk0 != NULL) {
                new_var->unk4 = new_var->unk0->shape.rot.y;
                if (new_var->unk4 == new_var2->unkE) {
                    this->unk15C |= D_80893F1C[i];
                } else {
                    this->unk15C &= ~D_80893F1C[i];
                }
                if (new_var->unk0->update == NULL) {
                    osSyncPrintf("Error : コブラ削除された (%s %d)\n", "../z_bg_jya_bigmirror.c", 203);
                }
            } else {
                new_var->unk0 = Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_BG_JYA_COBRA,
                                                   new_var2->unk0.x, new_var2->unk0.y, new_var2->unk0.z, 0,
                                                   new_var->unk4, 0, new_var2->unkC);
                this->actor.child = NULL;
                if (new_var->unk0 == NULL) {
                    osSyncPrintf("Error : コブラ発生失敗 (%s %d)\n", "../z_bg_jya_bigmirror.c", 221);
                }
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            new_var = &this->unk14C[i];
            if (new_var->unk0 != NULL) {
                if (new_var->unk0->child != NULL) {
                    Actor_Kill(new_var->unk0->child);
                    new_var->unk0->child = NULL;
                }
                Actor_Kill(new_var->unk0);
                new_var->unk0 = NULL;
            }
        }
    }
}

void func_8089394C(BgJyaBigmirror* this, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, 0x29)) {
        this->unk15C |= 4;
    } else {
        this->unk15C &= ~4;
    }
}

void func_8089399C(BgJyaBigmirror* this2, GlobalContext* globalCtx) {
    s32 var_s1;
    s32 var_v0;
    s32 sp74[3];
    BgJyaBigmirror* this = this2;
    s32 temp_v0;

    temp_v0 = Object_GetIndex(&globalCtx->objectCtx, OBJECT_MIR_RAY);
    if ((temp_v0 < 0) || ((temp_v0 != this->unk16C))) {
        this->unk160[2] = NULL;
        this->unk160[1] = NULL;
        this->unk160[0] = NULL;
    } else {
        var_v0 = (this->unk15C & 0x18) != 0;
        if (var_v0 != 0) {
            var_v0 = (this->unk15C & 2) != 0;
            if (var_v0 != 0) {
                var_v0 = (this->unk15C & 1) != 0;
            }
        }
        sp74[0] = var_v0;
        sp74[1] = sp74[2] = var_v0 = this->unk15C & 0x30;

        for (var_s1 = 0; var_s1 < 3; var_s1++) {
            if (sp74[var_s1] != 0) {
                if ((this->unk160[var_s1] == NULL) && (Object_IsLoaded(&globalCtx->objectCtx, temp_v0) != 0)) {
                    this->unk160[var_s1] =
                        Actor_Spawn(&globalCtx->actorCtx, globalCtx, 0xB7, D_80893F28[var_s1].x, D_80893F28[var_s1].y,
                                    D_80893F28[var_s1].z, 0, 0, 0, D_80893F20[var_s1]);
                    if (this->unk160[var_s1] == NULL) {
                        osSyncPrintf("Error : Mir Ray 発生失敗 (%s %d)\n", "../z_bg_jya_bigmirror.c", 310);
                    }
                }
            } else {
                if (this->unk160[var_s1] != NULL) {
                    Actor_Kill(this->unk160[var_s1]);
                    this->unk160[var_s1] = NULL;
                }
            }
        }
    }
    this->unk16C = temp_v0;
}

void BgJyaBigmirror_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* this = (BgJyaBigmirror*)thisx;

    if (D_80893ED0 != 0) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_SetScale(&this->actor, 0.1f);
    this->unk14C[0].unk4 = D_80893EF4[0].unk10;
    this->unk14C[1].unk4 = D_80893EF4[1].unk10;
    this->actor.room = -1;
    D_80893ED0 = 1;
    this->unk15D = 1;
    this->unk16C = -1;
    osSyncPrintf("(jya 大鏡)(arg_data 0x%04x)\n", this->actor.params);
}

void BgJyaBigmirror_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* this = (BgJyaBigmirror*)thisx;

    if (this->unk15D != 0) {
        D_80893ED0 = 0;
    }
}

void BgJyaBigmirror_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* this = (BgJyaBigmirror*)thisx;

    func_808936E0(this, globalCtx);
    func_80893750(this, globalCtx);
    func_8089394C(this, globalCtx);
    func_8089399C(this, globalCtx);
}

void func_80893C68(BgJyaBigmirror* this, GlobalContext* globalCtx) {
    s32 pad;
    Actor* temp_v0;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 435);
    func_80093D84(globalCtx->state.gfxCtx);
    temp_v0 = Actor_Find(&globalCtx->actorCtx, ACTOR_BG_JYA_LIFT, ACTORTYPE_BG);
    if (temp_v0 != NULL) {
        this->unk170 = temp_v0->posRot.pos.y;
    }
    func_800D1694(this->actor.posRot.pos.x, this->actor.posRot.pos.y + 40.0f, this->actor.posRot.pos.z,
                  &this->actor.shape.rot);
    Matrix_Scale(0.1f, (this->unk170 * -0.00078125f) + 1.3901563f, 0.1f, MTXMODE_APPLY);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 457),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (1) {}
    gSPDisplayList(POLY_XLU_DISP++, D_600BC70);
    if (temp_v0 != NULL) {
        func_800D1694(temp_v0->posRot.pos.x, temp_v0->posRot.pos.y, temp_v0->posRot.pos.z, &D_80893F4C);
        Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 467),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, D_600BD80);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_jya_bigmirror.c", 476);
}

void BgJyaBigmirror_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgJyaBigmirror* this = (BgJyaBigmirror*)thisx;

    if (this->unk15C & 0x10) {
        Gfx_DrawDListOpa(globalCtx, D_600E1B0);
        Gfx_DrawDListXlu(globalCtx, D_600E2D0);
    }
    if ((this->unk15C & 0x38) && (this->unk15C & 2) && (this->unk15C & 1)) {
        func_80893C68(this, globalCtx);
    }
}
