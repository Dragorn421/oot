/*
 * File: z_bg_dy_yoseizo.c
 * Overlay: ovl_Bg_Dy_Yoseizo
 * Description: Great Fairy
 */

#include "z_bg_dy_yoseizo.h"

// TODO remove EnDyExtra struct and use include:
// #include "overlays/actors/ovl_En_Dy_Extra/z_en_dy_extra.h"
struct EnDyExtra;
typedef void (*EnDyExtraActionFunc)(struct EnDyExtra*, GlobalContext*);
typedef struct EnDyExtra {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnDyExtraActionFunc actionFunc;
    /* 0x0150 */ s16 type;
    /* 0x0152 */ s16 unk_152;
    /* 0x0154 */ s16 unk_154;
    /* 0x0158 */ f32 unk_158;
    /* 0x015C */ Vec3f unk_15C;
    /* 0x0168 */ Vec3f unk_168;
} EnDyExtra; // size = 0x0174

// TODO remove EnExItem struct and use include:
// #include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
struct EnExItem;
typedef void (*EnExItemActionFunc)(struct EnExItem* this, GlobalContext* globalCtx);
typedef void (*EnExItemLightFunc)(Actor*, GlobalContext*, s32);
typedef struct EnExItem {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnExItemActionFunc actionFunc;
    /* 0x0150 */ s16 getItemObjId;
    /* 0x0152 */ s16 type;
    /* 0x0152 */ s16 unusedParam;
    /* 0x0156 */ s16 drawItemId;
    /* 0x0158 */ s16 stopRotate;
    /* 0x015A */ s16 timer;
    /* 0x015A */ s16 chestKillTimer;
    /* 0x015A */ s16 prizeRotateTimer;
    /* 0x0160 */ s16 killItem;
    /* 0x0164 */ f32 scale;
    /* 0x0168 */ f32 unkFloat; // set to the same value as scale, but unused
    /* 0x016C */ s8 objectIdx;
    /* 0x0170 */ Vec3f initPos; // unused
    /* 0x017C */ EnExItemLightFunc unk_17C;
    /* 0x0180 */ EnExItemLightFunc unk_180;
} EnExItem; // size = 0x0184

#define FLAGS 0x02000030

#define THIS ((BgDyYoseizo*)thisx)

void BgDyYoseizo_Init(Actor* thisx, GlobalContext* globalCtx);
void BgDyYoseizo_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgDyYoseizo_Update(Actor* thisx, GlobalContext* globalCtx);

void func_80872D20(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80872DE4(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_8087328C(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873380(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_808734DC(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_8087358C(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_808736A4(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873780(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873868(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873B3C(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873C14(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873D14(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873E04(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873EA4(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80873FD8(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_80874304(BgDyYoseizo* this, GlobalContext* globalCtx);
void BgDyYoseizo_Draw(Actor* thisx, GlobalContext* globalCtx);
void func_80874D9C(BgDyYoseizo* this, Vec3f*, Vec3f*, Vec3f*, Color_RGB8*, Color_RGB8*, f32, s16, s16);
void func_80874EAC(BgDyYoseizo* this, GlobalContext* globalCtx);
void func_808751A0(BgDyYoseizo* this, GlobalContext* globalCtx);

extern CutsceneData D_2000130[];
extern CutsceneData D_2000160[];
extern CutsceneData D_2001020[];
extern CutsceneData D_20013E0[];
extern CutsceneData D_2001F40[];
extern CutsceneData D_20025D0[];
extern AnimationHeader D_600092C;
extern AnimationHeader D_6001DF0;
extern AnimationHeader D_60031C0;
extern AnimationHeader D_6004344;
extern AnimationHeader D_6005810;
extern Gfx D_6005860[];
extern Gfx D_60058D8[];
extern AnimationHeader D_6005E60;
extern AnimationHeader D_60069E8;
extern AnimationHeader D_6007CA8;
extern AnimationHeader D_6008698;
extern FlexSkeletonHeader D_601C450;
extern AnimationHeader D_601D514;

static s32 D_80875440[3] = { 0x5D, 0x5E, 0x5C };
const ActorInit Bg_Dy_Yoseizo_InitVars = {
    ACTOR_BG_DY_YOSEIZO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_DY_OBJ,
    sizeof(BgDyYoseizo),
    (ActorFunc)BgDyYoseizo_Init,
    (ActorFunc)BgDyYoseizo_Destroy,
    (ActorFunc)BgDyYoseizo_Update,
    NULL,
};
static Color_RGB8 D_8087546C[9] = {
    { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x64 }, { 0x64, 0xFF, 0x64 }, { 0xFF, 0x64, 0x64 }, { 0xFF, 0xFF, 0xAA },
    { 0xFF, 0xFF, 0x64 }, { 0x64, 0xFF, 0x64 }, { 0xFF, 0x64, 0x64 }, { 0xFF, 0xFF, 0xAA },
};
static Color_RGB8 D_80875488[9] = {
    { 0x9B, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x64 }, { 0x64, 0xFF, 0x64 }, { 0xFF, 0x64, 0x64 }, { 0xFF, 0x64, 0xFF },
    { 0xFF, 0xFF, 0x64 }, { 0x64, 0xFF, 0x64 }, { 0xFF, 0x64, 0x64 }, { 0x64, 0xFF, 0xFF },
};
static Vec3f D_808754A4 = { 0.0f, 0.0f, 0.0f };
static s16 D_808754B0[4] = { 2, 0, 1, 0 };
static s16 D_808754B8[4] = { 0x11, 0x10, 0x12, 0 };
static s16 D_808754C0[4] = { 0x100, 0x200, 0x400, 0 };
static u8 D_808754C8[4] = { 0xD, 5, 0x13, 0 };
static s32 D_808754CC[3] = { 0x06017930, 0x06018130, 0x06018930 };
static s32 D_808754D8[2] = { 0x06019130, 0x0601A130 };

void BgDyYoseizo_Init(Actor* thisx, GlobalContext* globalCtx2) {
    BgDyYoseizo* this = (BgDyYoseizo*)thisx;
    GlobalContext* globalCtx = globalCtx2;

    this->unk2EC = (s16)globalCtx->curSpawn;
    if (this->unk2EC < 0) {
        this->unk2EC = 0;
    }
    this->unk310 = this->actor.world.pos.y;
    this->unk30C = this->actor.world.pos.y + 40.0f;
    this->actor.focus.pos = this->actor.world.pos;
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        osSyncPrintf("\x1b[32m☆☆☆☆☆ 大妖精の泉 ☆☆☆☆☆ %d\n\x1b[m", globalCtx->curSpawn);
        SkelAnime_InitFlex(globalCtx, &this->skelAnime, &D_601C450, &D_6008698, this->unk194, this->unk23C, 28);
    } else {
        osSyncPrintf("\x1b[32m☆☆☆☆☆ 石妖精の泉 ☆☆☆☆☆ %d\n\x1b[m", globalCtx->curSpawn);
        SkelAnime_InitFlex(globalCtx, &this->skelAnime, &D_601C450, &D_60031C0, this->unk194, this->unk23C, 28);
    }
    this->actionFunc = func_80872D20;
}

void BgDyYoseizo_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_80872960(BgDyYoseizo* this, GlobalContext* globalCtx, s16 arg2) {
    Vec3f spC4;
    Vec3f spB8;
    Vec3f spAC;
    Color_RGB8 spA8;
    Color_RGB8 spA4;
    f32 temp_fs0;
    f32 var_fs2;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;

    spC4 = D_808754A4;
    if (!(this->unk308 < 0.01f)) {
        temp_fs0 = this->unk308 * 3500.0f;
        spB8.x = Rand_ZeroOne() - 0.5f;
        spB8.y = Rand_ZeroOne() - 0.5f;
        spB8.z = Rand_ZeroOne() - 0.5f;
        for (var_s3 = 0; var_s3 < 2; var_s3++) {
            if (arg2 == 0) {
                var_s1 = 0;
                var_fs2 = 0.4f;
                var_s2 = 0x5A;
                spAC.x = this->actor.world.pos.x;
                spAC.y = this->actor.world.pos.y + temp_fs0 + (temp_fs0 * 0.5f * (Rand_ZeroOne() - 0.5f));
                spAC.z = this->actor.world.pos.z + 30.0f;
            } else {
                var_fs2 = 0.2f;
                var_s2 = 0x32;
                var_s1 = arg2;
                spAC.x = Rand_CenteredFloat(10.0f) + this->actor.world.pos.x;
                if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                    spAC.y = this->actor.world.pos.y + temp_fs0 + 50.0f + (temp_fs0 * 0.1f * (Rand_ZeroOne() - 0.5f));
                    spAC.z = this->actor.world.pos.z + 30.0f;
                } else {
                    spAC.y = this->actor.world.pos.y + temp_fs0 - 30.0f + (temp_fs0 * 0.1f * (Rand_ZeroOne() - 0.5f));
                    spAC.z = this->actor.world.pos.z + 60.0f;
                }
                if (LINK_IS_ADULT) {
                    spAC.y += 20.0f;
                }
            }
            spA8.r = D_8087546C[var_s1].r;
            spA8.g = D_8087546C[var_s1].g;
            spA8.b = D_8087546C[var_s1].b;
            spA4.r = D_80875488[var_s1].r;
            spA4.g = D_80875488[var_s1].g;
            spA4.b = D_80875488[var_s1].b;
            func_80874D9C(this, &spAC, &spC4, &spB8, &spA8, &spA4, var_fs2, var_s2, var_s1);
        }
    }
}

void func_80872C58(BgDyYoseizo* this, GlobalContext* globalCtx) {
    this->unk31C = this->unk30C + this->unk320;
    Math_ApproachF(&this->actor.world.pos.y, this->unk31C, 0.1f, 10.0f);
    Math_ApproachF(&this->unk320, 10.0f, 0.1f, 0.5f);
    if (globalCtx->csCtx.state == CS_STATE_IDLE) {
        this->actor.velocity.y = Math_SinS((s16)(s32)this->unk324);
    } else {
        this->actor.velocity.y = Math_SinS((s16)(s32)this->unk324) * 0.4f;
    }
}

void func_80872D20(BgDyYoseizo* this, GlobalContext* globalCtx) {
    if (Flags_GetSwitch(globalCtx, 0x38)) {
        globalCtx->msgCtx.unk_E3EE = 4;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            if (!gSaveContext.magicAcquired && (this->unk2EC != 0)) {
                Actor_Kill(&this->actor);
                return;
            }
        } else {
            if (!gSaveContext.magicAcquired) {
                Actor_Kill(&this->actor);
                return;
            }
        }
        func_8002DF54(globalCtx, &this->actor, 1U);
        this->actionFunc = func_80872DE4;
    }
}

void func_80872DE4(BgDyYoseizo* this, GlobalContext* globalCtx) {
    s32 var_v1;

    func_8002DF54(globalCtx, &this->actor, 1U);
    osSyncPrintf("\x1b[33m☆☆☆☆☆ もうど ☆☆☆☆☆ %d\n\x1b[m", globalCtx->msgCtx.unk_E3EE);
    var_v1 = 0;
    if (globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) {
        switch (this->unk2EC) {
            case 0:
                if (!(gSaveContext.itemGetInf[1] & 0x100)) {
                    var_v1 = 1;
                }
                break;

            case 1:
                if (!(gSaveContext.itemGetInf[1] & 0x200)) {
                    var_v1 = 1;
                }
                break;

            case 2:
                if (!(gSaveContext.itemGetInf[1] & 0x400)) {
                    var_v1 = 1;
                }
                break;
        }
    } else {
        switch (this->unk2EC) {
            case 0:
                if (!gSaveContext.magicAcquired || (gGameInfo->data[0x962] != 0)) {
                    osSyncPrintf("\x1b[32m ☆☆☆☆☆ 回転切り速度ＵＰ ☆☆☆☆☆ \n\x1b[m");
                    var_v1 = 1;
                    this->unk2EA = 1;
                }
                break;

            case 1:
                if (!gSaveContext.doubleMagic) {
                    osSyncPrintf("\x1b[33m ☆☆☆☆☆ 魔法ゲージメーター倍増 ☆☆☆☆☆ \n\x1b[m");
                    var_v1 = 1;
                    this->unk2EA = 1;
                }
                break;

            case 2:
                if (!gSaveContext.doubleDefense) {
                    osSyncPrintf("\x1b[35m ☆☆☆☆☆ ダメージ半減 ☆☆☆☆☆ \n\x1b[m");
                    var_v1 = 1;
                    this->unk2EA = 1;
                }
                break;
        }
    }
    if (var_v1 != 0) {
        if (gSaveContext.sceneSetupIndex < 4) {
            if (globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) {
                switch (this->unk2EC) {
                    case 0:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_2000160);
                        gSaveContext.cutsceneTrigger = 1;
                        break;

                    case 1:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_2001020);
                        gSaveContext.cutsceneTrigger = 1;
                        break;

                    case 2:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_2001F40);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                }
            } else {
                switch (this->unk2EC) {
                    case 0:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_2000130);
                        gSaveContext.cutsceneTrigger = 1;
                        break;

                    case 1:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_20013E0);
                        gSaveContext.cutsceneTrigger = 1;
                        break;

                    case 2:
                        globalCtx->csCtx.segment = SEGMENTED_TO_VIRTUAL(D_20025D0);
                        gSaveContext.cutsceneTrigger = 1;
                        break;
                }
            }
        }
        this->actionFunc = func_80873EA4;
    } else {
        globalCtx->envCtx.unk_BF = 2;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            func_800800F8(globalCtx, 0x219B, -0x63, NULL, 0);
        } else {
            func_800800F8(globalCtx, 0x219C, -0x63, NULL, 0);
        }
        Audio_PlayActorSound2(&this->actor, NA_SE_EV_GREAT_FAIRY_APPEAR);
        this->actor.draw = BgDyYoseizo_Draw;
        this->actionFunc = func_8087328C;
    }
}

void func_8087328C(BgDyYoseizo* this, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        this->unk32C = Animation_GetLastFrame(&D_6008698);
        Animation_Change(&this->skelAnime, &D_6008698, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    } else {
        this->unk32C = Animation_GetLastFrame(&D_60031C0);
        Animation_Change(&this->skelAnime, &D_60031C0, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    }
    Audio_PlayActorSound2(&this->actor, NA_SE_VO_FR_LAUGH_0);
    func_8002DF54(globalCtx, &this->actor, 1U);
    this->actionFunc = func_80873380;
}

void func_80873380(BgDyYoseizo* this, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &this->actor, 1U);
    Math_ApproachF(&this->actor.world.pos.y, this->unk30C, this->unk314, 100.0f);
    Math_ApproachF(&this->unk308, 0.035f, this->unk318, 0.005f);
    Math_ApproachF(&this->unk314, 0.8f, 0.1f, 0.02f);
    Math_ApproachF(&this->unk318, 0.2f, 0.03f, 0.05f);
    if (this->unk308 >= 0.034f) {
        if ((this->actor.shape.rot.y >= -0x1F3F) && (this->actor.shape.rot.y < 0x3E8)) {
            SkelAnime_Update(&this->skelAnime);
            Math_SmoothStepToS(&this->actor.shape.rot.y, 0, 5, 0x3E8, 0);
            if (fabsf(this->actor.shape.rot.y) < 50.0f) {
                this->actionFunc = func_808734DC;
            }
        } else {
            this->actor.shape.rot.y += 0xBB8;
        }
    } else {
        this->actor.shape.rot.y += 0xBB8;
    }
    func_80872960(this, globalCtx, 0);
}

void func_808734DC(BgDyYoseizo* this, GlobalContext* globalCtx) {
    f32 sp1C;

    sp1C = this->skelAnime.curFrame;
    func_8002DF54(globalCtx, &this->actor, 1U);
    if ((this->unk32C * 1273.0f) <= this->unk324) {
        this->unk324 = 0.0f;
    }
    SkelAnime_Update(&this->skelAnime);
    if ((this->unk32C <= sp1C) && (this->unk2FC == 0)) {
        this->actionFunc = func_8087358C;
    }
}

void func_8087358C(BgDyYoseizo* this, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &this->actor, 1U);
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        this->unk32C = Animation_GetLastFrame(&D_601D514);
        Animation_Change(&this->skelAnime, &D_601D514, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
    } else {
        this->unk32C = Animation_GetLastFrame(&D_6001DF0);
        Animation_Change(&this->skelAnime, &D_6001DF0, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
    }
    this->actor.textId = 0xDB;
    this->unk2EE = 5;
    func_8010B680(globalCtx, this->actor.textId, NULL);
    func_80872960(this, globalCtx, 0);
    this->actionFunc = func_808736A4;
}

void func_808736A4(BgDyYoseizo* this, GlobalContext* globalCtx) {
    func_8002DF54(globalCtx, &this->actor, 1U);
    this->unk324 = this->skelAnime.curFrame * 1273.0f;
    if (this->unk324 >= (this->unk32C * 1273.0f)) {
        this->unk324 = 0.0f;
    }
    SkelAnime_Update(&this->skelAnime);
    if ((this->unk2EE == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        Interface_ChangeAlpha(5U);
        this->actionFunc = func_80873780;
    }
    func_80872C58(this, globalCtx);
    func_80872960(this, globalCtx, 0);
}

void func_80873780(BgDyYoseizo* this, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        this->unk32C = Animation_GetLastFrame(&D_60069E8);
        Animation_Change(&this->skelAnime, &D_60069E8, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    } else {
        this->unk32C = Animation_GetLastFrame(&D_6005810);
        Animation_Change(&this->skelAnime, &D_6005810, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    }
    Audio_PlayActorSound2(&this->actor, NA_SE_VO_FR_SMILE_0);
    this->unk2F6 = 1;
    this->actionFunc = func_80873868;
}

void func_80873868(BgDyYoseizo* this, GlobalContext* globalCtx) {
    Player* player;
    f32 temp_fv1;
    s16 var_v0;
    Vec3f vec;

    player = PLAYER;
    temp_fv1 = this->skelAnime.curFrame;
    if (this->unk2FC != 0) {
        this->unk324 = this->skelAnime.curFrame * 1300.0f;
        if ((this->unk32C * 1300.0f) <= this->unk324) {
            this->unk324 = 0.0f;
        }
    }
    SkelAnime_Update(&this->skelAnime);
    if ((this->unk32C <= temp_fv1) && (this->unk2FC == 0)) {
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            this->unk32C = Animation_GetLastFrame(&D_6007CA8);
            Animation_Change(&this->skelAnime, &D_6007CA8, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
        } else {
            this->unk32C = Animation_GetLastFrame(&D_6004344);
            Animation_Change(&this->skelAnime, &D_6004344, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
        }
        this->unk302 = 0x96;
        this->unk2FC = 1;
        if (this->unk2EA == 0) {
            vec.x = player->actor.world.pos.x;
            vec.y = player->actor.world.pos.y + 200.0f;
            vec.z = player->actor.world.pos.z;
            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                var_v0 = 0;
            } else {
                var_v0 = 1;
            }
            this->unk340 = (EnDyExtra*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx,
                                                          ACTOR_EN_DY_EXTRA, vec.x, vec.y, vec.z, 0, 0, 0, var_v0);
        }
    }
    if (this->unk306 >= 2) {
        this->unk306--;
    }
    if (this->unk302 >= 0x6E) {
        this->unk302--;
    }
    if (this->unk302 == 0x6E) {
        gSaveContext.healthAccumulator = 0x140;
        Magic_Fill(globalCtx);
        this->unk306 = 0xC8;
    }
    if (((gSaveContext.healthCapacity == gSaveContext.health) && (gSaveContext.magic == gSaveContext.unk_13F4)) ||
        (this->unk306 == 1)) {
        this->unk302--;
        if (this->unk302 == 0x5A) {
            if (this->unk2EA == 0) {
                this->unk340->unk_152 = 1;
            }
            this->unk2EA = 0;
        }
    }
    if (this->unk302 == 1) {
        this->actor.textId = 0xDA;
        this->unk2EE = 5;
        func_8010B720(globalCtx, this->actor.textId);
        this->actionFunc = func_80873B3C;
    } else {
        func_80872C58(this, globalCtx);
    }
}

void func_80873B3C(BgDyYoseizo* this, GlobalContext* globalCtx) {
    this->unk324 = this->skelAnime.curFrame * 1400.0f;
    if (this->unk324 >= (this->unk32C * 1400.0f)) {
        this->unk324 = 0.0f;
    }
    SkelAnime_Update(&this->skelAnime);
    if ((this->unk2EE == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        this->unk2F6 = 0;
        this->actionFunc = func_80873C14;
        func_8005B1A4(globalCtx->cameraPtrs[globalCtx->activeCamera]);
    }
    func_80872C58(this, globalCtx);
    func_80872960(this, globalCtx, 0);
}

void func_80873C14(BgDyYoseizo* this, GlobalContext* globalCtx) {
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        this->unk32C = Animation_GetLastFrame(&D_6005E60);
        Animation_Change(&this->skelAnime, &D_6005E60, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    } else {
        this->unk32C = Animation_GetLastFrame(&D_600092C);
        Animation_Change(&this->skelAnime, &D_600092C, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
    }
    this->unk2E8 = 5;
    this->unk318 = 0.0f;
    this->unk314 = 0.0f;
    Audio_PlayActorSound2(&this->actor, NA_SE_VO_FR_LAUGH_0);
    Audio_PlayActorSound2(&this->actor, NA_SE_EV_GREAT_FAIRY_VANISH);
    this->actionFunc = func_80873D14;
}

void func_80873D14(BgDyYoseizo* this, GlobalContext* globalCtx) {
    SkelAnime_Update(&this->skelAnime);
    if (this->unk2E8 == 0) {
        if (this->unk308 < 0.003f) {
            this->unk2E8 = 0x1E;
            this->actionFunc = func_80873E04;
        } else {
            Math_ApproachF(&this->actor.world.pos.y, this->unk310, this->unk314, 100.0f);
            Math_ApproachZeroF(&this->unk308, this->unk318, 0.005f);
            Math_ApproachF(&this->unk314, 0.8f, 0.1f, 0.02f);
            Math_ApproachF(&this->unk318, 0.2f, 0.03f, 0.05f);
            this->actor.shape.rot.y += 0xBB8;
            func_80872960(this, globalCtx, 0);
        }
    }
}

void func_80873E04(BgDyYoseizo* this, GlobalContext* globalCtx) {
    Actor* var_a0;

    if (this->unk2E8 == 0) {
        func_8002DF54(globalCtx, &this->actor, 7U);
        globalCtx->envCtx.unk_BF = 0;
        var_a0 = globalCtx->actorCtx.actorLists[ACTORCAT_PROP].head;
        while (var_a0 != NULL) {
            if (var_a0->id != ACTOR_EN_OKARINA_TAG) {
                var_a0 = var_a0->next;
            } else {
                Actor_Kill(var_a0);
                break;
            }
        }
        Flags_UnsetSwitch(globalCtx, 0x38);
        Actor_Kill(&this->actor);
    }
}

void func_80873EA4(BgDyYoseizo* this, GlobalContext* globalCtx) {
    if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[0] != NULL) &&
        (globalCtx->csCtx.npcActions[0]->action == 2)) {
        this->actor.draw = BgDyYoseizo_Draw;
        func_8002DF54(globalCtx, &this->actor, 1U);
        this->unk2FE = 0;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            this->unk32C = Animation_GetLastFrame(&D_6008698);
            Animation_Change(&this->skelAnime, &D_6008698, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
        } else {
            this->unk32C = Animation_GetLastFrame(&D_60031C0);
            Animation_Change(&this->skelAnime, &D_60031C0, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
        }
        Audio_PlayActorSound2(&this->actor, NA_SE_EV_GREAT_FAIRY_APPEAR);
        this->actionFunc = func_80873FD8;
    }
}

void func_80873FD8(BgDyYoseizo* this, GlobalContext* globalCtx) {
    f32 temp_fv0;

    temp_fv0 = this->skelAnime.curFrame;
    if (this->unk2FE == 0) {
        Math_ApproachF(&this->actor.world.pos.y, this->unk30C, this->unk314, 100.0f);
        Math_ApproachF(&this->unk308, 0.035f, this->unk318, 0.005f);
        Math_ApproachF(&this->unk314, 0.8f, 0.1f, 0.02f);
        Math_ApproachF(&this->unk318, 0.2f, 0.03f, 0.05f);
        if (this->unk308 >= 0.034f) {
            if ((this->actor.shape.rot.y >= -0x1F3F) && (this->actor.shape.rot.y < 0x3E8)) {
                SkelAnime_Update(&this->skelAnime);
                Math_ApproachS(&this->actor.shape.rot.y, 0, 5, 0x3E8);
                if (fabsf(this->actor.shape.rot.y) < 50.0f) {
                    this->unk2FE = 1;
                }
            } else {
                this->actor.shape.rot.y += 0xBB8;
            }
        } else {
            this->actor.shape.rot.y += 0xBB8;
        }
    } else {
        SkelAnime_Update(&this->skelAnime);
        if ((this->unk32C <= temp_fv0) && (this->unk2FC == 0)) {
            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                this->unk32C = Animation_GetLastFrame(&D_601D514);
                Animation_Change(&this->skelAnime, &D_601D514, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
            } else {
                this->unk32C = Animation_GetLastFrame(&D_6001DF0);
                Animation_Change(&this->skelAnime, &D_6001DF0, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
            }
            this->unk2FC = 1;
        }
        if ((globalCtx->csCtx.state != CS_STATE_IDLE) && (globalCtx->csCtx.npcActions[0] != NULL) &&
            (globalCtx->csCtx.npcActions[0]->action == 3)) {
            this->unk2FE = this->unk2FC = 0;
            if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
                this->unk32C = Animation_GetLastFrame(&D_60069E8);
                Animation_Change(&this->skelAnime, &D_60069E8, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
            } else {
                this->unk32C = Animation_GetLastFrame(&D_6005810);
                Animation_Change(&this->skelAnime, &D_6005810, 1.0f, 0.0f, this->unk32C, ANIMMODE_ONCE, -10.0f);
            }
            this->unk2F6 = 1;
            this->actionFunc = func_80874304;
        }
    }
    func_80872960(this, globalCtx, 0);
}

void func_80874304(BgDyYoseizo* this, GlobalContext* globalCtx) {
    f32 temp_fv1_sp5C;
    Player* player;
    s16 var_v1;
    s16 sp56;
    Vec3f var_fv1;

    temp_fv1_sp5C = this->skelAnime.curFrame;
    player = PLAYER;
    if (this->unk2FC != 0) {
        this->unk324 = this->skelAnime.curFrame * 1400.0f;
        if (this->unk324 >= (this->unk32C * 1400.0f)) {
            this->unk324 = 0.0f;
        }
    }
    SkelAnime_Update(&this->skelAnime);
    if ((this->unk32C <= temp_fv1_sp5C) && (this->unk2FC == 0)) {
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            this->unk32C = Animation_GetLastFrame(&D_6007CA8);
            Animation_Change(&this->skelAnime, &D_6007CA8, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
        } else {
            this->unk32C = Animation_GetLastFrame(&D_6004344);
            Animation_Change(&this->skelAnime, &D_6004344, 1.0f, 0.0f, this->unk32C, ANIMMODE_LOOP, -10.0f);
        }
        this->unk2FC = 1;
    }
    if (globalCtx->csCtx.npcActions[0]->action == 0xD) {
        this->actionFunc = func_80873C14;
        return;
    }
    if ((globalCtx->csCtx.npcActions[0]->action >= 4) && (globalCtx->csCtx.npcActions[0]->action < 7)) {
        var_v1 = globalCtx->csCtx.npcActions[0]->action - 4;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            var_v1 += 1;
            func_80872960(this, globalCtx, var_v1);
        } else if (this->unk2E4 == 0) {
            sp56 = D_808754B0[var_v1] << 0xC;
            sp56 |= 0x12;
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DEMO_EFFECT, this->actor.world.pos.x,
                        this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, sp56);
            this->unk2E4 = 1;
        }
    } else {
        func_80872960(this, globalCtx, 0);
    }
    if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
        if ((globalCtx->csCtx.npcActions[0]->action >= 0xA) && (globalCtx->csCtx.npcActions[0]->action < 0xD)) {
            var_v1 = globalCtx->csCtx.npcActions[0]->action - 0xA;
            switch (var_v1) {
                case 0:
                    gSaveContext.magicAcquired = 1;
                    gSaveContext.unk_13F6 = 0x30;
                    Interface_ChangeAlpha(9U);
                    break;

                case 1:
                    if (gSaveContext.magicAcquired == 0) {
                        gSaveContext.magicAcquired = 1;
                    }
                    gSaveContext.doubleMagic = 1;
                    gSaveContext.unk_13F6 = 0x60;
                    gSaveContext.magicLevel = 0;
                    Interface_ChangeAlpha(9U);
                    break;

                case 2:
                    gSaveContext.doubleDefense = 1;
                    Interface_ChangeAlpha(9U);
                    break;
            }
            if (this->unk2E6 == 0) {
                gSaveContext.healthAccumulator = 0x140;
                this->unk2E6 = 1;
                if (var_v1 == 2) {
                    Magic_Fill(globalCtx);
                }
            }
        }
    }
    if (globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) {
        if ((globalCtx->csCtx.npcActions[0]->action >= 0xE) && (globalCtx->csCtx.npcActions[0]->action < 0x11)) {
            var_v1 = globalCtx->csCtx.npcActions[0]->action - 0xE;
            if (this->unk300 == 0) {
                var_fv1.x = player->actor.world.pos.x;
                if (LINK_IS_ADULT) {
                    var_fv1.y = player->actor.world.pos.y + 73.0f;
                } else {
                    var_fv1.y = player->actor.world.pos.y + 53.0f;
                }
                var_fv1.z = player->actor.world.pos.z;
                this->unk344 =
                    (EnExItem*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                                  var_fv1.x, var_fv1.y, var_fv1.z, 0, 0, 0, D_808754B8[var_v1]);
                if (this->unk344 != NULL) {
                    if (!gSaveContext.magicAcquired) {
                        gSaveContext.magicAcquired = true;
                    } else {
                        Magic_Fill(globalCtx);
                    }
                    this->unk300 = 1;
                    gSaveContext.healthAccumulator = 0x140;
                    Interface_ChangeAlpha(9U);
                    gSaveContext.itemGetInf[1] |= D_808754C0[var_v1];
                    Item_Give(globalCtx, D_808754C8[var_v1]);
                }
            } else {
                this->unk344->actor.world.pos.x = player->actor.world.pos.x;
                if (LINK_IS_ADULT) {
                    this->unk344->actor.world.pos.y = player->actor.world.pos.y + 73.0f;
                } else {
                    this->unk344->actor.world.pos.y = player->actor.world.pos.y + 53.0f;
                }
                this->unk344->actor.world.pos.z = player->actor.world.pos.z;
                this->unk344->scale = 0.3f;
            }
        }
    }
    if ((globalCtx->sceneNum != SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action == 0x11)) {
        if (this->unk344 != NULL) {
            Actor_Kill(&this->unk344->actor);
            this->unk344 = NULL;
        }
    }
    if ((globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) && (globalCtx->csCtx.npcActions[0]->action == 0x12)) {
        this->unk2E5 = 1;
    }
    if (this->unk2E5 != 0) {
        if (gSaveContext.inventory.defenseHearts < 20) {
            gSaveContext.inventory.defenseHearts += 1;
        }
    }
    if ((globalCtx->csCtx.npcActions[0]->action >= 0x13) && (globalCtx->csCtx.npcActions[0]->action < 0x16) &&
        (this->unk304 == 0)) {
        var_v1 = globalCtx->csCtx.npcActions[0]->action - 0xB;
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_DOOR_WARP1, player->actor.world.pos.x,
                    player->actor.world.pos.y, player->actor.world.pos.z, 0, 0, 0, var_v1);
        this->unk304 = 1;
    }
    func_80872C58(this, globalCtx);
}

void BgDyYoseizo_Update(Actor* thisx, GlobalContext* globalCtx2) {
    BgDyYoseizo* this = (BgDyYoseizo*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s32 var_v1;

    this->unk2F0 += 1;
    if (this->unk2E8 != 0) {
        this->unk2E8--;
    }
    if (this->unk2F8 != 0) {
        this->unk2F8--;
    }
    if (this->unk2FA != 0) {
        this->unk2FA--;
    }
    this->actionFunc(this, globalCtx);
    if (globalCtx->csCtx.state != CS_STATE_IDLE) {
        var_v1 = 0;
        if (globalCtx->sceneNum == SCENE_DAIYOUSEI_IZUMI) {
            if ((globalCtx->csCtx.frames == 32) || (globalCtx->csCtx.frames == 291) ||
                (globalCtx->csCtx.frames == 426) || (globalCtx->csCtx.frames == 851)) {
                var_v1 = 1;
            }
            if (globalCtx->csCtx.frames == 101) {
                var_v1 = 2;
            }
        } else {
            if ((globalCtx->csCtx.frames == 35) || (globalCtx->csCtx.frames == 181) ||
                (globalCtx->csCtx.frames == 462) || (globalCtx->csCtx.frames == 795)) {
                var_v1 = 1;
            }
            if (globalCtx->csCtx.frames == 90) {
                var_v1 = 2;
            }
        }
        if (var_v1 == 1) {
            Audio_PlayActorSound2(&this->actor, NA_SE_VO_FR_SMILE_0);
        }
        if (var_v1 == 2) {
            Audio_PlayActorSound2(&this->actor, NA_SE_VO_FR_LAUGH_0);
        }
    }
    if ((this->unk2F8 == 0) && (this->actionFunc != func_80873868)) {
        this->unk2F2 += 1;
        this->unk2F4 += 1;
        if (this->unk2F2 >= 3) {
            this->unk2F4 = 0;
            this->unk2F2 = this->unk2F4;
            this->unk2F8 = (s16)Rand_ZeroFloat(60.0f) + 0x14;
        }
    }
    Actor_MoveForward(&this->actor);
    this->unk328 = this->unk308 * 7500.0f;
    Actor_SetFocus(&this->actor, this->unk328);
    this->actor.focus.pos.y = this->unk328;
    func_80038290(globalCtx, &this->actor, &this->unk334, &this->unk33A, this->actor.focus.pos);
    func_80874EAC(this, globalCtx);
    Actor_SetScale(&this->actor, this->unk308);
}

s32 func_80874B7C(GlobalContext* globalCtx, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx) {
    BgDyYoseizo* this = thisx;

    if (limbIndex == 8) {
        rot->x += this->unk33A.y;
    }
    if (limbIndex == 15) {
        rot->x += this->unk334.y;
        rot->z += this->unk334.z;
    }
    return 0;
}

void BgDyYoseizo_Draw(Actor* thisx, GlobalContext* globalCtx) {
    BgDyYoseizo* this = (BgDyYoseizo*)thisx;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_dy_yoseizo.c", 1609);
    if (this->actionFunc != func_80873E04) {
        func_80093D18(globalCtx->state.gfxCtx);
        gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_808754CC[this->unk2F2]));
        gSPSegment(POLY_OPA_DISP++, 9, SEGMENTED_TO_VIRTUAL(D_808754CC[this->unk2F4]));
        gSPSegment(POLY_OPA_DISP++, 10, SEGMENTED_TO_VIRTUAL(D_808754D8[this->unk2F6]));
        SkelAnime_DrawFlexOpa(globalCtx, this->skelAnime.skeleton, this->skelAnime.jointTable,
                              this->skelAnime.dListCount, func_80874B7C, NULL, this);
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_dy_yoseizo.c", 1629);
    func_808751A0(this, globalCtx);
}

void func_80874D9C(BgDyYoseizo* this, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, Color_RGB8* arg4, Color_RGB8* arg5,
                   f32 arg6, s16 arg7, s16 arg8) {
    struct_BgDyYoseizo_394* var_v0;
    s16 var_v1;

    var_v0 = this->unk394;
    for (var_v1 = 0; var_v1 < ARRAY_COUNT(this->unk394); var_v1++, var_v0++) {
        if (var_v0->unk0 == 0) {
            var_v0->unk0 = 1;
            var_v0->unk4 = *arg1;
            var_v0->unk10 = *arg2;
            var_v0->unk1C = *arg3;
            var_v0->unk28 = *arg4;
            var_v0->unk2E = 0;
            var_v0->unk2B = *arg5;
            var_v0->unk30 = arg6;
            var_v0->unk34 = arg7;
            var_v0->unk36 = arg8;
            var_v0->unk38 = 0.0f;
            var_v0->unk3C = Rand_CenteredFloat(30000.0f);
            var_v0->unk40 = 0.0f;
            break;
        }
    }
}

void func_80874EAC(BgDyYoseizo* this, GlobalContext* globalCtx) {
    s16 var_s5;
    Player* player;
    Vec3f sp94;
    Vec3f sp88;
    struct_BgDyYoseizo_394* var_s0;
    f32 temp_fs0;
    f32 temp_fs1;

    var_s0 = this->unk394;
    player = PLAYER;
    for (var_s5 = 0; var_s5 < ARRAY_COUNT(this->unk394); var_s5++, var_s0++) {
        if (var_s0->unk0 != 0) {
            var_s0->unk40 += 3000.0f;
            if (var_s0->unk36 == 0) {
                var_s0->unk4.x += var_s0->unk10.x;
                var_s0->unk4.y += var_s0->unk10.y;
                var_s0->unk4.z += var_s0->unk10.z;
                var_s0->unk10.x += var_s0->unk1C.x;
                var_s0->unk10.y += var_s0->unk1C.y;
                var_s0->unk10.z += var_s0->unk1C.z;
            } else {
                Audio_PlayActorSound2(&this->actor, NA_SE_EV_HEALING - SFX_FLAG);
                sp94 = player->actor.world.pos;
                sp94.y = player->actor.world.pos.y - 150.0f;
                sp94.z = player->actor.world.pos.z - 50.0f;
                temp_fs0 = Math_Vec3f_Pitch(&var_s0->unk4, &sp94);
                temp_fs1 = Math_Vec3f_Yaw(&var_s0->unk4, &sp94);
                Math_ApproachF(&var_s0->unk38, temp_fs0, 0.9f, 5000.0f);
                Math_ApproachF(&var_s0->unk3C, temp_fs1, 0.9f, 5000.0f);
                Matrix_Push();
                Matrix_RotateY(BINANG_TO_RAD(var_s0->unk3C), MTXMODE_NEW);
                Matrix_RotateX(BINANG_TO_RAD(var_s0->unk38), MTXMODE_APPLY);
                sp94.z = 3.0f;
                sp94.y = 3.0f;
                sp94.x = 3.0f;
                Matrix_MultVec3f(&sp94, &sp88);
                Matrix_Pull();
                var_s0->unk4.x += sp88.x;
                var_s0->unk4.y += sp88.y;
                var_s0->unk4.z += sp88.z;
            }
        }
        if (var_s0->unk34 != 0) {
            var_s0->unk34 -= 1;
            var_s0->unk2E += 0x1E;
            if (var_s0->unk2E >= 0x100) {
                var_s0->unk2E = 0xFF;
            }
        } else {
            var_s0->unk2E -= 0x1E;
            if (var_s0->unk2E <= 0) {
                var_s0->unk0 = 0;
                var_s0->unk2E = var_s0->unk0;
            }
        }
    }
}

void func_808751A0(BgDyYoseizo* this, GlobalContext* globalCtx) {
    GraphicsContext* gfxCtx;
    s16 var_s4;
    struct_BgDyYoseizo_394* var_s0;
    u8 materialFlag;

    gfxCtx = globalCtx->state.gfxCtx;
    var_s0 = this->unk394;
    materialFlag = 0;
    OPEN_DISPS(gfxCtx, "../z_bg_dy_yoseizo.c", 1767);
    func_80093D84(globalCtx->state.gfxCtx);
    for (var_s4 = 0; var_s4 < ARRAY_COUNT(this->unk394); var_s4++, var_s0++) {
        if (var_s0->unk0 == 1) {
            if (materialFlag == 0) {
                gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(D_6005860));
                gDPPipeSync(POLY_XLU_DISP++);
                materialFlag++;
            }
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, var_s0->unk28.r, var_s0->unk28.g, var_s0->unk28.b,
                            var_s0->unk2E);
            gDPSetEnvColor(POLY_XLU_DISP++, var_s0->unk2B.r, var_s0->unk2B.g, var_s0->unk2B.b, 0);
            Matrix_Translate(var_s0->unk4.x, var_s0->unk4.y, var_s0->unk4.z, MTXMODE_NEW);
            func_800D1FD4(&globalCtx->mf_11DA0);
            Matrix_Scale(var_s0->unk30, var_s0->unk30, 1.0f, MTXMODE_APPLY);
            Matrix_RotateZ(var_s0->unk40, MTXMODE_APPLY);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx, "../z_bg_dy_yoseizo.c", 1810),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, SEGMENTED_TO_VIRTUAL(D_60058D8));
        }
    }
    CLOSE_DISPS(gfxCtx, "../z_bg_dy_yoseizo.c", 1819);
}
