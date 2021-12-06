#include "z_en_bom_bowl_man.h"
#include "overlays/actors/ovl_En_Bom_Bowl_Pit/z_en_bom_bowl_pit.h"
#include "overlays/actors/ovl_En_Syateki_Niw/z_en_syateki_niw.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
#include "objects/object_bg/object_bg.h"
#include "z64.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3 | ACTOR_FLAG_4 | ACTOR_FLAG_5 | ACTOR_FLAG_27)

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

const ActorInit En_Bom_Bowl_Man_InitVars = {
    ACTOR_EN_BOM_BOWL_MAN,
    ACTORCAT_NPC,
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

void EnBomBowlMan_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnBomBowlMan* this = (EnBomBowlMan*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s32 var_s1;
    EnSyatekiNiw* temp_v0;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    SkelAnime_InitFlex(globalCtx, &this->unk14C, &object_bg_006EB0_Skel, &object_bg_000710_Anim, &this->unk190,
                       &this->unk1D2, 0xB);
    osSyncPrintf("\x1b[32m☆ もー 肩こっちゃうよねぇ〜 \t\t ☆ \n\x1b[m");
    osSyncPrintf("\x1b[32m☆ もっとラクしてもうかるバイトないかしら？ ☆ %d\n\x1b[m", globalCtx->bombchuBowlingStatus);
    this->unk248 = this->actor.world.pos;
    this->actor.shape.yOffset = -60.0f;
    Actor_SetScale(&this->actor, 0.013f);
    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        temp_v0 = (EnSyatekiNiw*)Actor_Spawn(&globalCtx->actorCtx, globalCtx, 0x143, D_809C4A28[var_s1].x,
                                             D_809C4A28[var_s1].y, D_809C4A28[var_s1].z, 0, 0, 0, 1);
        if (temp_v0 != NULL) {
            temp_v0->unk_2F4 = D_809C4A40[var_s1];
            temp_v0->collider.dim.radius = (s16)(s32)D_809C4A10[var_s1].unk0;
            temp_v0->collider.dim.height = (s16)(s32)D_809C4A10[var_s1].unk4;
        }
    }
    this->unk242 = (u16)(s32)Rand_ZeroFloat(4.99f);
    this->actor.targetMode = 1;
    this->actionFunc = func_809C3820;
}

void EnBomBowlMan_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_809C3820(EnBomBowlMan* this, GlobalContext* globalCtx) {
    this->unk254_curAnimFraceCount = Animation_GetLastFrame(&object_bg_000710_Anim);
    Animation_Change(&this->unk14C, &object_bg_000710_Anim, 1.0f, 0.0f, this->unk254_curAnimFraceCount, ANIMMODE_LOOP,
                     -10.0f);
    this->actor.textId = 0xC0;
    this->unk22E = TEXT_STATE_EVENT;
    this->actionFunc = func_809C38A8_WaitTalk;
}

void func_809C38A8_WaitTalk(EnBomBowlMan* this, GlobalContext* globalCtx) {
    s16 relYawTowardsPlayer;
    s16 relYawTowardsPlayerAbs;

    SkelAnime_Update(&this->unk14C);
    if (Actor_ProcessTalkRequest(&this->actor, globalCtx)) {
        this->actionFunc = func_809C395C;
    } else {
        relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        relYawTowardsPlayerAbs = ABS(relYawTowardsPlayer);
        if (!(this->actor.xzDistToPlayer > 120.0f) && (relYawTowardsPlayerAbs < 0x4300)) {
            func_8002F2CC(&this->actor, globalCtx, 120.0f);
        }
    }
}

void func_809C395C(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        globalCtx->msgCtx.msgMode = MSGMODE_PAUSED;
        this->actionFunc = func_809C39D0;
    }
}

void func_809C39D0(EnBomBowlMan* this, GlobalContext* globalCtx) {
    this->unk254_curAnimFraceCount = Animation_GetLastFrame(&object_bg_000080_Anim);
    Animation_Change(&this->unk14C, &object_bg_000080_Anim, 1.0f, 0.0f, this->unk254_curAnimFraceCount, ANIMMODE_ONCE,
                     -10.0f);
    this->unk238 = 1;
    this->actionFunc = func_809C3A54;
}

void func_809C3A54(EnBomBowlMan* this, GlobalContext* globalCtx) {
    f32 sp1C;

    sp1C = this->unk14C.curFrame;
    SkelAnime_Update(&this->unk14C);
    if (sp1C == 30.0f) {
        this->unk22E = TEXT_STATE_EVENT;
        if ((gSaveContext.eventChkInf[2] & 0x20) || (gGameInfo->data[0x962] != 0)) {
            this->actor.textId = 0xBF;
        } else {
            this->actor.textId = 0x7058;
        }
    }
    Message_ContinueTextbox(globalCtx, this->actor.textId);
    if ((this->unk234 == 0) && (this->unk238 == 2) && (this->unk236 == 0)) {
        this->unk234 = 2;
        this->unk23A += 1;
        if (this->unk23A >= 3) {
            this->actionFunc = func_809C3B50;
        }
    }
}

void func_809C3B50(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
        this->unk254_curAnimFraceCount = Animation_GetLastFrame(&object_bg_0072AC_Anim);
        Animation_Change(&this->unk14C, &object_bg_0072AC_Anim, 1.0f, 0.0f, this->unk254_curAnimFraceCount,
                         ANIMMODE_LOOP, -10.0f);
        this->unk238 = 3;
        this->unk236 = (s16)Rand_ZeroFloat(60.0f) + 0x14;
        if (!(gSaveContext.eventChkInf[2] & 0x20) && (gGameInfo->data[0x962] == 0)) {
            this->actionFunc = func_809C3C7C;
        } else {
            this->actor.textId = 0x18;
            this->unk22E = TEXT_STATE_CHOICE;
            Message_ContinueTextbox(globalCtx, this->actor.textId);
            this->actionFunc = func_809C4040;
        }
    }
}

void func_809C3C7C(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if (Actor_ProcessTalkRequest(&this->actor, globalCtx)) {
        this->actionFunc = func_809C3CD4;
    } else {
        func_8002F2CC(&this->actor, globalCtx, 120.0f);
    }
}

void func_809C3CD4(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
        this->actionFunc = func_809C3C7C;
    }
}

void func_809C3D40(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if (this->unk_258 == 0) {
        if (this->unk232 == 0) {
            this->actor.textId = 0x18;
        } else {
            this->actor.textId = 0x1A;
        }
        this->unk22E = TEXT_STATE_CHOICE;
    } else {
        this->actor.textId = 0x19;
        this->unk22E = TEXT_STATE_EVENT;
    }
    this->actionFunc = func_809C3DC4;
}

void func_809C3DC4(EnBomBowlMan* this, GlobalContext* globalCtx) {
    s16 relYawTowardsPlayer;
    s16 relYawTowardsPlayerAbs;

    SkelAnime_Update(&this->unk14C);
    if (gGameInfo->data[0x963] != 0) {
        osSyncPrintf("\x1b[31m☆ game_play->bomchu_game_flag ☆ %d\n\x1b[m", globalCtx->bombchuBowlingStatus);
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
        if ((globalCtx->bombchuBowlingStatus == -1) && (globalCtx->actorCtx.actorLists[3].length == 0) &&
            (this->unk25C->unk164 == 0) && (this->unk23E_arr[0] != 1) && (this->unk23E_arr[1] != 1)) {
            this->unk244 = 2;
            osSyncPrintf("\x1b[35m☆☆☆☆☆ ボムチュウ消化 ☆☆☆☆☆ \n\x1b[m");
        }
    }
    if (this->unk244 != 0) {
        this->actor.textId = 0x1A;
        this->unk22E = TEXT_STATE_CHOICE;
        this->unk_258 = 0;
        if ((this->unk260 != NULL) && (this->unk260->actor.update != NULL)) {
            this->unk260->killItem = 1;
            this->unk260 = NULL;
        }
        globalCtx->bombchuBowlingStatus = 0;
        this->unk23C = 1;
        Message_StartTextbox(globalCtx, this->actor.textId, NULL);
        if (this->unk244 == 2) {
            func_8002DF54(globalCtx, NULL, 8U);
        }
        this->actionFunc = func_809C4040;
    } else if (Actor_ProcessTalkRequest(&this->actor, globalCtx)) {
        if (this->unk_258 == 0) {
            this->actionFunc = func_809C4040;
        } else {
            this->actionFunc = func_809C41FC;
        }
    } else {
        relYawTowardsPlayer = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;
        relYawTowardsPlayerAbs = ABS(relYawTowardsPlayer);
        if (!(this->actor.xzDistToPlayer > 120.0f) && (relYawTowardsPlayerAbs < 0x4300)) {
            func_8002F2CC(&this->actor, globalCtx, 120.0f);
        }
    }
}

void func_809C4040(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                if (gSaveContext.rupees >= 0x1E) {
                    Rupees_ChangeBy(-0x1E);
                    this->unk23E_arr[1] = 0;
                    this->unk_258 = 1;
                    this->unk23E_arr[0] = this->unk23E_arr[1];
                    globalCtx->bombchuBowlingStatus = 0xA;
                    Flags_SetSwitch(globalCtx, 0x38);
                    if ((this->unk232 == 0) && (this->unk23C == 0)) {
                        this->actor.textId = 0x19;
                        Message_ContinueTextbox(globalCtx, this->actor.textId);
                        this->unk22E = TEXT_STATE_EVENT;
                        this->actionFunc = func_809C41FC;
                    } else {
                        this->actor.textId = 0x1B;
                        Message_ContinueTextbox(globalCtx, this->actor.textId);
                        this->unk22E = TEXT_STATE_EVENT;
                        OnePointCutscene_Init(globalCtx, 8010, -99, NULL, MAIN_CAM);
                        func_8002DF54(globalCtx, NULL, 8U);
                        this->actionFunc = func_809C4318;
                    }
                } else {
                    this->unk23C = 0;
                    this->actor.textId = 0x85;
                    Message_ContinueTextbox(globalCtx, this->actor.textId);
                    this->unk22E = TEXT_STATE_EVENT;
                    this->actionFunc = func_809C41FC;
                }
                break;
            case 1:
                this->unk23C = 0;
                this->actor.textId = 0x2D;
                Message_ContinueTextbox(globalCtx, this->actor.textId);
                this->unk22E = TEXT_STATE_EVENT;
                this->actionFunc = func_809C41FC;
                break;
        }
    }
}

void func_809C41FC(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
        if (((this->actor.textId == 0x2D) || (this->actor.textId == 0x85)) && Flags_GetSwitch(globalCtx, 0x38)) {
            Flags_UnsetSwitch(globalCtx, 0x38);
        }
        if (this->unk_258 == 1) {
            this->actor.textId = 0x1B;
            Message_ContinueTextbox(globalCtx, this->actor.textId);
            this->unk22E = TEXT_STATE_EVENT;
            OnePointCutscene_Init(globalCtx, 8010, -99, NULL, MAIN_CAM);
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

    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
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

    SkelAnime_Update(&this->unk14C);
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
        this->unk260 = (EnExItem*)Actor_SpawnAsChild(
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
        Message_ContinueTextbox(globalCtx, this->actor.textId);
        this->unk22E = TEXT_STATE_EVENT;
        this->unk242 += 1;
        if ((s16)this->unk242 >= 5) {
            this->unk242 = 0;
        }
        this->actionFunc = func_809C4664;
    }
}

void func_809C4664(EnBomBowlMan* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->unk14C);
    if ((this->unk22E == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
        func_8005B1A4(globalCtx->cameraPtrs[globalCtx->activeCamera]);
        this->unk232 = 1;
        if (gGameInfo->data[0x962] != 0) {
            gGameInfo->data[0x962] = 0;
        }
        osSyncPrintf("\x1b[33m☆ わー ☆ %d\n\x1b[m", globalCtx->bombchuBowlingStatus);
        func_8002DF54(globalCtx, NULL, 7U);
        this->actionFunc = func_809C3D40;
    }
}

void EnBomBowlMan_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlMan* this = (EnBomBowlMan*)thisx;

    this->unk22C += 1;
    this->actor.focus.pos.y = 60.0f;
    Actor_SetFocus(&this->actor, 60.0f);
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
                    this->unk236 = (s16)Rand_ZeroFloat(60.0f) + 0x14;
                }
            }
            func_80038290(globalCtx, &this->actor, &this->unk218, &this->unk224, this->actor.focus.pos);
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
    SkelAnime_DrawFlexOpa(globalCtx, this->unk14C.skeleton, this->unk14C.jointTable, this->unk14C.dListCount,
                          EnBomBowlMan_OverrideLimbDraw, NULL, this);
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_en_bom_bowl_man.c", 923);
}
