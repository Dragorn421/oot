#include "z_demo_ext.h"

#define FLAGS 0x00000010

#define THIS ((DemoExt*)thisx)

void DemoExt_Init(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Destroy(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Update(Actor* thisx, GlobalContext* globalCtx);
void DemoExt_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_80977854(DemoExt*, GlobalContext*);
void func_80977874(DemoExt* arg0, GlobalContext* arg1);
void func_809778AC(DemoExt* arg0, GlobalContext* arg1);
void func_80977944(DemoExt* arg0, GlobalContext* arg1);
void func_80977950(DemoExt* arg0, GlobalContext* arg1);

extern Gfx D_600FAA0[];

typedef void (*DemoExtUpdateFunc)(DemoExt*, GlobalContext*);
static DemoExtUpdateFunc sUpdateFuncs[] = {
    func_80977854,
    func_80977874,
    func_809778AC,
};
typedef void (*DemoExtDrawFunc)(DemoExt*, GlobalContext*);
static DemoExtDrawFunc sDrawFuncs[] = {
    func_80977944,
    func_80977950,
};
const ActorInit Demo_Ext_InitVars = {
    ACTOR_DEMO_EXT,
    ACTORTYPE_NPC,
    FLAGS,
    OBJECT_FHG,
    sizeof(DemoExt),
    (ActorFunc)DemoExt_Init,
    (ActorFunc)DemoExt_Destroy,
    (ActorFunc)DemoExt_Update,
    (ActorFunc)DemoExt_Draw,
};

void DemoExt_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void DemoExt_Init(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* this = (DemoExt*)thisx;

    this->texScrollStep[0] = 25;
    this->texScrollStep[1] = 40;
    this->texScrollStep[2] = 5;
    this->texScrollStep[3] = 30;
    this->unk170 = gGameInfo->data[0xA3C] + 0xFF;
    this->unk174 = gGameInfo->data[0xA40] + 0xFF;
    this->unk178.x = gGameInfo->data[0xA33] + 400.0f;
    this->unk178.y = gGameInfo->data[0xA34] + 100.0f;
    this->unk178.z = gGameInfo->data[0xA35] + 400.0f;
}

void func_80977450(DemoExt* this) {
    if (this->unk16C <= (gGameInfo->data[0xA43] + 40.0f - 15.0f)) {
        Audio_PlaySoundGeneral(NA_SE_EV_FANTOM_WARP_L - SFX_FLAG, &this->actor.projectedPos, 4U, &D_801333E0,
                               &D_801333E0, &D_801333E8);
    }
}

CsCmdActorAction* func_809774D8(GlobalContext* globalCtx, s32 arg1) {
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        return globalCtx->csCtx.npcActions[arg1];
    }
    return NULL;
}

void func_809774FC(DemoExt* this) {
    this->unk14C = 0;
    this->unk150 = 0;
}

void func_80977508(DemoExt* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;
    s16 temp_v1;

    temp_v0 = func_809774D8(globalCtx, 5);
    if (temp_v0 != NULL) {
        this->actor.posRot.pos.x = temp_v0->startPos.x;
        this->actor.posRot.pos.y = temp_v0->startPos.y;
        this->actor.posRot.pos.z = temp_v0->startPos.z;
        temp_v1 = temp_v0->rot.y;
        this->actor.shape.rot.y = temp_v1;
        this->actor.posRot.rot.y = temp_v1;
    }
    this->unk14C = 1;
    this->unk150 = 1;
}

void func_80977590(DemoExt* this) {
    this->unk14C = 2;
    this->unk150 = 1;
}

void func_809775A4(DemoExt* this) {
    this->unk16C += 1.0f;
    if ((gGameInfo->data[0xA43] + 40.0f) <= this->unk16C) {
        Actor_Kill(&this->actor);
    }
}

void func_80977610(DemoExt* this, GlobalContext* globalCtx) {
    CsCmdActorAction* temp_v0;
    s32 temp_a2;
    s32 v;

    temp_v0 = func_809774D8(globalCtx, 5);
    if (temp_v0 != NULL) {
        temp_a2 = temp_v0->action;
        v = this->unk154;
        if (temp_a2 != v) {
            switch (temp_a2) {
                case 1:
                    func_809774FC(this);
                    break;

                case 2:
                    func_80977508(this, globalCtx);
                    break;

                case 3:
                    func_80977590(this);
                    break;

                default:
                    osSyncPrintf("Demo_Ext_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            this->unk154 = temp_a2;
        }
    }
}

void func_809776D0(DemoExt* this) {
    s16* texScrollPos = this->texScrollPos;
    s16* texScrollStep = this->texScrollStep;
    s32 var_v0;

    for (var_v0 = 3; var_v0 != 0; var_v0--) {
        texScrollPos[var_v0] += texScrollStep[var_v0];
    }
    this->unk168 += (s16)(gGameInfo->data[0xA42] + 0x3E8);
}

void func_8097771C(DemoExt* this) {
    Vec3f* temp_v0;
    f32 var_fv0;

    temp_v0 = &this->unk178;
    var_fv0 = ((gGameInfo->data[0xA43] + 40.0f) - this->unk16C) / (gGameInfo->data[0xA43] + 40.0f);
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    this->unk170 = (s32)((u32)(gGameInfo->data[0xA3C] + 0xFF) * var_fv0);
    this->unk174 = (s32)((u32)(gGameInfo->data[0xA40] + 0xFF) * var_fv0);
    temp_v0->x = (gGameInfo->data[0xA33] + 400.0f) * var_fv0;
    temp_v0->y = (gGameInfo->data[0xA34] + 100.0f) * var_fv0;
    temp_v0->z = (gGameInfo->data[0xA35] + 400.0f) * var_fv0;
}

void func_80977854(DemoExt* this, GlobalContext* globalCtx) {
    func_80977610(this, globalCtx);
}

void func_80977874(DemoExt* this, GlobalContext* globalCtx) {
    func_80977450(this);
    func_809776D0(this);
    func_80977610(this, globalCtx);
}

void func_809778AC(DemoExt* this, GlobalContext* globalCtx) {
    func_80977450(this);
    func_809776D0(this);
    func_8097771C(this);
    func_809775A4(this);
}

void DemoExt_Update(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* this = (DemoExt*)thisx;

    if ((this->unk14C < 0) || (this->unk14C >= ARRAY_COUNT(sUpdateFuncs)) || (sUpdateFuncs[this->unk14C] == NULL)) {
        osSyncPrintf("\x1b[31mメインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n\x1b[m");
        return;
    }
    sUpdateFuncs[this->unk14C](this, globalCtx);
}

void func_80977944(DemoExt* this, GlobalContext* globalCtx) {
}

void func_80977950(DemoExt* this, GlobalContext* globalCtx) {
    GraphicsContext* gfxCtx;
    Mtx* mtx;
    Vec3f* temp_v0;
    s16* texScrollPos;
    s32 pad;

    gfxCtx = globalCtx->state.gfxCtx;
    texScrollPos = this->texScrollPos;
    temp_v0 = &this->unk178;
    mtx = Graph_Alloc(gfxCtx, 0x40U);
    OPEN_DISPS(gfxCtx, "../z_demo_ext.c", 460);
    Matrix_Push();
    Matrix_Scale(temp_v0->x, temp_v0->y, temp_v0->z, MTXMODE_APPLY);
    Matrix_RotateRPY(gGameInfo->data[0xA30] + 0x4000, this->unk168, gGameInfo->data[0xA32], MTXMODE_APPLY);
    Matrix_Translate(gGameInfo->data[0xA36], gGameInfo->data[0xA37], gGameInfo->data[0xA38], MTXMODE_APPLY);
    Matrix_ToMtx(mtx, "../z_demo_ext.c", 476);
    Matrix_Pull();
    func_80093D84(gfxCtx);
    gDPSetPrimColor(POLY_XLU_DISP++, 0x00, gGameInfo->data[0xA41] + 0x80, gGameInfo->data[0xA39] + 0x8C,
                    gGameInfo->data[0xA3A] + 0x50, gGameInfo->data[0xA3B] + 0x8C, this->unk170);
    gDPSetEnvColor(POLY_XLU_DISP++, gGameInfo->data[0xA3D] + 0x5A, gGameInfo->data[0xA3E] + 0x32,
                   gGameInfo->data[0xA3F] + 0x5F, this->unk174);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(gfxCtx, 0, texScrollPos[0], texScrollPos[1], 64, 64, 1, texScrollPos[2],
                                texScrollPos[3], 64, 64));
    gSPMatrix(POLY_XLU_DISP++, mtx, G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, D_600FAA0);
    gSPPopMatrix(POLY_XLU_DISP++, G_MTX_MODELVIEW);
    CLOSE_DISPS(gfxCtx, "../z_demo_ext.c", 512);
}

void DemoExt_Draw(Actor* thisx, GlobalContext* globalCtx) {
    DemoExt* this = (DemoExt*)thisx;

    if ((this->unk150 < 0) || (this->unk150 >= ARRAY_COUNT(sDrawFuncs)) || (sDrawFuncs[this->unk150] == NULL)) {
        osSyncPrintf("\x1b[31m描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n\x1b[m");
        return;
    }
    sDrawFuncs[this->unk150](this, globalCtx);
}
