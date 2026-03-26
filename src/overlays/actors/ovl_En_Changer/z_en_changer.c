#include "z_en_changer.h"

#define FLAGS 0x00000000

#define THIS ((EnChanger*)thisx)

void EnChanger_Init(Actor* thisx, GlobalContext* globalCtx);
void EnChanger_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnChanger_Update(Actor* thisx, GlobalContext* globalCtx);

void func_809D2CCC(EnChanger* this, GlobalContext* globalCtx);
void func_809D2D70(EnChanger* this, GlobalContext* globalCtx);
void func_809D2F74(EnChanger* this, GlobalContext* globalCtx);

const ActorInit En_Changer_InitVars = {
    ACTOR_EN_CHANGER,
    ACTORTYPE_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnChanger),
    (ActorFunc)EnChanger_Init,
    (ActorFunc)EnChanger_Destroy,
    (ActorFunc)EnChanger_Update,
    NULL,
};
static Vec3f sLeftChestPositions[6] = {
    { 0.0f, 0.0f, 0.0f },         { -100.0f, 20.0f, -245.0f },  { -100.0f, 20.0f, -685.0f },
    { -100.0f, 20.0f, -1125.0f }, { -100.0f, 20.0f, -1565.0f }, { -100.0f, 20.0f, -2005.0f },
};
static Vec3f sRightChestPositions[6] = {
    { 0.0f, 0.0f, 0.0f },        { 140.0f, 20.0f, -245.0f },  { 140.0f, 20.0f, -685.0f },
    { 140.0f, 20.0f, -1125.0f }, { 140.0f, 20.0f, -1565.0f }, { 140.0f, 20.0f, -2005.0f },
};
static s32 D_809D3130[6] = { 0, 0x72, 0x72, 0x73, 0x73, 0x74 };
static s32 D_809D3148[6] = { 0, 8, 8, 9, 9, 0xA };
static s32 sTreasureFlags[6] = { 0, 2, 4, 6, 8, 0xA };

void EnChanger_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void EnChanger_Init(Actor* thisx, GlobalContext* globalCtx2) {
    EnChanger* this = (EnChanger*)thisx;
    GlobalContext* globalCtx = globalCtx2;
    s16 sp6E_leftChestParams;
    s16 sp6C_rightChestParams;
    s16 sp6A_finalChestParams;
    s16 temp_v1_curRoomIndex;
    s16 sp66_rightChestThing;
    s16 sp64_leftChestThing;
    s32 new_var_someParams;

    temp_v1_curRoomIndex = globalCtx->roomCtx.curRoom.num - 1;
    if (temp_v1_curRoomIndex < 0) {
        temp_v1_curRoomIndex = 0;
    }
    if (Flags_GetTreasure(globalCtx, sTreasureFlags[temp_v1_curRoomIndex])) {
        this->unk168 = 1;
    }
    osSyncPrintf("\n\n");
    osSyncPrintf("\x1b[32m☆☆☆☆☆ 宝発生(部屋はどれ？) %d\n\x1b[m", globalCtx->roomCtx.curRoom.num);
    osSyncPrintf("\x1b[32m☆☆☆☆☆ ビットは？ \t     %x\n\x1b[m", globalCtx->actorCtx.flags.chest);
    osSyncPrintf("\x1b[32m☆☆☆☆☆ セーブＢＩＴは？     %x\n\x1b[m", sTreasureFlags[temp_v1_curRoomIndex]);
    osSyncPrintf("\x1b[32m☆☆☆☆☆ もう、ゾンビ？\t     %d\n\x1b[m", this->unk168);
    osSyncPrintf("\n\n");
    temp_v1_curRoomIndex = temp_v1_curRoomIndex * 2;
    if (globalCtx->roomCtx.curRoom.num >= 6) {
        sp6A_finalChestParams = (gSaveContext.itemGetInf[1] & 0x800) ? 0x4EA0 : 0x4EC0;
        sp6A_finalChestParams = sTreasureFlags[5] | sp6A_finalChestParams;
        this->unk158 = (ActorA*)Actor_SpawnAsChild(&globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_BOX, 20.0f,
                                                   20.0f, -2500.0f, 0, 0x7FFF, 0, sp6A_finalChestParams);
        if (this->unk158 != NULL) {
            if (this->unk168 != 0) {
                Flags_SetTreasure(globalCtx, sp6A_finalChestParams & 0x1F);
                Actor_Kill(&this->actor);
                return;
            }
            new_var_someParams = ((gSaveContext.itemGetInf[1] & 0x800) ? 0xB : 0xC) & 0xFF;
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_ETCETERA, 20.0f, 20.0f, -2500.0f, 0, 0, 0,
                        ((sTreasureFlags[5] & 0x1F) << 8) + new_var_someParams);
            osSyncPrintf("\x1b[33m☆☆☆☆☆ 中央宝発生(ＧＲＥＡＴ) ☆☆☆☆☆ %x\n\x1b[m", sp6A_finalChestParams);
            this->unk14C = func_809D2F74;
            return;
        }
    }
    sp6E_leftChestParams = (D_809D3130[globalCtx->roomCtx.curRoom.num] << 5) | 0x4000;
    this->unk15C_l = temp_v1_curRoomIndex;
    sp6E_leftChestParams |= temp_v1_curRoomIndex;
    this->unk160 = D_809D3130[globalCtx->roomCtx.curRoom.num];
    sp64_leftChestThing = D_809D3148[globalCtx->roomCtx.curRoom.num];
    sp6C_rightChestParams = temp_v1_curRoomIndex | 0x4E21;
    this->unk15E_r = temp_v1_curRoomIndex | 1;
    this->unk162 = 0x71;
    sp66_rightChestThing = 0xD;
    if (Math_Rand_ZeroFloat(1.99f) < 1.0f) {
        sp6C_rightChestParams = (D_809D3130[globalCtx->roomCtx.curRoom.num] << 5) | 0x4000;
        this->unk15E_r = temp_v1_curRoomIndex;
        this->unk162 = D_809D3130[globalCtx->roomCtx.curRoom.num];
        sp66_rightChestThing = D_809D3148[globalCtx->roomCtx.curRoom.num];
        sp6E_leftChestParams = temp_v1_curRoomIndex | 0x4E21;
        sp6C_rightChestParams |= temp_v1_curRoomIndex;
        this->unk15C_l = temp_v1_curRoomIndex | 1;
        this->unk160 = 0x71;
        sp64_leftChestThing = 0xD;
    }
    this->unk150 = (ActorA*)Actor_SpawnAsChild(
        &globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_BOX,
        sLeftChestPositions[globalCtx->roomCtx.curRoom.num].x, sLeftChestPositions[globalCtx->roomCtx.curRoom.num].y,
        sLeftChestPositions[globalCtx->roomCtx.curRoom.num].z, 0, -0x3FFF, 0, sp6E_leftChestParams);
    if (this->unk150 != NULL) {
        osSyncPrintf("\x1b[35m☆☆☆☆☆ 左宝発生(ナニがはいってるの？) ☆☆☆☆☆ %x\n\x1b[m", sp6E_leftChestParams);
        osSyncPrintf("\x1b[35m☆☆☆☆☆ 部屋番号は？  %x\n\x1b[m", globalCtx->roomCtx.curRoom.num);
        osSyncPrintf("\x1b[35m☆☆☆☆☆ ビットはなぁに？  %x\n\x1b[m", this->unk15E_r);
        osSyncPrintf("\x1b[35m☆☆☆☆☆ すけすけ君？ %x\n\x1b[m", sp66_rightChestThing);
        osSyncPrintf("\n\n");
        if (this->unk168 != 0) {
            Flags_SetTreasure(globalCtx, this->unk15C_l & 0x1F);
        } else {
            Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_ETCETERA,
                        sLeftChestPositions[globalCtx->roomCtx.curRoom.num].x,
                        sLeftChestPositions[globalCtx->roomCtx.curRoom.num].y,
                        sLeftChestPositions[globalCtx->roomCtx.curRoom.num].z, 0, 0, 0,
                        ((this->unk15C_l & 0x1F) << 8) + (sp64_leftChestThing & 0xFF));
        }
    }
    this->unk154 = (ActorA*)Actor_SpawnAsChild(
        &globalCtx->actorCtx, &this->actor, globalCtx, ACTOR_EN_BOX,
        sRightChestPositions[globalCtx->roomCtx.curRoom.num].x, sRightChestPositions[globalCtx->roomCtx.curRoom.num].y,
        sRightChestPositions[globalCtx->roomCtx.curRoom.num].z, 0, 0x3FFF, 0, sp6C_rightChestParams);
    if (this->unk154 != NULL) {
        osSyncPrintf("\x1b[36m☆☆☆☆☆ 右宝発生(ナニがはいってるの？) ☆☆☆☆☆ %x\n\x1b[m", sp6C_rightChestParams);
        osSyncPrintf("\x1b[36m☆☆☆☆☆ 部屋番号は？  %d\n\x1b[m", globalCtx->roomCtx.curRoom.num);
        osSyncPrintf("\x1b[36m☆☆☆☆☆ ビットはなぁに？  %x\n\x1b[m", this->unk15C_l);
        osSyncPrintf("\x1b[36m☆☆☆☆☆ すけすけ君？ %x\n\x1b[m", sp64_leftChestThing);
        osSyncPrintf("\n\n");
        if (this->unk168 != 0) {
            Flags_SetTreasure(globalCtx, this->unk15E_r & 0x1F);
            Actor_Kill(&this->actor);
            return;
        }
        Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_ITEM_ETCETERA,
                    sRightChestPositions[globalCtx->roomCtx.curRoom.num].x,
                    sRightChestPositions[globalCtx->roomCtx.curRoom.num].y,
                    sRightChestPositions[globalCtx->roomCtx.curRoom.num].z, 0, 0, 0,
                    ((this->unk15E_r & 0x1F) << 8) + (sp66_rightChestThing & 0xFF));
    }
    this->actor.flags &= ~1;
    this->unk14C = func_809D2CCC;
}

void func_809D2CCC(EnChanger* this, GlobalContext* globalCtx) {
    if (this->unk150->unk1F4 != 0) {
        this->unk166 = 0x50;
        Flags_SetTreasure(globalCtx, this->unk15E_r & 0x1F);
        this->unk14C = func_809D2D70;
    } else if (this->unk154->unk1F4 != 0) {
        this->unk164 = 1;
        this->unk166 = 0x50;
        Flags_SetTreasure(globalCtx, this->unk15C_l & 0x1F);
        this->unk14C = func_809D2D70;
    }
}

void func_809D2D70(EnChanger* this, GlobalContext* globalCtx) {
    Vec3f pos;
    ActorA* temp_v0;
    ActorA* temp_v1;
    s16 temp_s0;

    temp_v0 = this->unk150;
    temp_v1 = this->unk154;
    if (this->unk166 == 0) {
        switch (this->unk164) {
            case 0:
                pos.x = temp_v1->actor.posRot.pos.x;
                pos.y = temp_v1->actor.posRot.pos.y;
                pos.z = temp_v1->actor.posRot.pos.z;
                if (this->unk162 == 0x71) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_EX_ITEM, pos.x, pos.y, pos.z, 0, 0, 0, 0xF);
                    Flags_SetSwitch(globalCtx, 0x32);
                } else {
                    temp_s0 = (s16)(this->unk162 - 0x72) + 0xA;
                    osSyncPrintf("\x1b[32m☆☆☆☆☆ 右宝開く ☆☆☆☆☆ %d\n\x1b[m", temp_s0);
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_EX_ITEM, pos.x, pos.y, pos.z, 0, 0, 0,
                                temp_s0);
                }
                break;
            case 1:
                pos.x = temp_v0->actor.posRot.pos.x;
                pos.y = temp_v0->actor.posRot.pos.y;
                pos.z = temp_v0->actor.posRot.pos.z;
                if (this->unk160 == 0x71) {
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_EX_ITEM, pos.x, pos.y, pos.z, 0, 0, 0, 0xF);
                    Flags_SetSwitch(globalCtx, 0x32);
                } else {
                    temp_s0 = (s16)(this->unk160 - 0x72) + 0xA;
                    osSyncPrintf("\x1b[32m☆☆☆☆☆ 左宝開く ☆☆☆☆☆ %d\n\x1b[m", temp_s0);
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_EX_ITEM, pos.x, pos.y, pos.z, 0, 0, 0,
                                temp_s0);
                }
                break;
        }
        Actor_Kill(&this->actor);
    }
}

void func_809D2F74(EnChanger* this, GlobalContext* globalCtx) {
    if (this->unk158->unk1F4 != 0) {
        if (!(gSaveContext.itemGetInf[1] & 0x800)) {
            gSaveContext.itemGetInf[1] |= 0x800;
        }
        Actor_Kill(&this->actor);
    }
}

void EnChanger_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnChanger* this = (EnChanger*)thisx;

    this->unk14C(this, globalCtx);
    if (this->unk166 != 0) {
        this->unk166--;
    }
    if (gGameInfo->data[0x960] != 0) {
        DebugDisplay_AddObject(this->actor.posRot.pos.x, this->actor.posRot.pos.y, this->actor.posRot.pos.z,
                               this->actor.posRot.rot.x, this->actor.posRot.rot.y, this->actor.posRot.rot.z, 1.0f, 1.0f,
                               1.0f, 255, 0, 255, 255, 4, globalCtx->state.gfxCtx);
    }
}
