/*
 * File: z_obj_timeblock.c
 * Overlay: ovl_Obj_Timeblock
 * Description: Song of Time Block
 */

#include "z_obj_timeblock.h"

#define FLAGS 0x0A000011

#define THIS ((ObjTimeblock*)thisx)

void ObjTimeblock_Init(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Update(Actor* thisx, GlobalContext* globalCtx);
void ObjTimeblock_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 func_80BA040C(ObjTimeblock*, GlobalContext*);
s32 func_80BA0480(ObjTimeblock*, GlobalContext*);
void func_80BA0508(ObjTimeblock* arg0, GlobalContext* arg1);
void func_80BA0514(ObjTimeblock* arg0);
void func_80BA0524(ObjTimeblock* arg0, GlobalContext* arg1);
void func_80BA0758(ObjTimeblock* arg0);
void func_80BA0768(ObjTimeblock* arg0, GlobalContext* arg1);
void func_80BA083C(ObjTimeblock* arg0);
void func_80BA084C(ObjTimeblock* arg0, GlobalContext* arg1);

extern Gfx D_6000980[];
extern UNK_TYPE D_6000B30;

const ActorInit Obj_Timeblock_InitVars = {
    ACTOR_OBJ_TIMEBLOCK,
    ACTORTYPE_ITEMACTION,
    FLAGS,
    OBJECT_TIMEBLOCK,
    sizeof(ObjTimeblock),
    (ActorFunc)ObjTimeblock_Init,
    (ActorFunc)ObjTimeblock_Destroy,
    (ActorFunc)ObjTimeblock_Update,
    (ActorFunc)ObjTimeblock_Draw,
};
typedef struct struct_80BA0AF0 {
    f32 unk0;
    f32 unk4;
    s16 unk8;
} struct_80BA0AF0;
struct_80BA0AF0 D_80BA0AF0[2] = { { 1.0f, 60.0f, 0x18 }, { 0.6f, 40.0f, 0x19 } };
static f32 D_80BA0B08[8] = { 60.0f, 100.0f, 140.0f, 180.0f, 220.0f, 260.0f, 300.0f, 300.0f };
static InitChainEntry D_80BA0B28[4] = {
    ICHAIN_U8(unk_1F, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1500, ICHAIN_STOP),
};
static Color_RGB8 D_80BA0B38[8] = {
    { 0x64, 0x78, 0x8C }, { 0x50, 0x8C, 0xC8 }, { 0x64, 0x96, 0xC8 }, { 0x64, 0xC8, 0xF0 },
    { 0x50, 0x6E, 0x8C }, { 0x46, 0xA0, 0xE1 }, { 0x50, 0x64, 0x82 }, { 0x64, 0x6E, 0xBE },
};

s32 func_80B9FFA0(ObjTimeblock* this) {
    s32 var_a1;
    s32 var_v1;
    s32 var_v1_2;

    if (!((this->dyna.actor.params >> 0xA) & 1)) {
        if (this->unk177 == 0) {
            return this->unk175;
        }
        if ((this->dyna.actor.params >> 0xF) & 1) {
            var_a1 = 1;
        } else {
            var_a1 = 0;
        }
        if (this->unk177 == 1) {
            return this->unk174 ^ var_a1;
        }
        if (LINK_AGE_IN_YEARS == 5) {
            var_v1_2 = 1;
        } else {
            var_v1_2 = 0;
        }
        return this->unk174 ^ var_a1 ^ var_v1_2;
    }
    return (((this->dyna.actor.params >> 0xF) & 1) ? 1 : 0) ^ this->unk174;
}

void func_80BA0058(ObjTimeblock* this, GlobalContext* globalCtx) {
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, this->dyna.actor.posRot.pos.x,
                this->dyna.actor.posRot.pos.y, this->dyna.actor.posRot.pos.z, 0, 0, 0,
                (D_80BA0AF0[(this->dyna.actor.params >> 8) & 1].unk8));
}

void func_80BA00CC(GlobalContext* globalCtx, s32 arg1) {
    if (Flags_GetSwitch(globalCtx, arg1) != 0) {
        Flags_UnsetSwitch(globalCtx, arg1);
        return;
    }
    Flags_SetSwitch(globalCtx, arg1);
}

void ObjTimeblock_Init(Actor* thisx, GlobalContext* globalCtx) {
    ObjTimeblock* this = (ObjTimeblock*)thisx;
    s32 pad;
    s32 sp2C;

    sp2C = 0;
    DynaPolyInfo_SetActorMove(&this->dyna, DPM_UNK);
    this->dyna.actor.posRot.rot.z = this->dyna.actor.shape.rot.z = 0;
    DynaPolyInfo_Alloc(&D_6000B30, &sp2C);
    this->dyna.dynaPolyId = DynaPolyInfo_RegisterActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, sp2C);
    Actor_ProcessInitChain(&this->dyna.actor, D_80BA0B28);
    Actor_SetScale(&this->dyna.actor, D_80BA0AF0[(this->dyna.actor.params >> 8) & 1].unk0);
    if ((this->dyna.actor.params >> 6) & 1) {
        this->unk177 = 0;
    } else if ((this->dyna.actor.params & 0x3F) < 0x38) {
        this->unk177 = 2;
    } else {
        this->unk177 = 1;
    }
    this->unk168 = func_80BA040C;
    Actor_SetHeight(&this->dyna.actor, D_80BA0AF0[(this->dyna.actor.params >> 8) & 1].unk4);
    if (Flags_GetSwitch(globalCtx, this->dyna.actor.params & 0x3F)) {
        this->unk174 = 1;
    } else {
        this->unk174 = 0;
    }
    if ((this->dyna.actor.params >> 0xF) & 1) {
        this->unk175 = 1;
    } else {
        this->unk175 = 0;
    }
    this->unk178 = func_80B9FFA0(this);
    if (((this->dyna.actor.params >> 0xA) & 1) == 0) {
        func_80BA0514(this);
    } else if (this->unk178 != 0) {
        func_80BA0758(this);
    } else {
        func_80BA083C(this);
    }
    osSyncPrintf("時のブロック (<arg> %04xH <type> save:%d color:%d range:%d move:%d)\n",
                 this->dyna.actor.params & 0xFFFF, this->unk177, this->dyna.actor.initPosRot.rot.z & 7,
                 (this->dyna.actor.params >> 0xB) & 7, (this->dyna.actor.params >> 0xA) & 1);
}

void ObjTimeblock_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    ObjTimeblock* this = (ObjTimeblock*)thisx;

    DynaPolyInfo_Free(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
}

s32 func_80BA032C(ObjTimeblock* this, GlobalContext* globalCtx) {
    Vec3f sp1C;
    f32 temp_fv1;

    if ((this->unk178 != 0) && (func_80043590(&this->dyna) != 0)) {
        return 0;
    }
    if ((this->dyna.actor.xzDistFromLink <= D_80BA0B08[(this->dyna.actor.params >> 0xB) & 7])) {
        func_8002DBD0(&this->dyna.actor, &sp1C, &PLAYER->actor.posRot.pos);
        temp_fv1 = (this->dyna.actor.scale.x * 50.0f) + 6.0f;
        if ((temp_fv1 < fabsf(sp1C.x)) || (temp_fv1 < fabsf(sp1C.z))) {
            return 1;
        }
    }
    return 0;
}

s32 func_80BA040C(ObjTimeblock* this, GlobalContext* globalCtx) {
    Player* temp_v1;

    temp_v1 = PLAYER;
    if (func_80BA032C(this, globalCtx) != 0) {
        if (temp_v1->stateFlags2 & 0x01000000) {
            func_8010BD58(globalCtx, 1U);
            this->unk168 = func_80BA0480;
        } else {
            temp_v1->stateFlags2 |= 0x800000;
        }
    }
    return 0;
}

s32 func_80BA0480(ObjTimeblock* this, GlobalContext* globalCtx) {
    if (globalCtx->msgCtx.unk_E3EE == 4) {
        this->unk168 = func_80BA040C;
    }
    if (globalCtx->msgCtx.unk_E3EC == 0xA) {
        if (this->unk172 == 0xFE) {
            this->unk16E = 0x6E;
        } else {
            this->unk16E -= 1;
            if (this->unk16E == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void func_80BA04F8(ObjTimeblock* this) {
    this->unk164 = func_80BA0508;
}

void func_80BA0508(ObjTimeblock* this, GlobalContext* globalCtx) {
}

void func_80BA0514(ObjTimeblock* this) {
    this->unk164 = func_80BA0524;
}

void func_80BA0524(ObjTimeblock* this, GlobalContext* globalCtx) {
    s32 var_v1;

    if ((this->unk168(this, globalCtx) != 0) && (this->unk16C <= 0)) {
        func_80BA0058(this, globalCtx);
        this->unk16C = 0xA0;
        func_80080480(globalCtx, &this->dyna.actor);
        osSyncPrintf("◯◯◯◯ Time Block 注目カメラ (frame counter  %d)\n", globalCtx->state.frames);
        this->unk170 = 0xC;
        if (this->unk177 == 0) {
            this->dyna.actor.params ^= 0x8000;
        } else {
            func_80BA00CC(globalCtx, this->dyna.actor.params & 0x3F);
        }
    }
    this->unk172 = globalCtx->msgCtx.unk_E3EC;
    if (this->unk170 > 0) {
        this->unk170 -= 1;
        if (this->unk170 == 0) {
            if (this->unk177 == 0) {
                if ((this->dyna.actor.params >> 0xF) & 1) {
                    this->unk175 = 1;
                } else {
                    this->unk175 = 0;
                }
            } else if (Flags_GetSwitch(globalCtx, this->dyna.actor.params & 0x3F) != 0) {
                this->unk174 = 1;
            } else {
                this->unk174 = 0;
            }
        }
    }
    var_v1 = func_80B9FFA0(this);
    if ((this->unk177 == 1) && (var_v1 != this->unk178)) {
        func_80BA04F8(this);
    }
    this->unk178 = var_v1;
    if (this->unk16C == 0x32) {
        func_80078884(0x4807U);
    }
}

void func_80BA06AC(ObjTimeblock* this, GlobalContext* globalCtx) {
    s32 sp24;
    s16 temp_v0;

    sp24 = this->dyna.actor.params & 0x3F;
    temp_v0 = this->unk170;
    this->unk172 = globalCtx->msgCtx.unk_E3EC;
    if (temp_v0 > 0) {
        this->unk170 = temp_v0 - 1;
        if (this->unk170 == 0) {
            if (Flags_GetSwitch(globalCtx, sp24) != 0) {
                this->unk174 = 1;
            } else {
                this->unk174 = 0;
            }
        }
    }
    this->unk178 = func_80B9FFA0(this);
    if (Flags_GetSwitch(globalCtx, sp24) != 0) {
        this->unk176 = 1;
        return;
    }
    this->unk176 = 0;
}

void func_80BA0758(ObjTimeblock* this) {
    this->unk164 = func_80BA0768;
}

void func_80BA0768(ObjTimeblock* this, GlobalContext* globalCtx) {
    if ((this->unk168(this, globalCtx) != 0) && (this->unk16C <= 0)) {
        this->unk170 = 0xC;
        func_80BA0058(this, globalCtx);
        this->unk16C = 0xA0;
        func_80080480(globalCtx, &this->dyna.actor);
        osSyncPrintf("◯◯◯◯ Time Block 注目カメラ (frame counter  %d)\n", globalCtx->state.frames);
        func_80BA00CC(globalCtx, this->dyna.actor.params & 0x3F);
    }
    func_80BA06AC(this, globalCtx);
    if (this->unk16C == 0x32) {
        func_80078884(NA_SE_SY_TRE_BOX_APPEAR);
    }
    if ((this->unk178 == 0) && (this->unk16C <= 0)) {
        func_80BA083C(this);
    }
}

void func_80BA083C(ObjTimeblock* this) {
    this->unk164 = func_80BA084C;
}

void func_80BA084C(ObjTimeblock* this, GlobalContext* globalCtx) {
    s32 var_v1;
    s32 flag = this->dyna.actor.params & 0x3F;

    if (Flags_GetSwitch(globalCtx, flag) != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if (this->unk176 ^ var_v1) {
        if (((((s16)this->dyna.actor.params >> 0xF) & 1) ? 1 : 0) ^ var_v1) {
            if (this->unk16C <= 0) {
                func_80BA0058(this, globalCtx);
                this->unk16C = 0xA0;
            }
            this->unk170 = 0xC;
        }
    }
    func_80BA06AC(this, globalCtx);
    if ((this->unk178 != 0) && (this->unk16C <= 0)) {
        func_80BA0758(this);
    }
}

void ObjTimeblock_Update(Actor* thisx, GlobalContext* globalCtx) {
    s16 temp_v0;
    ObjTimeblock* this = (ObjTimeblock*)thisx;

    this->unk164(this, globalCtx);
    temp_v0 = this->unk16C;
    if (temp_v0 > 0) {
        this->unk16C = temp_v0 - 1;
    }
    if (this->unk178 != 0) {
        func_8003EC50(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
        return;
    }
    func_8003EBF8(globalCtx, &globalCtx->colCtx.dyna, this->dyna.dynaPolyId);
}

void ObjTimeblock_Draw(Actor* thisx, GlobalContext* globalCtx) {
    Color_RGB8* sp44;

    if (((ObjTimeblock*)thisx)->unk178 != 0) {
        sp44 = &D_80BA0B38[thisx->initPosRot.rot.z & 7];
        OPEN_DISPS(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 762);
        func_80093D18(globalCtx->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 766),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_OPA_DISP++, 0x00, 0x00, sp44->r, sp44->g, sp44->b, 255);
        gSPDisplayList(POLY_OPA_DISP++, D_6000980);
        CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_obj_timeblock.c", 772);
    }
}
