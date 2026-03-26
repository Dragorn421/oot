#include "z_en_bom_bowl_man.h"
#include "overlays/actors/ovl_En_Bom_Bowl_Pit/z_en_bom_bowl_pit.h"

#define FLAGS 0x08000039

#define THIS ((EnBomBowlMan*)thisx)

void EnBomBowlMan_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Update(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlMan_Draw(Actor* thisx, GlobalContext* globalCtx);

void func_809C3820(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C38A8_WaitTalk(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C395C(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C39D0(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C3A54(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C3B50(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C3C7C(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C3CD4(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C3DC4(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C4040(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C41FC(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C4318(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C441C(EnBomBowlMan* this, GlobalContext* globalCtx);
void func_809C4664(EnBomBowlMan* this, GlobalContext* globalCtx);

extern AnimationHeader D_6000080;
extern AnimationHeader D_6000710;
extern FlexSkeletonHeader D_6006EB0;
extern AnimationHeader D_60072AC;

const ActorInit En_Bom_Bowl_Man_InitVars = {
    ACTOR_EN_BOM_BOWL_MAN,
    ACTORTYPE_NPC,
    FLAGS,
    OBJECT_BG,
    sizeof(EnBomBowlMan),
    (ActorFunc)EnBomBowlMan_Init,
    (ActorFunc)EnBomBowlMan_Destroy,
    (ActorFunc)EnBomBowlMan_Update,
    (ActorFunc)EnBomBowlMan_Draw,
};

typedef struct struct_809C4A10 {
    f32 unk0;
    f32 unk4;
    s32 unk8; // unused
} struct_809C4A10;
struct_809C4A10 D_809C4A10[2] = { { 16.0f, 46.0f, 0 }, { 36.0f, 56.0f, 0 } };
Vec3f D_809C4A28[2] = { { 60.0f, -60.0f, -430.0f }, { 0.0f, -120.0f, -620.0f } };
f32 D_809C4A40[2] = { 0.01f, 0.03f };
Vec3f D_809C4A48 = { 0.0f, 0.1f, 0.0f };
Vec3f D_809C4A54 = { 0.0f, 0.0f, 0.0f };
Vec3f D_809C4A60[5] = {
    { 0.0f, 22.0f, 0.0f }, { 0.0f, 22.0f, 0.0f }, { 0.0f, 8.0f, 0.0f }, { 0.0f, 9.0f, 0.0f }, { 0.0f, -2.0f, 0.0f },
};
static s16 D_809C4A9C[6] = { 0x4268, 0x4268, -0x3E8, 0, 0x4268, 0 };
static s32 D_809C4AA8[6] = { 0x06004110, 0x06004910, 0x06005110, 0, 0, 0 };

// EnSyatekiNiw
typedef struct Actor143 {
    Actor actor;
    char pad14C[0x2F4 - 0x14C];
    f32 unk2F4;
    char pad2F8[0x33C - 0x2F8];
    s16 unk33C;
    s16 unk33E;
} Actor143;

void EnBomBowlMan_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnBomBowlMan* this = (EnBomBowlMan*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s32 var_s1;
    Actor143* temp_v0;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFunc_Circle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &this->unk14C, &D_6006EB0, &D_6000710, &this->unk190, &this->unk1D2, 0xB);
    osSyncPrintf("\x1b[32m☆ もー 肩こっちゃうよねぇ〜 \t\t ☆ \n\x1b[m");
    osSyncPrintf("\x1b[32m☆ もっとラクしてもうかるバイトないかしら？ ☆ %d\n\x1b[m", globalCtx->bombchuBowlingAmmo);
    this->unk248 = this->actor.posRot.pos;
    this->actor.shape.unk_08 = -60.0f;
    Actor_SetScale(&this->actor, 0.013f);
    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        temp_v0 = (Actor143*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, 0x143, D_809C4A28[var_s1].x,
                                         D_809C4A28[var_s1].y, D_809C4A28[var_s1].z, 0, 0, 0, 1);
        if (temp_v0 != NULL) {
            temp_v0->unk2F4 = D_809C4A40[var_s1];
            temp_v0->unk33C = (s16)(s32)D_809C4A10[var_s1].unk0;
            temp_v0->unk33E = (s16)(s32)D_809C4A10[var_s1].unk4;
        }
    }
    this->unk242 = (u16)(s32)Math_Rand_ZeroFloat(4.99f);
    this->actor.unk_1F = 1;
    this->actionFunc = func_809C3820;
}

void EnBomBowlMan_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_809C3820(EnBomBowlMan* this, GlobalContext* globalCtx) {
    this->unk254_curAnimFraceCount = SkelAnime_GetFrameCount(&D_6000710);
    SkelAnime_ChangeAnim(&this->unk14C, &D_6000710, 1.0f, 0.0f, this->unk254_curAnimFraceCount, 0U, -10.0f);
    this->actor.textId = 0xC0;
    this->unk22E = 5;
    this->actionFunc = func_809C38A8_WaitTalk;
}

void func_809C38A8_WaitTalk(EnBomBowlMan* this, GlobalContext* globalCtx) {
    s16 relYawTowardsPlayer;
    s16 relYawTowardsPlayerAbs;

    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->actionFunc = func_809C395C;
    } else {
        relYawTowardsPlayer = this->actor.yawTowardsLink - this->actor.shape.rot.y;
        relYawTowardsPlayerAbs = ABS(relYawTowardsPlayer);
        if (!(this->actor.xzDistFromLink > 120.0f) && (relYawTowardsPlayerAbs < 0x4300)) {
            func_8002F2CC(&this->actor, globalCtx, 120.0f);
        }
    }
}

void func_809C395C(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        globalCtx->msgCtx.msgMode = 0x37;
        this->actionFunc = func_809C39D0;
    }
}

void func_809C39D0(EnBomBowlMan* this, GlobalContext* globalCtx) {
    this->unk254_curAnimFraceCount = SkelAnime_GetFrameCount(&D_6000080);
    SkelAnime_ChangeAnim(&this->unk14C, &D_6000080, 1.0f, 0.0f, this->unk254_curAnimFraceCount, 2U, -10.0f);
    this->unk238 = 1;
    this->actionFunc = func_809C3A54;
}

void func_809C3A54(EnBomBowlMan* this, GlobalContext* globalCtx) {
    f32 sp1C;

    sp1C = this->unk14C.animCurrentFrame;
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (sp1C == 30.0f) {
        this->unk22E = 5;
        if ((gSaveContext.eventChkInf[2] & 0x20) || (gGameInfo->data[0x962] != 0)) {
            this->actor.textId = 0xBF;
        } else {
            this->actor.textId = 0x7058;
        }
    }
    func_8010B720(globalCtx, this->actor.textId);
    if ((this->unk234 == 0) && (this->unk238 == 2) && (this->unk236 == 0)) {
        this->unk234 = 2;
        this->unk23A += 1;
        if (this->unk23A >= 3) {
            this->actionFunc = func_809C3B50;
        }
    }
}

void func_809C3B50(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->unk254_curAnimFraceCount = SkelAnime_GetFrameCount(&D_60072AC);
        SkelAnime_ChangeAnim(&this->unk14C, &D_60072AC, 1.0f, 0.0f, this->unk254_curAnimFraceCount, 0U, -10.0f);
        this->unk238 = 3;
        this->unk236 = (s16)Math_Rand_ZeroFloat(60.0f) + 0x14;
        if (!(gSaveContext.eventChkInf[2] & 0x20) && (gGameInfo->data[0x962] == 0)) {
            this->actionFunc = func_809C3C7C;
        } else {
            this->actor.textId = 0x18;
            this->unk22E = 4;
            func_8010B720(globalCtx, this->actor.textId);
            this->actionFunc = func_809C4040;
        }
    }
}

void func_809C3C7C(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->actionFunc = func_809C3CD4;
    } else {
        func_8002F2CC(&this->actor, globalCtx, 120.0f);
    }
}

void func_809C3CD4(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->actionFunc = func_809C3C7C;
    }
}

void func_809C3D40(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (this->unk_258 == 0) {
        if (this->unk232 == 0) {
            this->actor.textId = 0x18;
        } else {
            this->actor.textId = 0x1A;
        }
        this->unk22E = 4;
    } else {
        this->actor.textId = 0x19;
        this->unk22E = 5;
    }
    this->actionFunc = func_809C3DC4;
}

void func_809C3DC4(EnBomBowlMan* this, GlobalContext* globalCtx) {
    s16 relYawTowardsPlayer;
    s16 relYawTowardsPlayerAbs;

    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (gGameInfo->data[0x963] != 0) {
        osSyncPrintf("\x1b[31m☆ game_play->bomchu_game_flag ☆ %d\n\x1b[m", globalCtx->bombchuBowlingAmmo);
        osSyncPrintf("\x1b[31m☆ 壁１の状態どう？ ☆ %d\n\x1b[m", this->unk23E_arr[0]);
        osSyncPrintf("\x1b[31m☆ 壁２の状態どう？ ☆ %d\n\x1b[m", this->unk23E_arr[1]);
        osSyncPrintf("\x1b[31m☆ 穴情報\t     ☆ %d\n\x1b[m", this->unk25C->unk164);
        osSyncPrintf("\n\n");
    }
    this->unk244 = 0;
    if (this->unk25C != NULL) {
        if ((this->unk23E_arr[0] != 1) && (this->unk23E_arr[1] != 1) && (this->unk25C->unk164 == 2)) {
            this->unk244 = 1;
            this->unk25C->unk164 = 0U;
            osSyncPrintf("\x1b[35m☆☆☆☆☆ 中央ＨＩＴ！！！！ ☆☆☆☆☆ \n\x1b[m");
        }
        if ((globalCtx->bombchuBowlingAmmo == -1) && (globalCtx->actorCtx.actorList[3].length == 0) &&
            (this->unk25C->unk164 == 0) && (this->unk23E_arr[0] != 1) && (this->unk23E_arr[1] != 1)) {
            this->unk244 = 2;
            osSyncPrintf("\x1b[35m☆☆☆☆☆ ボムチュウ消化 ☆☆☆☆☆ \n\x1b[m");
        }
    }
    if (this->unk244 != 0) {
        this->actor.textId = 0x1A;
        this->unk22E = 4;
        this->unk_258 = 0;
        if ((this->unk260 != NULL) && (this->unk260->actor.update != NULL)) {
            this->unk260->unk160 = 1;
            this->unk260 = NULL;
        }
        globalCtx->bombchuBowlingAmmo = 0;
        this->unk23C = 1;
        func_8010B680(globalCtx, this->actor.textId, NULL);
        if (this->unk244 == 2) {
            func_8002DF54(globalCtx, NULL, 8U);
        }
        this->actionFunc = func_809C4040;
    } else if (func_8002F194(&this->actor, globalCtx) != 0) {
        if (this->unk_258 == 0) {
            this->actionFunc = func_809C4040;
        } else {
            this->actionFunc = func_809C41FC;
        }
    } else {
        relYawTowardsPlayer = this->actor.yawTowardsLink - this->actor.shape.rot.y;
        relYawTowardsPlayerAbs = ABS(relYawTowardsPlayer);
        if (!(this->actor.xzDistFromLink > 120.0f) && (relYawTowardsPlayerAbs < 0x4300)) {
            func_8002F2CC(&this->actor, globalCtx, 120.0f);
        }
    }
}

void func_809C4040(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees >= 0x1E) {
                    Rupees_ChangeBy(-0x1E);
                    this->unk23E_arr[1] = 0;
                    this->unk_258 = 1;
                    this->unk23E_arr[0] = this->unk23E_arr[1];
                    globalCtx->bombchuBowlingAmmo = 0xA;
                    Flags_SetSwitch(globalCtx, 0x38);
                    if ((this->unk232 == 0) && (this->unk23C == 0)) {
                        this->actor.textId = 0x19;
                        func_8010B720(globalCtx, this->actor.textId);
                        this->unk22E = 5;
                        this->actionFunc = func_809C41FC;
                    } else {
                        this->actor.textId = 0x1B;
                        func_8010B720(globalCtx, this->actor.textId);
                        this->unk22E = 5;
                        func_800800F8(globalCtx, 0x1F4A, -0x63, NULL, 0);
                        func_8002DF54(globalCtx, NULL, 8U);
                        this->actionFunc = func_809C4318;
                    }
                } else {
                    this->unk23C = 0;
                    this->actor.textId = 0x85;
                    func_8010B720(globalCtx, this->actor.textId);
                    this->unk22E = 5;
                    this->actionFunc = func_809C41FC;
                }
                break;
            case 1:
                this->unk23C = 0;
                this->actor.textId = 0x2D;
                func_8010B720(globalCtx, this->actor.textId);
                this->unk22E = 5;
                this->actionFunc = func_809C41FC;
                break;
        }
    }
}

void func_809C41FC(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        if (((this->actor.textId == 0x2D) || (this->actor.textId == 0x85)) && Flags_GetSwitch(globalCtx, 0x38)) {
            Flags_UnsetSwitch(globalCtx, 0x38);
        }
        if (this->unk_258 == 1) {
            this->actor.textId = 0x1B;
            func_8010B720(globalCtx, this->actor.textId);
            this->unk22E = 5;
            func_800800F8(globalCtx, 0x1F4A, -0x63, NULL, 0);
            func_8002DF54(globalCtx, NULL, 8U);
            this->actionFunc = func_809C4318;
        } else {
            if (this->unk244 == 2) {
                func_8002DF54(globalCtx, NULL, 7U);
            }
            this->actionFunc = func_809C3D40;
        }
    }
}

void func_809C4318(EnBomBowlMan* this, GlobalContext* globalCtx) {
    Vec3f sp44 = D_809C4A48;
    Vec3f sp38 = D_809C4A54;
    Vec3f sp2C;

    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        sp2C.x = 148.0f;
        sp2C.y = 40.0f;
        sp2C.z = 300.0f;
        EffectSsBomb2_SpawnLayered(globalCtx, &sp2C, &sp38, &sp44, 50, 15);
        Audio_PlayActorSound2(&this->actor, NA_SE_IT_GOODS_APPEAR);
        this->unk22A = 0xA;
        this->actionFunc = func_809C441C;
    }
}

void func_809C441C(EnBomBowlMan* this, GlobalContext* globalCtx) {
    s16 var_v1;
    s32 pad;

    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if (this->unk22A == 0) {
        switch (this->unk242) {
            case 0:
                var_v1 = 0;
                if (gSaveContext.itemGetInf[1] & 2) {
                    var_v1 = 4;
                }
                break;
            case 1:
                var_v1 = 4;
                break;
            case 2:
                var_v1 = 2;
                break;
            case 3:
                var_v1 = 1;
                if (gSaveContext.itemGetInf[1] & 4) {
                    var_v1 = 4;
                }
                break;
            case 4:
                var_v1 = 3;
                break;
        }
        this->unk230 = var_v1;
        if (gGameInfo->data[0x967] != 0) {
            this->unk230 = gGameInfo->data[0x967] - 1;
        }
        this->unk260 = (Actor168*)Actor_SpawnAsChild(
            &globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_EX_ITEM, D_809C4A60[this->unk230].x + 148.0f,
            D_809C4A60[this->unk230].y + 40.0f, D_809C4A60[this->unk230].z + 300.0f, 0, D_809C4A9C[this->unk230], 0,
            this->unk230 + 5);
        if (this->unk232 == 0) {
            this->unk25C = (EnBomBowlPit*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx,
                                                             ACTOR_EN_BOM_BOWL_PIT, 0.0f, 90.0f, -860.0f, 0, 0, 0, 0);
            if (this->unk25C != NULL) {
                this->unk25C->unk15A = (s16)this->unk230;
            }
        } else {
            this->unk25C->unk15A = (s16)this->unk230;
        }
        this->unk25C->unk15C = 1;
        this->unk_258 = 2;
        this->actor.textId = 0x405A;
        func_8010B720(globalCtx, this->actor.textId);
        this->unk22E = 5;
        this->unk242 += 1;
        if ((s16)this->unk242 >= 5) {
            this->unk242 = 0;
        }
        this->actionFunc = func_809C4664;
    }
}

void func_809C4664(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    if ((this->unk22E == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        func_8005B1A4(globalCtx->cameraPtrs[globalCtx->activeCamera]);
        this->unk232 = 1;
        if (gGameInfo->data[0x962] != 0) {
            gGameInfo->data[0x962] = 0;
        }
        osSyncPrintf("\x1b[33m☆ わー ☆ %d\n\x1b[m", globalCtx->bombchuBowlingAmmo);
        func_8002DF54(globalCtx, NULL, 7U);
        this->actionFunc = func_809C3D40;
    }
}

void EnBomBowlMan_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlMan* this = (EnBomBowlMan*)thisx;

    this->unk22C += 1;
    this->actor.posRot2.pos.y = 60.0f;
    Actor_SetHeight(&this->actor, 60.0f);
    switch (this->unk238) {
        case 0:
            this->unk234 = 2;
            break;
        case 1:
            if (this->unk234 > 0) {
                this->unk234 -= 1;
            } else {
                this->unk236 = 0x1E;
                this->unk238 = 2;
            }
            break;
        case 2:
            if (this->unk236 == 0) {
                if (this->unk234 > 0) {
                    this->unk234 -= 1;
                }
            }
            break;
        default:
            if (this->unk236 == 0) {
                this->unk234 += 1;
                if (this->unk234 >= 3) {
                    this->unk234 = 0;
                    this->unk236 = (s16)Math_Rand_ZeroFloat(60.0f) + 0x14;
                }
            }
            func_80038290(globalCtx, &this->actor, &this->unk218, &this->unk224, this->actor.posRot2.pos);
            break;
    }
    if (this->unk22A == 0) {

    } else {
        this->unk22A -= 1;
    }
    if (this->unk236 != 0) {
        this->unk236 -= 1;
    }
    this->actionFunc(this, globalCtx);
}

s32 EnBomBowlMan_OverrideLimbDraw(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot,
                                  void* thisx) {
    EnBomBowlMan* this = thisx;

    if (limbIndex == 4) {
        rot->x += this->unk218.y;
        rot->z += this->unk218.z;
    }
    return 0;
}

void EnBomBowlMan_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlMan* this = (EnBomBowlMan*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_en_bom_bowl_man.c", 907);
    func_80093D18(globalCtx->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(D_809C4AA8[this->unk234]));
    SkelAnime_DrawFlexOpa(globalCtx, this->unk14C.skeleton, this->unk14C.limbDrawTbl, this->unk14C.dListCount,
                          EnBomBowlMan_OverrideLimbDraw, NULL, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bom_bowl_man.c", 923);
}
