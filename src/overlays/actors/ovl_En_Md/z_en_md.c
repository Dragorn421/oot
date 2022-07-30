#include "global.h"
#include "z_en_md.h"
#include "macros.h"
#include "assets/objects/object_md/object_md.h"
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "z64cutscene.h"

#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3 | ACTOR_FLAG_4 | ACTOR_FLAG_25)

void EnMd_Init(Actor* thisx, PlayState* play);
void EnMd_Destroy(Actor* thisx, PlayState* play);
void EnMd_Update(Actor* thisx, PlayState* play);
void EnMd_Draw(Actor* thisx, PlayState* play);

void func_80AAB874(EnMd* this, PlayState* play);
void func_80AAB8F8(EnMd* this, PlayState* play);
void func_80AAB948(EnMd* this, PlayState* play);
void func_80AABC10(EnMd* this, PlayState* play);
void func_80AABD0C(EnMd* this, PlayState* play);

extern AnimationHeader D_60002C8;
extern SkeletonHeader D_6007FB8;

const ActorInit En_Md_InitVars = {
    /**/ ACTOR_EN_MD,
    /**/ ACTORCAT_NPC,
    /**/ FLAGS,
    /**/ OBJECT_MD,
    /**/ sizeof(EnMd),
    /**/ EnMd_Init,
    /**/ EnMd_Destroy,
    /**/ EnMd_Update,
    /**/ EnMd_Draw,
};

ColliderCylinderInit D_80AAC310 = {
    { 0xA, 0, 0, 0x39, 0x20, 1 },
    { 0, { 0, 0, 0 }, { 0, 0, 0 }, 0, 0, 1 },
    { 0x24, 0x2E, 0, { 0, 0, 0 } },
};
CollisionCheckInfoInit2 D_80AAC33C = { 0, 0, 0, 0, 0xFF };

typedef enum {
    /*  0 */ ENMD_ANIM_0,
    /*  1 */ ENMD_ANIM_1,
    /*  2 */ ENMD_ANIM_2,
    /*  3 */ ENMD_ANIM_3,
    /*  4 */ ENMD_ANIM_4,
    /*  5 */ ENMD_ANIM_5,
    /*  6 */ ENMD_ANIM_6,
    /*  7 */ ENMD_ANIM_7,
    /*  8 */ ENMD_ANIM_8,
    /*  9 */ ENMD_ANIM_9,
    /* 10 */ ENMD_ANIM_10,
    /* 11 */ ENMD_ANIM_11,
    /* 12 */ ENMD_ANIM_12,
    /* 13 */ ENMD_ANIM_13
} EnMdAnimation;

static AnimationInfo sAnimationInfo[] = {
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -10.0f },
    { &gMidoRaiseHand1Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoHaltAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoPutHandDownAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle1Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoAnnoyedPointedHeadIdle2Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoAnim_92B0, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoWalkingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoHandsOnHipsTransitionAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoHandsOnHipsIdleAnim, 0.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -8.0f },
    { &gMidoSlamAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
    { &gMidoRaiseHand2Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, -1.0f },
    { &gMidoAngryHeadTurnAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -1.0f },
};
Vec3f D_80AAC498 = { 400.0f, 0.0f, 0.0f };
void* D_80AAC4A4[3] = {
    gMidoEyeOpenTex,
    gMidoEyeHalfTex,
    gMidoEyeClosedTex,
};

void func_80AAA250(EnMd* this) {
    f32 temp_fv0;
    f32 temp_fv1;

    temp_fv0 = this->unk14C.startFrame;
    temp_fv1 = this->unk14C.endFrame;
    this->unk14C.startFrame = temp_fv1;
    this->unk14C.curFrame = temp_fv1;
    this->unk14C.endFrame = temp_fv0;
    this->unk14C.playSpeed = -1.0f;
}

void func_80AAA274(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_2);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_3);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA308(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_4);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_5);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA39C(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_2);
            func_80AAA250(this);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_7);
                this->unk20A++;
            } else {
                break;
            }
            FALLTHROUGH;
        case 2:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_8);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA474(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_7);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_10);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA508(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_2);
            func_80AAA250(this);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_10);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA5A4(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_9);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA638(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_9);
            func_80AAA250(this);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_10);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA6D4(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_11);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA768(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_12);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_3);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA7FC(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_13);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_6);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA890(EnMd* this) {
    switch (this->unk20A) {
        case 0:
            Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_7);
            func_80AAA250(this);
            this->unk20A++;
            FALLTHROUGH;
        case 1:
            if (Animation_OnFrame(&this->unk14C, this->unk14C.endFrame) != 0) {
                Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_10);
                this->unk20A++;
            }
            break;
    }
}

void func_80AAA92C(EnMd* this, u8 arg1) {
    this->unk20B = arg1;
    this->unk20A = 0;
}

void func_80AAA93C(EnMd* this) {
    switch (this->unk20B) {
        case 1:
            func_80AAA274(this);
            break;
        case 2:
            func_80AAA308(this);
            break;
        case 3:
            func_80AAA39C(this);
            break;
        case 4:
            func_80AAA474(this);
            break;
        case 5:
            func_80AAA508(this);
            break;
        case 6:
            func_80AAA5A4(this);
            break;
        case 7:
            func_80AAA638(this);
            break;
        case 8:
            func_80AAA6D4(this);
            break;
        case 9:
            func_80AAA768(this);
            break;
        case 10:
            func_80AAA7FC(this);
            break;
        case 11:
            func_80AAA890(this);
            break;
        default:
            break;
    }
}

void func_80AAAA24(EnMd* this) {
    if (this->unk1E0.unk_00 != 0) {
        switch (this->actor.textId) {
            case 0x102F:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                if ((this->unk208 == 2) && (this->unk20B != 2)) {
                    func_80AAA92C(this, 2);
                }
                if ((this->unk208 == 5) && (this->unk20B != 8)) {
                    func_80AAA92C(this, 8);
                }
                if ((this->unk208 == 0xB) && (this->unk20B != 9)) {
                    func_80AAA92C(this, 9);
                }
                break;
            case 0x1033:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                if ((this->unk208 == 1) && (this->unk20B != 2)) {
                    func_80AAA92C(this, 2);
                }
                if ((this->unk208 == 5) && (this->unk20B != 0xA)) {
                    func_80AAA92C(this, 0xA);
                }
                if ((this->unk208 == 7) && (this->unk20B != 9)) {
                    func_80AAA92C(this, 9);
                }
                break;
            case 0x1030:
            case 0x1034:
            case 0x1045:
                if ((this->unk208 == 0) && (this->unk20B != 1)) {
                    func_80AAA92C(this, 1);
                }
                break;
            case 0x1046:
                if ((this->unk208 == 0) && (this->unk20B != 6)) {
                    func_80AAA92C(this, 6);
                }
                break;
        }
    } else if (this->unk14C.animation != &gMidoHandsOnHipsIdleAnim) {
        Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_10);
        func_80AAA92C(this, 0);
    }
    func_80AAA93C(this);
}

s16 func_80AAAC78(Actor* thisx, PlayState* play) {
    EnMd* this = (EnMd*)thisx;
    s16 temp_v0;

    temp_v0 = Message_GetState(&play->msgCtx);
    if (((this->unk209 == TEXT_STATE_AWAITING_NEXT) || (this->unk209 == TEXT_STATE_EVENT) ||
         (this->unk209 == TEXT_STATE_CLOSING) || (this->unk209 == TEXT_STATE_DONE_HAS_NEXT)) &&
        (this->unk209 != temp_v0)) {
        this->unk208++;
    }
    this->unk209 = temp_v0;
    return temp_v0;
}

u16 func_80AAACF8(PlayState* play, EnMd* this) {
    u16 temp_v0;

    temp_v0 = Text_GetFaceReaction(play, 0x11);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    this->unk208 = 0;
    this->unk209 = TEXT_STATE_NONE;
    if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) {
        return 0x1045;
    }
    if (GET_EVENTCHKINF(EVENTCHKINF_04)) {
        return 0x1034;
    }
    if ((CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) == EQUIP_VALUE_SHIELD_DEKU) &&
        (CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == EQUIP_VALUE_SWORD_KOKIRI)) {
        return 0x1033;
    }
    if (GET_INFTABLE(INFTABLE_0C)) {
        return 0x1030;
    }
    return 0x102F;
}

u16 func_80AAADE0(PlayState* play, EnMd* this) {
    this->unk208 = 0;
    this->unk209 = TEXT_STATE_NONE;
    if (GET_EVENTCHKINF(EVENTCHKINF_40)) {
        return 0x1028;
    }
    return 0x1046;
}

u16 func_80AAAE14(PlayState* play, EnMd* this) {
    this->unk208 = 0;
    this->unk209 = TEXT_STATE_NONE;
    if (GET_EVENTCHKINF(EVENTCHKINF_48)) {
        if (GET_INFTABLE(INFTABLE_19)) {
            return 0x1071;
        } else {
            return 0x1070;
        }
    } else if (GET_EVENTCHKINF(EVENTCHKINF_0A)) {
        return 0x1068;
    } else if (GET_INFTABLE(INFTABLE_15)) {
        return 0x1061;
    } else {
        return 0x1060;
    }
}

u16 func_80AAAE94(PlayState* play, Actor* thisx) {
    EnMd* this = (EnMd*)thisx;

    switch (play->sceneNum) {
        case SCENE_SPOT04:
            return func_80AAACF8(play, this);
        case SCENE_KOKIRI_HOME4:
            return func_80AAADE0(play, this);
        case SCENE_SPOT10:
            return func_80AAAE14(play, this);
        default:
            return 0;
    }
}

s16 func_80AAAF04(PlayState* play, Actor* thisx) {
    switch (func_80AAAC78(thisx, play)) {
        case TEXT_STATE_NONE:
        case TEXT_STATE_DONE_HAS_NEXT:
        case TEXT_STATE_DONE_FADING:
        case TEXT_STATE_CHOICE:
        case TEXT_STATE_DONE:
        case TEXT_STATE_SONG_DEMO_DONE:
        case TEXT_STATE_8:
        case TEXT_STATE_9:
            return 1;

        case TEXT_STATE_CLOSING:
            switch (thisx->textId) {
                case 0x1028:
                    SET_EVENTCHKINF(EVENTCHKINF_0F);
                    break;
                case 0x102F:
                    SET_EVENTCHKINF(EVENTCHKINF_02);
                    SET_INFTABLE(INFTABLE_0C);
                    break;
                case 0x1060:
                    SET_INFTABLE(INFTABLE_15);
                    break;
                case 0x1070:
                    SET_INFTABLE(INFTABLE_19);
                    break;
                case 0x1033:
                case 0x1067:
                    return 2;
            }
            return 0;

        case TEXT_STATE_EVENT:
            if (Message_ShouldAdvance(play)) {
                return 2;
            }
            break;
    }
    return 1;
}

s32 func_80AAB03C(EnMd* this, PlayState* play) {
    if ((play->sceneNum == SCENE_SPOT04) && !GET_EVENTCHKINF(EVENTCHKINF_1C) && !GET_EVENTCHKINF(EVENTCHKINF_40)) {
        return 1;
    } else if ((play->sceneNum == SCENE_KOKIRI_HOME4) &&
               ((gSaveContext.eventChkInf[1] & 0x1000) || (gSaveContext.eventChkInf[4] & 1)) && !LINK_IS_ADULT) {
        return 1;
    } else if (play->sceneNum == SCENE_SPOT10) {
        return 1;
    } else {
        return 0;
    }
}

void func_80AAB0E0(EnMd* arg0) {
    if (DECR(arg0->unk20C) == 0) {
        arg0->unk20E += 1;
        if (arg0->unk20E >= 3) {
            arg0->unk20C = Rand_S16Offset(0x1E, 0x1E);
            arg0->unk20E = 0;
        }
    }
}

void func_80AAB158(EnMd* this, PlayState* play) {
    Player* temp_a2;
    s16 temp_v1;
    s16 temp_ft1;
    s16 var_v1_real;
    s16 var_a3_real;

    temp_a2 = GET_PLAYER(play);
    if (this->actor.xzDistToPlayer < 170.0f) {
        temp_ft1 = (f32)this->actor.yawTowardsPlayer - (f32)this->actor.shape.rot.y;
        temp_v1 = ABS(temp_ft1);
        if (temp_v1 <= func_800347E8(2)) {
            var_a3_real = 2;
            var_v1_real = 1;
        } else {
            var_a3_real = 1;
            var_v1_real = 1;
        }
    } else {
        var_a3_real = 1;
        var_v1_real = 0;
    }
    if (this->unk1E0.unk_00 != 0) {
        var_a3_real = 4;
    }
    if (this->unk190 == func_80AABD0C) {
        var_a3_real = 1;
        var_v1_real = 0;
    }
    if (this->unk190 == func_80AAB8F8) {
        var_a3_real = 4;
        var_v1_real = 1;
    }
    if ((play->csCtx.state != CS_STATE_IDLE) || (gDbgCamEnabled != 0)) {
        this->unk1E0.unk_18 = play->view.eye;
        this->unk1E0.unk_14 = 40.0f;
        var_a3_real = 2;
    } else {
        this->unk1E0.unk_18 = temp_a2->actor.world.pos;
        if (gSaveContext.linkAge > 0) {
            this->unk1E0.unk_14 = 0.0f;
        } else {
            this->unk1E0.unk_14 = -18.0f;
        }
    }
    func_80034A14(&this->actor, &this->unk1E0, 2, var_a3_real);
    if ((this->unk190 != func_80AABC10) && (var_v1_real != 0)) {
        func_800343CC(play, &this->actor, &this->unk1E0.unk_00, this->unk194.dim.radius + 30.0f, func_80AAAE94,
                      func_80AAAF04);
    }
}

s32 func_80AAB370(EnMd* this, PlayState* play) {
    Path* temp_a0;
    s32 pad;
    f32 temp_fa0;
    f32 temp_fa1;
    Vec3s* temp_v0;
    s32 temp_a2;

    temp_a2 = this->actor.params & 0xFF00;
    if (temp_a2 == 0xFF00) {
        return 0;
    }
    temp_a0 = &play->setupPathList[temp_a2 >> 8];
    temp_v0 = SEGMENTED_TO_VIRTUAL(temp_a0->points);
    temp_v0 += this->unk212;
    temp_fa0 = (f32)temp_v0->x - this->actor.world.pos.x;
    temp_fa1 = (f32)temp_v0->z - this->actor.world.pos.z;
    Math_SmoothStepToS(&this->actor.world.rot.y, (Math_FAtan2F(temp_fa0, temp_fa1) * 10430.378f), 4, 0xFA0, 1);
    if (((temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1)) < 100.0f) {
        this->unk212++;
        if (this->unk212 >= temp_a0->count) {
            this->unk212 = 0;
        }
        return 1;
    }
    return 0;
}

s32 func_80AAB4DC(EnMd* this, PlayState* play) {
    Path* temp_a1;
    s32 temp_v1;
    Vec3s* temp_a3;

    temp_v1 = this->actor.params & 0xFF00;
    if (temp_v1 == 0xFF00) {
        return 0;
    }
    temp_a1 = &play->setupPathList[temp_v1 >> 8];
    temp_a3 = SEGMENTED_TO_VIRTUAL(temp_a1->points);
    temp_a3 += temp_a1->count - 1;
    this->actor.world.pos.x = temp_a3->x;
    this->actor.world.pos.y = temp_a3->y;
    this->actor.world.pos.z = temp_a3->z;
    return 1;
}

void func_80AAB5A4(EnMd* this, PlayState* play) {
    f32 var_fv0;

    if (play->sceneNum != SCENE_KOKIRI_HOME4) {
        if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !GET_EVENTCHKINF(EVENTCHKINF_1C) &&
            (play->sceneNum == SCENE_SPOT04)) {
            var_fv0 = 100.0f;
        } else {
            var_fv0 = 400.0f;
        }
        this->unk210 = func_80034DD4(&this->actor, play, this->unk210, var_fv0);
        this->actor.shape.shadowAlpha = (u8)this->unk210;
    } else {
        this->unk210 = 255;
        this->actor.shape.shadowAlpha = (u8)this->unk210;
    }
}

void EnMd_Init(Actor* thisx, PlayState* play) {
    EnMd* this = (EnMd*)thisx;
    s32 pad;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(play, &this->unk14C, &gMidoSkel, NULL, this->unk258, this->unk2BE, ENMD_LIMB_MAX);
    Collider_InitCylinder(play, &this->unk194);
    Collider_SetCylinder(play, &this->unk194, &this->actor, &D_80AAC310);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, NULL, &D_80AAC33C);
    if (func_80AAB03C(this, play) == 0) {
        Actor_Kill(&this->actor);
        return;
    }
    Animation_ChangeByInfo(&this->unk14C, sAnimationInfo, ENMD_ANIM_0);
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = 6;
    this->unk210 = 255;
    Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_ELF, this->actor.world.pos.x,
                       this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, FAIRY_KOKIRI);
    if (((play->sceneNum == SCENE_SPOT04) && !GET_EVENTCHKINF(EVENTCHKINF_04)) ||
        ((play->sceneNum == SCENE_SPOT04) && GET_EVENTCHKINF(EVENTCHKINF_04) &&
         CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD)) ||
        ((play->sceneNum == SCENE_SPOT10) && !GET_EVENTCHKINF(EVENTCHKINF_0A))) {
        this->actor.home.pos = this->actor.world.pos;
        this->unk190 = func_80AAB948;
    } else {
        if (play->sceneNum != SCENE_KOKIRI_HOME4) {
            func_80AAB4DC(this, play);
        }
        this->unk190 = func_80AAB874;
    }
}

void EnMd_Destroy(Actor* thisx, PlayState* play) {
    EnMd* this = (EnMd*)thisx;

    Collider_DestroyCylinder(play, &this->unk194);
}

void func_80AAB874(EnMd* this, PlayState* play) {
    if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(play, this->unk214, this->unk236, ENMD_LIMB_MAX);
    } else if ((this->unk1E0.unk_00 == 0) && (this->unk20B != 7)) {
        func_80AAA92C(this, 7);
    }
    func_80AAAA24(this);
}

void func_80AAB8F8(EnMd* this, PlayState* play) {
    if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
        func_80034F54(play, this->unk214, this->unk236, ENMD_LIMB_MAX);
    }
    func_80AAA93C(this);
}

void func_80AAB948(EnMd* this, PlayState* play) {
    Player* sp2C;
    f32 temp_fv1;
    Player* sp24;
    s16 temp_v0_3;
    s32 temp_v0_4;

    sp2C = GET_PLAYER(play);
    sp24 = GET_PLAYER(play);
    func_80AAAA24(this);
    if (this->unk1E0.unk_00 == 0) {
        this->actor.shape.rot.y = this->actor.world.rot.y = this->actor.yawTowardsPlayer;
        temp_v0_3 = Math_Vec3f_Yaw(&this->actor.home.pos, &sp24->actor.world.pos);
        this->actor.world.pos.x = this->actor.home.pos.x;
        this->actor.world.pos.x += 60.0f * Math_SinS(temp_v0_3);
        this->actor.world.pos.z = this->actor.home.pos.z;
        this->actor.world.pos.z += 60.0f * Math_CosS(temp_v0_3);
        temp_fv1 = fabsf((f32)this->actor.yawTowardsPlayer - (f32)temp_v0_3) * 0.001f * 3.0f;
        this->unk14C.playSpeed = CLAMP(temp_fv1, 1.0f, 3.0f);
    }
    if (this->unk1E0.unk_00 == 2) {
        if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !GET_EVENTCHKINF(EVENTCHKINF_1C) &&
            (play->sceneNum == SCENE_SPOT04)) {
            play->msgCtx.msgMode = MSGMODE_PAUSED;
        }
        if (play->sceneNum == SCENE_SPOT04) {
            SET_EVENTCHKINF(EVENTCHKINF_04);
        }
        if (play->sceneNum == SCENE_SPOT10) {
            SET_EVENTCHKINF(EVENTCHKINF_0A);
        }
        func_80AAA92C(this, 3);
        func_80AAA93C(this);
        this->unk212 = 1;
        this->unk1E0.unk_00 = 0;
        this->unk190 = func_80AABD0C;
        this->actor.speedXZ = 1.5f;
    } else {
        if (this->unk14C.animation == &gMidoHandsOnHipsIdleAnim) {
            func_80034F54(play, this->unk214, this->unk236, ENMD_LIMB_MAX);
        }
        if ((this->unk1E0.unk_00 == 0) && (play->sceneNum == SCENE_SPOT10)) {
            if (sp2C->stateFlags2 & PLAYER_STATE2_24) {
                sp2C->stateFlags2 |= PLAYER_STATE2_25;
                sp2C->unk_6A8 = &this->actor;
                func_8010BD58(play, OCARINA_ACTION_CHECK_SARIA);
                this->unk190 = func_80AABC10;
            } else if (this->actor.xzDistToPlayer < (30.0f + (f32)this->unk194.dim.radius)) {
                sp2C->stateFlags2 |= PLAYER_STATE2_23;
            }
        }
    }
}

void func_80AABC10(EnMd* this, PlayState* play) {
    Player* temp_v0;

    temp_v0 = GET_PLAYER(play);
    if (play->msgCtx.ocarinaMode >= OCARINA_MODE_04) {
        this->unk190 = func_80AAB948;
        play->msgCtx.ocarinaMode = OCARINA_MODE_04;
    } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
        Audio_PlaySfxGeneral(0x4802U, &gSfxDefaultPos, 4U, &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                               &gSfxDefaultReverb);
        this->actor.textId = 0x1067;
        func_8002F2CC(&this->actor, play, (f32)this->unk194.dim.radius + 30.0f);
        this->unk190 = func_80AAB948;
        play->msgCtx.ocarinaMode = OCARINA_MODE_04;
    } else {
        temp_v0->stateFlags2 |= PLAYER_STATE2_23;
    }
}

void func_80AABD0C(EnMd* this, PlayState* play) {
    func_80034F54(play, this->unk214, this->unk236, ENMD_LIMB_MAX);
    func_80AAA93C(this);
    if ((func_80AAB370(this, play) == 0) || (this->unk212 != 0)) {
        this->actor.shape.rot = this->actor.world.rot;
    } else if (CHECK_QUEST_ITEM(QUEST_KOKIRI_EMERALD) && !GET_EVENTCHKINF(EVENTCHKINF_1C) &&
               (play->sceneNum == SCENE_SPOT04)) {
        Message_CloseTextbox(play);
        SET_EVENTCHKINF(EVENTCHKINF_1C);
        Actor_Kill(&this->actor);
    } else {
        func_80AAA92C(this, 0xB);
        this->unk14C.playSpeed = 0.0f;
        this->actor.speedXZ = 0.0f;
        this->actor.home.pos = this->actor.world.pos;
        this->unk190 = func_80AAB8F8;
    }
}

void EnMd_Update(Actor* thisx, PlayState* play) {
    EnMd* this = (EnMd*)thisx;
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->unk194);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->unk194.base);
    SkelAnime_Update(&this->unk14C);
    func_80AAB0E0(this);
    func_80AAB5A4(this, play);
    Actor_MoveForward(&this->actor);
    func_80AAB158(this, play);
    Actor_UpdateBgCheckInfo(play, &this->actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_2);
    this->unk190(this, play);
}

s32 func_80AABEF0(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnMd* this = (EnMd*)thisx;
    Vec3s sp2C;

    if (limbIndex == ENMD_LIMB_HEAD) {
        Matrix_Translate(1200.0f, 0.0f, 0.0f, 1U);
        sp2C = this->unk1E0.unk_08;
        Matrix_RotateX(BINANG_TO_RAD_ALT(sp2C.y), MTXMODE_APPLY);
        Matrix_RotateZ(BINANG_TO_RAD_ALT(sp2C.x), MTXMODE_APPLY);
        Matrix_Translate(-1200.0f, 0.0f, 0.0f, 1U);
    }
    if (limbIndex == ENMD_LIMB_TORSO) {
        sp2C = this->unk1E0.unk_0E;
        Matrix_RotateX(BINANG_TO_RAD_ALT(sp2C.x), MTXMODE_APPLY);
        Matrix_RotateY(BINANG_TO_RAD_ALT(sp2C.y), MTXMODE_APPLY);
    }
    if ((limbIndex == ENMD_LIMB_TORSO) || (limbIndex == ENMD_LIMB_LEFT_UPPER_ARM) ||
        (limbIndex == ENMD_LIMB_RIGHT_UPPER_ARM)) {
        rot->y += (Math_SinS(this->unk214[limbIndex]) * 200.0f);
        rot->z += (Math_CosS(this->unk236[limbIndex]) * 200.0f);
    }
    return 0;
}

void func_80AAC104(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx, Gfx** gfx) {
    EnMd* this = (EnMd*)thisx;
    Vec3f sp18;

    sp18 = D_80AAC498;
    if (limbIndex == ENMD_LIMB_HEAD) {
        Matrix_MultVec3f(&sp18, &this->actor.focus.pos);
    }
}

void EnMd_Draw(Actor* thisx, PlayState* play) {
    EnMd* this = (EnMd*)thisx;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_md.c", 1280);
    if (this->unk210 == 255) {
        gSPSegment(POLY_OPA_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80AAC4A4[this->unk20E]));
        func_80034BA0(play, &this->unk14C, func_80AABEF0, func_80AAC104, &this->actor, this->unk210);
    } else if (this->unk210 != 0) {
        gSPSegment(POLY_XLU_DISP++, 8, SEGMENTED_TO_VIRTUAL(D_80AAC4A4[this->unk20E]));
        func_80034CC4(play, &this->unk14C, func_80AABEF0, func_80AAC104, &this->actor, this->unk210);
    }
    CLOSE_DISPS(play->state.gfxCtx, "../z_en_md.c", 1317);
}
