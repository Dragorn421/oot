#include "global.h"
#include "vt.h"
#include "assets/textures/parameter_static/parameter_static.h"
#include "assets/textures/do_action_static/do_action_static.h"
#include "assets/textures/icon_item_static/icon_item_static.h"

// TODO extract this information from the texture definitions themselves
#define DO_ACTION_TEX_WIDTH 48
#define DO_ACTION_TEX_HEIGHT 16
#define DO_ACTION_TEX_SIZE ((DO_ACTION_TEX_WIDTH * DO_ACTION_TEX_HEIGHT) / 2) // (sizeof(gCheckDoActionENGTex))

typedef struct {
    /* 0x00 */ u8 sceneId;
    /* 0x01 */ u8 flags1;
    /* 0x02 */ u8 flags2;
    /* 0x03 */ u8 flags3;
} RestrictionFlags; // size = 0x4

static RestrictionFlags sRestrictionFlags[] = {
    { SCENE_SPOT00, 0x00, 0x00, 0x10 },
    { SCENE_SPOT01, 0x00, 0x00, 0x10 },
    { SCENE_SPOT02, 0x00, 0x00, 0x10 },
    { SCENE_SPOT03, 0x00, 0x00, 0x10 },
    { SCENE_SPOT04, 0x00, 0x00, 0x10 },
    { SCENE_SPOT05, 0x00, 0x00, 0x10 },
    { SCENE_SPOT06, 0x00, 0x00, 0x10 },
    { SCENE_SPOT07, 0x00, 0x00, 0x10 },
    { SCENE_SPOT08, 0x00, 0x00, 0x10 },
    { SCENE_SPOT09, 0x00, 0x00, 0x10 },
    { SCENE_SPOT10, 0x00, 0x00, 0x10 },
    { SCENE_SPOT11, 0x00, 0x00, 0x10 },
    { SCENE_SPOT12, 0x00, 0x00, 0x10 },
    { SCENE_SPOT13, 0x00, 0x00, 0x10 },
    { SCENE_SPOT15, 0x00, 0x00, 0x10 },
    { SCENE_GANON_TOU, 0x00, 0x00, 0x10 },
    { SCENE_SPOT16, 0x00, 0x00, 0x10 },
    { SCENE_SPOT17, 0x00, 0x00, 0x10 },
    { SCENE_SPOT18, 0x00, 0x00, 0x10 },
    { SCENE_SPOT20, 0x00, 0x00, 0x10 },
    { SCENE_TOKINOMA, 0x00, 0x10, 0x15 },
    { SCENE_KENJYANOMA, 0xA2, 0xAA, 0xAA },
    { SCENE_SYATEKIJYOU, 0x11, 0x55, 0x55 },
    { SCENE_HAIRAL_NIWA, 0x11, 0x55, 0x55 },
    { SCENE_HAIRAL_NIWA_N, 0x11, 0x55, 0x55 },
    { SCENE_HAKAANA, 0x00, 0x00, 0xD0 },
    { SCENE_HAKAANA2, 0x00, 0x00, 0xD0 },
    { SCENE_HAKAANA_OUKE, 0x00, 0x00, 0xD0 },
    { SCENE_DAIYOUSEI_IZUMI, 0x00, 0x00, 0x10 },
    { SCENE_YOUSEI_IZUMI_TATE, 0x00, 0x00, 0xD0 },
    { SCENE_YOUSEI_IZUMI_YOKO, 0x00, 0x00, 0x10 },
    { SCENE_GANON_FINAL, 0x00, 0x05, 0x50 },
    { SCENE_NAKANIWA, 0x00, 0x05, 0x54 },
    { SCENE_TURIBORI, 0x11, 0x55, 0x55 },
    { SCENE_BOWLING, 0x11, 0x55, 0x55 },
    { SCENE_SOUKO, 0x00, 0x10, 0x15 },
    { SCENE_MIHARIGOYA, 0x00, 0x10, 0x14 },
    { SCENE_MAHOUYA, 0x10, 0x15, 0x55 },
    { SCENE_TAKARAYA, 0x10, 0x15, 0x55 },
    { SCENE_KINSUTA, 0x00, 0x10, 0x15 },
    { SCENE_ENTRA, 0x00, 0x10, 0x15 },
    { SCENE_ENTRA_N, 0x00, 0x10, 0x15 },
    { SCENE_ENRUI, 0x00, 0x10, 0xD5 },
    { SCENE_MARKET_DAY, 0x00, 0x10, 0x15 },
    { SCENE_MARKET_NIGHT, 0x00, 0x10, 0x15 },
    { SCENE_MARKET_RUINS, 0x00, 0x10, 0xD5 },
    { SCENE_MARKET_ALLEY, 0x00, 0x10, 0x15 },
    { SCENE_MARKET_ALLEY_N, 0x00, 0x10, 0x15 },
    { SCENE_SHRINE, 0x00, 0x10, 0x15 },
    { SCENE_SHRINE_N, 0x00, 0x10, 0x15 },
    { SCENE_SHRINE_R, 0x00, 0x10, 0xD5 },
    { SCENE_LINK_HOME, 0x10, 0x10, 0x15 },
    { SCENE_KAKARIKO, 0x10, 0x10, 0x15 },
    { SCENE_KAKARIKO3, 0x10, 0x10, 0x15 },
    { SCENE_KOKIRI_HOME, 0x10, 0x10, 0x15 },
    { SCENE_KOKIRI_HOME3, 0x10, 0x10, 0x15 },
    { SCENE_KOKIRI_HOME4, 0x10, 0x10, 0x15 },
    { SCENE_KOKIRI_HOME5, 0x10, 0x10, 0x15 },
    { SCENE_MALON_STABLE, 0x10, 0x10, 0x15 },
    { SCENE_HUT, 0x10, 0x10, 0x15 },
    { SCENE_IMPA, 0x10, 0x10, 0x15 },
    { SCENE_LABO, 0x10, 0x10, 0x15 },
    { SCENE_HYLIA_LABO, 0x00, 0x10, 0x15 },
    { SCENE_TENT, 0x10, 0x10, 0x15 },
    { SCENE_SHOP1, 0x10, 0x10, 0x15 },
    { SCENE_KOKIRI_SHOP, 0x10, 0x10, 0x15 },
    { SCENE_GOLON, 0x10, 0x10, 0x15 },
    { SCENE_ZOORA, 0x10, 0x10, 0x15 },
    { SCENE_DRAG, 0x10, 0x10, 0x15 },
    { SCENE_ALLEY_SHOP, 0x10, 0x10, 0x15 },
    { SCENE_NIGHT_SHOP, 0x10, 0x10, 0x15 },
    { SCENE_FACE_SHOP, 0x10, 0x10, 0x15 },
    { SCENE_MEN, 0x00, 0x03, 0x10 },
    { SCENE_YDAN, 0x00, 0x00, 0x00 },
    { SCENE_YDAN_BOSS, 0x00, 0x45, 0x50 },
    { SCENE_DDAN, 0x00, 0x00, 0x00 },
    { SCENE_DDAN_BOSS, 0x00, 0x45, 0x50 },
    { SCENE_BDAN, 0x00, 0x00, 0x00 },
    { SCENE_BDAN_BOSS, 0x00, 0x45, 0x50 },
    { SCENE_BMORI1, 0x00, 0x00, 0x00 },
    { SCENE_MORIBOSSROOM, 0x00, 0x45, 0x50 },
    { SCENE_HAKADANCH, 0x00, 0x00, 0x00 },
    { SCENE_HAKADAN, 0x00, 0x00, 0x00 },
    { SCENE_HAKADAN_BS, 0x00, 0x45, 0x50 },
    { SCENE_HIDAN, 0x00, 0x00, 0x00 },
    { SCENE_FIRE_BS, 0x00, 0x45, 0x50 },
    { SCENE_MIZUSIN, 0x00, 0x00, 0x00 },
    { SCENE_MIZUSIN_BS, 0x00, 0x45, 0x50 },
    { SCENE_JYASINZOU, 0x00, 0x00, 0x00 },
    { SCENE_JYASINBOSS, 0x00, 0x45, 0x50 },
    { SCENE_GANON, 0x00, 0x00, 0x00 },
    { SCENE_GANON_BOSS, 0x00, 0x45, 0x50 },
    { SCENE_ICE_DOUKUTO, 0x00, 0x00, 0xC0 },
    { SCENE_HAKASITARELAY, 0x00, 0x03, 0x14 },
    { SCENE_GANONTIKA, 0x00, 0x03, 0x10 },
    { SCENE_GANON_DEMO, 0x00, 0x45, 0x50 },
    { SCENE_GANON_SONOGO, 0x00, 0x05, 0x50 },
    { SCENE_GANONTIKA_SONOGO, 0x00, 0x05, 0x50 },
    { SCENE_GERUDOWAY, 0x00, 0x00, 0x10 },
    { SCENE_KAKUSIANA, 0x00, 0x00, 0xD0 },
    { 0xFF, 0x00, 0x00, 0x00 },
};

static s16 sHBAScoreTier = 0;
static u16 sHBAScoreDigits[] = { 0, 0, 0, 0 };

static u16 sCUpInvisible = 0;
static u16 sCUpTimer = 0;

s16 gSpoilingItems[] = { ITEM_ODD_MUSHROOM, ITEM_FROG, ITEM_EYEDROPS };
s16 gSpoilingItemReverts[] = { ITEM_COJIRO, ITEM_PRESCRIPTION, ITEM_PRESCRIPTION };

static s16 sMagicBorderR = 255;
static s16 sMagicBorderG = 255;
static s16 sMagicBorderB = 255;

static s16 sExtraItemBases[] = {
    ITEM_STICK,   // ITEM_STICKS_5
    ITEM_STICK,   // ITEM_STICKS_10
    ITEM_NUT,     // ITEM_NUTS_5
    ITEM_NUT,     // ITEM_NUTS_10
    ITEM_BOMB,    // ITEM_BOMBS_5
    ITEM_BOMB,    // ITEM_BOMBS_10
    ITEM_BOMB,    // ITEM_BOMBS_20
    ITEM_BOMB,    // ITEM_BOMBS_30
    ITEM_BOW,     // ITEM_ARROWS_SMALL
    ITEM_BOW,     // ITEM_ARROWS_MEDIUM
    ITEM_BOW,     // ITEM_ARROWS_LARGE
    ITEM_SEEDS,   // ITEM_SEEDS_30
    ITEM_BOMBCHU, // ITEM_BOMBCHUS_5
    ITEM_BOMBCHU, // ITEM_BOMBCHUS_20
    ITEM_STICK,   // ITEM_STICK_UPGRADE_20
    ITEM_STICK,   // ITEM_STICK_UPGRADE_30
    ITEM_NUT,     // ITEM_NUT_UPGRADE_30
    ITEM_NUT,     // ITEM_NUT_UPGRADE_40
};

static s16 D_80125A58 = 0;
static s16 D_80125A5C = false;

static Gfx sSetupDL_80125A60[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_SHADING_SMOOTH | G_LOD),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_1PRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsSPEndDisplayList(),
};

// original name: "alpha_change"
void Interface_ChangeAlpha(u16 alphaType) {
    if (alphaType != GET_UNK_13EA) {
        osSyncPrintf("ＡＬＰＨＡーＴＹＰＥ＝%d  LAST_TIME_TYPE=%d\n", alphaType, GET_UNK_13EE);
        SET_UNK_13EA(SET_UNK_13E8(alphaType));
        SET_UNK_13EC(1);
    }
}

void func_80082644(PlayState* play, s16 alpha) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (GET_BUTTONSTATUS_A0(0) == BTN_DISABLED) {
        if (interfaceCtx->bAlpha != 70) {
            interfaceCtx->bAlpha = 70;
        }
    } else {
        if (interfaceCtx->bAlpha != 255) {
            interfaceCtx->bAlpha = alpha;
        }
    }

    if (GET_BUTTONSTATUS_A0(1) == BTN_DISABLED) {
        if (interfaceCtx->cLeftAlpha != 70) {
            interfaceCtx->cLeftAlpha = 70;
        }
    } else {
        if (interfaceCtx->cLeftAlpha != 255) {
            interfaceCtx->cLeftAlpha = alpha;
        }
    }

    if (GET_BUTTONSTATUS_A0(2) == BTN_DISABLED) {
        if (interfaceCtx->cDownAlpha != 70) {
            interfaceCtx->cDownAlpha = 70;
        }
    } else {
        if (interfaceCtx->cDownAlpha != 255) {
            interfaceCtx->cDownAlpha = alpha;
        }
    }

    if (GET_BUTTONSTATUS_A0(3) == BTN_DISABLED) {
        if (interfaceCtx->cRightAlpha != 70) {
            interfaceCtx->cRightAlpha = 70;
        }
    } else {
        if (interfaceCtx->cRightAlpha != 255) {
            interfaceCtx->cRightAlpha = alpha;
        }
    }

    if (GET_BUTTONSTATUS_A0(4) == BTN_DISABLED) {
        if (interfaceCtx->aAlpha != 70) {
            interfaceCtx->aAlpha = 70;
        }
    } else {
        if (interfaceCtx->aAlpha != 255) {
            interfaceCtx->aAlpha = alpha;
        }
    }
}

void func_8008277C(PlayState* play, s16 maxAlpha, s16 alpha) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (GET_UNK_13E7 != 0) {
        func_80082644(play, alpha);
        return;
    }

    if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
        interfaceCtx->bAlpha = maxAlpha;
    }

    if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
        interfaceCtx->aAlpha = maxAlpha;
    }

    if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
        interfaceCtx->cLeftAlpha = maxAlpha;
    }

    if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
        interfaceCtx->cDownAlpha = maxAlpha;
    }

    if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
        interfaceCtx->cRightAlpha = maxAlpha;
    }
}

void func_80082850(PlayState* play, s16 maxAlpha) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 alpha = 255 - maxAlpha;

    switch (GET_UNK_13E8) {
        case 1:
        case 2:
        case 8:
            osSyncPrintf("a_alpha=%d, c_alpha=%d   →   ", interfaceCtx->aAlpha, interfaceCtx->cLeftAlpha);

            if (GET_UNK_13E8 == 8) {
                if (interfaceCtx->bAlpha != 255) {
                    interfaceCtx->bAlpha = alpha;
                }
            } else {
                if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                    interfaceCtx->bAlpha = maxAlpha;
                }
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            osSyncPrintf("a_alpha=%d, c_alpha=%d\n", interfaceCtx->aAlpha, interfaceCtx->cLeftAlpha);

            break;
        case 3:
            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            func_8008277C(play, maxAlpha, alpha);

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            break;
        case 4:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            break;
        case 5:
            func_8008277C(play, maxAlpha, alpha);

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha;
            }

            break;
        case 6:
            func_8008277C(play, maxAlpha, alpha);

            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha;
            }

            switch (play->sceneId) {
                case SCENE_SPOT00:
                case SCENE_SPOT01:
                case SCENE_SPOT02:
                case SCENE_SPOT03:
                case SCENE_SPOT04:
                case SCENE_SPOT05:
                case SCENE_SPOT06:
                case SCENE_SPOT07:
                case SCENE_SPOT08:
                case SCENE_SPOT09:
                case SCENE_SPOT10:
                case SCENE_SPOT11:
                case SCENE_SPOT12:
                case SCENE_SPOT13:
                case SCENE_SPOT15:
                case SCENE_SPOT16:
                case SCENE_SPOT17:
                case SCENE_SPOT18:
                case SCENE_SPOT20:
                case SCENE_GANON_TOU:
                    if (interfaceCtx->minimapAlpha < 170) {
                        interfaceCtx->minimapAlpha = alpha;
                    } else {
                        interfaceCtx->minimapAlpha = 170;
                    }
                    break;
                default:
                    if (interfaceCtx->minimapAlpha != 255) {
                        interfaceCtx->minimapAlpha = alpha;
                    }
                    break;
            }
            break;
        case 7:
            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            func_80082644(play, alpha);

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha;
            }

            break;
        case 9:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha;
            }

            break;
        case 10:
            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if (interfaceCtx->bAlpha != 255) {
                interfaceCtx->bAlpha = alpha;
            }

            break;
        case 11:
            if ((interfaceCtx->bAlpha != 0) && (interfaceCtx->bAlpha > maxAlpha)) {
                interfaceCtx->bAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            break;
        case 12:
            if (interfaceCtx->aAlpha != 255) {
                interfaceCtx->aAlpha = alpha;
            }

            if (interfaceCtx->bAlpha != 255) {
                interfaceCtx->bAlpha = alpha;
            }

            if (interfaceCtx->minimapAlpha != 255) {
                interfaceCtx->minimapAlpha = alpha;
            }

            if ((interfaceCtx->cLeftAlpha != 0) && (interfaceCtx->cLeftAlpha > maxAlpha)) {
                interfaceCtx->cLeftAlpha = maxAlpha;
            }

            if ((interfaceCtx->cDownAlpha != 0) && (interfaceCtx->cDownAlpha > maxAlpha)) {
                interfaceCtx->cDownAlpha = maxAlpha;
            }

            if ((interfaceCtx->cRightAlpha != 0) && (interfaceCtx->cRightAlpha > maxAlpha)) {
                interfaceCtx->cRightAlpha = maxAlpha;
            }

            if ((interfaceCtx->magicAlpha != 0) && (interfaceCtx->magicAlpha > maxAlpha)) {
                interfaceCtx->magicAlpha = maxAlpha;
            }

            if ((interfaceCtx->healthAlpha != 0) && (interfaceCtx->healthAlpha > maxAlpha)) {
                interfaceCtx->healthAlpha = maxAlpha;
            }

            break;
        case 13:
            func_8008277C(play, maxAlpha, alpha);

            if ((interfaceCtx->minimapAlpha != 0) && (interfaceCtx->minimapAlpha > maxAlpha)) {
                interfaceCtx->minimapAlpha = maxAlpha;
            }

            if ((interfaceCtx->aAlpha != 0) && (interfaceCtx->aAlpha > maxAlpha)) {
                interfaceCtx->aAlpha = maxAlpha;
            }

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha;
            }

            break;
    }

    if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_1) && (interfaceCtx->minimapAlpha >= 255)) {
        interfaceCtx->minimapAlpha = 255;
    }
}

void func_80083108(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;
    Player* player = GET_PLAYER(play);
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 i;
    s16 sp28 = false;

    if ((GET_CUTSCENEINDEX < 0xFFF0) ||
        ((play->sceneId == SCENE_SPOT20) && (GET_CUTSCENEINDEX == 0xFFF0))) {
        SET_UNK_13E7(0);

        if ((player->stateFlags1 & PLAYER_STATE1_23) || (play->shootingGalleryStatus > 1) ||
            ((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38))) {
            if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
                SET_UNK_13E7(1);

                if (GET_BUTTONSTATUS_A0(0) == BTN_DISABLED) {
                    SET_BUTTONSTATUS_A0(0, SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_ENABLED))));
                }

                if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SLINGSHOT) &&
                    (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_BOW) &&
                    (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_BOMBCHU) &&
                    (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE)) {
                    SET_BUTTONSTATUS_A0(0, GET_EQUIPS_BUTTONITEMS_A0(0));

                    if ((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38)) {
                        SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_BOMBCHU);
                        Interface_LoadItemIcon1(play, 0);
                    } else {
                        SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_BOW);
                        if (play->shootingGalleryStatus > 1) {
                            if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
                                SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SLINGSHOT);
                            }

                            Interface_LoadItemIcon1(play, 0);
                        } else {
                            if (GET_INVENTORY_ITEMS_A0(SLOT_BOW) == ITEM_NONE) {
                                SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_NONE);
                            } else {
                                Interface_LoadItemIcon1(play, 0);
                            }
                        }
                    }

                    SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_DISABLED)));
                    Interface_ChangeAlpha(6);
                }

                if (play->transitionMode != TRANS_MODE_OFF) {
                    Interface_ChangeAlpha(1);
                } else if (GET_MINIGAMESTATE == 1) {
                    Interface_ChangeAlpha(8);
                } else if (play->shootingGalleryStatus > 1) {
                    Interface_ChangeAlpha(8);
                } else if ((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38)) {
                    Interface_ChangeAlpha(8);
                } else if (player->stateFlags1 & PLAYER_STATE1_23) {
                    Interface_ChangeAlpha(12);
                }
            } else {
                if (player->stateFlags1 & PLAYER_STATE1_23) {
                    Interface_ChangeAlpha(12);
                }
            }
        } else if (play->sceneId == SCENE_KENJYANOMA) {
            Interface_ChangeAlpha(1);
        } else if (play->sceneId == SCENE_TURIBORI) {
            SET_UNK_13E7(2);
            if (play->interfaceCtx.unk_260 != 0) {
                if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_FISHING_POLE) {
                    SET_BUTTONSTATUS_A0(0, GET_EQUIPS_BUTTONITEMS_A0(0));
                    SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_FISHING_POLE);
                    SET_UNK_13EA(0);
                    Interface_LoadItemIcon1(play, 0);
                    Interface_ChangeAlpha(12);
                }

                if (GET_UNK_13EA != 12) {
                    Interface_ChangeAlpha(12);
                }
            } else if (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_FISHING_POLE) {
                SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                SET_UNK_13EA(0);

                if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
                    Interface_LoadItemIcon1(play, 0);
                }

                SET_BUTTONSTATUS_A0(0, SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_DISABLED))));
                Interface_ChangeAlpha(50);
            } else {
                if (GET_BUTTONSTATUS_A0(0) == BTN_ENABLED) {
                    SET_UNK_13EA(0);
                }

                SET_BUTTONSTATUS_A0(0, SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_DISABLED))));
                Interface_ChangeAlpha(50);
            }
        } else if (msgCtx->msgMode == MSGMODE_NONE) {
            if ((func_8008F2F8(play) >= 2) && (func_8008F2F8(play) < 5)) {
                if (GET_BUTTONSTATUS_A0(0) != BTN_DISABLED) {
                    sp28 = true;
                }

                SET_BUTTONSTATUS_A0(0, BTN_DISABLED);

                for (i = 1; i < 4; i++) {
                    if (func_8008F2F8(play) == 2) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_HOOKSHOT) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_LONGSHOT)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        } else {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    } else {
                        if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                            sp28 = true;
                        }

                        SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                    }
                }

                if (sp28) {
                    SET_UNK_13EA(0);
                }

                Interface_ChangeAlpha(50);
            } else if ((player->stateFlags1 & PLAYER_STATE1_21) || (player->stateFlags2 & PLAYER_STATE2_18)) {
                if (GET_BUTTONSTATUS_A0(0) != BTN_DISABLED) {
                    SET_BUTTONSTATUS_A0(0, BTN_DISABLED);
                    SET_BUTTONSTATUS_A0(1, BTN_DISABLED);
                    SET_BUTTONSTATUS_A0(2, BTN_DISABLED);
                    SET_BUTTONSTATUS_A0(3, BTN_DISABLED);
                    SET_UNK_13EA(0);
                    Interface_ChangeAlpha(50);
                }
            } else if (GET_EVENTINF_HORSES_STATE() == EVENTINF_HORSES_STATE_1) {
                if (player->stateFlags1 & PLAYER_STATE1_23) {
                    if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) &&
                        (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_BOW)) {
                        if (GET_INVENTORY_ITEMS_A0(SLOT_BOW) == ITEM_NONE) {
                            SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_NONE);
                        } else {
                            SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_BOW);
                            sp28 = true;
                        }
                    }
                } else {
                    if ((GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_NONE) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOW)) {

                        if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KOKIRI) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_MASTER) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_BGS) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KNIFE)) {
                            SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                        } else {
                            SET_BUTTONSTATUS_A0(0, GET_EQUIPS_BUTTONITEMS_A0(0));
                        }
                    }
                    sp28 = true;
                }

                if (sp28) {
                    Interface_LoadItemIcon1(play, 0);
                    sp28 = false;
                }

                for (i = 1; i < 4; i++) {
                    if ((GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_FAIRY) &&
                        (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_TIME)) {
                        if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                            sp28 = true;
                        }

                        SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                    } else {
                        if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                            sp28 = true;
                        }

                        SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                    }
                }

                if (sp28) {
                    SET_UNK_13EA(0);
                }

                Interface_ChangeAlpha(50);
            } else {
                if (interfaceCtx->restrictions.bButton == 0) {
                    if ((GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_SLINGSHOT) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOW) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOMBCHU) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_NONE)) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) ||
                            (GET_INFTABLE_A0(INFTABLE_1DX_INDEX) == 0)) {
                            SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                            sp28 = true;

                            if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
                                Interface_LoadItemIcon1(play, 0);
                            }
                        }
                    } else if ((GET_BUTTONSTATUS_A0(0) & 0xFF) == BTN_DISABLED) {
                        sp28 = true;

                        if (((GET_BUTTONSTATUS_A0(0) & 0xFF) == BTN_DISABLED) ||
                            ((GET_BUTTONSTATUS_A0(0) & 0xFF) == BTN_ENABLED)) {
                            SET_BUTTONSTATUS_A0(0, BTN_ENABLED);
                        } else {
                            SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0) & 0xFF);
                        }
                    }
                } else if (interfaceCtx->restrictions.bButton == 1) {
                    if ((GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_SLINGSHOT) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOW) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOMBCHU) ||
                        (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_NONE)) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) ||
                            (GET_INFTABLE_A0(INFTABLE_1DX_INDEX) == 0)) {
                            SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                            sp28 = true;

                            if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
                                Interface_LoadItemIcon1(play, 0);
                            }
                        }
                    } else {
                        if (GET_BUTTONSTATUS_A0(0) == BTN_ENABLED) {
                            sp28 = true;
                        }

                        SET_BUTTONSTATUS_A0(0, BTN_DISABLED);
                    }
                }

                if (interfaceCtx->restrictions.bottles != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        }
                    }
                } else if (interfaceCtx->restrictions.bottles == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.tradeItems != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        }
                    }
                } else if (interfaceCtx->restrictions.tradeItems == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.hookshot != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_HOOKSHOT) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_LONGSHOT)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        }
                    }
                } else if (interfaceCtx->restrictions.hookshot == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_HOOKSHOT) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_LONGSHOT)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.ocarina != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_OCARINA_FAIRY) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_OCARINA_TIME)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        }
                    }
                } else if (interfaceCtx->restrictions.ocarina == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_OCARINA_FAIRY) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_OCARINA_TIME)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.farores != 0) {
                    for (i = 1; i < 4; i++) {
                        if (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_FARORES_WIND) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                            osSyncPrintf("***(i=%d)***  ", i);
                        }
                    }
                } else if (interfaceCtx->restrictions.farores == 0) {
                    for (i = 1; i < 4; i++) {
                        if (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_FARORES_WIND) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.dinsNayrus != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_DINS_FIRE) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_NAYRUS_LOVE)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                        }
                    }
                } else if (interfaceCtx->restrictions.dinsNayrus == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_DINS_FIRE) ||
                            (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_NAYRUS_LOVE)) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }

                if (interfaceCtx->restrictions.all != 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_FAIRY) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_TIME) &&
                            !((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                              (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) &&
                            !((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                              (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK))) {
                            if ((play->sceneId != SCENE_TAKARAYA) ||
                                (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_LENS)) {
                                if (GET_BUTTONSTATUS_A0(i) == BTN_ENABLED) {
                                    sp28 = true;
                                }

                                SET_BUTTONSTATUS_A0(i, BTN_DISABLED);
                            } else {
                                if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                    sp28 = true;
                                }

                                SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                            }
                        }
                    }
                } else if (interfaceCtx->restrictions.all == 0) {
                    for (i = 1; i < 4; i++) {
                        if ((GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_DINS_FIRE) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_HOOKSHOT) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_LONGSHOT) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_FARORES_WIND) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_NAYRUS_LOVE) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_FAIRY) &&
                            (GET_EQUIPS_BUTTONITEMS_A0(i) != ITEM_OCARINA_TIME) &&
                            !((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                              (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) &&
                            !((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                              (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK))) {
                            if (GET_BUTTONSTATUS_A0(i) == BTN_DISABLED) {
                                sp28 = true;
                            }

                            SET_BUTTONSTATUS_A0(i, BTN_ENABLED);
                        }
                    }
                }
            }
        }
    }

    if (sp28) {
        SET_UNK_13EA(0);
        if ((play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->transitionMode == TRANS_MODE_OFF)) {
            Interface_ChangeAlpha(50);
            osSyncPrintf("????????  alpha_change( 50 );  ?????\n");
        } else {
            osSyncPrintf("game_play->fade_direction || game_play->fbdemo_wipe_modem");
        }
    }
}

void Interface_SetSceneRestrictions(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 i;
    u8 sceneId;

    interfaceCtx->restrictions.hGauge = interfaceCtx->restrictions.bButton = interfaceCtx->restrictions.aButton =
        interfaceCtx->restrictions.bottles = interfaceCtx->restrictions.tradeItems =
            interfaceCtx->restrictions.hookshot = interfaceCtx->restrictions.ocarina =
                interfaceCtx->restrictions.warpSongs = interfaceCtx->restrictions.sunsSong =
                    interfaceCtx->restrictions.farores = interfaceCtx->restrictions.dinsNayrus =
                        interfaceCtx->restrictions.all = 0;

    i = 0;

    // "Data settings related to button display scene_data_ID=%d\n"
    osSyncPrintf("ボタン表示関係データ設定 scene_data_ID=%d\n", play->sceneId);

    do {
        sceneId = (u8)play->sceneId;
        if (sRestrictionFlags[i].sceneId == sceneId) {
            interfaceCtx->restrictions.hGauge = (sRestrictionFlags[i].flags1 & 0xC0) >> 6;
            interfaceCtx->restrictions.bButton = (sRestrictionFlags[i].flags1 & 0x30) >> 4;
            interfaceCtx->restrictions.aButton = (sRestrictionFlags[i].flags1 & 0x0C) >> 2;
            interfaceCtx->restrictions.bottles = (sRestrictionFlags[i].flags1 & 0x03) >> 0;
            interfaceCtx->restrictions.tradeItems = (sRestrictionFlags[i].flags2 & 0xC0) >> 6;
            interfaceCtx->restrictions.hookshot = (sRestrictionFlags[i].flags2 & 0x30) >> 4;
            interfaceCtx->restrictions.ocarina = (sRestrictionFlags[i].flags2 & 0x0C) >> 2;
            interfaceCtx->restrictions.warpSongs = (sRestrictionFlags[i].flags2 & 0x03) >> 0;
            interfaceCtx->restrictions.sunsSong = (sRestrictionFlags[i].flags3 & 0xC0) >> 6;
            interfaceCtx->restrictions.farores = (sRestrictionFlags[i].flags3 & 0x30) >> 4;
            interfaceCtx->restrictions.dinsNayrus = (sRestrictionFlags[i].flags3 & 0x0C) >> 2;
            interfaceCtx->restrictions.all = (sRestrictionFlags[i].flags3 & 0x03) >> 0;

            osSyncPrintf(VT_FGCOL(YELLOW));
            osSyncPrintf("parameter->button_status = %x,%x,%x\n", sRestrictionFlags[i].flags1,
                         sRestrictionFlags[i].flags2, sRestrictionFlags[i].flags3);
            osSyncPrintf("h_gage=%d, b_button=%d, a_button=%d, c_bottle=%d\n", interfaceCtx->restrictions.hGauge,
                         interfaceCtx->restrictions.bButton, interfaceCtx->restrictions.aButton,
                         interfaceCtx->restrictions.bottles);
            osSyncPrintf("c_warasibe=%d, c_hook=%d, c_ocarina=%d, c_warp=%d\n", interfaceCtx->restrictions.tradeItems,
                         interfaceCtx->restrictions.hookshot, interfaceCtx->restrictions.ocarina,
                         interfaceCtx->restrictions.warpSongs);
            osSyncPrintf("c_sunmoon=%d, m_wind=%d, m_magic=%d, another=%d\n", interfaceCtx->restrictions.sunsSong,
                         interfaceCtx->restrictions.farores, interfaceCtx->restrictions.dinsNayrus,
                         interfaceCtx->restrictions.all);
            osSyncPrintf(VT_RST);
            return;
        }
        i++;
    } while (sRestrictionFlags[i].sceneId != 0xFF);
}

Gfx* Gfx_TextureIA8(Gfx* displayListHead, void* texture, s16 textureWidth, s16 textureHeight, s16 rectLeft, s16 rectTop,
                    s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy) {
    gDPLoadTextureBlock(displayListHead++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, textureWidth, textureHeight, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSPTextureRectangle(displayListHead++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2,
                        (rectTop + rectHeight) << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);

    return displayListHead;
}

Gfx* Gfx_TextureI8(Gfx* displayListHead, void* texture, s16 textureWidth, s16 textureHeight, s16 rectLeft, s16 rectTop,
                   s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy) {
    gDPLoadTextureBlock(displayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, textureWidth, textureHeight, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSPTextureRectangle(displayListHead++, rectLeft << 2, rectTop << 2, (rectLeft + rectWidth) << 2,
                        (rectTop + rectHeight) << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);

    return displayListHead;
}

void Inventory_SwapAgeEquipment(void) {
    s16 i;
    u16 shieldEquipValue;

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        for (i = 0; i < 4; i++) {
            if (i != 0) {
                SET_CHILDEQUIPS_BUTTONITEMS_A0(i, GET_EQUIPS_BUTTONITEMS_A0(i));
            } else {
                SET_CHILDEQUIPS_BUTTONITEMS_A0(i, ITEM_SWORD_KOKIRI);
            }

            if (i != 0) {
                SET_CHILDEQUIPS_CBUTTONSLOTS_A0(i-1, GET_EQUIPS_CBUTTONSLOTS_A0(i-1));
            }
        }

        SET_CHILDEQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT);

        if (GET_ADULTEQUIPS_BUTTONITEMS_A0(0) == ITEM_NONE) {
            SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SWORD_MASTER);

            if (GET_INVENTORY_ITEMS_A0(SLOT_NUT) != ITEM_NONE) {
                SET_EQUIPS_BUTTONITEMS_A0(1, ITEM_NUT);
                SET_EQUIPS_CBUTTONSLOTS_A0(0, SLOT_NUT);
            } else {
                SET_EQUIPS_BUTTONITEMS_A0(1, SET_EQUIPS_CBUTTONSLOTS_A0(0, ITEM_NONE));
            }

            SET_EQUIPS_BUTTONITEMS_A0(2, ITEM_BOMB);
            SET_EQUIPS_BUTTONITEMS_A0(3, GET_INVENTORY_ITEMS_A0(SLOT_OCARINA));
            SET_EQUIPS_CBUTTONSLOTS_A0(1, SLOT_BOMB);
            SET_EQUIPS_CBUTTONSLOTS_A0(2, SLOT_OCARINA);
            SET_EQUIPS_EQUIPMENT((EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)) |
                                                      (EQUIP_VALUE_SHIELD_HYLIAN << (EQUIP_TYPE_SHIELD * 4)) |
                                                      (EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4)) |
                                                      (EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4)));
        } else {
            for (i = 0; i < 4; i++) {
                SET_EQUIPS_BUTTONITEMS_A0(i, GET_ADULTEQUIPS_BUTTONITEMS_A0(i));

                if (i != 0) {
                    SET_EQUIPS_CBUTTONSLOTS_A0(i-1, GET_ADULTEQUIPS_CBUTTONSLOTS_A0(i-1));
                }

                if (((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) ||
                    ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK))) {
                    osSyncPrintf("Register_Item_Pt(%d)=%d\n", i, GET_EQUIPS_CBUTTONSLOTS_A0(i-1));
                    SET_EQUIPS_BUTTONITEMS_A0(i, GET_INVENTORY_ITEMS[GET_EQUIPS_CBUTTONSLOTS_A0(i-1)]);
                }
            }

            SET_EQUIPS_EQUIPMENT(GET_ADULTEQUIPS_EQUIPMENT);
        }
    } else {
        for (i = 0; i < 4; i++) {
            SET_ADULTEQUIPS_BUTTONITEMS_A0(i, GET_EQUIPS_BUTTONITEMS_A0(i));

            if (i != 0) {
                SET_ADULTEQUIPS_CBUTTONSLOTS_A0(i-1, GET_EQUIPS_CBUTTONSLOTS_A0(i-1));
            }
        }

        SET_ADULTEQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT);

        if (GET_CHILDEQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
            for (i = 0; i < 4; i++) {
                SET_EQUIPS_BUTTONITEMS_A0(i, GET_CHILDEQUIPS_BUTTONITEMS_A0(i));

                if (i != 0) {
                    SET_EQUIPS_CBUTTONSLOTS_A0(i-1, GET_CHILDEQUIPS_CBUTTONSLOTS_A0(i-1));
                }

                if (((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_BOTTLE) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_POE)) ||
                    ((GET_EQUIPS_BUTTONITEMS_A0(i) >= ITEM_WEIRD_EGG) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(i) <= ITEM_CLAIM_CHECK))) {
                    osSyncPrintf("Register_Item_Pt(%d)=%d\n", i, GET_EQUIPS_CBUTTONSLOTS_A0(i-1));
                    SET_EQUIPS_BUTTONITEMS_A0(i, GET_INVENTORY_ITEMS[GET_EQUIPS_CBUTTONSLOTS_A0(i-1)]);
                }
            }

            SET_EQUIPS_EQUIPMENT(GET_CHILDEQUIPS_EQUIPMENT);
            SET_EQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT & ((u16) ~(0xF << (EQUIP_TYPE_SWORD * 4))));
            SET_EQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT | (EQUIP_VALUE_SWORD_KOKIRI << (EQUIP_TYPE_SWORD * 4)));
        }
    }

    shieldEquipValue = gEquipMasks[EQUIP_TYPE_SHIELD] & GET_EQUIPS_EQUIPMENT;
    if (shieldEquipValue != 0) {
        shieldEquipValue >>= gEquipShifts[EQUIP_TYPE_SHIELD];
        if (!CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SHIELD, shieldEquipValue - 1)) {
            SET_EQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT & (gEquipNegMasks[EQUIP_TYPE_SHIELD]));
        }
    }
}

void Interface_InitHorsebackArchery(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    SET_MINIGAMESTATE(1);
    interfaceCtx->unk_23C = interfaceCtx->unk_240 = interfaceCtx->unk_242 = 0;
    SET_MINIGAMESCORE(sHBAScoreTier = 0);
    interfaceCtx->hbaAmmo = 20;
}

void func_800849EC(PlayState* play) {
    SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT | (OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BGS)));
    SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT ^ (OWNED_EQUIP_FLAG_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)));

    if (CHECK_OWNED_EQUIP_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)) {
        SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SWORD_KNIFE);
    } else {
        SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SWORD_BGS);
    }

    Interface_LoadItemIcon1(play, 0);
}

void Interface_LoadItemIcon1(PlayState* play, u16 button) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, 1);
    DmaMgr_SendRequest2(&interfaceCtx->dmaRequest_160, interfaceCtx->iconItemSegment + button * 0x1000,
                        (uintptr_t)_icon_item_staticSegmentRomStart +
                            (GET_EQUIPS_BUTTONITEMS_A0(button) * 0x1000),
                        0x1000, 0, &interfaceCtx->loadQueue, NULL, "../z_parameter.c", 1171);
    osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
}

void Interface_LoadItemIcon2(PlayState* play, u16 button) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, 1);
    DmaMgr_SendRequest2(&interfaceCtx->dmaRequest_180, interfaceCtx->iconItemSegment + button * 0x1000,
                        (uintptr_t)_icon_item_staticSegmentRomStart +
                            (GET_EQUIPS_BUTTONITEMS_A0(button) * 0x1000),
                        0x1000, 0, &interfaceCtx->loadQueue, NULL, "../z_parameter.c", 1193);
    osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
}

void func_80084BF4(PlayState* play, u16 flag) {
    if (flag) {
        if ((GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_SLINGSHOT) ||
            (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOW) ||
            (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOMBCHU) ||
            (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_FISHING_POLE) ||
            (GET_BUTTONSTATUS_A0(0) == BTN_DISABLED)) {
            if ((GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_SLINGSHOT) ||
                (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOW) ||
                (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_BOMBCHU) ||
                (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_FISHING_POLE)) {
                SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                Interface_LoadItemIcon1(play, 0);
            }
        } else if (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_NONE) {
            if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) ||
                (GET_INFTABLE_A0(INFTABLE_1DX_INDEX) == 0)) {
                SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                Interface_LoadItemIcon1(play, 0);
            }
        }

        SET_BUTTONSTATUS_A0(0, SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_ENABLED))));
        Interface_ChangeAlpha(7);
    } else {
        SET_BUTTONSTATUS_A0(0, SET_BUTTONSTATUS_A0(1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, BTN_ENABLED))));
        func_80083108(play);
    }
}

u8 Item_Give(PlayState* play, u8 item) {
    static s16 sAmmoRefillCounts[] = { 5, 10, 20, 30 }; // Sticks, nuts, bombs
    static s16 sArrowRefillCounts[] = { 5, 10, 30 };
    static s16 sBombchuRefillCounts[] = { 5, 20 };
    static s16 sRupeeRefillCounts[] = { 1, 5, 20, 50, 200, 10 };
    s16 i;
    s16 slot;
    s16 temp;

    slot = SLOT(item);
    if (item >= ITEM_STICKS_5) {
        slot = SLOT(sExtraItemBases[item - ITEM_STICKS_5]);
    }

    osSyncPrintf(VT_FGCOL(YELLOW));
    osSyncPrintf("item_get_setting=%d  pt=%d  z=%x\n", item, slot, GET_INVENTORY_ITEMS_A0(slot));
    osSyncPrintf(VT_RST);

    if ((item >= ITEM_MEDALLION_FOREST) && (item <= ITEM_MEDALLION_LIGHT)) {
        SET_INVENTORY_QUESTITEMS(GET_INVENTORY_QUESTITEMS | (gBitFlags[item - ITEM_MEDALLION_FOREST + QUEST_MEDALLION_FOREST]));

        osSyncPrintf(VT_FGCOL(YELLOW));
        osSyncPrintf("封印 = %x\n", GET_INVENTORY_QUESTITEMS); // "Seals = %x"
        osSyncPrintf(VT_RST);

        if (item == ITEM_MEDALLION_WATER) {
            func_8006D0AC(play);
        }

        return ITEM_NONE;
    } else if ((item >= ITEM_SONG_MINUET) && (item <= ITEM_SONG_STORMS)) {
        SET_INVENTORY_QUESTITEMS(GET_INVENTORY_QUESTITEMS | (gBitFlags[item - ITEM_SONG_MINUET + QUEST_SONG_MINUET]));

        osSyncPrintf(VT_FGCOL(YELLOW));
        osSyncPrintf("楽譜 = %x\n", GET_INVENTORY_QUESTITEMS); // "Musical scores = %x"
        // "Musical scores = %x (%x) (%x)"
        osSyncPrintf("楽譜 = %x (%x) (%x)\n", GET_INVENTORY_QUESTITEMS,
                     gBitFlags[item - ITEM_SONG_MINUET + QUEST_SONG_MINUET], gBitFlags[item - ITEM_SONG_MINUET]);
        osSyncPrintf(VT_RST);

        return ITEM_NONE;
    } else if ((item >= ITEM_KOKIRI_EMERALD) && (item <= ITEM_ZORA_SAPPHIRE)) {
        SET_INVENTORY_QUESTITEMS(GET_INVENTORY_QUESTITEMS | (gBitFlags[item - ITEM_KOKIRI_EMERALD + QUEST_KOKIRI_EMERALD]));

        osSyncPrintf(VT_FGCOL(YELLOW));
        osSyncPrintf("精霊石 = %x\n", GET_INVENTORY_QUESTITEMS); // "Spiritual Stones = %x"
        osSyncPrintf(VT_RST);

        return ITEM_NONE;
    } else if ((item == ITEM_STONE_OF_AGONY) || (item == ITEM_GERUDO_CARD)) {
        SET_INVENTORY_QUESTITEMS(GET_INVENTORY_QUESTITEMS | (gBitFlags[item - ITEM_STONE_OF_AGONY + QUEST_STONE_OF_AGONY]));

        osSyncPrintf(VT_FGCOL(YELLOW));
        osSyncPrintf("アイテム = %x\n", GET_INVENTORY_QUESTITEMS); // "Items = %x"
        osSyncPrintf(VT_RST);

        return ITEM_NONE;
    } else if (item == ITEM_SKULL_TOKEN) {
        SET_INVENTORY_QUESTITEMS(GET_INVENTORY_QUESTITEMS | (gBitFlags[item - ITEM_SKULL_TOKEN + QUEST_SKULL_TOKEN]));
        GET_INVENTORY_GSTOKENS++;

        osSyncPrintf(VT_FGCOL(YELLOW));
        // "N Coins = %x(%d)"
        osSyncPrintf("Ｎコイン = %x(%d)\n", GET_INVENTORY_QUESTITEMS,
                     GET_INVENTORY_GSTOKENS);
        osSyncPrintf(VT_RST);

        return ITEM_NONE;
    } else if ((item >= ITEM_SWORD_KOKIRI) && (item <= ITEM_SWORD_BGS)) {
        SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT | (OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, item - ITEM_SWORD_KOKIRI + EQUIP_INV_SWORD_KOKIRI)));

        if (item == ITEM_SWORD_BGS) {
            SET_SWORDHEALTH(8);

            if (ALL_EQUIP_VALUE(EQUIP_TYPE_SWORD) ==
                ((1 << EQUIP_INV_SWORD_KOKIRI) | (1 << EQUIP_INV_SWORD_MASTER) | (1 << EQUIP_INV_SWORD_BGS) |
                 (1 << EQUIP_INV_SWORD_BROKENGIANTKNIFE))) {
                SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT ^ (OWNED_EQUIP_FLAG_ALT(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE)));
                if (GET_EQUIPS_BUTTONITEMS_A0(0) == ITEM_SWORD_KNIFE) {
                    SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SWORD_BGS);
                    Interface_LoadItemIcon1(play, 0);
                }
            }
        } else if (item == ITEM_SWORD_MASTER) {
            SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_SWORD_MASTER);
            SET_EQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT & ((u16) ~(0xF << (EQUIP_TYPE_SWORD * 4))));
            SET_EQUIPS_EQUIPMENT(GET_EQUIPS_EQUIPMENT | (EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)));
            Interface_LoadItemIcon1(play, 0);
        }

        return ITEM_NONE;
    } else if ((item >= ITEM_SHIELD_DEKU) && (item <= ITEM_SHIELD_MIRROR)) {
        SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT | (OWNED_EQUIP_FLAG(EQUIP_TYPE_SHIELD, item - ITEM_SHIELD_DEKU)));
        return ITEM_NONE;
    } else if ((item >= ITEM_TUNIC_KOKIRI) && (item <= ITEM_TUNIC_ZORA)) {
        SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT | (OWNED_EQUIP_FLAG(EQUIP_TYPE_TUNIC, item - ITEM_TUNIC_KOKIRI)));
        return ITEM_NONE;
    } else if ((item >= ITEM_BOOTS_KOKIRI) && (item <= ITEM_BOOTS_HOVER)) {
        SET_INVENTORY_EQUIPMENT(GET_INVENTORY_EQUIPMENT | (OWNED_EQUIP_FLAG(EQUIP_TYPE_BOOTS, item - ITEM_BOOTS_KOKIRI)));
        return ITEM_NONE;
    } else if ((item == ITEM_KEY_BOSS) || (item == ITEM_COMPASS) || (item == ITEM_DUNGEON_MAP)) {
        SET_INVENTORY_DUNGEONITEMS_A0(GET_MAPINDEX, GET_INVENTORY_DUNGEONITEMS_A0(GET_MAPINDEX) | (gBitFlags[item - ITEM_KEY_BOSS]));
        return ITEM_NONE;
    } else if (item == ITEM_KEY_SMALL) {
        if (GET_INVENTORY_DUNGEONKEYS_A0(GET_MAPINDEX) < 0) {
            SET_INVENTORY_DUNGEONKEYS_A0(GET_MAPINDEX, 1);
            return ITEM_NONE;
        } else {
            GET_INVENTORY_DUNGEONKEYS_A0(GET_MAPINDEX)++;
            return ITEM_NONE;
        }
    } else if ((item == ITEM_QUIVER_30) || (item == ITEM_BOW)) {
        if (CUR_UPG_VALUE(UPG_QUIVER) == 0) {
            Inventory_ChangeUpgrade(UPG_QUIVER, 1);
            INV_CONTENT(ITEM_BOW) = ITEM_BOW;
            AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 1);
            return ITEM_NONE;
        } else {
            AMMO(ITEM_BOW)++;
            if (AMMO(ITEM_BOW) > CUR_CAPACITY(UPG_QUIVER)) {
                AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
            }
        }
    } else if (item == ITEM_QUIVER_40) {
        Inventory_ChangeUpgrade(UPG_QUIVER, 2);
        AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 2);
        return ITEM_NONE;
    } else if (item == ITEM_QUIVER_50) {
        Inventory_ChangeUpgrade(UPG_QUIVER, 3);
        AMMO(ITEM_BOW) = CAPACITY(UPG_QUIVER, 3);
        return ITEM_NONE;
    } else if (item == ITEM_BULLET_BAG_40) {
        Inventory_ChangeUpgrade(UPG_BULLET_BAG, 2);
        AMMO(ITEM_SLINGSHOT) = CAPACITY(UPG_BULLET_BAG, 2);
        return ITEM_NONE;
    } else if (item == ITEM_BULLET_BAG_50) {
        Inventory_ChangeUpgrade(UPG_BULLET_BAG, 3);
        AMMO(ITEM_SLINGSHOT) = CAPACITY(UPG_BULLET_BAG, 3);
        return ITEM_NONE;
    } else if (item == ITEM_BOMB_BAG_20) {
        if (CUR_UPG_VALUE(UPG_BOMB_BAG) == 0) {
            Inventory_ChangeUpgrade(UPG_BOMB_BAG, 1);
            INV_CONTENT(ITEM_BOMB) = ITEM_BOMB;
            AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 1);
            return ITEM_NONE;
        } else {
            AMMO(ITEM_BOMB)++;
            if (AMMO(ITEM_BOMB) > CUR_CAPACITY(UPG_BOMB_BAG)) {
                AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
            }
        }
    } else if (item == ITEM_BOMB_BAG_30) {
        Inventory_ChangeUpgrade(UPG_BOMB_BAG, 2);
        AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 2);
        return ITEM_NONE;
    } else if (item == ITEM_BOMB_BAG_40) {
        Inventory_ChangeUpgrade(UPG_BOMB_BAG, 3);
        AMMO(ITEM_BOMB) = CAPACITY(UPG_BOMB_BAG, 3);
        return ITEM_NONE;
    } else if (item == ITEM_BRACELET) {
        Inventory_ChangeUpgrade(UPG_STRENGTH, 1);
        return ITEM_NONE;
    } else if (item == ITEM_GAUNTLETS_SILVER) {
        Inventory_ChangeUpgrade(UPG_STRENGTH, 2);
        return ITEM_NONE;
    } else if (item == ITEM_GAUNTLETS_GOLD) {
        Inventory_ChangeUpgrade(UPG_STRENGTH, 3);
        return ITEM_NONE;
    } else if (item == ITEM_SCALE_SILVER) {
        Inventory_ChangeUpgrade(UPG_SCALE, 1);
        return ITEM_NONE;
    } else if (item == ITEM_SCALE_GOLDEN) {
        Inventory_ChangeUpgrade(UPG_SCALE, 2);
        return ITEM_NONE;
    } else if (item == ITEM_WALLET_ADULT) {
        Inventory_ChangeUpgrade(UPG_WALLET, 1);
        return ITEM_NONE;
    } else if (item == ITEM_WALLET_GIANT) {
        Inventory_ChangeUpgrade(UPG_WALLET, 2);
        return ITEM_NONE;
    } else if (item == ITEM_STICK_UPGRADE_20) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_STICK) = ITEM_STICK;
        }
        Inventory_ChangeUpgrade(UPG_STICKS, 2);
        AMMO(ITEM_STICK) = CAPACITY(UPG_STICKS, 2);
        return ITEM_NONE;
    } else if (item == ITEM_STICK_UPGRADE_30) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_STICK) = ITEM_STICK;
        }
        Inventory_ChangeUpgrade(UPG_STICKS, 3);
        AMMO(ITEM_STICK) = CAPACITY(UPG_STICKS, 3);
        return ITEM_NONE;
    } else if (item == ITEM_NUT_UPGRADE_30) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_NUT) = ITEM_NUT;
        }
        Inventory_ChangeUpgrade(UPG_NUTS, 2);
        AMMO(ITEM_NUT) = CAPACITY(UPG_NUTS, 2);
        return ITEM_NONE;
    } else if (item == ITEM_NUT_UPGRADE_40) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_NUT) = ITEM_NUT;
        }
        Inventory_ChangeUpgrade(UPG_NUTS, 3);
        AMMO(ITEM_NUT) = CAPACITY(UPG_NUTS, 3);
        return ITEM_NONE;
    } else if (item == ITEM_LONGSHOT) {
        INV_CONTENT(item) = item;
        for (i = 1; i < 4; i++) {
            if (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_HOOKSHOT) {
                SET_EQUIPS_BUTTONITEMS_A0(i, ITEM_LONGSHOT);
                Interface_LoadItemIcon1(play, i);
            }
        }
        return ITEM_NONE;
    } else if (item == ITEM_STICK) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            Inventory_ChangeUpgrade(UPG_STICKS, 1);
            AMMO(ITEM_STICK) = 1;
        } else {
            AMMO(ITEM_STICK)++;
            if (AMMO(ITEM_STICK) > CUR_CAPACITY(UPG_STICKS)) {
                AMMO(ITEM_STICK) = CUR_CAPACITY(UPG_STICKS);
            }
        }
    } else if ((item == ITEM_STICKS_5) || (item == ITEM_STICKS_10)) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            Inventory_ChangeUpgrade(UPG_STICKS, 1);
            AMMO(ITEM_STICK) = sAmmoRefillCounts[item - ITEM_STICKS_5];
        } else {
            AMMO(ITEM_STICK) += sAmmoRefillCounts[item - ITEM_STICKS_5];
            if (AMMO(ITEM_STICK) > CUR_CAPACITY(UPG_STICKS)) {
                AMMO(ITEM_STICK) = CUR_CAPACITY(UPG_STICKS);
            }
        }
        item = ITEM_STICK;
    } else if (item == ITEM_NUT) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            Inventory_ChangeUpgrade(UPG_NUTS, 1);
            AMMO(ITEM_NUT) = ITEM_NUT;
        } else {
            AMMO(ITEM_NUT)++;
            if (AMMO(ITEM_NUT) > CUR_CAPACITY(UPG_NUTS)) {
                AMMO(ITEM_NUT) = CUR_CAPACITY(UPG_NUTS);
            }
        }
    } else if ((item == ITEM_NUTS_5) || (item == ITEM_NUTS_10)) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            Inventory_ChangeUpgrade(UPG_NUTS, 1);
            AMMO(ITEM_NUT) += sAmmoRefillCounts[item - ITEM_NUTS_5];
            // "Deku Nuts %d(%d)=%d BS_count=%d"
            osSyncPrintf("デクの実 %d(%d)=%d  BS_count=%d\n", item, ITEM_NUTS_5, item - ITEM_NUTS_5,
                         sAmmoRefillCounts[item - ITEM_NUTS_5]);
        } else {
            AMMO(ITEM_NUT) += sAmmoRefillCounts[item - ITEM_NUTS_5];
            if (AMMO(ITEM_NUT) > CUR_CAPACITY(UPG_NUTS)) {
                AMMO(ITEM_NUT) = CUR_CAPACITY(UPG_NUTS);
            }
        }
        item = ITEM_NUT;
    } else if (item == ITEM_BOMB) {
        // "Bomb  Bomb  Bomb  Bomb Bomb   Bomb Bomb"
        osSyncPrintf(" 爆弾  爆弾  爆弾  爆弾 爆弾   爆弾 爆弾 \n");
        if ((AMMO(ITEM_BOMB) += 1) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;
    } else if ((item >= ITEM_BOMBS_5) && (item <= ITEM_BOMBS_30)) {
        if ((AMMO(ITEM_BOMB) += sAmmoRefillCounts[item - ITEM_BOMBS_5]) > CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
        }
        return ITEM_NONE;
    } else if (item == ITEM_BOMBCHU) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_BOMBCHU) = ITEM_BOMBCHU;
            AMMO(ITEM_BOMBCHU) = 10;
            return ITEM_NONE;
        } else {
            AMMO(ITEM_BOMBCHU) += 10;
            if (AMMO(ITEM_BOMBCHU) > 50) {
                AMMO(ITEM_BOMBCHU) = 50;
            }
            return ITEM_NONE;
        }
    } else if ((item == ITEM_BOMBCHUS_5) || (item == ITEM_BOMBCHUS_20)) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(ITEM_BOMBCHU) = ITEM_BOMBCHU;
            AMMO(ITEM_BOMBCHU) += sBombchuRefillCounts[item - ITEM_BOMBCHUS_5];
            return ITEM_NONE;
        } else {
            AMMO(ITEM_BOMBCHU) += sBombchuRefillCounts[item - ITEM_BOMBCHUS_5];
            if (AMMO(ITEM_BOMBCHU) > 50) {
                AMMO(ITEM_BOMBCHU) = 50;
            }
            return ITEM_NONE;
        }
    } else if ((item >= ITEM_ARROWS_SMALL) && (item <= ITEM_ARROWS_LARGE)) {
        AMMO(ITEM_BOW) += sArrowRefillCounts[item - ITEM_ARROWS_SMALL];

        if ((AMMO(ITEM_BOW) >= CUR_CAPACITY(UPG_QUIVER)) || (AMMO(ITEM_BOW) < 0)) {
            AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
        }

        osSyncPrintf("%d本  Item_MaxGet=%d\n", AMMO(ITEM_BOW), CUR_CAPACITY(UPG_QUIVER));

        return ITEM_BOW;
    } else if (item == ITEM_SLINGSHOT) {
        Inventory_ChangeUpgrade(UPG_BULLET_BAG, 1);
        INV_CONTENT(ITEM_SLINGSHOT) = ITEM_SLINGSHOT;
        AMMO(ITEM_SLINGSHOT) = 30;
        return ITEM_NONE;
    } else if (item == ITEM_SEEDS) {
        AMMO(ITEM_SLINGSHOT) += 5;

        if (AMMO(ITEM_SLINGSHOT) >= CUR_CAPACITY(UPG_BULLET_BAG)) {
            AMMO(ITEM_SLINGSHOT) = CUR_CAPACITY(UPG_BULLET_BAG);
        }

        if (!GET_ITEMGETINF(ITEMGETINF_13)) {
            SET_ITEMGETINF(ITEMGETINF_13);
            return ITEM_NONE;
        }

        return ITEM_SEEDS;
    } else if (item == ITEM_SEEDS_30) {
        AMMO(ITEM_SLINGSHOT) += 30;

        if (AMMO(ITEM_SLINGSHOT) >= CUR_CAPACITY(UPG_BULLET_BAG)) {
            AMMO(ITEM_SLINGSHOT) = CUR_CAPACITY(UPG_BULLET_BAG);
        }

        if (!GET_ITEMGETINF(ITEMGETINF_13)) {
            SET_ITEMGETINF(ITEMGETINF_13);
            return ITEM_NONE;
        }

        return ITEM_SEEDS;
    } else if (item == ITEM_OCARINA_FAIRY) {
        INV_CONTENT(ITEM_OCARINA_FAIRY) = ITEM_OCARINA_FAIRY;
        return ITEM_NONE;
    } else if (item == ITEM_OCARINA_TIME) {
        INV_CONTENT(ITEM_OCARINA_TIME) = ITEM_OCARINA_TIME;
        for (i = 1; i < 4; i++) {
            if (GET_EQUIPS_BUTTONITEMS_A0(i) == ITEM_OCARINA_FAIRY) {
                SET_EQUIPS_BUTTONITEMS_A0(i, ITEM_OCARINA_TIME);
                Interface_LoadItemIcon1(play, i);
            }
        }
        return ITEM_NONE;
    } else if (item == ITEM_BEAN) {
        if (GET_INVENTORY_ITEMS_A0(slot) == ITEM_NONE) {
            INV_CONTENT(item) = item;
            AMMO(ITEM_BEAN) = 1;
            BEANS_BOUGHT = 1;
        } else {
            AMMO(ITEM_BEAN)++;
            BEANS_BOUGHT++;
        }
        return ITEM_NONE;
    } else if ((item == ITEM_HEART_PIECE_2) || (item == ITEM_HEART_PIECE)) {
        GET_INVENTORY_QUESTITEMS += 1 << QUEST_HEART_PIECE_COUNT;
        return ITEM_NONE;
    } else if (item == ITEM_HEART_CONTAINER) {
        GET_HEALTHCAPACITY += 0x10;
        GET_HEALTH += 0x10;
        return ITEM_NONE;
    } else if (item == ITEM_RECOVERY_HEART) {
        osSyncPrintf("回復ハート回復ハート回復ハート\n"); // "Recovery Heart"
        Health_ChangeBy(play, 0x10);
        return item;
    } else if (item == ITEM_MAGIC_SMALL) {
        if (GET_MAGICSTATE != MAGIC_STATE_ADD) {
            // This function is only used to store the magicState.
            // Setting the state to FILL gets immediately overwritten in Magic_RequestChange.
            // I.e. magic is added not filled
            Magic_Fill(play);
        }

        Magic_RequestChange(play, 12, MAGIC_ADD);

        if (!GET_INFTABLE(INFTABLE_198)) {
            SET_INFTABLE(INFTABLE_198);
            return ITEM_NONE;
        }

        return item;
    } else if (item == ITEM_MAGIC_LARGE) {
        if (GET_MAGICSTATE != MAGIC_STATE_ADD) {
            // This function is only used to store the magicState.
            // Setting the state to FILL gets immediately overwritten in Magic_RequestChange.
            // I.e. magic is added not filled.
            Magic_Fill(play);
        }

        Magic_RequestChange(play, 24, MAGIC_ADD);

        if (!GET_INFTABLE(INFTABLE_198)) {
            SET_INFTABLE(INFTABLE_198);
            return ITEM_NONE;
        }

        return item;
    } else if ((item >= ITEM_RUPEE_GREEN) && (item <= ITEM_INVALID_8)) {
        Rupees_ChangeBy(sRupeeRefillCounts[item - ITEM_RUPEE_GREEN]);
        return ITEM_NONE;
    } else if (item == ITEM_BOTTLE) {
        temp = SLOT(item);

        for (i = 0; i < 4; i++) {
            if (GET_INVENTORY_ITEMS_A0(temp+i) == ITEM_NONE) {
                SET_INVENTORY_ITEMS_A0(temp+i, item);
                return ITEM_NONE;
            }
        }
    } else if (((item >= ITEM_POTION_RED) && (item <= ITEM_POE)) || (item == ITEM_MILK)) {
        temp = SLOT(item);

        if ((item != ITEM_MILK_BOTTLE) && (item != ITEM_LETTER_RUTO)) {
            if (item == ITEM_MILK) {
                item = ITEM_MILK_BOTTLE;
                temp = SLOT(item);
            }

            for (i = 0; i < 4; i++) {
                if (GET_INVENTORY_ITEMS_A0(temp+i) == ITEM_BOTTLE) {
                    // "Item_Pt(1)=%d Item_Pt(2)=%d Item_Pt(3)=%d   Empty Bottle=%d   Content=%d"
                    osSyncPrintf("Item_Pt(1)=%d Item_Pt(2)=%d Item_Pt(3)=%d   空瓶=%d   中味=%d\n",
                                 GET_EQUIPS_CBUTTONSLOTS_A0(0),
                                 GET_EQUIPS_CBUTTONSLOTS_A0(1),
                                 GET_EQUIPS_CBUTTONSLOTS_A0(2), temp + i, item);

                    if ((temp + i) == GET_EQUIPS_CBUTTONSLOTS_A0(0)) {
                        SET_EQUIPS_BUTTONITEMS_A0(1, item);
                        Interface_LoadItemIcon2(play, 1);
                        SET_BUTTONSTATUS_A0(1, BTN_ENABLED);
                    } else if ((temp + i) == GET_EQUIPS_CBUTTONSLOTS_A0(1)) {
                        SET_EQUIPS_BUTTONITEMS_A0(2, item);
                        Interface_LoadItemIcon2(play, 2);
                        SET_BUTTONSTATUS_A0(2, BTN_ENABLED);
                    } else if ((temp + i) == GET_EQUIPS_CBUTTONSLOTS_A0(2)) {
                        SET_EQUIPS_BUTTONITEMS_A0(3, item);
                        Interface_LoadItemIcon1(play, 3);
                        SET_BUTTONSTATUS_A0(3, BTN_ENABLED);
                    }

                    SET_INVENTORY_ITEMS_A0(temp+i, item);
                    return ITEM_NONE;
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (GET_INVENTORY_ITEMS_A0(temp+i) == ITEM_NONE) {
                    SET_INVENTORY_ITEMS_A0(temp+i, item);
                    return ITEM_NONE;
                }
            }
        }
    } else if ((item >= ITEM_WEIRD_EGG) && (item <= ITEM_CLAIM_CHECK)) {
        if (item == ITEM_SAW) {
            SET_ITEMGETINF(ITEMGETINF_1F);
        }

        temp = INV_CONTENT(item);
        INV_CONTENT(item) = item;

        if (temp != ITEM_NONE) {
            for (i = 1; i < 4; i++) {
                if (temp == GET_EQUIPS_BUTTONITEMS_A0(i)) {
                    if (item != ITEM_SOLD_OUT) {
                        SET_EQUIPS_BUTTONITEMS_A0(i, item);
                        Interface_LoadItemIcon1(play, i);
                    } else {
                        SET_EQUIPS_BUTTONITEMS_A0(i, ITEM_NONE);
                    }
                    return ITEM_NONE;
                }
            }
        }

        return ITEM_NONE;
    }

    temp = GET_INVENTORY_ITEMS_A0(slot);
    osSyncPrintf("Item_Register(%d)=%d  %d\n", slot, item, temp);
    INV_CONTENT(item) = item;

    return temp;
}

u8 Item_CheckObtainability(u8 item) {
    s16 i;
    s16 slot = SLOT(item);
    s32 temp;

    if (item >= ITEM_STICKS_5) {
        slot = SLOT(sExtraItemBases[item - ITEM_STICKS_5]);
    }

    osSyncPrintf(VT_FGCOL(GREEN));
    osSyncPrintf("item_get_non_setting=%d  pt=%d  z=%x\n", item, slot, GET_INVENTORY_ITEMS_A0(slot));
    osSyncPrintf(VT_RST);

    if ((item >= ITEM_MEDALLION_FOREST) && (item <= ITEM_MEDALLION_LIGHT)) {
        return ITEM_NONE;
    } else if ((item >= ITEM_KOKIRI_EMERALD) && (item <= ITEM_SKULL_TOKEN)) {
        return ITEM_NONE;
    } else if ((item >= ITEM_SWORD_KOKIRI) && (item <= ITEM_SWORD_BGS)) {
        if (item == ITEM_SWORD_BGS) {
            return ITEM_NONE;
        } else if (CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, item - ITEM_SWORD_KOKIRI + EQUIP_INV_SWORD_KOKIRI)) {
            return item;
        } else {
            return ITEM_NONE;
        }
    } else if ((item >= ITEM_SHIELD_DEKU) && (item <= ITEM_SHIELD_MIRROR)) {
        if (CHECK_OWNED_EQUIP(EQUIP_TYPE_SHIELD, item - ITEM_SHIELD_DEKU + EQUIP_INV_SHIELD_DEKU)) {
            return item;
        } else {
            return ITEM_NONE;
        }
    } else if ((item >= ITEM_TUNIC_KOKIRI) && (item <= ITEM_TUNIC_ZORA)) {
        if (CHECK_OWNED_EQUIP(EQUIP_TYPE_TUNIC, item - ITEM_TUNIC_KOKIRI + EQUIP_INV_TUNIC_KOKIRI)) {
            return item;
        } else {
            return ITEM_NONE;
        }
    } else if ((item >= ITEM_BOOTS_KOKIRI) && (item <= ITEM_BOOTS_HOVER)) {
        if (CHECK_OWNED_EQUIP(EQUIP_TYPE_BOOTS, item - ITEM_BOOTS_KOKIRI + EQUIP_INV_BOOTS_KOKIRI)) {
            return item;
        } else {
            return ITEM_NONE;
        }
    } else if ((item == ITEM_KEY_BOSS) || (item == ITEM_COMPASS) || (item == ITEM_DUNGEON_MAP)) {
        return ITEM_NONE;
    } else if (item == ITEM_KEY_SMALL) {
        return ITEM_NONE;
    } else if ((item >= ITEM_SLINGSHOT) && (item <= ITEM_BOMBCHU)) {
        return ITEM_NONE;
    } else if ((item == ITEM_BOMBCHUS_5) || (item == ITEM_BOMBCHUS_20)) {
        return ITEM_NONE;
    } else if ((item == ITEM_QUIVER_30) || (item == ITEM_BOW)) {
        if (CUR_UPG_VALUE(UPG_QUIVER) == 0) {
            return ITEM_NONE;
        } else {
            return 0;
        }
    } else if ((item == ITEM_QUIVER_40) || (item == ITEM_QUIVER_50)) {
        return ITEM_NONE;
    } else if ((item == ITEM_BULLET_BAG_40) || (item == ITEM_BULLET_BAG_50)) {
        return ITEM_NONE;
    } else if ((item == ITEM_BOMB_BAG_20) || (item == ITEM_BOMB)) {
        if (CUR_UPG_VALUE(UPG_BOMB_BAG) == 0) {
            return ITEM_NONE;
        } else {
            return 0;
        }
    } else if ((item >= ITEM_STICK_UPGRADE_20) && (item <= ITEM_NUT_UPGRADE_40)) {
        return ITEM_NONE;
    } else if ((item >= ITEM_BOMB_BAG_30) && (item <= ITEM_WALLET_GIANT)) {
        return ITEM_NONE;
    } else if (item == ITEM_LONGSHOT) {
        return ITEM_NONE;
    } else if ((item == ITEM_SEEDS) || (item == ITEM_SEEDS_30)) {
        if (!GET_ITEMGETINF(ITEMGETINF_13)) {
            return ITEM_NONE;
        } else {
            return ITEM_SEEDS;
        }
    } else if (item == ITEM_BEAN) {
        return ITEM_NONE;
    } else if ((item == ITEM_HEART_PIECE_2) || (item == ITEM_HEART_PIECE)) {
        return ITEM_NONE;
    } else if (item == ITEM_HEART_CONTAINER) {
        return ITEM_NONE;
    } else if (item == ITEM_RECOVERY_HEART) {
        return ITEM_RECOVERY_HEART;
    } else if ((item == ITEM_MAGIC_SMALL) || (item == ITEM_MAGIC_LARGE)) {
        // "Magic Pot Get_Inf_Table( 25, 0x0100)=%d"
        osSyncPrintf("魔法の壷 Get_Inf_Table( 25, 0x0100)=%d\n", GET_INFTABLE(INFTABLE_198));
        if (!GET_INFTABLE(INFTABLE_198)) {
            return ITEM_NONE;
        } else {
            return item;
        }
    } else if ((item >= ITEM_RUPEE_GREEN) && (item <= ITEM_INVALID_8)) {
        return ITEM_NONE;
    } else if (item == ITEM_BOTTLE) {
        return ITEM_NONE;
    } else if (((item >= ITEM_POTION_RED) && (item <= ITEM_POE)) || (item == ITEM_MILK)) {
        temp = SLOT(item);

        if ((item != ITEM_MILK_BOTTLE) && (item != ITEM_LETTER_RUTO)) {
            if (item == ITEM_MILK) {
                item = ITEM_MILK_BOTTLE;
                temp = SLOT(item);
            }

            for (i = 0; i < 4; i++) {
                if (GET_INVENTORY_ITEMS_A0(temp+i) == ITEM_BOTTLE) {
                    return ITEM_NONE;
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (GET_INVENTORY_ITEMS_A0(temp+i) == ITEM_NONE) {
                    return ITEM_NONE;
                }
            }
        }
    } else if ((item >= ITEM_WEIRD_EGG) && (item <= ITEM_CLAIM_CHECK)) {
        return ITEM_NONE;
    }

    return GET_INVENTORY_ITEMS_A0(slot);
}

void Inventory_DeleteItem(u16 item, u16 invSlot) {
    s16 i;

    if (item == ITEM_BEAN) {
        BEANS_BOUGHT = 0;
    }

    SET_INVENTORY_ITEMS_A0(invSlot, ITEM_NONE);

    osSyncPrintf("\nItem_Register(%d)\n", invSlot, GET_INVENTORY_ITEMS_A0(invSlot));

    for (i = 1; i < 4; i++) {
        if (GET_EQUIPS_BUTTONITEMS_A0(i) == item) {
            SET_EQUIPS_BUTTONITEMS_A0(i, ITEM_NONE);
            SET_EQUIPS_CBUTTONSLOTS_A0(i-1, SLOT_NONE);
        }
    }
}

s32 Inventory_ReplaceItem(PlayState* play, u16 oldItem, u16 newItem) {
    s16 i;

    for (i = 0; i < ARRAY_COUNT(GET_INVENTORY_ITEMS); i++) {
        if (GET_INVENTORY_ITEMS_A0(i) == oldItem) {
            SET_INVENTORY_ITEMS_A0(i, newItem);
            osSyncPrintf("アイテム消去(%d)\n", i); // "Item Purge (%d)"
            for (i = 1; i < 4; i++) {
                if (GET_EQUIPS_BUTTONITEMS_A0(i) == oldItem) {
                    SET_EQUIPS_BUTTONITEMS_A0(i, newItem);
                    Interface_LoadItemIcon1(play, i);
                    break;
                }
            }
            return true;
        }
    }

    return false;
}

s32 Inventory_HasEmptyBottle(void) {
    u8* items = GET_INVENTORY_ITEMS;

    if (items[SLOT_BOTTLE_1] == ITEM_BOTTLE) {
        return true;
    } else if (items[SLOT_BOTTLE_2] == ITEM_BOTTLE) {
        return true;
    } else if (items[SLOT_BOTTLE_3] == ITEM_BOTTLE) {
        return true;
    } else if (items[SLOT_BOTTLE_4] == ITEM_BOTTLE) {
        return true;
    } else {
        return false;
    }
}

s32 Inventory_HasSpecificBottle(u8 bottleItem) {
    u8* items = GET_INVENTORY_ITEMS;

    if (items[SLOT_BOTTLE_1] == bottleItem) {
        return true;
    } else if (items[SLOT_BOTTLE_2] == bottleItem) {
        return true;
    } else if (items[SLOT_BOTTLE_3] == bottleItem) {
        return true;
    } else if (items[SLOT_BOTTLE_4] == bottleItem) {
        return true;
    } else {
        return false;
    }
}

void Inventory_UpdateBottleItem(PlayState* play, u8 item, u8 button) {
    osSyncPrintf("item_no=%x,  c_no=%x,  Pt=%x  Item_Register=%x\n", item, button,
                 GET_EQUIPS_CBUTTONSLOTS_A0(button-1),
                 GET_INVENTORY_ITEMS[GET_EQUIPS_CBUTTONSLOTS_A0(button-1)]);

    // Special case to only empty half of a Lon Lon Milk Bottle
    if ((GET_INVENTORY_ITEMS[GET_EQUIPS_CBUTTONSLOTS_A0(button-1)] ==
         ITEM_MILK_BOTTLE) &&
        (item == ITEM_BOTTLE)) {
        item = ITEM_MILK_HALF;
    }

    GET_INVENTORY_ITEMS[GET_EQUIPS_CBUTTONSLOTS_A0(button-1)] = item;
    SET_EQUIPS_BUTTONITEMS_A0(button, item);

    Interface_LoadItemIcon1(play, button);

    play->pauseCtx.cursorItem[PAUSE_ITEM] = item;
    SET_BUTTONSTATUS_A0(button, BTN_ENABLED);
}

s32 Inventory_ConsumeFairy(PlayState* play) {
    s32 bottleSlot = SLOT(ITEM_FAIRY);
    s16 i;
    s16 j;

    for (i = 0; i < 4; i++) {
        if (GET_INVENTORY_ITEMS_A0(bottleSlot+i) == ITEM_FAIRY) {
            for (j = 1; j < 4; j++) {
                if (GET_EQUIPS_BUTTONITEMS_A0(j) == ITEM_FAIRY) {
                    SET_EQUIPS_BUTTONITEMS_A0(j, ITEM_BOTTLE);
                    Interface_LoadItemIcon1(play, j);
                    i = 0;
                    bottleSlot = GET_EQUIPS_CBUTTONSLOTS_A0(j-1);
                    break;
                }
            }
            osSyncPrintf("妖精使用＝%d\n", bottleSlot); // "Fairy Usage＝%d"
            SET_INVENTORY_ITEMS_A0(bottleSlot+i, ITEM_BOTTLE);
            return true;
        }
    }

    return false;
}

void func_80086D5C(s32* buf, u16 size) {
    u16 i;

    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}

void Interface_LoadActionLabel(InterfaceContext* interfaceCtx, u16 action, s16 loadOffset) {
    static void* sDoActionTextures[] = { gAttackDoActionENGTex, gCheckDoActionENGTex };

    if (action >= DO_ACTION_MAX) {
        action = DO_ACTION_NONE;
    }

    if (GET_LANGUAGE != LANGUAGE_ENG) {
        action += DO_ACTION_MAX;
    }

    if (GET_LANGUAGE == LANGUAGE_FRA) {
        action += DO_ACTION_MAX;
    }

    if ((action != DO_ACTION_NONE) && (action != DO_ACTION_MAX + DO_ACTION_NONE) &&
        (action != 2 * DO_ACTION_MAX + DO_ACTION_NONE)) {
        osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, 1);
        DmaMgr_SendRequest2(&interfaceCtx->dmaRequest_160,
                            interfaceCtx->doActionSegment + (loadOffset * DO_ACTION_TEX_SIZE),
                            (uintptr_t)_do_action_staticSegmentRomStart + (action * DO_ACTION_TEX_SIZE),
                            DO_ACTION_TEX_SIZE, 0, &interfaceCtx->loadQueue, NULL, "../z_parameter.c", 2145);
        osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);
    } else {
        gSegments[7] = VIRTUAL_TO_PHYSICAL(interfaceCtx->doActionSegment);
        func_80086D5C(SEGMENTED_TO_VIRTUAL(sDoActionTextures[loadOffset]), DO_ACTION_TEX_SIZE / 4);
    }
}

void Interface_SetDoAction(PlayState* play, u16 action) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    PauseContext* pauseCtx = &play->pauseCtx;

    if (interfaceCtx->unk_1F0 != action) {
        interfaceCtx->unk_1F0 = action;
        interfaceCtx->unk_1EC = 1;
        interfaceCtx->unk_1F4 = 0.0f;
        Interface_LoadActionLabel(interfaceCtx, action, 1);
        if (pauseCtx->state != 0) {
            interfaceCtx->unk_1EC = 3;
        }
    }
}

void Interface_SetNaviCall(PlayState* play, u16 naviCallState) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (((naviCallState == 0x1D) || (naviCallState == 0x1E)) && !interfaceCtx->naviCalling &&
        (play->csCtx.state == CS_STATE_IDLE)) {
        // clang-format off
        if (naviCallState == 0x1E) { Audio_PlaySfxGeneral(NA_SE_VO_NAVY_CALL, &gSfxDefaultPos, 4,
                                                            &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                            &gSfxDefaultReverb);
        }
        // clang-format on

        if (naviCallState == 0x1D) {
            func_800F4524(&gSfxDefaultPos, NA_SE_VO_NA_HELLO_2, 32);
        }

        interfaceCtx->naviCalling = true;
        sCUpInvisible = 0;
        sCUpTimer = 10;
    } else if ((naviCallState == 0x1F) && interfaceCtx->naviCalling) {
        interfaceCtx->naviCalling = false;
    }
}

void Interface_LoadActionLabelB(PlayState* play, u16 action) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    if (GET_LANGUAGE != LANGUAGE_ENG) {
        action += DO_ACTION_MAX;
    }

    if (GET_LANGUAGE == LANGUAGE_FRA) {
        action += DO_ACTION_MAX;
    }

    interfaceCtx->unk_1FC = action;

    osCreateMesgQueue(&interfaceCtx->loadQueue, &interfaceCtx->loadMsg, 1);
    DmaMgr_SendRequest2(&interfaceCtx->dmaRequest_160, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE,
                        (uintptr_t)_do_action_staticSegmentRomStart + (action * DO_ACTION_TEX_SIZE), DO_ACTION_TEX_SIZE,
                        0, &interfaceCtx->loadQueue, NULL, "../z_parameter.c", 2228);
    osRecvMesg(&interfaceCtx->loadQueue, NULL, OS_MESG_BLOCK);

    interfaceCtx->unk_1FA = true;
}

/**
 * @return false if player is out of health
 */
s32 Health_ChangeBy(PlayState* play, s16 amount) {
    u16 heartCount;
    u16 healthLevel;

    // "＊＊＊＊＊ Fluctuation=%d (now=%d, max=%d) ＊＊＊"
    osSyncPrintf("＊＊＊＊＊  増減=%d (now=%d, max=%d)  ＊＊＊", amount, GET_HEALTH,
                 GET_HEALTHCAPACITY);

    // clang-format off
    if (amount > 0) { Audio_PlaySfxGeneral(NA_SE_SY_HP_RECOVER, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
    } else if (GET_ISDOUBLEDEFENSEACQUIRED && (amount < 0)) {
        amount >>= 1;
        osSyncPrintf("ハート減少半分！！＝%d\n", amount); // "Heart decrease halved!!＝%d"
    }
    // clang-format on

    GET_HEALTH += amount;

    if (GET_HEALTH > GET_HEALTHCAPACITY) {
        SET_HEALTH(GET_HEALTHCAPACITY);
    }

    heartCount = GET_HEALTH % 0x10;

    healthLevel = heartCount;
    if (heartCount != 0) {
        if (heartCount > 10) {
            healthLevel = 3;
        } else if (heartCount > 5) {
            healthLevel = 2;
        } else {
            healthLevel = 1;
        }
    }

    // "Life=%d ＊＊＊  %d ＊＊＊＊＊＊"
    osSyncPrintf("  ライフ=%d  ＊＊＊  %d  ＊＊＊＊＊＊\n", GET_HEALTH, healthLevel);

    if (GET_HEALTH <= 0) {
        SET_HEALTH(0);
        return false;
    } else {
        return true;
    }
}

void Health_GiveHearts(s16 hearts) {
    GET_HEALTHCAPACITY += hearts * 0x10;
}

void Rupees_ChangeBy(s16 rupeeChange) {
    GET_RUPEEACCUMULATOR += rupeeChange;
}

void Inventory_ChangeAmmo(s16 item, s16 ammoChange) {
    // "Item = (%d)    Amount = (%d + %d)"
    osSyncPrintf("アイテム = (%d)    数 = (%d + %d)  ", item, AMMO(item), ammoChange);

    if (item == ITEM_STICK) {
        AMMO(ITEM_STICK) += ammoChange;

        if (AMMO(ITEM_STICK) >= CUR_CAPACITY(UPG_STICKS)) {
            AMMO(ITEM_STICK) = CUR_CAPACITY(UPG_STICKS);
        } else if (AMMO(ITEM_STICK) < 0) {
            AMMO(ITEM_STICK) = 0;
        }
    } else if (item == ITEM_NUT) {
        AMMO(ITEM_NUT) += ammoChange;

        if (AMMO(ITEM_NUT) >= CUR_CAPACITY(UPG_NUTS)) {
            AMMO(ITEM_NUT) = CUR_CAPACITY(UPG_NUTS);
        } else if (AMMO(ITEM_NUT) < 0) {
            AMMO(ITEM_NUT) = 0;
        }
    } else if (item == ITEM_BOMBCHU) {
        AMMO(ITEM_BOMBCHU) += ammoChange;

        if (AMMO(ITEM_BOMBCHU) >= 50) {
            AMMO(ITEM_BOMBCHU) = 50;
        } else if (AMMO(ITEM_BOMBCHU) < 0) {
            AMMO(ITEM_BOMBCHU) = 0;
        }
    } else if (item == ITEM_BOW) {
        AMMO(ITEM_BOW) += ammoChange;

        if (AMMO(ITEM_BOW) >= CUR_CAPACITY(UPG_QUIVER)) {
            AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
        } else if (AMMO(ITEM_BOW) < 0) {
            AMMO(ITEM_BOW) = 0;
        }
    } else if ((item == ITEM_SLINGSHOT) || (item == ITEM_SEEDS)) {
        AMMO(ITEM_SLINGSHOT) += ammoChange;

        if (AMMO(ITEM_SLINGSHOT) >= CUR_CAPACITY(UPG_BULLET_BAG)) {
            AMMO(ITEM_SLINGSHOT) = CUR_CAPACITY(UPG_BULLET_BAG);
        } else if (AMMO(ITEM_SLINGSHOT) < 0) {
            AMMO(ITEM_SLINGSHOT) = 0;
        }
    } else if (item == ITEM_BOMB) {
        AMMO(ITEM_BOMB) += ammoChange;

        if (AMMO(ITEM_BOMB) >= CUR_CAPACITY(UPG_BOMB_BAG)) {
            AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
        } else if (AMMO(ITEM_BOMB) < 0) {
            AMMO(ITEM_BOMB) = 0;
        }
    } else if (item == ITEM_BEAN) {
        AMMO(ITEM_BEAN) += ammoChange;
    }

    osSyncPrintf("合計 = (%d)\n", AMMO(item)); // "Total = (%d)"
}

void Magic_Fill(PlayState* play) {
    if (GET_ISMAGICACQUIRED) {
        SET_PREVMAGICSTATE(GET_MAGICSTATE);
        SET_MAGICFILLTARGET((GET_ISDOUBLEMAGICACQUIRED + 1) * MAGIC_NORMAL_METER);
        SET_MAGICSTATE(MAGIC_STATE_FILL);
    }
}

void Magic_Reset(PlayState* play) {
    if ((GET_MAGICSTATE != MAGIC_STATE_STEP_CAPACITY) && (GET_MAGICSTATE != MAGIC_STATE_FILL)) {
        if (GET_MAGICSTATE == MAGIC_STATE_ADD) {
            SET_PREVMAGICSTATE(GET_MAGICSTATE);
        }
        SET_MAGICSTATE(MAGIC_STATE_RESET);
    }
}

/**
 * Request to either increase or consume magic.
 * @param amount the positive-valued amount to either increase or decrease magic by
 * @param type how the magic is increased or consumed.
 * @return false if the request failed
 */
s32 Magic_RequestChange(PlayState* play, s16 amount, s16 type) {
    if (!GET_ISMAGICACQUIRED) {
        return false;
    }

    if ((type != MAGIC_ADD) && (GET_MAGIC - amount) < 0) {
        if (GET_MAGICCAPACITY != 0) {
            Audio_PlaySfxGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        }
        return false;
    }

    switch (type) {
        case MAGIC_CONSUME_NOW:
        case MAGIC_CONSUME_NOW_ALT:
            // Consume magic immediately
            if ((GET_MAGICSTATE == MAGIC_STATE_IDLE) ||
                (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS)) {
                if (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS) {
                    play->actorCtx.lensActive = false;
                }
                SET_MAGICTARGET(GET_MAGIC - amount);
                SET_MAGICSTATE(MAGIC_STATE_CONSUME_SETUP);
                return true;
            } else {
                Audio_PlaySfxGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                return false;
            }

        case MAGIC_CONSUME_WAIT_NO_PREVIEW:
            // Sets consume target but waits to consume.
            // No yellow magic to preview target consumption.
            // Unused
            if ((GET_MAGICSTATE == MAGIC_STATE_IDLE) ||
                (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS)) {
                if (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS) {
                    play->actorCtx.lensActive = false;
                }
                SET_MAGICTARGET(GET_MAGIC - amount);
                SET_MAGICSTATE(MAGIC_STATE_METER_FLASH_3);
                return true;
            } else {
                Audio_PlaySfxGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                return false;
            }

        case MAGIC_CONSUME_LENS:
            if (GET_MAGICSTATE == MAGIC_STATE_IDLE) {
                if (GET_MAGIC != 0) {
                    play->interfaceCtx.lensMagicConsumptionTimer = 80;
                    SET_MAGICSTATE(MAGIC_STATE_CONSUME_LENS);
                    return true;
                } else {
                    return false;
                }
            } else if (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS) {
                return true;
            } else {
                return false;
            }

        case MAGIC_CONSUME_WAIT_PREVIEW:
            // Sets consume target but waits to consume.
            // Preview consumption with a yellow bar
            if ((GET_MAGICSTATE == MAGIC_STATE_IDLE) ||
                (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS)) {
                if (GET_MAGICSTATE == MAGIC_STATE_CONSUME_LENS) {
                    play->actorCtx.lensActive = false;
                }
                SET_MAGICTARGET(GET_MAGIC - amount);
                SET_MAGICSTATE(MAGIC_STATE_METER_FLASH_2);
                return true;
            } else {
                Audio_PlaySfxGeneral(NA_SE_SY_ERROR, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                return false;
            }

        case MAGIC_ADD:
            // Sets target for magic to increase to
            if (GET_MAGIC <= GET_MAGICCAPACITY) {
                SET_MAGICTARGET(GET_MAGIC + amount);

                if (GET_MAGICTARGET >= GET_MAGICCAPACITY) {
                    SET_MAGICTARGET(GET_MAGICCAPACITY);
                }

                SET_MAGICSTATE(MAGIC_STATE_ADD);
                return true;
            }
            break;
    }

    return false;
}

void Magic_Update(PlayState* play) {
    static s16 sMagicBorderColors[][3] = {
        { 255, 255, 255 },
        { 150, 150, 150 },
        { 255, 255, 150 }, // unused
        { 255, 255, 50 },  // unused
    };
    static s16 sMagicBorderIndices[] = { 0, 1, 1, 0 };
    static s16 sMagicBorderRatio = 2;
    static s16 sMagicBorderStep = 1;
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 borderChangeR;
    s16 borderChangeG;
    s16 borderChangeB;
    s16 temp; // target for magicCapacity, or magicBorderIndex

    switch (GET_MAGICSTATE) {
        case MAGIC_STATE_STEP_CAPACITY:
            // Step magicCapacity to the capacity determined by magicLevel
            // This changes the width of the magic meter drawn
            temp = GET_MAGICLEVEL * MAGIC_NORMAL_METER;
            if (GET_MAGICCAPACITY != temp) {
                if (GET_MAGICCAPACITY < temp) {
                    GET_MAGICCAPACITY += 8;
                    if (GET_MAGICCAPACITY > temp) {
                        SET_MAGICCAPACITY(temp);
                    }
                } else {
                    GET_MAGICCAPACITY -= 8;
                    if (GET_MAGICCAPACITY <= temp) {
                        SET_MAGICCAPACITY(temp);
                    }
                }
            } else {
                // Once the capacity has reached its target,
                // follow up by filling magic to magicFillTarget
                SET_MAGICSTATE(MAGIC_STATE_FILL);
            }
            break;

        case MAGIC_STATE_FILL:
            // Add magic until magicFillTarget is reached
            GET_MAGIC += 4;

            if (GET_GAMEMODE == GAMEMODE_NORMAL && !IS_CUTSCENE_LAYER) {
                Audio_PlaySfxGeneral(NA_SE_SY_GAUGE_UP - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }

            // "Storage  MAGIC_NOW=%d (%d)"
            osSyncPrintf("蓄電  MAGIC_NOW=%d (%d)\n", GET_MAGIC,
                         GET_MAGICFILLTARGET);

            if (GET_MAGIC >= GET_MAGICFILLTARGET) {
                SET_MAGIC(GET_MAGICFILLTARGET);
                SET_MAGICSTATE(GET_PREVMAGICSTATE);
                SET_PREVMAGICSTATE(MAGIC_STATE_IDLE);
            }
            break;

        case MAGIC_STATE_CONSUME_SETUP:
            // Sets the speed at which magic border flashes
            sMagicBorderRatio = 2;
            SET_MAGICSTATE(MAGIC_STATE_CONSUME);
            break;

        case MAGIC_STATE_CONSUME:
            // Consume magic until target is reached or no more magic is available
            GET_MAGIC -= 2;
            if (GET_MAGIC <= 0) {
                SET_MAGIC(0);
                SET_MAGICSTATE(MAGIC_STATE_METER_FLASH_1);
                sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            } else if (GET_MAGIC == GET_MAGICTARGET) {
                SET_MAGICSTATE(MAGIC_STATE_METER_FLASH_1);
                sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            }
            FALLTHROUGH; // Flash border while magic is being consumed
        case MAGIC_STATE_METER_FLASH_1:
        case MAGIC_STATE_METER_FLASH_2:
        case MAGIC_STATE_METER_FLASH_3:
            temp = sMagicBorderIndices[sMagicBorderStep];
            borderChangeR = ABS(sMagicBorderR - sMagicBorderColors[temp][0]) / sMagicBorderRatio;
            borderChangeG = ABS(sMagicBorderG - sMagicBorderColors[temp][1]) / sMagicBorderRatio;
            borderChangeB = ABS(sMagicBorderB - sMagicBorderColors[temp][2]) / sMagicBorderRatio;

            if (sMagicBorderR >= sMagicBorderColors[temp][0]) {
                sMagicBorderR -= borderChangeR;
            } else {
                sMagicBorderR += borderChangeR;
            }

            if (sMagicBorderG >= sMagicBorderColors[temp][1]) {
                sMagicBorderG -= borderChangeG;
            } else {
                sMagicBorderG += borderChangeG;
            }

            if (sMagicBorderB >= sMagicBorderColors[temp][2]) {
                sMagicBorderB -= borderChangeB;
            } else {
                sMagicBorderB += borderChangeB;
            }

            sMagicBorderRatio--;
            if (sMagicBorderRatio == 0) {
                sMagicBorderR = sMagicBorderColors[temp][0];
                sMagicBorderG = sMagicBorderColors[temp][1];
                sMagicBorderB = sMagicBorderColors[temp][2];
                sMagicBorderRatio = YREG(40 + sMagicBorderStep);
                sMagicBorderStep++;
                if (sMagicBorderStep >= 4) {
                    sMagicBorderStep = 0;
                }
            }
            break;

        case MAGIC_STATE_RESET:
            sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
            SET_MAGICSTATE(MAGIC_STATE_IDLE);
            break;

        case MAGIC_STATE_CONSUME_LENS:
            // Slowly consume magic while lens is on
            if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) && (msgCtx->msgMode == MSGMODE_NONE) &&
                (play->gameOverCtx.state == GAMEOVER_INACTIVE) && (play->transitionTrigger == TRANS_TRIGGER_OFF) &&
                (play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play)) {
                if ((GET_MAGIC == 0) ||
                    ((func_8008F2F8(play) >= 2) && (func_8008F2F8(play) < 5)) ||
                    ((GET_EQUIPS_BUTTONITEMS_A0(1) != ITEM_LENS) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(2) != ITEM_LENS) &&
                     (GET_EQUIPS_BUTTONITEMS_A0(3) != ITEM_LENS)) ||
                    !play->actorCtx.lensActive) {
                    // Force lens off and set magic meter state to idle
                    play->actorCtx.lensActive = false;
                    Audio_PlaySfxGeneral(NA_SE_SY_GLASSMODE_OFF, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                    SET_MAGICSTATE(MAGIC_STATE_IDLE);
                    sMagicBorderR = sMagicBorderG = sMagicBorderB = 255;
                    break;
                }

                interfaceCtx->lensMagicConsumptionTimer--;
                if (interfaceCtx->lensMagicConsumptionTimer == 0) {
                    GET_MAGIC--;
                    interfaceCtx->lensMagicConsumptionTimer = 80;
                }
            }

            temp = sMagicBorderIndices[sMagicBorderStep];
            borderChangeR = ABS(sMagicBorderR - sMagicBorderColors[temp][0]) / sMagicBorderRatio;
            borderChangeG = ABS(sMagicBorderG - sMagicBorderColors[temp][1]) / sMagicBorderRatio;
            borderChangeB = ABS(sMagicBorderB - sMagicBorderColors[temp][2]) / sMagicBorderRatio;

            if (sMagicBorderR >= sMagicBorderColors[temp][0]) {
                sMagicBorderR -= borderChangeR;
            } else {
                sMagicBorderR += borderChangeR;
            }

            if (sMagicBorderG >= sMagicBorderColors[temp][1]) {
                sMagicBorderG -= borderChangeG;
            } else {
                sMagicBorderG += borderChangeG;
            }

            if (sMagicBorderB >= sMagicBorderColors[temp][2]) {
                sMagicBorderB -= borderChangeB;
            } else {
                sMagicBorderB += borderChangeB;
            }

            sMagicBorderRatio--;
            if (sMagicBorderRatio == 0) {
                sMagicBorderR = sMagicBorderColors[temp][0];
                sMagicBorderG = sMagicBorderColors[temp][1];
                sMagicBorderB = sMagicBorderColors[temp][2];
                sMagicBorderRatio = YREG(40 + sMagicBorderStep);
                sMagicBorderStep++;
                if (sMagicBorderStep >= 4) {
                    sMagicBorderStep = 0;
                }
            }
            break;

        case MAGIC_STATE_ADD:
            // Add magic until target is reached
            GET_MAGIC += 4;
            Audio_PlaySfxGeneral(NA_SE_SY_GAUGE_UP - SFX_FLAG, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            if (GET_MAGIC >= GET_MAGICTARGET) {
                SET_MAGIC(GET_MAGICTARGET);
                SET_MAGICSTATE(GET_PREVMAGICSTATE);
                SET_PREVMAGICSTATE(MAGIC_STATE_IDLE);
            }
            break;

        default:
            SET_MAGICSTATE(MAGIC_STATE_IDLE);
            break;
    }
}

void Magic_DrawMeter(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 magicMeterY;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 2650);

    if (GET_MAGICLEVEL != 0) {
        if (GET_HEALTHCAPACITY > 0xA0) {
            magicMeterY = R_MAGIC_METER_Y_LOWER; // two rows of hearts
        } else {
            magicMeterY = R_MAGIC_METER_Y_HIGHER; // one row of hearts
        }

        Gfx_SetupDL_39Overlay(play->state.gfxCtx);

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sMagicBorderR, sMagicBorderG, sMagicBorderB, interfaceCtx->magicAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 100, 50, 50, 255);

        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gMagicMeterEndTex, 8, 16, R_MAGIC_METER_X, magicMeterY, 8, 16,
                                      1 << 10, 1 << 10);

        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gMagicMeterMidTex, 24, 16, R_MAGIC_METER_X + 8, magicMeterY,
                                      GET_MAGICCAPACITY, 16, 1 << 10, 1 << 10);

        gDPLoadTextureBlock(OVERLAY_DISP++, gMagicMeterEndTex, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 16, 0,
                            G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 3, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSPTextureRectangle(OVERLAY_DISP++, (R_MAGIC_METER_X + GET_MAGICCAPACITY + 8) << 2, magicMeterY << 2,
                            (R_MAGIC_METER_X + GET_MAGICCAPACITY + 16) << 2, (magicMeterY + 16) << 2,
                            G_TX_RENDERTILE, 256, 0, 1 << 10, 1 << 10);

        gDPPipeSync(OVERLAY_DISP++);
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE, PRIMITIVE,
                          ENVIRONMENT, TEXEL0, ENVIRONMENT, 0, 0, 0, PRIMITIVE);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);

        if (GET_MAGICSTATE == MAGIC_STATE_METER_FLASH_2) {
            // Yellow part of the meter indicating the amount of magic to be subtracted
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 250, 250, 0, interfaceCtx->magicAlpha);

            gDPLoadMultiBlock_4b(OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, 16, 16, 0,
                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + GET_MAGIC) << 2,
                                (magicMeterY + 10) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

            // Fill the rest of the meter with the normal magic color
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_MAGIC_FILL_COLOR(0), R_MAGIC_FILL_COLOR(1), R_MAGIC_FILL_COLOR(2),
                            interfaceCtx->magicAlpha);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + GET_MAGICTARGET) << 2, (magicMeterY + 10) << 2,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        } else {
            // Fill the whole meter with the normal magic color
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_MAGIC_FILL_COLOR(0), R_MAGIC_FILL_COLOR(1), R_MAGIC_FILL_COLOR(2),
                            interfaceCtx->magicAlpha);

            gDPLoadMultiBlock_4b(OVERLAY_DISP++, gMagicMeterFillTex, 0x0000, G_TX_RENDERTILE, G_IM_FMT_I, 16, 16, 0,
                                 G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                 G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_MAGIC_FILL_X << 2, (magicMeterY + 3) << 2,
                                (R_MAGIC_FILL_X + GET_MAGIC) << 2,
                                (magicMeterY + 10) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 2731);
}

void func_80088AA0(s16 arg0) {
    SET_TIMERX_A0(1, 140);
    SET_TIMERY_A0(1, 80);
    D_80125A5C = false;
    SET_TIMER2VALUE(arg0);

    if (arg0 != 0) {
        SET_TIMER2STATE(1);
    } else {
        SET_TIMER2STATE(7);
    }
}

void func_80088AF0(PlayState* play) {
    if (GET_TIMER2STATE != 0) {
        if (GET_EVENTINF(EVENTINF_10)) {
            SET_TIMER2VALUE(239);
        } else {
            SET_TIMER2VALUE(1);
        }
    }
}

void func_80088B34(s16 arg0) {
    SET_TIMERX_A0(0, 140);
    SET_TIMERY_A0(0, 80);
    D_80125A5C = false;
    SET_TIMER1VALUE(arg0);

    if (arg0 != 0) {
        SET_TIMER1STATE(5);
    } else {
        SET_TIMER1STATE(11);
    }
}

void Interface_DrawActionLabel(GraphicsContext* gfxCtx, void* texture) {
    OPEN_DISPS(gfxCtx, "../z_parameter.c", 2820);

    gDPLoadTextureBlock_4b(OVERLAY_DISP++, texture, G_IM_FMT_IA, DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);

    gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);

    CLOSE_DISPS(gfxCtx, "../z_parameter.c", 2829);
}

void Interface_DrawItemButtons(PlayState* play) {
    static void* cUpLabelTextures[] = { gNaviCUpENGTex, gNaviCUpENGTex, gNaviCUpENGTex };
    static s16 startButtonLeftPos[] = { 132, 130, 130 };
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Player* player = GET_PLAYER(play);
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 temp; // Used as both an alpha value and a button index
    s16 texCoordScale;
    s16 width;
    s16 height;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 2900);

    // B Button Color & Texture
    // Also loads the Item Button Texture reused by other buttons afterwards
    gDPPipeSync(OVERLAY_DISP++);
    gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_B_BTN_COLOR(0), R_B_BTN_COLOR(1), R_B_BTN_COLOR(2), interfaceCtx->bAlpha);
    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);
    OVERLAY_DISP =
        Gfx_TextureIA8(OVERLAY_DISP, gButtonBackgroundTex, 32, 32, R_ITEM_BTN_X(0), R_ITEM_BTN_Y(0),
                       R_ITEM_BTN_WIDTH(0), R_ITEM_BTN_WIDTH(0), R_ITEM_BTN_DD(0) << 1, R_ITEM_BTN_DD(0) << 1);

    // C-Left Button Color & Texture
    gDPPipeSync(OVERLAY_DISP++);
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                    interfaceCtx->cLeftAlpha);
    gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(1) << 2, R_ITEM_BTN_Y(1) << 2,
                        (R_ITEM_BTN_X(1) + R_ITEM_BTN_WIDTH(1)) << 2, (R_ITEM_BTN_Y(1) + R_ITEM_BTN_WIDTH(1)) << 2,
                        G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(1) << 1, R_ITEM_BTN_DD(1) << 1);

    // C-Down Button Color & Texture
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                    interfaceCtx->cDownAlpha);
    gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(2) << 2, R_ITEM_BTN_Y(2) << 2,
                        (R_ITEM_BTN_X(2) + R_ITEM_BTN_WIDTH(2)) << 2, (R_ITEM_BTN_Y(2) + R_ITEM_BTN_WIDTH(2)) << 2,
                        G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(2) << 1, R_ITEM_BTN_DD(2) << 1);

    // C-Right Button Color & Texture
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                    interfaceCtx->cRightAlpha);
    gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_BTN_X(3) << 2, R_ITEM_BTN_Y(3) << 2,
                        (R_ITEM_BTN_X(3) + R_ITEM_BTN_WIDTH(3)) << 2, (R_ITEM_BTN_Y(3) + R_ITEM_BTN_WIDTH(3)) << 2,
                        G_TX_RENDERTILE, 0, 0, R_ITEM_BTN_DD(3) << 1, R_ITEM_BTN_DD(3) << 1);

    if ((pauseCtx->state < 8) || (pauseCtx->state >= 18)) {
        if ((play->pauseCtx.state != 0) || (play->pauseCtx.debugState != 0)) {
            // Start Button Texture, Color & Label
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 120, 120, interfaceCtx->startAlpha);
            gSPTextureRectangle(OVERLAY_DISP++, startButtonLeftPos[GET_LANGUAGE] << 2, 17 << 2,
                                (startButtonLeftPos[GET_LANGUAGE] + 22) << 2, 39 << 2, G_TX_RENDERTILE, 0, 0,
                                (s32)(1.4277344 * (1 << 10)), (s32)(1.4277344 * (1 << 10)));
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->startAlpha);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            gDPLoadTextureBlock_4b(OVERLAY_DISP++, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE * 2, G_IM_FMT_IA,
                                   DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            texCoordScale = (1 << 10) / (R_START_LABEL_DD(GET_LANGUAGE) / 100.0f);
            width = DO_ACTION_TEX_WIDTH / (R_START_LABEL_DD(GET_LANGUAGE) / 100.0f);
            height = DO_ACTION_TEX_HEIGHT / (R_START_LABEL_DD(GET_LANGUAGE) / 100.0f);
            gSPTextureRectangle(OVERLAY_DISP++, R_START_LABEL_X(GET_LANGUAGE) << 2,
                                R_START_LABEL_Y(GET_LANGUAGE) << 2,
                                (R_START_LABEL_X(GET_LANGUAGE) + width) << 2,
                                (R_START_LABEL_Y(GET_LANGUAGE) + height) << 2, G_TX_RENDERTILE, 0, 0,
                                texCoordScale, texCoordScale);
        }
    }

    if (interfaceCtx->naviCalling && (play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
        (play->csCtx.state == CS_STATE_IDLE)) {
        if (!sCUpInvisible) {
            // C-Up Button Texture, Color & Label (Navi Text)
            gDPPipeSync(OVERLAY_DISP++);

            if ((GET_UNK_13EA == 1) || (GET_UNK_13EA == 2) || (GET_UNK_13EA == 5)) {
                temp = 0;
            } else if ((player->stateFlags1 & PLAYER_STATE1_21) || (func_8008F2F8(play) == 4) ||
                       (player->stateFlags2 & PLAYER_STATE2_18)) {
                temp = 70;
            } else {
                temp = interfaceCtx->healthAlpha;
            }

            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2), temp);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gSPTextureRectangle(OVERLAY_DISP++, R_C_UP_BTN_X << 2, R_C_UP_BTN_Y << 2, (R_C_UP_BTN_X + 16) << 2,
                                (R_C_UP_BTN_Y + 16) << 2, G_TX_RENDERTILE, 0, 0, 2 << 10, 2 << 10);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, temp);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            gDPLoadTextureBlock_4b(OVERLAY_DISP++, cUpLabelTextures[GET_LANGUAGE], G_IM_FMT_IA, 32, 8, 0,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                   G_TX_NOLOD, G_TX_NOLOD);

            gSPTextureRectangle(OVERLAY_DISP++, R_C_UP_ICON_X << 2, R_C_UP_ICON_Y << 2, (R_C_UP_ICON_X + 32) << 2,
                                (R_C_UP_ICON_Y + 8) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        }

        sCUpTimer--;
        if (sCUpTimer == 0) {
            sCUpInvisible ^= 1;
            sCUpTimer = 10;
        }
    }

    gDPPipeSync(OVERLAY_DISP++);

    // Empty C Button Arrows
    for (temp = 1; temp < 4; temp++) {
        if (GET_EQUIPS_BUTTONITEMS_A0(temp) > 0xF0) {
            if (temp == 1) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cLeftAlpha);
            } else if (temp == 2) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cDownAlpha);
            } else {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_C_BTN_COLOR(0), R_C_BTN_COLOR(1), R_C_BTN_COLOR(2),
                                interfaceCtx->cRightAlpha);
            }

            OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gButtonBackgroundTex + ((32 * 32) * (temp + 1))), 32, 32,
                                          R_ITEM_BTN_X(temp), R_ITEM_BTN_Y(temp), R_ITEM_BTN_WIDTH(temp),
                                          R_ITEM_BTN_WIDTH(temp), R_ITEM_BTN_DD(temp) << 1, R_ITEM_BTN_DD(temp) << 1);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3071);
}

void Interface_DrawItemIconTexture(PlayState* play, void* texture, s16 button) {
    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3079);

    gDPLoadTextureBlock(OVERLAY_DISP++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, R_ITEM_ICON_X(button) << 2, R_ITEM_ICON_Y(button) << 2,
                        (R_ITEM_ICON_X(button) + R_ITEM_ICON_WIDTH(button)) << 2,
                        (R_ITEM_ICON_Y(button) + R_ITEM_ICON_WIDTH(button)) << 2, G_TX_RENDERTILE, 0, 0,
                        R_ITEM_ICON_DD(button) << 1, R_ITEM_ICON_DD(button) << 1);

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3094);
}

void Interface_DrawAmmoCount(PlayState* play, s16 button, s16 alpha) {
    s16 i;
    s16 ammo;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3105);

    i = GET_EQUIPS_BUTTONITEMS_A0(button);

    if ((i == ITEM_STICK) || (i == ITEM_NUT) || (i == ITEM_BOMB) || (i == ITEM_BOW) ||
        ((i >= ITEM_BOW_ARROW_FIRE) && (i <= ITEM_BOW_ARROW_LIGHT)) || (i == ITEM_SLINGSHOT) || (i == ITEM_BOMBCHU) ||
        (i == ITEM_BEAN)) {

        if ((i >= ITEM_BOW_ARROW_FIRE) && (i <= ITEM_BOW_ARROW_LIGHT)) {
            i = ITEM_BOW;
        }

        ammo = AMMO(i);

        gDPPipeSync(OVERLAY_DISP++);

        if ((button == 0) && (GET_MINIGAMESTATE == 1)) {
            ammo = play->interfaceCtx.hbaAmmo;
        } else if ((button == 0) && (play->shootingGalleryStatus > 1)) {
            ammo = play->shootingGalleryStatus - 1;
        } else if ((button == 0) && (play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38)) {
            ammo = play->bombchuBowlingStatus;
            if (ammo < 0) {
                ammo = 0;
            }
        } else if (((i == ITEM_BOW) && (AMMO(i) == CUR_CAPACITY(UPG_QUIVER))) ||
                   ((i == ITEM_BOMB) && (AMMO(i) == CUR_CAPACITY(UPG_BOMB_BAG))) ||
                   ((i == ITEM_SLINGSHOT) && (AMMO(i) == CUR_CAPACITY(UPG_BULLET_BAG))) ||
                   ((i == ITEM_STICK) && (AMMO(i) == CUR_CAPACITY(UPG_STICKS))) ||
                   ((i == ITEM_NUT) && (AMMO(i) == CUR_CAPACITY(UPG_NUTS))) || ((i == ITEM_BOMBCHU) && (ammo == 50)) ||
                   ((i == ITEM_BEAN) && (ammo == 15))) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, alpha);
        }

        if (ammo == 0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, alpha);
        }

        for (i = 0; ammo >= 10; i++) {
            ammo -= 10;
        }

        if (i != 0) {
            OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * i)), 8, 8,
                                          R_ITEM_AMMO_X(button), R_ITEM_AMMO_Y(button), 8, 8, 1 << 10, 1 << 10);
        }

        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, ((u8*)gAmmoDigit0Tex + ((8 * 8) * ammo)), 8, 8,
                                      R_ITEM_AMMO_X(button) + 6, R_ITEM_AMMO_Y(button), 8, 8, 1 << 10, 1 << 10);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3158);
}

void Interface_DrawActionButton(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3172);

    Matrix_Translate(0.0f, 0.0f, XREG(18) / 10.0f, MTXMODE_NEW);
    Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
    Matrix_RotateX(interfaceCtx->unk_1F4 / 10000.0f, MTXMODE_APPLY);

    gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_parameter.c", 3177),
              G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[0], 4, 0);

    gDPLoadTextureBlock(OVERLAY_DISP++, gButtonBackgroundTex, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 3187);
}

void Interface_InitVertices(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 i;

    interfaceCtx->actionVtx = Graph_Alloc(play->state.gfxCtx, 8 * sizeof(Vtx));

    interfaceCtx->actionVtx[0].v.ob[0] = interfaceCtx->actionVtx[2].v.ob[0] = -14;
    interfaceCtx->actionVtx[1].v.ob[0] = interfaceCtx->actionVtx[3].v.ob[0] = interfaceCtx->actionVtx[0].v.ob[0] + 28;

    interfaceCtx->actionVtx[0].v.ob[1] = interfaceCtx->actionVtx[1].v.ob[1] = 14;
    interfaceCtx->actionVtx[2].v.ob[1] = interfaceCtx->actionVtx[3].v.ob[1] = interfaceCtx->actionVtx[0].v.ob[1] - 28;

    interfaceCtx->actionVtx[4].v.ob[0] = interfaceCtx->actionVtx[6].v.ob[0] = -(XREG(21) / 2);
    interfaceCtx->actionVtx[5].v.ob[0] = interfaceCtx->actionVtx[7].v.ob[0] =
        interfaceCtx->actionVtx[4].v.ob[0] + XREG(21);

    interfaceCtx->actionVtx[4].v.ob[1] = interfaceCtx->actionVtx[5].v.ob[1] = XREG(28) / 2;
    interfaceCtx->actionVtx[6].v.ob[1] = interfaceCtx->actionVtx[7].v.ob[1] =
        interfaceCtx->actionVtx[4].v.ob[1] - XREG(28);

    for (i = 0; i < 8; i += 4) {
        interfaceCtx->actionVtx[i].v.ob[2] = interfaceCtx->actionVtx[i + 1].v.ob[2] =
            interfaceCtx->actionVtx[i + 2].v.ob[2] = interfaceCtx->actionVtx[i + 3].v.ob[2] = 0;

        interfaceCtx->actionVtx[i].v.flag = interfaceCtx->actionVtx[i + 1].v.flag =
            interfaceCtx->actionVtx[i + 2].v.flag = interfaceCtx->actionVtx[i + 3].v.flag = 0;

        interfaceCtx->actionVtx[i].v.tc[0] = interfaceCtx->actionVtx[i].v.tc[1] =
            interfaceCtx->actionVtx[i + 1].v.tc[1] = interfaceCtx->actionVtx[i + 2].v.tc[0] = 0;
        interfaceCtx->actionVtx[i + 1].v.tc[0] = interfaceCtx->actionVtx[i + 2].v.tc[1] =
            interfaceCtx->actionVtx[i + 3].v.tc[0] = interfaceCtx->actionVtx[i + 3].v.tc[1] = 1024;

        interfaceCtx->actionVtx[i].v.cn[0] = interfaceCtx->actionVtx[i + 1].v.cn[0] =
            interfaceCtx->actionVtx[i + 2].v.cn[0] = interfaceCtx->actionVtx[i + 3].v.cn[0] =
                interfaceCtx->actionVtx[i].v.cn[1] = interfaceCtx->actionVtx[i + 1].v.cn[1] =
                    interfaceCtx->actionVtx[i + 2].v.cn[1] = interfaceCtx->actionVtx[i + 3].v.cn[1] =
                        interfaceCtx->actionVtx[i].v.cn[2] = interfaceCtx->actionVtx[i + 1].v.cn[2] =
                            interfaceCtx->actionVtx[i + 2].v.cn[2] = interfaceCtx->actionVtx[i + 3].v.cn[2] = 255;

        interfaceCtx->actionVtx[i].v.cn[3] = interfaceCtx->actionVtx[i + 1].v.cn[3] =
            interfaceCtx->actionVtx[i + 2].v.cn[3] = interfaceCtx->actionVtx[i + 3].v.cn[3] = 255;
    }

    interfaceCtx->actionVtx[5].v.tc[0] = interfaceCtx->actionVtx[7].v.tc[0] = 1536;
    interfaceCtx->actionVtx[6].v.tc[1] = interfaceCtx->actionVtx[7].v.tc[1] = 512;

    interfaceCtx->beatingHeartVtx = Graph_Alloc(play->state.gfxCtx, 4 * sizeof(Vtx));

    interfaceCtx->beatingHeartVtx[0].v.ob[0] = interfaceCtx->beatingHeartVtx[2].v.ob[0] = -8;
    interfaceCtx->beatingHeartVtx[1].v.ob[0] = interfaceCtx->beatingHeartVtx[3].v.ob[0] = 8;
    interfaceCtx->beatingHeartVtx[0].v.ob[1] = interfaceCtx->beatingHeartVtx[1].v.ob[1] = 8;
    interfaceCtx->beatingHeartVtx[2].v.ob[1] = interfaceCtx->beatingHeartVtx[3].v.ob[1] = -8;

    interfaceCtx->beatingHeartVtx[0].v.ob[2] = interfaceCtx->beatingHeartVtx[1].v.ob[2] =
        interfaceCtx->beatingHeartVtx[2].v.ob[2] = interfaceCtx->beatingHeartVtx[3].v.ob[2] = 0;

    interfaceCtx->beatingHeartVtx[0].v.flag = interfaceCtx->beatingHeartVtx[1].v.flag =
        interfaceCtx->beatingHeartVtx[2].v.flag = interfaceCtx->beatingHeartVtx[3].v.flag = 0;

    interfaceCtx->beatingHeartVtx[0].v.tc[0] = interfaceCtx->beatingHeartVtx[0].v.tc[1] =
        interfaceCtx->beatingHeartVtx[1].v.tc[1] = interfaceCtx->beatingHeartVtx[2].v.tc[0] = 0;
    interfaceCtx->beatingHeartVtx[1].v.tc[0] = interfaceCtx->beatingHeartVtx[2].v.tc[1] =
        interfaceCtx->beatingHeartVtx[3].v.tc[0] = interfaceCtx->beatingHeartVtx[3].v.tc[1] = 512;

    interfaceCtx->beatingHeartVtx[0].v.cn[0] = interfaceCtx->beatingHeartVtx[1].v.cn[0] =
        interfaceCtx->beatingHeartVtx[2].v.cn[0] = interfaceCtx->beatingHeartVtx[3].v.cn[0] =
            interfaceCtx->beatingHeartVtx[0].v.cn[1] = interfaceCtx->beatingHeartVtx[1].v.cn[1] =
                interfaceCtx->beatingHeartVtx[2].v.cn[1] = interfaceCtx->beatingHeartVtx[3].v.cn[1] =
                    interfaceCtx->beatingHeartVtx[0].v.cn[2] = interfaceCtx->beatingHeartVtx[1].v.cn[2] =
                        interfaceCtx->beatingHeartVtx[2].v.cn[2] = interfaceCtx->beatingHeartVtx[3].v.cn[2] =
                            interfaceCtx->beatingHeartVtx[0].v.cn[3] = interfaceCtx->beatingHeartVtx[1].v.cn[3] =
                                interfaceCtx->beatingHeartVtx[2].v.cn[3] = interfaceCtx->beatingHeartVtx[3].v.cn[3] =
                                    255;
}

void func_8008A8B8(PlayState* play, s32 topY, s32 bottomY, s32 leftX, s32 rightX) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Vec3f eye;
    Vec3f lookAt;
    Vec3f up;

    eye.x = eye.y = eye.z = 0.0f;
    lookAt.x = lookAt.y = 0.0f;
    lookAt.z = -1.0f;
    up.x = up.z = 0.0f;
    up.y = 1.0f;

    View_LookAt(&interfaceCtx->view, &eye, &lookAt, &up);

    interfaceCtx->viewport.topY = topY;
    interfaceCtx->viewport.bottomY = bottomY;
    interfaceCtx->viewport.leftX = leftX;
    interfaceCtx->viewport.rightX = rightX;
    View_SetViewport(&interfaceCtx->view, &interfaceCtx->viewport);

    View_SetPerspective(&interfaceCtx->view, 60.0f, 10.0f, 60.0f);
    View_ApplyPerspectiveToOverlay(&interfaceCtx->view);
}

void func_8008A994(InterfaceContext* interfaceCtx) {
    SET_FULLSCREEN_VIEWPORT(&interfaceCtx->view);
    View_ApplyOrthoToOverlay(&interfaceCtx->view);
}

void Interface_Draw(PlayState* play) {
    static s16 magicArrowEffectsR[] = { 255, 100, 255 };
    static s16 magicArrowEffectsG[] = { 0, 100, 255 };
    static s16 magicArrowEffectsB[] = { 0, 255, 100 };
    static s16 timerDigitLeftPos[] = { 16, 25, 34, 42, 51 };
    static s16 digitWidth[] = { 9, 9, 8, 9, 9 };
    // unused, most likely colors
    static s16 D_80125B1C[][3] = {
        { 0, 150, 0 }, { 100, 255, 0 }, { 255, 255, 255 }, { 0, 0, 0 }, { 255, 255, 255 },
    };
    static s16 rupeeDigitsFirst[] = { 1, 0, 0 };
    static s16 rupeeDigitsCount[] = { 2, 3, 3 };
    static s16 spoilingItemEntrances[] = { ENTR_SPOT10_2, ENTR_SPOT07_3, ENTR_SPOT07_3 };
    static f32 D_80125B54[] = { -40.0f, -35.0f }; // unused
    static s16 D_80125B5C[] = { 91, 91 };         // unused
    static s16 D_8015FFE0;
    static s16 D_8015FFE2;
    static s16 D_8015FFE4;
    static s16 D_8015FFE6;
    static s16 timerDigits[5];
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    PauseContext* pauseCtx = &play->pauseCtx;
    MessageContext* msgCtx = &play->msgCtx;
    Player* player = GET_PLAYER(play);
    s16 svar1;
    s16 svar2;
    s16 svar3;
    s16 svar4;
    s16 svar5;
    s16 svar6;

    OPEN_DISPS(play->state.gfxCtx, "../z_parameter.c", 3405);

    gSPSegment(OVERLAY_DISP++, 0x02, interfaceCtx->parameterSegment);
    gSPSegment(OVERLAY_DISP++, 0x07, interfaceCtx->doActionSegment);
    gSPSegment(OVERLAY_DISP++, 0x08, interfaceCtx->iconItemSegment);
    gSPSegment(OVERLAY_DISP++, 0x0B, interfaceCtx->mapSegment);

    if (pauseCtx->debugState == 0) {
        Interface_InitVertices(play);
        func_8008A994(interfaceCtx);
        Health_DrawMeter(play);

        Gfx_SetupDL_39Overlay(play->state.gfxCtx);

        // Rupee Icon
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 255, 100, interfaceCtx->magicAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 80, 0, 255);
        OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gRupeeCounterIconTex, 16, 16, 26, 206, 16, 16, 1 << 10, 1 << 10);

        switch (play->sceneId) {
            case SCENE_BMORI1:
            case SCENE_HIDAN:
            case SCENE_MIZUSIN:
            case SCENE_JYASINZOU:
            case SCENE_HAKADAN:
            case SCENE_HAKADANCH:
            case SCENE_ICE_DOUKUTO:
            case SCENE_GANON:
            case SCENE_MEN:
            case SCENE_GERUDOWAY:
            case SCENE_GANONTIKA:
            case SCENE_GANON_SONOGO:
            case SCENE_GANONTIKA_SONOGO:
            case SCENE_TAKARAYA:
                if (GET_INVENTORY_DUNGEONKEYS_A0(GET_MAPINDEX) >= 0) {
                    // Small Key Icon
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 230, 255, interfaceCtx->magicAlpha);
                    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 20, 255);
                    OVERLAY_DISP = Gfx_TextureIA8(OVERLAY_DISP, gSmallKeyCounterIconTex, 16, 16, 26, 190, 16, 16,
                                                  1 << 10, 1 << 10);

                    // Small Key Counter
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->magicAlpha);
                    gDPSetCombineLERP(OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                                      TEXEL0, 0, PRIMITIVE, 0);

                    interfaceCtx->counterDigits[2] = 0;
                    interfaceCtx->counterDigits[3] =
                        GET_INVENTORY_DUNGEONKEYS_A0(GET_MAPINDEX);

                    while (interfaceCtx->counterDigits[3] >= 10) {
                        interfaceCtx->counterDigits[2]++;
                        interfaceCtx->counterDigits[3] -= 10;
                    }

                    svar3 = 42;

                    if (interfaceCtx->counterDigits[2] != 0) {
                        OVERLAY_DISP = Gfx_TextureI8(
                            OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[2])), 8, 16,
                            svar3, 190, 8, 16, 1 << 10, 1 << 10);
                        svar3 += 8;
                    }

                    OVERLAY_DISP = Gfx_TextureI8(OVERLAY_DISP,
                                                 ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[3])),
                                                 8, 16, svar3, 190, 8, 16, 1 << 10, 1 << 10);
                }
                break;
            default:
                break;
        }

        // Rupee Counter
        gDPPipeSync(OVERLAY_DISP++);

        if (GET_RUPEES == CUR_CAPACITY(UPG_WALLET)) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, interfaceCtx->magicAlpha);
        } else if (GET_RUPEES != 0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->magicAlpha);
        } else {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, interfaceCtx->magicAlpha);
        }

        gDPSetCombineLERP(OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                          PRIMITIVE, 0);

        interfaceCtx->counterDigits[0] = interfaceCtx->counterDigits[1] = 0;
        interfaceCtx->counterDigits[2] = GET_RUPEES;

        if ((interfaceCtx->counterDigits[2] > 9999) || (interfaceCtx->counterDigits[2] < 0)) {
            interfaceCtx->counterDigits[2] &= 0xDDD;
        }

        while (interfaceCtx->counterDigits[2] >= 100) {
            interfaceCtx->counterDigits[0]++;
            interfaceCtx->counterDigits[2] -= 100;
        }

        while (interfaceCtx->counterDigits[2] >= 10) {
            interfaceCtx->counterDigits[1]++;
            interfaceCtx->counterDigits[2] -= 10;
        }

        svar2 = rupeeDigitsFirst[CUR_UPG_VALUE(UPG_WALLET)];
        svar5 = rupeeDigitsCount[CUR_UPG_VALUE(UPG_WALLET)];

        for (svar1 = 0, svar3 = 42; svar1 < svar5; svar1++, svar2++, svar3 += 8) {
            OVERLAY_DISP =
                Gfx_TextureI8(OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * interfaceCtx->counterDigits[svar2])), 8,
                              16, svar3, 206, 8, 16, 1 << 10, 1 << 10);
        }

        Magic_DrawMeter(play);
        Minimap_Draw(play);

        if ((R_PAUSE_MENU_MODE != 2) && (R_PAUSE_MENU_MODE != 3)) {
            func_8002C124(&play->actorCtx.targetCtx, play); // Draw Z-Target
        }

        Gfx_SetupDL_39Overlay(play->state.gfxCtx);

        Interface_DrawItemButtons(play);

        gDPPipeSync(OVERLAY_DISP++);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);

        if (!(interfaceCtx->unk_1FA)) {
            // B Button Icon & Ammo Count
            if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_NONE) {
                Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment, 0);

                if ((player->stateFlags1 & PLAYER_STATE1_23) || (play->shootingGalleryStatus > 1) ||
                    ((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38))) {
                    gDPPipeSync(OVERLAY_DISP++);
                    gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE,
                                      0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
                    Interface_DrawAmmoCount(play, 0, interfaceCtx->bAlpha);
                }
            }
        } else {
            // B Button Do Action Label
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);

            gDPLoadTextureBlock_4b(OVERLAY_DISP++, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE, G_IM_FMT_IA,
                                   DO_ACTION_TEX_WIDTH, DO_ACTION_TEX_HEIGHT, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                   G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            R_B_LABEL_DD = (1 << 10) / (WREG(37 + GET_LANGUAGE) / 100.0f);
            gSPTextureRectangle(OVERLAY_DISP++, R_B_LABEL_X(GET_LANGUAGE) << 2,
                                R_B_LABEL_Y(GET_LANGUAGE) << 2,
                                (R_B_LABEL_X(GET_LANGUAGE) + DO_ACTION_TEX_WIDTH) << 2,
                                (R_B_LABEL_Y(GET_LANGUAGE) + DO_ACTION_TEX_HEIGHT) << 2, G_TX_RENDERTILE, 0, 0,
                                R_B_LABEL_DD, R_B_LABEL_DD);
        }

        gDPPipeSync(OVERLAY_DISP++);

        // C-Left Button Icon & Ammo Count
        if (GET_EQUIPS_BUTTONITEMS_A0(1) < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cLeftAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x1000, 1);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            Interface_DrawAmmoCount(play, 1, interfaceCtx->cLeftAlpha);
        }

        gDPPipeSync(OVERLAY_DISP++);

        // C-Down Button Icon & Ammo Count
        if (GET_EQUIPS_BUTTONITEMS_A0(2) < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cDownAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x2000, 2);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            Interface_DrawAmmoCount(play, 2, interfaceCtx->cDownAlpha);
        }

        gDPPipeSync(OVERLAY_DISP++);

        // C-Right Button Icon & Ammo Count
        if (GET_EQUIPS_BUTTONITEMS_A0(3) < 0xF0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->cRightAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            Interface_DrawItemIconTexture(play, interfaceCtx->iconItemSegment + 0x3000, 3);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            Interface_DrawAmmoCount(play, 3, interfaceCtx->cRightAlpha);
        }

        // A Button
        Gfx_SetupDL_42Overlay(play->state.gfxCtx);
        func_8008A8B8(play, R_A_BTN_Y, R_A_BTN_Y + 45, R_A_BTN_X, R_A_BTN_X + 45);
        gSPClearGeometryMode(OVERLAY_DISP++, G_CULL_BOTH);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, R_A_BTN_COLOR(0), R_A_BTN_COLOR(1), R_A_BTN_COLOR(2),
                        interfaceCtx->aAlpha);
        Interface_DrawActionButton(play);
        gDPPipeSync(OVERLAY_DISP++);
        func_8008A8B8(play, R_A_ICON_Y, R_A_ICON_Y + 45, R_A_ICON_X, R_A_ICON_X + 45);
        gSPSetGeometryMode(OVERLAY_DISP++, G_CULL_BACK);
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                          PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
        Matrix_Translate(0.0f, 0.0f, WREG(46 + GET_LANGUAGE) / 10.0f, MTXMODE_NEW);
        Matrix_Scale(1.0f, 1.0f, 1.0f, MTXMODE_APPLY);
        Matrix_RotateX(interfaceCtx->unk_1F4 / 10000.0f, MTXMODE_APPLY);
        gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_parameter.c", 3701),
                  G_MTX_MODELVIEW | G_MTX_LOAD);
        gSPVertex(OVERLAY_DISP++, &interfaceCtx->actionVtx[4], 4, 0);

        if ((interfaceCtx->unk_1EC < 2) || (interfaceCtx->unk_1EC == 3)) {
            Interface_DrawActionLabel(play->state.gfxCtx, interfaceCtx->doActionSegment);
        } else {
            Interface_DrawActionLabel(play->state.gfxCtx, interfaceCtx->doActionSegment + DO_ACTION_TEX_SIZE);
        }

        gDPPipeSync(OVERLAY_DISP++);

        func_8008A994(interfaceCtx);

        if ((pauseCtx->state == 6) && (pauseCtx->unk_1E4 == 3)) {
            // Inventory Equip Effects
            gSPSegment(OVERLAY_DISP++, 0x08, pauseCtx->iconItemSegment);
            Gfx_SetupDL_42Overlay(play->state.gfxCtx);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            gSPMatrix(OVERLAY_DISP++, &gMtxClear, G_MTX_MODELVIEW | G_MTX_LOAD);

            pauseCtx->cursorVtx[16].v.ob[0] = pauseCtx->cursorVtx[18].v.ob[0] = pauseCtx->equipAnimX / 10;
            pauseCtx->cursorVtx[17].v.ob[0] = pauseCtx->cursorVtx[19].v.ob[0] =
                pauseCtx->cursorVtx[16].v.ob[0] + WREG(90) / 10;
            pauseCtx->cursorVtx[16].v.ob[1] = pauseCtx->cursorVtx[17].v.ob[1] = pauseCtx->equipAnimY / 10;
            pauseCtx->cursorVtx[18].v.ob[1] = pauseCtx->cursorVtx[19].v.ob[1] =
                pauseCtx->cursorVtx[16].v.ob[1] - WREG(90) / 10;

            if (pauseCtx->equipTargetItem < 0xBF) {
                // Normal Equip (icon goes from the inventory slot to the C button when equipping it)
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, pauseCtx->equipAnimAlpha);
                gSPVertex(OVERLAY_DISP++, &pauseCtx->cursorVtx[16], 4, 0);

                gDPLoadTextureBlock(OVERLAY_DISP++, gItemIcons[pauseCtx->equipTargetItem], G_IM_FMT_RGBA, G_IM_SIZ_32b,
                                    32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                                    G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            } else {
                // Magic Arrow Equip Effect
                svar1 = pauseCtx->equipTargetItem - 0xBF;
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, magicArrowEffectsR[svar1], magicArrowEffectsG[svar1],
                                magicArrowEffectsB[svar1], pauseCtx->equipAnimAlpha);

                if ((pauseCtx->equipAnimAlpha > 0) && (pauseCtx->equipAnimAlpha < 255)) {
                    svar1 = (pauseCtx->equipAnimAlpha / 8) / 2;
                    pauseCtx->cursorVtx[16].v.ob[0] = pauseCtx->cursorVtx[18].v.ob[0] =
                        pauseCtx->cursorVtx[16].v.ob[0] - svar1;
                    pauseCtx->cursorVtx[17].v.ob[0] = pauseCtx->cursorVtx[19].v.ob[0] =
                        pauseCtx->cursorVtx[16].v.ob[0] + svar1 * 2 + 32;
                    pauseCtx->cursorVtx[16].v.ob[1] = pauseCtx->cursorVtx[17].v.ob[1] =
                        pauseCtx->cursorVtx[16].v.ob[1] + svar1;
                    pauseCtx->cursorVtx[18].v.ob[1] = pauseCtx->cursorVtx[19].v.ob[1] =
                        pauseCtx->cursorVtx[16].v.ob[1] - svar1 * 2 - 32;
                }

                gSPVertex(OVERLAY_DISP++, &pauseCtx->cursorVtx[16], 4, 0);
                gDPLoadTextureBlock(OVERLAY_DISP++, gMagicArrowEquipEffectTex, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
            }

            gSP1Quadrangle(OVERLAY_DISP++, 0, 2, 3, 1, 0);
        }

        Gfx_SetupDL_39Overlay(play->state.gfxCtx);

        if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0)) {
            if (GET_MINIGAMESTATE != 1) {
                // Carrots rendering if the action corresponds to riding a horse
                if (interfaceCtx->unk_1EE == 8) {
                    // Load Carrot Icon
                    gDPLoadTextureBlock(OVERLAY_DISP++, gCarrotIconTex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 16, 16, 0,
                                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                        G_TX_NOLOD, G_TX_NOLOD);

                    // Draw 6 carrots
                    for (svar1 = 1, svar5 = ZREG(14); svar1 < 7; svar1++, svar5 += 16) {
                        // Carrot Color (based on availability)
                        if ((interfaceCtx->numHorseBoosts == 0) || (interfaceCtx->numHorseBoosts < svar1)) {
                            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 150, 255, interfaceCtx->aAlpha);
                        } else {
                            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->aAlpha);
                        }

                        gSPTextureRectangle(OVERLAY_DISP++, svar5 << 2, ZREG(15) << 2, (svar5 + 16) << 2,
                                            (ZREG(15) + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
                    }
                }
            } else {
                // Score for the Horseback Archery
                svar5 = WREG(32);
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->bAlpha);

                // Target Icon
                gDPLoadTextureBlock(OVERLAY_DISP++, gArcheryScoreIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 24, 16, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);

                gSPTextureRectangle(OVERLAY_DISP++, (svar5 + 28) << 2, ZREG(15) << 2, (svar5 + 52) << 2,
                                    (ZREG(15) + 16) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

                // Score Counter
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineLERP(OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                                  TEXEL0, 0, PRIMITIVE, 0);

                svar5 = WREG(32) + 6 * 9;

                for (svar1 = svar2 = 0; svar1 < 4; svar1++) {
                    if (sHBAScoreDigits[svar1] != 0 || (svar2 != 0) || (svar1 >= 3)) {
                        OVERLAY_DISP = Gfx_TextureI8(
                            OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * sHBAScoreDigits[svar1])), 8, 16, svar5,
                            (ZREG(15) - 2), digitWidth[0], VREG(42), VREG(43) << 1, VREG(43) << 1);
                        svar5 += 9;
                        svar2++;
                    }
                }

                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
            }
        }

        if ((GET_TIMER2STATE == 5) && (Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT)) {
            // Trade quest timer reached 0
            D_8015FFE6 = 40;
            SET_CUTSCENEINDEX(0);
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_WHITE;
            SET_TIMER2STATE(0);

            if ((GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KOKIRI) &&
                (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_MASTER) &&
                (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_BGS) &&
                (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KNIFE)) {
                if (GET_BUTTONSTATUS_A0(0) != BTN_ENABLED) {
                    SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                } else {
                    SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_NONE);
                }
            }

            // Revert any spoiling trade quest items
            for (svar1 = 0; svar1 < ARRAY_COUNT(gSpoilingItems); svar1++) {
                if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[svar1]) {
                    SET_EVENTINF_A0(EVENTINF_HORSES_INDEX, GET_EVENTINF_A0(EVENTINF_HORSES_INDEX) & ((u16) ~(EVENTINF_HORSES_STATE_MASK | EVENTINF_HORSES_HORSETYPE_MASK | EVENTINF_HORSES_05_MASK |
                                EVENTINF_HORSES_06_MASK | EVENTINF_HORSES_0F_MASK)));
                    osSyncPrintf("EVENT_INF=%x\n", GET_EVENTINF_A0(EVENTINF_HORSES_INDEX));
                    play->nextEntranceIndex = spoilingItemEntrances[svar1];
                    INV_CONTENT(gSpoilingItemReverts[svar1]) = gSpoilingItemReverts[svar1];

                    for (svar2 = 1; svar2 < 4; svar2++) {
                        if (GET_EQUIPS_BUTTONITEMS_A0(svar2) == gSpoilingItems[svar1]) {
                            SET_EQUIPS_BUTTONITEMS_A0(svar2, gSpoilingItemReverts[svar1]);
                            Interface_LoadItemIcon1(play, svar2);
                        }
                    }
                }
            }
        }

        if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
            (play->gameOverCtx.state == GAMEOVER_INACTIVE) && (msgCtx->msgMode == MSGMODE_NONE) &&
            !(player->stateFlags2 & PLAYER_STATE2_24) && (play->transitionTrigger == TRANS_TRIGGER_OFF) &&
            (play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play) && (GET_MINIGAMESTATE != 1) &&
            (play->shootingGalleryStatus <= 1) && !((play->sceneId == SCENE_BOWLING) && Flags_GetSwitch(play, 0x38))) {
            svar6 = 0;
            switch (GET_TIMER1STATE) {
                case 1:
                    D_8015FFE2 = 20;
                    D_8015FFE0 = 20;
                    SET_TIMER1VALUE(GET_HEALTH >> 1);
                    SET_TIMER1STATE(2);
                    break;
                case 2:
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        SET_TIMER1STATE(3);
                    }
                    break;
                case 5:
                case 11:
                    D_8015FFE2 = 20;
                    D_8015FFE0 = 20;
                    if (GET_TIMER1STATE == 5) {
                        SET_TIMER1STATE(6);
                    } else {
                        SET_TIMER1STATE(12);
                    }
                    break;
                case 6:
                case 12:
                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        if (GET_TIMER1STATE == 6) {
                            SET_TIMER1STATE(7);
                        } else {
                            SET_TIMER1STATE(13);
                        }
                    }
                    break;
                case 3:
                case 7:
                    svar1 = (GET_TIMERX_A0(0) - 26) / D_8015FFE2;
                    GET_TIMERX_A0(0) -= svar1;

                    if (GET_HEALTHCAPACITY > 0xA0) {
                        svar1 = (GET_TIMERY_A0(0) - 54) / D_8015FFE2;
                    } else {
                        svar1 = (GET_TIMERY_A0(0) - 46) / D_8015FFE2;
                    }
                    GET_TIMERY_A0(0) -= svar1;

                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        SET_TIMERX_A0(0, 26);

                        if (GET_HEALTHCAPACITY > 0xA0) {
                            SET_TIMERY_A0(0, 54);
                        } else {
                            SET_TIMERY_A0(0, 46);
                        }

                        if (GET_TIMER1STATE == 3) {
                            SET_TIMER1STATE(4);
                        } else {
                            SET_TIMER1STATE(8);
                        }
                    }
                    FALLTHROUGH;
                case 4:
                case 8:
                    if ((GET_TIMER1STATE == 4) || (GET_TIMER1STATE == 8)) {
                        if (GET_HEALTHCAPACITY > 0xA0) {
                            SET_TIMERY_A0(0, 54);
                        } else {
                            SET_TIMERY_A0(0, 46);
                        }
                    }

                    if ((GET_TIMER1STATE >= 3) && (msgCtx->msgLength == 0)) {
                        D_8015FFE0--;
                        if (D_8015FFE0 == 0) {
                            if (GET_TIMER1VALUE != 0) {
                                GET_TIMER1VALUE--;
                            }

                            D_8015FFE0 = 20;

                            if (GET_TIMER1VALUE == 0) {
                                SET_TIMER1STATE(10);
                                if (D_80125A5C) {
                                    SET_HEALTH(0);
                                    play->damagePlayer(play, -(GET_HEALTH + 2));
                                }
                                D_80125A5C = false;
                            } else if (GET_TIMER1VALUE > 60) {
                                if (timerDigits[4] == 1) {
                                    Audio_PlaySfxGeneral(NA_SE_SY_MESSAGE_WOMAN, &gSfxDefaultPos, 4,
                                                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                         &gSfxDefaultReverb);
                                }
                            } else if (GET_TIMER1VALUE >= 11) {
                                if (timerDigits[4] & 1) {
                                    Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                                         &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                         &gSfxDefaultReverb);
                                }
                            } else {
                                Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_E, &gSfxDefaultPos, 4,
                                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                     &gSfxDefaultReverb);
                            }
                        }
                    }
                    break;
                case 13:
                    svar1 = (GET_TIMERX_A0(0) - 26) / D_8015FFE2;
                    GET_TIMERX_A0(0) -= svar1;

                    if (GET_HEALTHCAPACITY > 0xA0) {
                        svar1 = (GET_TIMERY_A0(0) - 54) / D_8015FFE2;
                    } else {
                        svar1 = (GET_TIMERY_A0(0) - 46) / D_8015FFE2;
                    }
                    GET_TIMERY_A0(0) -= svar1;

                    D_8015FFE2--;
                    if (D_8015FFE2 == 0) {
                        D_8015FFE2 = 20;
                        SET_TIMERX_A0(0, 26);
                        if (GET_HEALTHCAPACITY > 0xA0) {
                            SET_TIMERY_A0(0, 54);
                        } else {
                            SET_TIMERY_A0(0, 46);
                        }

                        SET_TIMER1STATE(14);
                    }
                    FALLTHROUGH;
                case 14:
                    if (GET_TIMER1STATE == 14) {
                        if (GET_HEALTHCAPACITY > 0xA0) {
                            SET_TIMERY_A0(0, 54);
                        } else {
                            SET_TIMERY_A0(0, 46);
                        }
                    }

                    if (GET_TIMER1STATE >= 3) {
                        D_8015FFE0--;
                        if (D_8015FFE0 == 0) {
                            GET_TIMER1VALUE++;
                            D_8015FFE0 = 20;

                            if (GET_TIMER1VALUE == 3599) {
                                D_8015FFE2 = 40;
                                SET_TIMER1STATE(15);
                            } else {
                                Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                     &gSfxDefaultReverb);
                            }
                        }
                    }
                    break;
                case 10:
                    if (GET_TIMER2STATE != 0) {
                        D_8015FFE6 = 20;
                        D_8015FFE4 = 20;
                        SET_TIMERX_A0(1, 140);
                        SET_TIMERY_A0(1, 80);

                        if (GET_TIMER2STATE < 7) {
                            SET_TIMER2STATE(2);
                        } else {
                            SET_TIMER2STATE(8);
                        }

                        SET_TIMER1STATE(0);
                    } else {
                        SET_TIMER1STATE(0);
                    }
                case 15:
                    break;
                default:
                    svar6 = 1;
                    switch (GET_TIMER2STATE) {
                        case 1:
                        case 7:
                            D_8015FFE6 = 20;
                            D_8015FFE4 = 20;
                            SET_TIMERX_A0(1, 140);
                            SET_TIMERY_A0(1, 80);
                            if (GET_TIMER2STATE == 1) {
                                SET_TIMER2STATE(2);
                            } else {
                                SET_TIMER2STATE(8);
                            }
                            break;
                        case 2:
                        case 8:
                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                D_8015FFE6 = 20;
                                if (GET_TIMER2STATE == 2) {
                                    SET_TIMER2STATE(3);
                                } else {
                                    SET_TIMER2STATE(9);
                                }
                            }
                            break;
                        case 3:
                        case 9:
                            osSyncPrintf("event_xp[1]=%d,  event_yp[1]=%d  TOTAL_EVENT_TM=%d\n",
                                         (GET_TIMERX_A0_VOID0(1)), (GET_TIMERY_A0_VOID0(1)),
                                         GET_TIMER2VALUE);
                            svar1 = (GET_TIMERX_A0(1) - 26) / D_8015FFE6;
                            GET_TIMERX_A0(1) -= svar1;
                            if (GET_HEALTHCAPACITY > 0xA0) {
                                svar1 = (GET_TIMERY_A0(1) - 54) / D_8015FFE6;
                            } else {
                                svar1 = (GET_TIMERY_A0(1) - 46) / D_8015FFE6;
                            }
                            GET_TIMERY_A0(1) -= svar1;

                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                D_8015FFE6 = 20;
                                SET_TIMERX_A0(1, 26);

                                if (GET_HEALTHCAPACITY > 0xA0) {
                                    SET_TIMERY_A0(1, 54);
                                } else {
                                    SET_TIMERY_A0(1, 46);
                                }

                                if (GET_TIMER2STATE == 3) {
                                    SET_TIMER2STATE(4);
                                } else {
                                    SET_TIMER2STATE(10);
                                }
                            }
                            FALLTHROUGH;
                        case 4:
                        case 10:
                            if ((GET_TIMER2STATE == 4) || (GET_TIMER2STATE == 10)) {
                                if (GET_HEALTHCAPACITY > 0xA0) {
                                    SET_TIMERY_A0(1, 54);
                                } else {
                                    SET_TIMERY_A0(1, 46);
                                }
                            }

                            if (GET_TIMER2STATE >= 3) {
                                D_8015FFE4--;
                                if (D_8015FFE4 == 0) {
                                    D_8015FFE4 = 20;
                                    if (GET_TIMER2STATE == 4) {
                                        GET_TIMER2VALUE--;
                                        osSyncPrintf("TOTAL_EVENT_TM=%d\n", GET_TIMER2VALUE);

                                        if (GET_TIMER2VALUE <= 0) {
                                            if (!Flags_GetSwitch(play, 0x37) ||
                                                ((play->sceneId != SCENE_GANON_DEMO) &&
                                                 (play->sceneId != SCENE_GANON_FINAL) &&
                                                 (play->sceneId != SCENE_GANON_SONOGO) &&
                                                 (play->sceneId != SCENE_GANONTIKA_SONOGO))) {
                                                D_8015FFE6 = 40;
                                                SET_TIMER2STATE(5);
                                                SET_CUTSCENEINDEX(0);
                                                Message_StartTextbox(play, 0x71B0, NULL);
                                                func_8002DF54(play, NULL, 8);
                                            } else {
                                                D_8015FFE6 = 40;
                                                SET_TIMER2STATE(6);
                                            }
                                        } else if (GET_TIMER2VALUE > 60) {
                                            if (timerDigits[4] == 1) {
                                                Audio_PlaySfxGeneral(NA_SE_SY_MESSAGE_WOMAN, &gSfxDefaultPos, 4,
                                                                     &gSfxDefaultFreqAndVolScale,
                                                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                                            }
                                        } else if (GET_TIMER2VALUE > 10) {
                                            if (timerDigits[4] & 1) {
                                                Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                                                     &gSfxDefaultFreqAndVolScale,
                                                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                                            }
                                        } else {
                                            Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_E, &gSfxDefaultPos, 4,
                                                                 &gSfxDefaultFreqAndVolScale,
                                                                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
                                        }
                                    } else {
                                        GET_TIMER2VALUE++;
                                        if (GET_EVENTINF(EVENTINF_10)) {
                                            if (GET_TIMER2VALUE == 240) {
                                                Message_StartTextbox(play, 0x6083, NULL);
                                                CLEAR_EVENTINF(EVENTINF_10);
                                                SET_TIMER2STATE(0);
                                            }
                                        }
                                    }

                                    if ((GET_TIMER2VALUE % 60) == 0) {
                                        Audio_PlaySfxGeneral(NA_SE_SY_WARNING_COUNT_N, &gSfxDefaultPos, 4,
                                                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                                             &gSfxDefaultReverb);
                                    }
                                }
                            }
                            break;
                        case 6:
                            D_8015FFE6--;
                            if (D_8015FFE6 == 0) {
                                SET_TIMER2STATE(0);
                            }
                            break;
                    }
                    break;
            }

            if (((GET_TIMER1STATE != 0) && (GET_TIMER1STATE != 10)) ||
                (GET_TIMER2STATE != 0)) {
                timerDigits[0] = timerDigits[1] = timerDigits[3] = 0;
                timerDigits[2] = 10; // digit 10 is used as ':' (colon)

                if (GET_TIMER1STATE != 0) {
                    timerDigits[4] = GET_TIMER1VALUE;
                } else {
                    timerDigits[4] = GET_TIMER2VALUE;
                }

                while (timerDigits[4] >= 60) {
                    timerDigits[1]++;
                    if (timerDigits[1] >= 10) {
                        timerDigits[0]++;
                        timerDigits[1] -= 10;
                    }
                    timerDigits[4] -= 60;
                }

                while (timerDigits[4] >= 10) {
                    timerDigits[3]++;
                    timerDigits[4] -= 10;
                }

                // Clock Icon
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 0);
                OVERLAY_DISP =
                    Gfx_TextureIA8(OVERLAY_DISP, gClockIconTex, 16, 16, (GET_TIMERX_A0_VOID0(svar6)),
                                   (GET_TIMERY_A0_VOID0(svar6)) + 2, 16, 16, 1 << 10, 1 << 10);

                // Timer Counter
                gDPPipeSync(OVERLAY_DISP++);
                gDPSetCombineLERP(OVERLAY_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE,
                                  TEXEL0, 0, PRIMITIVE, 0);

                if (GET_TIMER1STATE != 0) {
                    if ((GET_TIMER1VALUE < 10) && (GET_TIMER1STATE < 11)) {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 50, 0, 255);
                    } else {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);
                    }
                } else {
                    if ((GET_TIMER2VALUE < 10) && (GET_TIMER2STATE < 6)) {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 50, 0, 255);
                    } else {
                        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 0, 255);
                    }
                }

                for (svar1 = 0; svar1 < 5; svar1++) {
                    OVERLAY_DISP =
                        Gfx_TextureI8(OVERLAY_DISP, ((u8*)gCounterDigit0Tex + (8 * 16 * timerDigits[svar1])), 8, 16,
                                      (GET_TIMERX_A0_VOID0(svar6)) + timerDigitLeftPos[svar1],
                                      (GET_TIMERY_A0_VOID0(svar6)), digitWidth[svar1], VREG(42), VREG(43) << 1,
                                      VREG(43) << 1);
                }
            }
        }
    }

    if (pauseCtx->debugState == 3) {
        FlagSet_Update(play);
    }

    if (interfaceCtx->unk_244 != 0) {
        gDPPipeSync(OVERLAY_DISP++);
        gSPDisplayList(OVERLAY_DISP++, sSetupDL_80125A60);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 0, 0, interfaceCtx->unk_244);
        gDPFillRectangle(OVERLAY_DISP++, 0, 0, gScreenWidth - 1, gScreenHeight - 1);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_parameter.c", 4269);
}

void Interface_Update(PlayState* play) {
    static u8 D_80125B60 = false;
    static s16 sPrevTimeSpeed = 0;
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Player* player = GET_PLAYER(play);
    s16 alpha;
    s16 alpha1;
    u16 action;
    Input* debugInput = &play->state.input[2];

    if (CHECK_BTN_ALL(debugInput->press.button, BTN_DLEFT)) {
        SET_LANGUAGE(LANGUAGE_ENG);
        osSyncPrintf("J_N=%x J_N=%x\n", GET_LANGUAGE, &GET_LANGUAGE);
    } else if (CHECK_BTN_ALL(debugInput->press.button, BTN_DUP)) {
        SET_LANGUAGE(LANGUAGE_GER);
        osSyncPrintf("J_N=%x J_N=%x\n", GET_LANGUAGE, &GET_LANGUAGE);
    } else if (CHECK_BTN_ALL(debugInput->press.button, BTN_DRIGHT)) {
        SET_LANGUAGE(LANGUAGE_FRA);
        osSyncPrintf("J_N=%x J_N=%x\n", GET_LANGUAGE, &GET_LANGUAGE);
    }

    if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0)) {
        if ((GET_MINIGAMESTATE == 1) || !IS_CUTSCENE_LAYER ||
            ((play->sceneId == SCENE_SPOT20) && (GET_SCENELAYER == 4))) {
            if ((msgCtx->msgMode == MSGMODE_NONE) ||
                ((msgCtx->msgMode != MSGMODE_NONE) && (play->sceneId == SCENE_BOWLING))) {
                if (play->gameOverCtx.state == GAMEOVER_INACTIVE) {
                    func_80083108(play);
                }
            }
        }
    }

    switch (GET_UNK_13E8) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            alpha = 255 - (GET_UNK_13EC << 5);
            if (alpha < 0) {
                alpha = 0;
            }

            func_80082850(play, alpha);
            GET_UNK_13EC++;

            if (alpha == 0) {
                SET_UNK_13E8(0);
            }
            break;
        case 50:
            alpha = 255 - (GET_UNK_13EC << 5);
            if (alpha < 0) {
                alpha = 0;
            }

            alpha1 = 255 - alpha;
            if (alpha1 >= 255) {
                alpha1 = 255;
            }

            osSyncPrintf("case 50 : alpha=%d  alpha1=%d\n", alpha, alpha1);
            func_80082644(play, alpha1);

            if (interfaceCtx->healthAlpha != 255) {
                interfaceCtx->healthAlpha = alpha1;
            }

            if (interfaceCtx->magicAlpha != 255) {
                interfaceCtx->magicAlpha = alpha1;
            }

            switch (play->sceneId) {
                case SCENE_SPOT00:
                case SCENE_SPOT01:
                case SCENE_SPOT02:
                case SCENE_SPOT03:
                case SCENE_SPOT04:
                case SCENE_SPOT05:
                case SCENE_SPOT06:
                case SCENE_SPOT07:
                case SCENE_SPOT08:
                case SCENE_SPOT09:
                case SCENE_SPOT10:
                case SCENE_SPOT11:
                case SCENE_SPOT12:
                case SCENE_SPOT13:
                case SCENE_SPOT15:
                case SCENE_SPOT16:
                case SCENE_SPOT17:
                case SCENE_SPOT18:
                case SCENE_SPOT20:
                case SCENE_GANON_TOU:
                    if (interfaceCtx->minimapAlpha < 170) {
                        interfaceCtx->minimapAlpha = alpha1;
                    } else {
                        interfaceCtx->minimapAlpha = 170;
                    }
                    break;
                default:
                    if (interfaceCtx->minimapAlpha != 255) {
                        interfaceCtx->minimapAlpha = alpha1;
                    }
                    break;
            }

            GET_UNK_13EC++;
            if (alpha1 == 255) {
                SET_UNK_13E8(0);
            }

            break;
        case 52:
            SET_UNK_13E8(1);
            func_80082850(play, 0);
            SET_UNK_13E8(0);
        default:
            break;
    }

    Map_Update(play);

    if (GET_HEALTHACCUMULATOR != 0) {
        GET_HEALTHACCUMULATOR -= 4;
        GET_HEALTH += 4;

        if ((GET_HEALTH & 0xF) < 4) {
            Audio_PlaySfxGeneral(NA_SE_SY_HP_RECOVER, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                 &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
        }

        osSyncPrintf("now_life=%d  max_life=%d\n", GET_HEALTH,
                     GET_HEALTHCAPACITY);

        if (GET_HEALTH >= GET_HEALTHCAPACITY) {
            SET_HEALTH(GET_HEALTHCAPACITY);
            osSyncPrintf("S_Private.now_life=%d  S_Private.max_life=%d\n", GET_HEALTH,
                         GET_HEALTHCAPACITY);
            SET_HEALTHACCUMULATOR(0);
        }
    }

    Health_UpdateBeatingHeart(play);
    D_80125A58 = func_8008F2F8(play);

    if (D_80125A58 == 1) {
        if (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) == EQUIP_VALUE_TUNIC_GORON) {
            D_80125A58 = 0;
        }
    } else if ((func_8008F2F8(play) >= 2) && (func_8008F2F8(play) < 5)) {
        if (CUR_EQUIP_VALUE(EQUIP_TYPE_TUNIC) == EQUIP_VALUE_TUNIC_ZORA) {
            D_80125A58 = 0;
        }
    }

    Health_UpdateMeter(play);

    if ((GET_TIMER1STATE >= 3) && (play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) &&
        (msgCtx->msgMode == MSGMODE_NONE) && !(player->stateFlags2 & PLAYER_STATE2_24) &&
        (play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->transitionMode == TRANS_MODE_OFF) &&
        !Play_InCsMode(play)) {}

    if (GET_RUPEEACCUMULATOR != 0) {
        if (GET_RUPEEACCUMULATOR > 0) {
            if (GET_RUPEES < CUR_CAPACITY(UPG_WALLET)) {
                GET_RUPEEACCUMULATOR--;
                GET_RUPEES++;
                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            } else {
                // "Rupee Amount MAX = %d"
                osSyncPrintf("ルピー数ＭＡＸ = %d\n", CUR_CAPACITY(UPG_WALLET));
                SET_RUPEES(CUR_CAPACITY(UPG_WALLET));
                SET_RUPEEACCUMULATOR(0);
            }
        } else if (GET_RUPEES != 0) {
            if (GET_RUPEEACCUMULATOR <= -50) {
                GET_RUPEEACCUMULATOR += 10;
                GET_RUPEES -= 10;

                if (GET_RUPEES < 0) {
                    SET_RUPEES(0);
                }

                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            } else {
                GET_RUPEEACCUMULATOR++;
                GET_RUPEES--;
                Audio_PlaySfxGeneral(NA_SE_SY_RUPY_COUNT, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        } else {
            SET_RUPEEACCUMULATOR(0);
        }
    }

    switch (interfaceCtx->unk_1EC) {
        case 1:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 15700.0f) {
                interfaceCtx->unk_1F4 = -15700.0f;
                interfaceCtx->unk_1EC = 2;
            }
            break;
        case 2:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 0.0f) {
                interfaceCtx->unk_1F4 = 0.0f;
                interfaceCtx->unk_1EC = 0;
                interfaceCtx->unk_1EE = interfaceCtx->unk_1F0;
                action = interfaceCtx->unk_1EE;
                if ((action == DO_ACTION_MAX) || (action == DO_ACTION_MAX + 1)) {
                    action = DO_ACTION_NONE;
                }
                Interface_LoadActionLabel(interfaceCtx, action, 0);
            }
            break;
        case 3:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 15700.0f) {
                interfaceCtx->unk_1F4 = -15700.0f;
                interfaceCtx->unk_1EC = 2;
            }
            break;
        case 4:
            interfaceCtx->unk_1F4 += 31400.0f / WREG(5);
            if (interfaceCtx->unk_1F4 >= 0.0f) {
                interfaceCtx->unk_1F4 = 0.0f;
                interfaceCtx->unk_1EC = 0;
                interfaceCtx->unk_1EE = interfaceCtx->unk_1F0;
                action = interfaceCtx->unk_1EE;
                if ((action == DO_ACTION_MAX) || (action == DO_ACTION_MAX + 1)) {
                    action = DO_ACTION_NONE;
                }
                Interface_LoadActionLabel(interfaceCtx, action, 0);
            }
            break;
    }

    WREG(7) = interfaceCtx->unk_1F4;

    // Update Magic
    if ((play->pauseCtx.state == 0) && (play->pauseCtx.debugState == 0) && (msgCtx->msgMode == MSGMODE_NONE) &&
        (play->transitionTrigger == TRANS_TRIGGER_OFF) && (play->gameOverCtx.state == GAMEOVER_INACTIVE) &&
        (play->transitionMode == TRANS_MODE_OFF) && ((play->csCtx.state == CS_STATE_IDLE) || !Player_InCsMode(play))) {

        if (GET_ISMAGICACQUIRED && (GET_MAGICLEVEL == 0)) {
            SET_MAGICLEVEL(GET_ISDOUBLEMAGICACQUIRED + 1);
            SET_MAGICSTATE(MAGIC_STATE_STEP_CAPACITY);
            osSyncPrintf(VT_FGCOL(YELLOW));
            osSyncPrintf("魔法スター─────ト！！！！！！！！！\n"); // "Magic Start!!!!!!!!!"
            osSyncPrintf("MAGIC_MAX=%d\n", GET_MAGICLEVEL);
            osSyncPrintf("MAGIC_NOW=%d\n", GET_MAGIC);
            osSyncPrintf("Z_MAGIC_NOW_NOW=%d\n", GET_MAGICFILLTARGET);
            osSyncPrintf("Z_MAGIC_NOW_MAX=%d\n", GET_MAGICCAPACITY);
            osSyncPrintf(VT_RST);
        }

        Magic_Update(play);
    }

    if (GET_TIMER1STATE == 0) {
        if (((D_80125A58 == 1) || (D_80125A58 == 2) || (D_80125A58 == 4)) &&
            ((GET_HEALTH >> 1) != 0)) {
            SET_TIMER1STATE(1);
            SET_TIMERX_A0(0, 140);
            SET_TIMERY_A0(0, 80);
            D_80125A5C = true;
        }
    } else {
        if (((D_80125A58 == 0) || (D_80125A58 == 3)) && (GET_TIMER1STATE < 5)) {
            SET_TIMER1STATE(0);
        }
    }

    if (GET_MINIGAMESTATE == 1) {
        GET_MINIGAMESCORE += interfaceCtx->unk_23C;
        interfaceCtx->unk_23C = 0;

        if (sHBAScoreTier == 0) {
            if (GET_MINIGAMESCORE >= 1000) {
                sHBAScoreTier++;
            }
        } else if (sHBAScoreTier == 1) {
            if (GET_MINIGAMESCORE >= 1500) {
                sHBAScoreTier++;
            }
        }

        sHBAScoreDigits[0] = sHBAScoreDigits[1] = 0;
        sHBAScoreDigits[2] = 0;
        sHBAScoreDigits[3] = GET_MINIGAMESCORE;

        while (sHBAScoreDigits[3] >= 1000) {
            sHBAScoreDigits[0]++;
            sHBAScoreDigits[3] -= 1000;
        }

        while (sHBAScoreDigits[3] >= 100) {
            sHBAScoreDigits[1]++;
            sHBAScoreDigits[3] -= 100;
        }

        while (sHBAScoreDigits[3] >= 10) {
            sHBAScoreDigits[2]++;
            sHBAScoreDigits[3] -= 10;
        }
    }

    if (GET_SUNSSONGSTATE != SUNSSONG_INACTIVE) {
        // exit out of ocarina mode after suns song finishes playing
        if ((msgCtx->ocarinaAction != OCARINA_ACTION_CHECK_NOWARP_DONE) &&
            (GET_SUNSSONGSTATE == SUNSSONG_START)) {
            play->msgCtx.ocarinaMode = OCARINA_MODE_04;
        }

        // handle suns song in areas where time moves
        if (play->envCtx.sceneTimeSpeed != 0) {
            if (GET_SUNSSONGSTATE != SUNSSONG_SPEED_TIME) {
                D_80125B60 = false;
                if ((GET_DAYTIME >= CLOCK_TIME(6, 30)) &&
                    (GET_DAYTIME <= CLOCK_TIME(18, 0) + 1)) {
                    D_80125B60 = true;
                }

                SET_SUNSSONGSTATE(SUNSSONG_SPEED_TIME);
                sPrevTimeSpeed = gTimeSpeed;
                gTimeSpeed = 400;
            } else if (!D_80125B60) {
                if ((GET_DAYTIME >= CLOCK_TIME(6, 30)) &&
                    (GET_DAYTIME <= CLOCK_TIME(18, 0) + 1)) {
                    SET_SUNSSONGSTATE(SUNSSONG_INACTIVE);
                    gTimeSpeed = sPrevTimeSpeed;
                    play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                }
            } else if (GET_DAYTIME > CLOCK_TIME(18, 0) + 1) {
                SET_SUNSSONGSTATE(SUNSSONG_INACTIVE);
                gTimeSpeed = sPrevTimeSpeed;
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            }
        } else if ((play->roomCtx.curRoom.behaviorType1 != ROOM_BEHAVIOR_TYPE1_1) &&
                   (interfaceCtx->restrictions.sunsSong != 3)) {
            if ((GET_DAYTIME >= CLOCK_TIME(6, 30)) &&
                (GET_DAYTIME < CLOCK_TIME(18, 0) + 1)) {
                SET_NEXTDAYTIME(NEXT_TIME_NIGHT);
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                SET_NEXTTRANSITIONTYPE(TRANS_TYPE_FADE_BLACK);
                play->unk_11DE9 = true;
            } else {
                SET_NEXTDAYTIME(NEXT_TIME_DAY);
                play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                SET_NEXTTRANSITIONTYPE(TRANS_TYPE_FADE_WHITE);
                play->unk_11DE9 = true;
            }

            if (play->sceneId == SCENE_SPOT13) {
                play->transitionType = TRANS_TYPE_SANDSTORM_PERSIST;
                SET_NEXTTRANSITIONTYPE(TRANS_TYPE_SANDSTORM_PERSIST);
            }

            SET_RESPAWNFLAG(-2);
            play->nextEntranceIndex = GET_ENTRANCEINDEX;
            play->transitionTrigger = TRANS_TRIGGER_START;
            SET_SUNSSONGSTATE(SUNSSONG_INACTIVE);
            func_800F6964(30);
            SET_SEQID((u8)NA_BGM_DISABLED);
            SET_NATUREAMBIENCEID(NATURE_ID_DISABLED);
        } else {
            SET_SUNSSONGSTATE(SUNSSONG_SPECIAL);
        }
    }
}
