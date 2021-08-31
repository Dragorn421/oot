#include "z_en_bom_bowl_pit.h"
#include "overlays/actors/ovl_En_Bom_Chu/z_en_bom_chu.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"
#include "z64camera.h"

#define FLAGS 0x00000010

#define THIS ((EnBomBowlPit*)thisx)

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

    if (globalCtx->cameraPtrs[MAIN_CAM]->setting == CAM_SET_FIXED1) {
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
                this->unk152 = Gameplay_CreateSubCamera(globalCtx);
                Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_WAIT);
                Gameplay_ChangeCameraStatus(globalCtx, this->unk152, CAM_STAT_ACTIVE);
                this->unk1C8.x = this->unk1C8.y = this->unk1C8.z = 0.1f;
                this->unk1A4.x = this->unk1A4.y = this->unk1A4.z = 0.1f;
                this->unk180.x = this->unk168.x = globalCtx->view.lookAt.x;
                this->unk180.y = this->unk168.y = globalCtx->view.lookAt.y;
                this->unk180.z = this->unk168.z = globalCtx->view.lookAt.z;
                this->unk18C.x = this->unk174.x = globalCtx->view.eye.x;
                this->unk18C.y = this->unk174.y = globalCtx->view.eye.y;
                this->unk18C.z = this->unk174.z = globalCtx->view.eye.z;
                this->unk1BC.x = 20.0f;
                this->unk1BC.y = 100.0f;
                this->unk1BC.z = -800.0f;
                this->unk198.x = 20.0f;
                this->unk198.y = 50.0f;
                this->unk198.z = -485.0f;
                this->unk1B0.x = fabsf(this->unk18C.x - 20.0f) * 0.02f;
                this->unk1B0.y = fabsf(this->unk18C.y - 50.0f) * 0.02f;
                this->unk1B0.z = fabsf(this->unk18C.z - -485.0f) * 0.02f;
                this->unk1D4.x = fabsf(this->unk180.x - this->unk1BC.x) * 0.02f;
                this->unk1D4.y = fabsf(this->unk180.y - this->unk1BC.y) * 0.02f;
                this->unk1D4.z = fabsf(this->unk180.z - this->unk1BC.z) * 0.02f;
                Gameplay_CameraSetAtEye(globalCtx, this->unk152, &this->unk180, &this->unk18C);
                this->actor.textId = 0xF;
                func_8010B680(globalCtx, this->actor.textId, NULL);
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
    if (this->unk152 != SUBCAM_FREE) {
        Math_ApproachF(&this->unk180.x, this->unk1BC.x, this->unk1C8.x, this->unk1D4.x);
        Math_ApproachF(&this->unk180.y, this->unk1BC.y, this->unk1C8.y, this->unk1D4.y);
        Math_ApproachF(&this->unk180.z, this->unk1BC.z, this->unk1C8.z, this->unk1D4.z);
        Math_ApproachF(&this->unk18C.x, this->unk198.x, this->unk1A4.x, this->unk1B0.x);
        Math_ApproachF(&this->unk18C.y, this->unk198.y, this->unk1A4.y, this->unk1B0.y);
        Math_ApproachF(&this->unk18C.z, this->unk198.z, this->unk1A4.z, this->unk1B0.z);
    }
    Gameplay_CameraSetAtEye(globalCtx, this->unk152, &this->unk180, &this->unk18C);
    if ((this->unk154 == func_8010BDBC(&globalCtx->msgCtx)) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
    }
    if ((fabsf(this->unk18C.x - this->unk198.x) < 5.0f) && (fabsf(this->unk18C.y - this->unk198.y) < 5.0f) &&
        (fabsf(this->unk18C.z - this->unk198.z) < 5.0f) && (fabsf(this->unk180.x - this->unk1BC.x) < 5.0f) &&
        (fabsf(this->unk180.y - this->unk1BC.y) < 5.0f) && (fabsf(this->unk180.z - this->unk1BC.z) < 5.0f)) {
        func_80106CCC(globalCtx);
        this->unk158 = 0x1E;
        this->unk14C = func_809C5360;
    }
}

void func_809C5360(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk158 == 0) {
        this->unk1E0 = Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_EX_ITEM,
                                          this->actor.world.pos.x, this->actor.world.pos.y,
                                          this->actor.world.pos.z - 70.0f, 0, 0, 0, this->unk15A);
        if (this->unk1E0 != NULL) {
            this->unk14C = func_809C53F0;
        }
    }
}

void func_809C53F0(EnBomBowlPit* this, GlobalContext* globalCtx) {
    if (this->unk156 != 0) {
        switch (this->unk15A) {
            case 0:
                gSaveContext.itemGetInf[1] |= 2;
                break;
            case EXITEM_HEART_PIECE_BOWLING:
                gSaveContext.itemGetInf[1] |= 4;
                break;
        }
        Gameplay_ClearCamera(globalCtx, this->unk152);
        Gameplay_ChangeCameraStatus(globalCtx, MAIN_CAM, CAM_STAT_ACTIVE);
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
    player->stateFlags1 &= ~0x20000000;
    this->actor.parent = NULL;
    func_8002F434(&this->actor, globalCtx, this->unk160, 2000.0f, 1000.0f);
    player->stateFlags1 |= 0x20000000;
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
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
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
