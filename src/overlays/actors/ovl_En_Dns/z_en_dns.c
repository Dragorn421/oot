#include "z_en_dns.h"

#define FLAGS 0x00000009

#define THIS ((EnDns*)thisx)

void EnDns_Init(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Update(Actor* thisx, GlobalContext* globalCtx);
void EnDns_Draw(Actor* thisx, GlobalContext* globalCtx);

u32 EnDns_CheckPurchase_DekuNuts(EnDns* arg0);
u32 EnDns_CheckPurchase_DekuSticks(EnDns* arg0);
u32 EnDns_CheckPurchase_Generic(EnDns* arg0);
u32 EnDns_CheckPurchase_DekuSeeds(EnDns* arg0);
u32 EnDns_CheckPurchase_DekuShield(EnDns* arg0);
u32 EnDns_CheckPurchase_Bombs(EnDns* arg0);
u32 EnDns_CheckPurchase_Arrows(EnDns* arg0);
u32 EnDns_CheckPurchase_Potion(EnDns* arg0);
void EnDns_ConcludePurchase_Generic(EnDns* arg0);
void EnDns_ConcludePurchase_DekuNuts(EnDns* arg0);
void EnDns_ConcludePurchase_HeartPiece(EnDns* arg0);
void EnDns_ConcludePurchase_Bombs(EnDns* arg0);
void EnDns_ConcludePurchase_Arrows(EnDns* arg0);
void EnDns_ConcludePurchase_DekuSticksCapacity(EnDns* arg0);
void EnDns_ConcludePurchase_DekuNutsCapacity(EnDns* arg0);
void func_809EFB84(EnDns*, GlobalContext*);
void func_809EFBC8(EnDns*, GlobalContext*);
void func_809EFC9C(EnDns*, GlobalContext*);
void func_809EFEE8(EnDns*, GlobalContext*);
void func_809EFF50(EnDns*, GlobalContext*);
void func_809EFF98(EnDns*, GlobalContext*);
void func_809F008C(EnDns*, GlobalContext*);
void func_809F0100(EnDns*, GlobalContext*);
void func_809F017C(EnDns*, GlobalContext*);

typedef enum EnDnsUnk8Result {
    EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES,
    EN_DNS_UNK8RESULT_ALREADY_FULL,
    EN_DNS_UNK8RESULT_OK_ALT,
    EN_DNS_UNK8RESULT_CANT_BUY_RIGHT_NOW,
    EN_DNS_UNK8RESULT_OK
} EnDnsUnk8Result;

typedef struct EnDnsPurchaseInfo {
    /* 0x00 */ s16 price;
    /* 0x02 */ s16 unk_2; // unused
    /* 0x04 */ s32 gid;
    /* 0x08 */ u32 (*checkPurchase)(struct EnDns*);
    /* 0x0C */ void (*concludePurchase)(struct EnDns*);
} EnDnsPurchaseInfo;

typedef struct _struct_D_809F0538_0xC {
    /* 0x0 */ AnimationHeader* unk0;
    /* 0x4 */ u8 unk4;
    /* 0x8 */ f32 unk8;
} _struct_D_809F0538_0xC;

extern AnimationHeader D_60009A0;
extern SkeletonHeader D_60041A8;
extern AnimationHeader D_6004404;

const ActorInit En_Dns_InitVars = {
    ACTOR_EN_DNS,
    ACTORTYPE_BG,
    FLAGS,
    OBJECT_SHOPNUTS,
    sizeof(EnDns),
    (ActorFunc)EnDns_Init,
    (ActorFunc)EnDns_Destroy,
    (ActorFunc)EnDns_Update,
    (ActorFunc)EnDns_Draw,
};
static ColliderCylinderSrcAlt D_809F03E0 = {
    { 0xA, 0, 9, 0x39, 1 },
    { 0, { 0, 0, 0 }, { 0xFFCFFFFF, 0, 0 }, 0, 1, 1 },
    { 0x12, 0x20, 0, { 0, 0, 0 } },
};
static u16 D_809F040C[] = {
    0x10A0, // EN_DNS_TYPE_DEKU_NUTS
    0x10A1, // EN_DNS_TYPE_DEKU_STICKS
    0x10A2, // EN_DNS_TYPE_HEART_PIECE
    0x10CA, // EN_DNS_TYPE_DEKU_SEEDS
    0x10CB, // EN_DNS_TYPE_DEKU_SHIELD
    0x10CC, // EN_DNS_TYPE_BOMBS
    0x10CD, // EN_DNS_TYPE_ARROWS
    0x10CE, // EN_DNS_TYPE_RED_POTION
    0x10CF, // EN_DNS_TYPE_GREEN_POTION
    0x10DC, // EN_DNS_TYPE_DEKU_STICKS_CAPACITY
    0x10DD, // EN_DNS_TYPE_DEKU_NUTS_CAPACITY
};
static char* D_809F0424[] = {
    "デクの実売り            ", // EN_DNS_TYPE_DEKU_NUTS
    "デクの棒売り            ", // EN_DNS_TYPE_DEKU_STICKS
    "ハートの欠片売り        ", // EN_DNS_TYPE_HEART_PIECE
    "デクの種売り            ", // EN_DNS_TYPE_DEKU_SEEDS
    "デクの盾売り            ", // EN_DNS_TYPE_DEKU_SHIELD
    "バクダン売り            ", // EN_DNS_TYPE_BOMBS
    "矢売り                  ", // EN_DNS_TYPE_ARROWS
    "赤のくすり売り          ", // EN_DNS_TYPE_RED_POTION
    "緑のくすり売り          ", // EN_DNS_TYPE_GREEN_POTION
    "デクの棒持てる数を増やす", // EN_DNS_TYPE_DEKU_STICKS_CAPACITY
    "デクの実持てる数を増やす", // EN_DNS_TYPE_DEKU_NUTS_CAPACITY
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuNuts = {
    20, 5, GI_NUTS_5_2, EnDns_CheckPurchase_DekuNuts, EnDns_ConcludePurchase_DekuNuts,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuSticks = {
    15, 1, GI_STICKS_1, EnDns_CheckPurchase_DekuSticks, EnDns_ConcludePurchase_Generic,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoHeartPiece = {
    10, 1, GI_HEART_PIECE, EnDns_CheckPurchase_Generic, EnDns_ConcludePurchase_HeartPiece,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuSeeds = {
    40, 30, GI_SEEDS_30, EnDns_CheckPurchase_DekuSeeds, EnDns_ConcludePurchase_Generic,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuShield = {
    50, 1, GI_SHIELD_DEKU, EnDns_CheckPurchase_DekuShield, EnDns_ConcludePurchase_Generic,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoBombs = {
    40, 5, GI_BOMBS_5, EnDns_CheckPurchase_Bombs, EnDns_ConcludePurchase_Bombs,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoArrows = {
    70, 20, GI_ARROWS_LARGE, EnDns_CheckPurchase_Arrows, EnDns_ConcludePurchase_Arrows,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoRedPotion = {
    40, 1, GI_POTION_RED, EnDns_CheckPurchase_Potion, EnDns_ConcludePurchase_Generic,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoGreenPotion = {
    40, 1, GI_POTION_GREEN, EnDns_CheckPurchase_Potion, EnDns_ConcludePurchase_Generic,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuSticksCapacity = {
    40, 1, GI_STICK_UPGRADE_20, EnDns_CheckPurchase_Generic, EnDns_ConcludePurchase_DekuSticksCapacity,
};
static EnDnsPurchaseInfo sEnDnsPurchaseInfoDekuNutsCapacity = {
    40, 1, GI_NUT_UPGRADE_30, EnDns_CheckPurchase_Generic, EnDns_ConcludePurchase_DekuNutsCapacity,
};
static EnDnsPurchaseInfo* sEnDnsPurchaseInfos[] = {
    &sEnDnsPurchaseInfoDekuNuts,           // EN_DNS_TYPE_DEKU_NUTS
    &sEnDnsPurchaseInfoDekuSticks,         // EN_DNS_TYPE_DEKU_STICKS
    &sEnDnsPurchaseInfoHeartPiece,         // EN_DNS_TYPE_HEART_PIECE
    &sEnDnsPurchaseInfoDekuSeeds,          // EN_DNS_TYPE_DEKU_SEEDS
    &sEnDnsPurchaseInfoDekuShield,         // EN_DNS_TYPE_DEKU_SHIELD
    &sEnDnsPurchaseInfoBombs,              // EN_DNS_TYPE_BOMBS
    &sEnDnsPurchaseInfoArrows,             // EN_DNS_TYPE_ARROWS
    &sEnDnsPurchaseInfoRedPotion,          // EN_DNS_TYPE_RED_POTION
    &sEnDnsPurchaseInfoGreenPotion,        // EN_DNS_TYPE_GREEN_POTION
    &sEnDnsPurchaseInfoDekuSticksCapacity, // EN_DNS_TYPE_DEKU_STICKS_CAPACITY
    &sEnDnsPurchaseInfoDekuNutsCapacity,   // EN_DNS_TYPE_DEKU_NUTS_CAPACITY
};
static InitChainEntry D_809F052C[3] = {
    ICHAIN_S8(naviEnemyId, 78, ICHAIN_CONTINUE),
    ICHAIN_U8(unk_1F, 2, ICHAIN_CONTINUE),
    ICHAIN_F32(unk_4C, 30, ICHAIN_STOP),
};
static struct _struct_D_809F0538_0xC D_809F0538[3] = {
    { 0x06001108, 0, 0.0f },
    { 0x06004404, 2, 0.0f },
    { 0x060009A0, 2, 0.0f },
};

void EnDns_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* this = (EnDns*)thisx;

    if (this->actor.params < 0) {
        osSyncPrintf("\x1b[31m引数エラー（売りナッツ）[ arg_data = %d ]\x1b[m\n", this->actor.params);
        Actor_Kill(&this->actor);
        return;
    }
    if ((this->actor.params == EN_DNS_TYPE_ARROWS) && (LINK_AGE_IN_YEARS == 5)) {
        this->actor.params = EN_DNS_TYPE_DEKU_SEEDS;
    }
    osSyncPrintf("\x1b[32m◆◆◆ 売りナッツ『%s』 ◆◆◆\x1b[m\n", D_809F0424[this->actor.params]);
    Actor_ProcessInitChain(&this->actor, D_809F052C);
    SkelAnime_InitSV(globalCtx, &this->unk14C, &D_60041A8, &D_60009A0, this->unk190, this->unk1FC, 0x12);
    Collider_InitCylinder(globalCtx, &this->unk26C);
    Collider_LoadCylinderAlt(globalCtx, &this->unk26C, &this->actor, &D_809F03E0);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawFunc_Circle, 35.0f);
    this->actor.textId = D_809F040C[this->actor.params];
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.collideData.mass = MASS_IMMOVABLE;
    this->unk2BB = 1;
    this->unk2BC = 1;
    this->unk2BD = 0;
    this->actor.speedXZ = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = -1.0f;
    this->unk2C0 = sEnDnsPurchaseInfos[this->actor.params];
    this->unk268 = func_809EFB84;
}

void EnDns_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* this = (EnDns*)thisx;

    Collider_DestroyCylinder(globalCtx, &this->unk26C);
}

void func_809EF51C(EnDns* this, u8 arg1) {
    s16 temp_ft0;

    temp_ft0 = SkelAnime_GetFrameCount(&D_809F0538[arg1].unk0->genericHeader);
    this->unk2BA = arg1;
    SkelAnime_ChangeAnim(&this->unk14C, D_809F0538[arg1].unk0, 1.0f, 0.0f, temp_ft0, D_809F0538[arg1].unk4,
                         D_809F0538[arg1].unk8);
}

u32 EnDns_CheckPurchase_DekuNuts(EnDns* this) {
    if ((CUR_CAPACITY(UPG_NUTS) != 0) && (AMMO(ITEM_NUT) >= (s32)CUR_CAPACITY(UPG_NUTS))) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    if (Item_CheckObtainability(ITEM_NUT) == ITEM_NONE) {
        return EN_DNS_UNK8RESULT_OK_ALT;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_DekuSticks(EnDns* this) {
    if ((CUR_CAPACITY(UPG_STICKS) != 0) && (AMMO(ITEM_STICK) >= (s32)CUR_CAPACITY(UPG_STICKS))) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    if (Item_CheckObtainability(ITEM_STICK) == ITEM_NONE) {
        return EN_DNS_UNK8RESULT_OK_ALT;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_Generic(EnDns* this) {
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_DekuSeeds(EnDns* this) {
    if (INV_CONTENT(ITEM_SLINGSHOT) == ITEM_NONE) {
        return EN_DNS_UNK8RESULT_CANT_BUY_RIGHT_NOW;
    }
    if (AMMO(ITEM_SLINGSHOT) >= CUR_CAPACITY(UPG_BULLET_BAG)) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    if (Item_CheckObtainability(ITEM_SEEDS) == ITEM_NONE) {
        return EN_DNS_UNK8RESULT_OK_ALT;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_DekuShield(EnDns* this) {
    if (gBitFlags[4] & gSaveContext.inventory.equipment) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_Bombs(EnDns* this) {
    if (!CHECK_QUEST_ITEM(QUEST_GORON_RUBY)) {
        return EN_DNS_UNK8RESULT_CANT_BUY_RIGHT_NOW;
    }
    if (AMMO(ITEM_BOMB) >= CUR_CAPACITY(UPG_BOMB_BAG)) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_Arrows(EnDns* this) {
    if (Item_CheckObtainability(ITEM_BOW) == ITEM_NONE) {
        return EN_DNS_UNK8RESULT_CANT_BUY_RIGHT_NOW;
    }
    if (AMMO(ITEM_BOW) >= CUR_CAPACITY(UPG_QUIVER)) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    return EN_DNS_UNK8RESULT_OK;
}

u32 EnDns_CheckPurchase_Potion(EnDns* this) {
    if (!Inventory_HasEmptyBottle()) {
        return EN_DNS_UNK8RESULT_ALREADY_FULL;
    }
    if (gSaveContext.rupees < this->unk2C0->price) {
        return EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES;
    }
    return EN_DNS_UNK8RESULT_OK;
}

void EnDns_ConcludePurchase_Generic(EnDns* this) {
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_DekuNuts(EnDns* this) {
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_HeartPiece(EnDns* this) {
    gSaveContext.itemGetInf[0] |= 0x800;
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_Bombs(EnDns* this) {
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_Arrows(EnDns* this) {
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_DekuSticksCapacity(EnDns* this) {
    gSaveContext.infTable[0x19] |= 4;
    Rupees_ChangeBy(-this->unk2C0->price);
}

void EnDns_ConcludePurchase_DekuNutsCapacity(EnDns* this) {
    gSaveContext.infTable[0x19] |= 8;
    Rupees_ChangeBy(-this->unk2C0->price);
}

void func_809EFB84(EnDns* this, GlobalContext* globalCtx) {
    if (this->unk14C.animCurrentFrame == this->unk14C.animFrameCount) {
        this->unk268 = func_809EFBC8;
        func_809EF51C(this, 0);
    }
}

void func_809EFBC8(EnDns* this, GlobalContext* globalCtx) {
    Math_SmoothScaleMaxMinS(&this->actor.shape.rot.y, this->actor.yawTowardsLink, 3, 0x7D0, 0);
    this->actor.posRot.rot.y = this->actor.shape.rot.y;
    if (func_8002F194(&this->actor, globalCtx) != 0) {
        this->unk268 = func_809EFC9C;
    } else {
        if ((this->unk26C.base.ocFlags1 & 2) || (this->actor.unk_10C != 0)) {
            this->actor.flags |= 0x10000;
        } else {
            this->actor.flags &= ~0x10000;
        }
        if (this->actor.xzDistFromLink < 130.0f) {
            func_8002F2F4(&this->actor, globalCtx);
        }
    }
}

void func_809EFC9C(EnDns* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 4) && (func_80106BC8(globalCtx) != 0)) {
        switch (globalCtx->msgCtx.choiceIndex) {
            case 0:
                switch (this->unk2C0->checkPurchase(this)) {
                    case EN_DNS_UNK8RESULT_NOT_ENOUGH_RUPEES:
                        func_8010B720(globalCtx, 0x10A5);
                        this->unk268 = func_809F008C;
                        return;
                    case EN_DNS_UNK8RESULT_ALREADY_FULL:
                        func_8010B720(globalCtx, 0x10A6);
                        this->unk268 = func_809F008C;
                        return;
                    case EN_DNS_UNK8RESULT_CANT_BUY_RIGHT_NOW:
                        func_8010B720(globalCtx, 0x10DE);
                        this->unk268 = func_809F008C;
                        return;
                    case EN_DNS_UNK8RESULT_OK_ALT:
                    case EN_DNS_UNK8RESULT_OK:
                        func_8010B720(globalCtx, 0x10A7);
                        this->unk268 = func_809EFEE8;
                        return;
                }
                break;
            case 1:
                func_8010B720(globalCtx, 0x10A4);
                this->unk268 = func_809F008C;
                break;
        }
    }
}

void EnDns_OfferItem(EnDns* this, GlobalContext* globalCtx) {
    if (this->actor.params == EN_DNS_TYPE_DEKU_STICKS_CAPACITY) {
        if (CUR_UPG_VALUE(UPG_STICKS) < 2) {
            func_8002F434(&this->actor, globalCtx, GI_STICK_UPGRADE_20, 130.0f, 100.0f);
        } else {
            func_8002F434(&this->actor, globalCtx, GI_STICK_UPGRADE_30, 130.0f, 100.0f);
        }
    } else if (this->actor.params == EN_DNS_TYPE_DEKU_NUTS_CAPACITY) {
        if (CUR_UPG_VALUE(UPG_NUTS) < 2) {
            func_8002F434(&this->actor, globalCtx, GI_NUT_UPGRADE_30, 130.0f, 100.0f);
        } else {
            func_8002F434(&this->actor, globalCtx, GI_NUT_UPGRADE_40, 130.0f, 100.0f);
        }
    } else {
        func_8002F434(&this->actor, globalCtx, this->unk2C0->gid, 130.0f, 100.0f);
    }
}

void func_809EFEE8(EnDns* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 5) && (func_80106BC8(globalCtx) != 0)) {
        func_80106CCC(globalCtx);
        EnDns_OfferItem(this, globalCtx);
        this->unk268 = func_809EFF50;
    }
}

void func_809EFF50(EnDns* this, GlobalContext* globalCtx) {
    if (Actor_HasParent(&this->actor, globalCtx)) {
        this->actor.parent = NULL;
        this->unk268 = func_809EFF98;
    } else {
        EnDns_OfferItem(this, globalCtx);
    }
}

void func_809EFF98(EnDns* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;

    if (player->stateFlags1 & 0x400) {
        if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
            this->unk2C0->concludePurchase(this);
            this->unk2BD = 1;
            this->unk2BB = 0;
            this->actor.flags &= ~1;
            func_809EF51C(this, 1);
            this->unk268 = func_809F0100;
        }
    } else {
        this->unk2C0->concludePurchase(this);
        this->unk2BD = 1;
        this->unk2BB = 0;
        this->actor.flags &= ~1;
        func_809EF51C(this, 1);
        this->unk268 = func_809F0100;
    }
}

void func_809F008C(EnDns* this, GlobalContext* globalCtx) {
    if ((func_8010BDBC(&globalCtx->msgCtx) == 6) && (func_80106BC8(globalCtx) != 0)) {
        this->unk2BB = 0;
        this->actor.flags &= ~1;
        func_809EF51C(this, 1);
        this->unk268 = func_809F0100;
    }
}

void func_809F0100(EnDns* this, GlobalContext* globalCtx) {
    f32 f = SkelAnime_GetFrameCount(&D_6004404);

    if (this->unk14C.animCurrentFrame == f) {
        Audio_PlayActorSound2(&this->actor, NA_SE_EN_AKINDONUTS_HIDE);
        this->unk268 = func_809F017C;
        this->unk2BC = 0;
        this->unk2C4 = this->actor.posRot.pos.y;
    }
}

void func_809F017C(EnDns* this, GlobalContext* globalCtx) {
    f32 var_fv0;
    Vec3f sp38;
    s32 var_s0;

    var_fv0 = this->unk2C4 - this->actor.posRot.pos.y;
    if ((this->unk2B8 % 4) == 0) {
        sp38.x = this->actor.posRot.pos.x;
        sp38.y = this->unk2C4;
        sp38.z = this->actor.posRot.pos.z;
        func_80028990(globalCtx, 20.0f, &sp38);
    }
    this->actor.shape.rot.y += 0x2000;
    if (var_fv0 > 400.0f) {
        if (this->unk2BD != 0) {
            sp38.x = this->actor.posRot.pos.x;
            sp38.y = this->unk2C4;
            sp38.z = this->actor.posRot.pos.z;
            for (var_s0 = 0; var_s0 < 3; var_s0++) {
                Item_DropCollectible(globalCtx, &sp38, 3);
            }
        }
        Actor_Kill(&this->actor);
    }
}

void EnDns_Update(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* this = (EnDns*)thisx;
    s32 pad;

    this->unk2B8++;
    this->actor.textId = D_809F040C[this->actor.params];
    Actor_SetHeight(&this->actor, 60.0f);
    Actor_SetScale(&this->actor, 0.01f);
    SkelAnime_FrameUpdateMatrix(&this->unk14C);
    Actor_MoveForward(&this->actor);
    this->unk268(this, globalCtx);
    if (this->unk2BC != 0) {
        func_8002E4B4(globalCtx, &this->actor, 20.0f, 20.0f, 20.0f, 4);
    }
    if (this->unk2BB != 0) {
        Collider_UpdateCylinderShape(&this->actor, &this->unk26C);
        Collider_AddOC(globalCtx, &globalCtx->colliderCtx, &this->unk26C.base);
    }
}

void EnDns_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnDns* this = (EnDns*)thisx;

    func_80093D18(globalCtx->state.gfxCtx);
    SkelAnime_DrawSV(globalCtx, this->unk14C.skeleton, this->unk14C.limbDrawTbl, this->unk14C.dListCount, NULL, NULL,
                     &this->actor);
}
