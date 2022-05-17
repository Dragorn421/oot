#include "z_en_bom_bowl_pit.h"
#include "overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
#include "z64camera.h"

#define FLAGS ACTOR_FLAG_4

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx);

void func_809C4E60(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C4E8C(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C5184(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C5360(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C53F0(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C54A8(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C55B0(EnBomBowlPit* this, GlobalContext* globalCtx);
void func_809C5608(EnBomBowlPit* this, GlobalContext* globalCtx);

static s32 D_809C56E0[5] = {
    GI_BOMB_BAG_30, GI_HEART_PIECE, GI_BOMBCHUS_10, GI_BOMBS_1, GI_RUPEE_PURPLE,
};

const ActorInit En_Bom_Bowl_Pit_InitVars = {
    ACTOR_EN_BOM_BOWL_PIT,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBomBowlPit),
    (ActorFunc)EnBomBowlPit_Init,
    (ActorFunc)EnBomBowlPit_Destroy,
    (ActorFunc)EnBomBowlPit_Update,
    NULL,
};

void EnBomBowlPit_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* this = (EnBomBowlPit*)thisx;

    this->unk14C = func_809C4E60;
}

void EnBomBowlPit_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void func_809C4E60(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk15C != 0) {
        this->unk15C = this->unk164 = 0;
        this->unk14C = func_809C4E8C;
    }
}

void func_809C4E8C(EnBomBowlPit* this, GlobalContext* globalCtx) {
    Actor* var_v1;
    EnBomChu* sp24;
    Vec3f diff;
    Actor* thisx = &this->actor;

    if (globalCtx->cameraPtrs[CAM_ID_MAIN]->setting == CAM_SET_CHU_BOWLING) {
        var_v1 = globalCtx->actorCtx.actorLists[3].head;
        while (var_v1 != NULL) {
            if ((var_v1 == thisx) || (var_v1->id != ACTOR_EN_BOM_CHU)) {
                var_v1 = var_v1->next;
                continue;
            }
            diff.x = var_v1->world.pos.x - this->actor.world.pos.x;
            diff.y = var_v1->world.pos.y - this->actor.world.pos.y;
            diff.z = var_v1->world.pos.z - this->actor.world.pos.z;
            if (((fabsf(diff.x) < 40.0f) || (gGameInfo->data[0x962] != 0)) &&
                ((fabsf(diff.y) < 40.0f) || (gGameInfo->data[0x962] != 0)) &&
                ((fabsf(diff.z) < 40.0f) || (gGameInfo->data[0x962] != 0))) {
                sp24 = (EnBomChu*)var_v1;
                func_8002DF54(globalCtx, NULL, 8U);
                sp24->timer = 1;
                this->subCamId = Play_CreateSubCamera(globalCtx);
                Play_ChangeCameraStatus(globalCtx, CAM_ID_MAIN, CAM_STAT_WAIT);
                Play_ChangeCameraStatus(globalCtx, this->subCamId, CAM_STAT_ACTIVE);
                this->subCamAtMaxVelFrac.x = this->subCamAtMaxVelFrac.y = this->subCamAtMaxVelFrac.z = 0.1f;
                this->subCamEyeMaxVelFrac.x = this->subCamEyeMaxVelFrac.y = this->subCamEyeMaxVelFrac.z = 0.1f;
                this->subCamAt.x = this->viewAt.x = globalCtx->view.at.x;
                this->subCamAt.y = this->viewAt.y = globalCtx->view.at.y;
                this->subCamAt.z = this->viewAt.z = globalCtx->view.at.z;
                this->subCamEye.x = this->viewEye.x = globalCtx->view.eye.x;
                this->subCamEye.y = this->viewEye.y = globalCtx->view.eye.y;
                this->subCamEye.z = this->viewEye.z = globalCtx->view.eye.z;
                this->subCamAtNext.x = 20.0f;
                this->subCamAtNext.y = 100.0f;
                this->subCamAtNext.z = -800.0f;
                this->subCamEyeNext.x = 20.0f;
                this->subCamEyeNext.y = 50.0f;
                this->subCamEyeNext.z = -485.0f;
                this->subCamEyeVel.x = fabsf(this->subCamEye.x - 20.0f) * 0.02f;
                this->subCamEyeVel.y = fabsf(this->subCamEye.y - 50.0f) * 0.02f;
                this->subCamEyeVel.z = fabsf(this->subCamEye.z - -485.0f) * 0.02f;
                this->subCamAtVel.x = fabsf(this->subCamAt.x - this->subCamAtNext.x) * 0.02f;
                this->subCamAtVel.y = fabsf(this->subCamAt.y - this->subCamAtNext.y) * 0.02f;
                this->subCamAtVel.z = fabsf(this->subCamAt.z - this->subCamAtNext.z) * 0.02f;
                Play_CameraSetAtEye(globalCtx, this->subCamId, &this->subCamAt, &this->subCamEye);
                this->actor.textId = 0xF;
                Message_StartTextbox(globalCtx, this->actor.textId, NULL);
                this->unk154 = 5;
                func_80078884(0x28D3U);
                func_8002DF54(globalCtx, NULL, 8U);
                this->unk164 = 1;
                this->unk14C = func_809C5184;
                return;
            }
            var_v1 = var_v1->next;
        }
    }
}

void func_809C5184(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->subCamId != SUB_CAM_ID_DONE) {
        Math_ApproachF(&this->subCamAt.x, this->subCamAtNext.x, this->subCamAtMaxVelFrac.x, this->subCamAtVel.x);
        Math_ApproachF(&this->subCamAt.y, this->subCamAtNext.y, this->subCamAtMaxVelFrac.y, this->subCamAtVel.y);
        Math_ApproachF(&this->subCamAt.z, this->subCamAtNext.z, this->subCamAtMaxVelFrac.z, this->subCamAtVel.z);
        Math_ApproachF(&this->subCamEye.x, this->subCamEyeNext.x, this->subCamEyeMaxVelFrac.x, this->subCamEyeVel.x);
        Math_ApproachF(&this->subCamEye.y, this->subCamEyeNext.y, this->subCamEyeMaxVelFrac.y, this->subCamEyeVel.y);
        Math_ApproachF(&this->subCamEye.z, this->subCamEyeNext.z, this->subCamEyeMaxVelFrac.z, this->subCamEyeVel.z);
    }
    Play_CameraSetAtEye(globalCtx, this->subCamId, &this->subCamAt, &this->subCamEye);
    if ((this->unk154 == Message_GetState(&globalCtx->msgCtx)) && Message_ShouldAdvance(globalCtx)) {
        Message_CloseTextbox(globalCtx);
    }
    if ((fabsf(this->subCamEye.x - this->subCamEyeNext.x) < 5.0f) &&
        (fabsf(this->subCamEye.y - this->subCamEyeNext.y) < 5.0f) &&
        (fabsf(this->subCamEye.z - this->subCamEyeNext.z) < 5.0f) &&
        (fabsf(this->subCamAt.x - this->subCamAtNext.x) < 5.0f) &&
        (fabsf(this->subCamAt.y - this->subCamAtNext.y) < 5.0f) &&
        (fabsf(this->subCamAt.z - this->subCamAtNext.z) < 5.0f)) {
        Message_CloseTextbox(globalCtx);
        this->unk158 = 0x1E;
        this->unk14C = func_809C5360;
    }
}

void func_809C5360(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk158 == 0) {
        this->unk1E0 =
            Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_EX_ITEM, this->actor.world.pos.x,
                               this->actor.world.pos.y, this->actor.world.pos.z - 70.0f, 0, 0, 0, this->unk15A);
        if (this->unk1E0 != NULL) {
            this->unk14C = func_809C53F0;
        }
    }
}

void func_809C53F0(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk156 != 0) {
        switch (this->unk15A) {
            case 0:
                SET_ITEMGETINF(ITEMGETINF_11);
                break;
            case EXITEM_HEART_PIECE_BOWLING:
                SET_ITEMGETINF(ITEMGETINF_12);
                break;
        }
        Play_ClearCamera(globalCtx, this->subCamId);
        Play_ChangeCameraStatus(globalCtx, CAM_ID_MAIN, CAM_STAT_ACTIVE);
        func_8002DF54(globalCtx, NULL, 8U);
        this->unk14C = func_809C54A8;
    }
}

void func_809C54A8(EnBomBowlPit* this, GlobalContext* globalCtx) {
    Player* player = GET_PLAYER(globalCtx);

    func_8002DF54(globalCtx, NULL, 7U);
    this->unk160 = D_809C56E0[this->unk15A];
    if ((this->unk160 == GI_BOMB_BAG_30) && (CUR_CAPACITY(1) == 30)) {
        this->unk160 = GI_BOMB_BAG_40;
    }
    player->stateFlags1 &= ~PLAYER_STATE1_29;
    this->actor.parent = NULL;
    func_8002F434(&this->actor, globalCtx, this->unk160, 2000.0f, 1000.0f);
    player->stateFlags1 |= PLAYER_STATE1_29;
    this->unk14C = func_809C55B0;
}

void func_809C55B0(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (Actor_HasParent(&this->actor, globalCtx)) {
        this->unk14C = func_809C5608;
        return;
    }
    func_8002F434(&this->actor, globalCtx, this->unk160, 2000.0f, 1000.0f);
}

void func_809C5608(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if ((Message_GetState(&globalCtx->msgCtx) == TEXT_STATE_DONE) && Message_ShouldAdvance(globalCtx)) {
        osSyncPrintf("\x1b[32m☆☆☆☆☆ 正常終了 ☆☆☆☆☆ \n\x1b[m");
        if (this->unk160 == GI_HEART_PIECE) {
            gSaveContext.healthAccumulator = 0x140;
            osSyncPrintf("\x1b[32m☆☆☆☆☆ あぁ回復！ ☆☆☆☆☆ \n\x1b[m");
        }
        this->unk156 = 0;
        this->unk164 = 2;
        this->unk14C = func_809C4E60;
    }
}

void EnBomBowlPit_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnBomBowlPit* this = (EnBomBowlPit*)thisx;

    this->unk14C(this, globalCtx);
    if (this->unk158 != 0) {
        this->unk158--;
    }
}
