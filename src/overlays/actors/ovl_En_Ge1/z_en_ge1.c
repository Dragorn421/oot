#include "z_en_ge1.h"

#define FLAGS 0x00000009

#define THIS ((EnGe1*)thisx)

void EnGe1_Init(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Update(Actor* thisx, GlobalContext* globalCtx);
void EnGe1_Draw(Actor* thisx, GlobalContext* globalCtx);

s32 func_80A30DCC(void);
void func_80A30F48(EnGe1* this, GlobalContext* globalCtx);
void func_80A31094(EnGe1* this, GlobalContext* globalCtx);
void func_80A310C4(EnGe1* this, GlobalContext* globalCtx);
void func_80A3118C(EnGe1* this, GlobalContext* globalCtx);
void func_80A31514(EnGe1* this, GlobalContext* globalCtx);
void func_80A317C0(EnGe1* this, GlobalContext* globalCtx);
void func_80A31880(EnGe1* this, GlobalContext* globalCtx);
void func_80A31E2C(EnGe1* this, GlobalContext* globalCtx);
void func_80A31FE0(EnGe1* this, GlobalContext* globalCtx);
void func_80A323B0(EnGe1* this);
void func_80A323EC(EnGe1* this);

extern AnimationHeader D_6000228;
extern FlexSkeletonHeader D_6000330;
extern Gfx D_6009198[];
extern Gfx D_6009430[];
extern Gfx D_6009690[];
extern AnimationHeader D_600A048;
extern AnimationHeader D_600A498;

const ActorInit En_Ge1_InitVars = {
    ACTOR_EN_GE1,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GE1,
    sizeof(EnGe1),
    (ActorFunc)EnGe1_Init,
    (ActorFunc)EnGe1_Destroy,
    (ActorFunc)EnGe1_Update,
    (ActorFunc)EnGe1_Draw,
};

static ColliderCylinderInit D_80A32770 = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000702, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, 0, { 0, 0, 0 } },
};

static Gfx* D_80A3279C[3] = { D_6009198, D_6009430, D_6009690 };
static Vec3f D_80A327A8 = { 600.0f, 700.0f, 0.0f };
static s32 D_80A327B4[3] = { 0x06000708, 0x06000F08, 0x06001708 };

void EnGe1_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnGe1* this = (EnGe1*)thisx;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &this->unk198, &D_6000330, &D_6000228, this->unk1DC, this->unk23C, 16);
    Animation_PlayOnce(&this->unk198, &D_6000228);
    Collider_InitCylinder(globalCtx, &this->unk14C);
    Collider_SetCylinder(globalCtx, &this->unk14C, &this->actor, &D_80A32770);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->unk2B0 = &D_6000228;
    this->unk2B8 = func_80A323B0;
    this->actor.targetMode = 6;
    Actor_SetScale(&this->actor, 0.01f);
    if (globalCtx->sceneNum == SCENE_SPOT09) {
        this->actor.uncullZoneForward = 1000.0f;
    } else {
        this->actor.uncullZoneForward = 1200.0f;
    }
    switch (this->actor.params & 0xFF) {
        case 0x0:
            this->unk2AE = 2;
            this->unk2B4 = func_80A31880;
            break;

        case 0x1:
            this->unk2AE = 1;
            if (func_80A30DCC() != 0) {
                this->unk2B4 = func_80A317C0;
            } else {
                this->unk2B4 = func_80A30F48;
            }
            break;

        case 0x4:
            this->unk2AE = 1;
            if (func_80A30DCC() != 0) {
                this->unk2B4 = func_80A31094;
            } else {
                this->unk2B4 = func_80A310C4;
            }
            break;

        case 0x5:
            if (LINK_IS_ADULT) {
                osSyncPrintf("\x1b[36m谷底 ゲルド 撤退 \n\x1b[m");
                Actor_Kill(&this->actor);
                return;
            }
            this->unk2AE = 0;
            this->unk2B4 = func_80A3118C;
            break;

        case 0x45:
            if (INV_CONTENT(ITEM_BOW) == ITEM_NONE) {
                Actor_Kill(&this->actor);
                return;
            }
            this->actor.targetMode = 3;
            this->unk2AE = 0;
            osSyncPrintf("\x1b[36mやぶさめ ゲルド EVENT_INF(0) = %x\n\x1b[m", gSaveContext.eventInf[0]);
            if (gSaveContext.eventInf[0] & 0x100) {
                this->unk2B4 = func_80A31E2C;
            } else if (func_80A30DCC() != 0) {
                this->unk2B4 = func_80A31FE0;
            } else {
                this->unk2B4 = func_80A30F48;
            }
            break;

        case 0x46:
            this->unk2AE = 1;
            if (func_80A30DCC() != 0) {
                this->unk2B4 = func_80A31514;
            } else {
                this->unk2B4 = func_80A30F48;
            }
            break;
    }
    this->unk2AC = 0;
}

void EnGe1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnGe1* this = (EnGe1*)thisx;

    Collider_DestroyCylinder(globalCtx, &this->unk14C);
}

s32 func_80A30C70(EnGe1* this, GlobalContext* globalCtx, u16 arg2, f32 arg3, EnGe1ActionFunc arg4) {
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk2B4 = arg4;
        this->unk2B8 = func_80A323EC;
        this->unk2AC &= ~4;
        this->unk2B0 = &D_6000228;
        Animation_Change(&this->unk198, &D_6000228, 1.0f, 0.0f, Animation_GetLastFrame(&D_6000228), ANIMMODE_ONCE,
                         -8.0f);
        return 1;
    } else {
        this->actor.textId = arg2;
        if (this->actor.xzDistToPlayer < arg3) {
            func_8002F2CC(&this->actor, globalCtx, arg3);
        }
        return 0;
    }
}

void func_80A30D48(EnGe1* this) {
    Animation_Change(&this->unk198, &D_6000228, -1.0f, Animation_GetLastFrame(&D_6000228), 0.0f, ANIMMODE_ONCE, 8.0f);
    this->unk2B0 = &D_6000228;
    this->unk2B8 = func_80A323B0;
}

s32 func_80A30DCC(void) {
    if (!(gSaveContext.eventChkInf[9] & 1) || !(gSaveContext.eventChkInf[9] & 2) ||
        !(gSaveContext.eventChkInf[9] & 4) || !(gSaveContext.eventChkInf[9] & 8)) {
        return 0;
    } else {
        return 1;
    }
}

void func_80A30E08(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if (this->unk2AF > 0) {
        this->unk2AF--;
    } else {
        func_8006D074(globalCtx);
        if ((INV_CONTENT(ITEM_HOOKSHOT) == ITEM_NONE) || (INV_CONTENT(ITEM_LONGSHOT) == ITEM_NONE)) {
            globalCtx->nextEntranceIndex = 0x1A5;
        } else if (gSaveContext.eventChkInf[0xC] & 0x80) {
            globalCtx->nextEntranceIndex = 0x5F8;
        } else {
            globalCtx->nextEntranceIndex = 0x3B4;
        }
        globalCtx->fadeTransition = 0x26;
        globalCtx->sceneLoadFlag = 0x14;
    }
}

void func_80A30EE8(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AF = 0x1E;
    this->unk2B4 = func_80A30E08;
    func_8002DF54(globalCtx, &this->actor, 0x5FU);
    func_80078884(NA_SE_SY_FOUND);
    func_8010B680(globalCtx, 0x6000U, &this->actor);
}

void func_80A30F48(EnGe1* this, GlobalContext* globalCtx) {
    s16 temp_v0;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if ((ABS(temp_v0) < 0x4301) && (this->actor.xzDistToPlayer < 100.0f)) {
        func_80A30EE8(this, globalCtx);
    }
    if (this->unk14C.base.acFlags & 2) {
        func_80A30EE8(this, globalCtx);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk14C.base);
}

void func_80A31000(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        switch (this->actor.textId) {
            case 0x6001:
                this->unk2B4 = func_80A31094;
                break;

            case 0x6019:
            case 0x601A:
                this->unk2B4 = func_80A3118C;
                break;

            case 0x6018:
                this->unk2B4 = func_80A317C0;
                break;

            default:
                this->unk2B4 = func_80A3118C;
                break;
        }
    }
}

void func_80A31094(EnGe1* this, GlobalContext* globalCtx) {
    func_80A30C70(this, globalCtx, 0x6001U, 100.0f, func_80A31000);
}

void func_80A310C4(EnGe1* this, GlobalContext* globalCtx) {
    s16 temp_v0;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if ((this->actor.xzDistToPlayer < 50.0f) || ((ABS(temp_v0) < 0x4301) && (this->actor.xzDistToPlayer < 400.0f))) {
        func_80A30EE8(this, globalCtx);
    }
    if (this->unk14C.base.acFlags & 2) {
        func_80A30EE8(this, globalCtx);
    }
    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->unk14C.base);
}

void func_80A3118C(EnGe1* this, GlobalContext* globalCtx) {
    u16 temp_v0;

    temp_v0 = Text_GetFaceReaction(globalCtx, 0x22U);
    if (temp_v0 == 0) {
        temp_v0 = 0x6019;
    }
    func_80A30C70(this, globalCtx, temp_v0, 100.0f, func_80A31000);
}

void func_80A311E0(EnGe1* this, GlobalContext* globalCtx) {
    if (this->unk2AF > 0) {
        this->unk2AF--;
    } else {
        func_80A30D48(this);
        this->unk2B4 = func_80A31094;
    }
    this->unk2AC |= 8;
}

void func_80A31234(EnGe1* this, GlobalContext* globalCtx) {
    if (this->unk2AC & 4) {
        this->unk2B4 = func_80A311E0;
        Flags_SetSwitch(globalCtx, ((s16)this->actor.params >> 8) & 0x3F);
        this->unk2AF = 0x32;
        func_80106CCC(globalCtx);
    } else if ((this->unk198.curFrame == 15.0f) || (this->unk198.curFrame == 19.0f)) {
        Audio_PlayActorSound2(&this->actor, 0x184DU);
    }
}

void func_80A312E4(EnGe1* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        this->unk2B4 = func_80A31234;
        Animation_Change(&this->unk198, &D_600A048, 1.0f, 0.0f, Animation_GetLastFrame(&D_600A048), ANIMMODE_ONCE,
                         -3.0f);
        this->unk2B0 = &D_600A048;
        this->unk2B8 = func_80A323EC;
        this->unk2AC &= 0xFFFB;
    }
}

void func_80A313A0(EnGe1* this, GlobalContext* globalCtx) {
    if (func_8010BDBC(&globalCtx->msgCtx) == 2) {
        this->unk2B4 = func_80A31514;
        func_80A30D48(this);
    }
}

void func_80A313E0(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees < 10) {
                    func_8010B720(globalCtx, 0x6016U);
                    this->unk2B4 = func_80A313A0;
                } else {
                    Rupees_ChangeBy(-10);
                    func_8010B720(globalCtx, 0x6015U);
                    this->unk2B4 = func_80A312E4;
                }
                break;

            case 1:
                this->unk2B4 = func_80A31514;
                func_80A30D48(this);
                break;
        }
    }
}

void func_80A314D0(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31514;
        func_80A30D48(this);
    }
}

void func_80A31514(EnGe1* this, GlobalContext* globalCtx) {
    if (gBitFlags[0x16] & gSaveContext.inventory.questItems) {
        func_80A30C70(this, globalCtx, 0x6014U, 100.0f, func_80A313E0);
    } else {
        func_80A30C70(this, globalCtx, 0x6013U, 100.0f, func_80A314D0);
    }
}

void func_80A3157C(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->unk2B4 = func_80A317C0;
        func_80A30D48(this);
    }
}

void func_80A315F0(EnGe1* this, GlobalContext* globalCtx) {
    if (this->unk2AF > 0) {
        this->unk2AF--;
    } else {
        func_80A30D48(this);
        this->unk2B4 = func_80A317C0;
    }
    this->unk2AC |= 8;
}

void func_80A31644(EnGe1* this, GlobalContext* globalCtx) {
    if (this->unk2AC & 4) {
        this->unk2B4 = func_80A315F0;
        Flags_SetSwitch(globalCtx, (this->actor.params >> 8) & 0x3F);
        this->unk2AF = 0x32;
        func_80106CCC(globalCtx);
    } else if ((this->unk198.curFrame == 15.0f) || (this->unk198.curFrame == 19.0f)) {
        Audio_PlayActorSound2(&this->actor, NA_SE_IT_HAND_CLAP);
    }
}

void func_80A316F4(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        this->unk2B4 = func_80A31644;
        Animation_Change(&this->unk198, &D_600A048, 1.0f, 0.0f, Animation_GetLastFrame(&D_600A048), ANIMMODE_ONCE,
                         -3.0f);
        this->unk2B0 = &D_600A048;
        this->unk2B8 = func_80A323EC;
        this->unk2AC &= ~4;
    }
}

void func_80A317C0(EnGe1* this, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, (this->actor.params >> 8) & 0x3F)) {
        func_80A30C70(this, globalCtx, 0x6018U, 100.0f, func_80A3157C);
    } else {
        func_80A30C70(this, globalCtx, 0x6017U, 100.0f, func_80A316F4);
    }
}

void func_80A3183C(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31880;
        func_80A30D48(this);
    }
}

void func_80A31880(EnGe1* this, GlobalContext* globalCtx) {
    u16 temp_v0;

    temp_v0 = Text_GetFaceReaction(globalCtx, 0x22);
    if (temp_v0 == 0) {
        temp_v0 = 0x6069;
    }
    if (func_80A30C70(this, globalCtx, temp_v0, 100.0f, func_80A3183C) != 0) {
        this->unk2B8 = func_80A323B0;
        this->unk2B0 = &D_600A498;
        Animation_Change(&this->unk198, &D_600A498, 1.0f, 0.0f, Animation_GetLastFrame(&D_600A498), ANIMMODE_ONCE,
                         -8.0f);
    }
}

void func_80A31934(EnGe1* this, GlobalContext* globalCtx) {
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31FE0;
        func_80A30D48(this);
    }
}

void func_80A3196C(EnGe1* this, GlobalContext* globalCtx) {
    s32 var_a2;

    if (Actor_HasParent(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31934;
        if (this->unk2AC & 2) {
            gSaveContext.itemGetInf[0] |= 0x8000;
            return;
        } else {
            gSaveContext.infTable[0x19] |= 1;
        }
        return;
    }
    if (this->unk2AC & 2) {
        switch (CUR_UPG_VALUE(UPG_QUIVER)) {
            case 1:
                var_a2 = 0x30;
                break;
            case 2:
                var_a2 = 0x31;
                break;
        }
    } else {
        var_a2 = 0x3E;
    }
    //! @bug var_a2 may be used uninitialized
    func_8002F434(&this->actor, globalCtx, var_a2, 10000.0f, 50.0f);
}

void func_80A31A5C(EnGe1* this, GlobalContext* globalCtx) {
    s32 var_a2;

    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->actor.flags &= ~0x10000;
        this->unk2B4 = func_80A3196C;
    }
    if (this->unk2AC & 2) {
        switch (CUR_UPG_VALUE(UPG_QUIVER)) {
            case 1:
                var_a2 = 0x30;
                break;
            case 2:
                var_a2 = 0x31;
                break;
        }
    } else {
        var_a2 = 0x3E;
    }
    //! @bug var_a2 may be used uninitialized
    func_8002F434(&this->actor, globalCtx, var_a2, 10000.0f, 50.0f);
}

void func_80A31B20(EnGe1* this, GlobalContext* globalCtx) {
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31A5C;
        this->actor.flags &= ~0x10000;
    } else {
        func_8002F2CC(&this->actor, globalCtx, 200.0f);
    }
}

void func_80A31B7C(EnGe1* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->unk2B4 = func_80A31FE0;
        func_80A30D48(this);
    }
}

void func_80A31BDC(EnGe1* this, GlobalContext* globalCtx) {
}

void func_80A31BE8(EnGe1* this, GlobalContext* globalCtx) {
    Player* player;
    Actor* temp_v0_2;

    player = PLAYER;
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        this->actor.flags &= ~0x10000;
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees < 20) {
                    func_8010B720(globalCtx, 0x85);
                    this->unk2B4 = func_80A31B7C;
                } else {
                    Rupees_ChangeBy(-20);
                    globalCtx->nextEntranceIndex = 0x129;
                    gSaveContext.nextCutsceneIndex = 0xFFF0;
                    globalCtx->fadeTransition = 0x26;
                    globalCtx->sceneLoadFlag = 0x14;
                    gSaveContext.eventInf[0] |= 0x100;
                    gSaveContext.eventChkInf[6] |= 0x100;
                    if (!(player->stateFlags1 & 0x800000)) {
                        func_8002DF54(globalCtx, &this->actor, 1U);
                    } else {
                        temp_v0_2 = Actor_FindNearby(globalCtx, &player->actor, ACTOR_EN_HORSE, 1U, 1200.0f);
                        player->actor.freezeTimer = 0x4B0;
                        if (temp_v0_2 != NULL) {
                            temp_v0_2->freezeTimer = 0x4B0;
                        }
                    }
                    this->unk2B4 = func_80A31BDC;
                }
                break;

            case 1:
                this->unk2B4 = func_80A31FE0;
                func_80106CCC(globalCtx);
                break;
        }
    }
}

void func_80A31D88(EnGe1* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_8010B720(globalCtx, 0x6041);
        this->unk2B4 = func_80A31BE8;
    }
}

void func_80A31DE4(EnGe1* this, GlobalContext* globalCtx) {
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31D88;
        return;
    }
    func_8002F2CC(&this->actor, globalCtx, 300.0f);
}

void func_80A31E2C(EnGe1* this, GlobalContext* globalCtx) {
    gSaveContext.eventInf[0] &= ~0x100;
    LogUtils_LogThreadId("../z_en_ge1.c", 0x456);
    osSyncPrintf("z_common_data.yabusame_total = %d\n", gSaveContext.minigameScore);
    if (1) {}
    if (1) {}
    if (1) {}
    LogUtils_LogThreadId("../z_en_ge1.c", 0x457);
    osSyncPrintf("z_common_data.memory.information.room_inf[127][ 0 ] = %d\n", gSaveContext.highScores[0]);
    this->actor.flags |= 0x10000;
    if (gSaveContext.highScores[0] < gSaveContext.minigameScore) {
        gSaveContext.highScores[0] = gSaveContext.minigameScore;
    }
    if (gSaveContext.minigameScore < 1000) {
        this->actor.textId = 0x6045;
        this->unk2B4 = func_80A31DE4;
    } else if (!(gSaveContext.infTable[0x19] & 1)) {
        this->actor.textId = 0x6046;
        this->unk2B4 = func_80A31B20;
        this->unk2AC &= ~2;
    } else if (gSaveContext.minigameScore < 1500) {
        this->actor.textId = 0x6047;
        this->unk2B4 = func_80A31DE4;
    } else if (gSaveContext.itemGetInf[0] & 0x8000) {
        this->actor.textId = 0x6047;
        this->unk2B4 = func_80A31DE4;
    } else {
        this->actor.textId = 0x6044;
        this->unk2B4 = func_80A31B20;
        this->unk2AC |= 2;
    }
}

void func_80A31F9C(EnGe1* this, GlobalContext* globalCtx) {
    this->unk2AC |= 1;
    if (func_8002F334(&this->actor, globalCtx) != 0) {
        this->unk2B4 = func_80A31FE0;
        func_80A30D48(this);
    }
}

void func_80A31FE0(EnGe1* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    u16 var_a2;

    if (!(player->stateFlags1 & 0x800000)) {
        func_80A30C70(this, globalCtx, 0x603FU, 100.0f, func_80A31F9C);
    } else {
        if (gSaveContext.eventChkInf[6] & 0x100) {
            if (gSaveContext.infTable[0x19] & 1) {
                var_a2 = 0x6042;
            } else {
                var_a2 = 0x6043;
            }
        } else {
            var_a2 = 0x6040;
        }
        func_80A30C70(this, globalCtx, var_a2, 200.0f, func_80A31D88);
    }
}

void func_80A32078(EnGe1* this, GlobalContext* globalCtx) {
    s32 pad;
    s16 temp_v0;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if (ABS(temp_v0) <= 0x4000) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 6, 0xFA0, 0x64);
        this->actor.world.rot.y = this->actor.shape.rot.y;
        func_80038290(globalCtx, &this->actor, &this->unk29C, &this->unk2A2, this->actor.focus.pos);
    } else {
        if (temp_v0 < 0) {
            Math_SmoothStepToS(&this->unk29C.y, -0x2000, 6, 0x1838, 0x100);
        } else {
            Math_SmoothStepToS(&this->unk29C.y, 0x2000, 6, 0x1838, 0x100);
        }
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0xC, 0x3E8, 0x64);
        this->actor.world.rot.y = this->actor.shape.rot.y;
    }
}

void func_80A32190(EnGe1* this, GlobalContext* globalCtx) {
    s16 temp_v0;

    temp_v0 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
    if ((ABS(temp_v0) <= 0x4300) && (this->actor.xzDistToPlayer < 100.0f)) {
        func_80038290(globalCtx, &this->actor, &this->unk29C, &this->unk2A2, this->actor.focus.pos);
    } else {
        Math_SmoothStepToS(&this->unk29C.x, 0, 6, 0x1838, 0x64);
        Math_SmoothStepToS(&this->unk29C.y, 0, 6, 0x1838, 0x64);
    }
}

void EnGe1_Update(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    EnGe1* this = (EnGe1*)thisx;

    Collider_UpdateCylinder(&this->actor, &this->unk14C);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->unk14C.base);
    Actor_MoveForward(&this->actor);
    Actor_UpdateBgCheckInfo(globalCtx, &this->actor, 40.0f, 25.0f, 40.0f, 5);
    this->unk2B8(this);
    this->unk2B4(this, globalCtx);
    if (this->unk2AC & 1) {
        func_80A32078(this, globalCtx);
        this->unk2AC &= ~1;
    } else {
        func_80A32190(this, globalCtx);
    }
    this->unk2A2.x = this->unk2A2.y = this->unk2A2.z = 0;
    if (DECR(this->unk2AA) == 0) {
        this->unk2AA = Rand_S16Offset(0x3C, 0x3C);
    }
    this->unk2A8 = this->unk2AA;
    if (this->unk2A8 >= 3) {
        this->unk2A8 = 0;
    }
}

void func_80A323B0(EnGe1* this) {
    if (SkelAnime_Update(&this->unk198) != 0) {
        Animation_PlayOnce(&this->unk198, this->unk2B0);
    }
}

void func_80A323EC(EnGe1* this) {
    if (!(this->unk2AC & 4)) {
        if (SkelAnime_Update(&this->unk198) != 0) {
            this->unk2AC |= 4;
        }
        this->unk2AC |= 8;
    }
}

s32 EnGe1_OverrideLimbDraw(GlobalContext* globalCtx, s32 arg1, Gfx** arg2, Vec3f* arg3, Vec3s* arg4, void* thisx) {
    EnGe1* this = thisx;
    s32 pad;

    if (arg1 == 15) {
        arg4->x += this->unk29C.y;
        arg4->z += this->unk29C.x;
    }
    if (this->unk2AC & 8) {
        this->unk2AC &= ~8;
        return 0;
    }
    if ((arg1 == 8) || (arg1 == 10) || (arg1 == 13)) {
        arg4->y += (Math_SinS((s16)(globalCtx->state.frames * ((arg1 * 0x32) + 0x814))) * 200.0f);
        arg4->z += (Math_CosS((s16)(globalCtx->state.frames * ((arg1 * 0x32) + 0x940))) * 200.0f);
    }
    return 0;
}

void EnGe1_PostLimbDraw(GlobalContext* globalCtx, s32 arg1, Gfx** arg2, Vec3s* arg3, void* thisx) {
    EnGe1* this = thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1419);
    if (arg1 == 15) {
        gSPDisplayList(POLY_OPA_DISP++, D_80A3279C[this->unk2AE]);
        Matrix_MultVec3f(&D_80A327A8, &this->actor.focus.pos);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1427);
}

void EnGe1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnGe1* this = (EnGe1*)thisx;
    s32 pad;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1442);
    func_800943C8(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80A327B4[this->unk2A8]));
    SkelAnime_DrawFlexOpa(globalCtx, this->unk198.skeleton, this->unk198.jointTable, this->unk198.dListCount,
                          EnGe1_OverrideLimbDraw, EnGe1_PostLimbDraw, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_ge1.c", 1459);
}
