#include "global.h"
#include "vt.h"

#define SLOT_SIZE (sizeof(SaveContext) + 0x28)
#define CHECKSUM_SIZE (sizeof(Save) / 2)

#define DEATHS offsetof(SaveContext, save.info.playerData.deaths)
#define NAME offsetof(SaveContext, save.info.playerData.playerName)
#define N64DD offsetof(SaveContext, save.info.playerData.n64ddFlag)
#define HEALTH_CAP offsetof(SaveContext, save.info.playerData.healthCapacity)
#define QUEST offsetof(SaveContext, save.info.inventory.questItems)
#define DEFENSE offsetof(SaveContext, save.info.inventory.defenseHearts)
#define HEALTH offsetof(SaveContext, save.info.playerData.health)

#define SLOT_OFFSET(index) (SRAM_HEADER_SIZE + 0x10 + (index * SLOT_SIZE))

u16 gSramSlotOffsets[] = {
    SLOT_OFFSET(0),
    SLOT_OFFSET(1),
    SLOT_OFFSET(2),
    // the latter three saves are backup saves for the former saves
    SLOT_OFFSET(3),
    SLOT_OFFSET(4),
    SLOT_OFFSET(5),
};

static char sZeldaMagic[] = { '\0', '\0', '\0', '\x98', '\x09', '\x10', '\x21', 'Z', 'E', 'L', 'D', 'A' };

static SavePlayerData sNewSavePlayerData = {
    { '\0', '\0', '\0', '\0', '\0', '\0' },             // newf
    0,                                                  // deaths
    { 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0x3E }, // playerName
    0,                                                  // n64ddFlag
    0x30,                                               // healthCapacity
    0x30,                                               // defense
    0,                                                  // magicLevel
    MAGIC_NORMAL_METER,                                 // magic
    0,                                                  // rupees
    0,                                                  // swordHealth
    0,                                                  // naviTimer
    false,                                              // isMagicAcquired
    0,                                                  // unk_1F
    false,                                              // isDoubleMagicAcquired
    false,                                              // isDoubleDefenseAcquired
    0,                                                  // bgsFlag
    0,                                                  // ocarinaGameRoundNum
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // childEquips
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // adultEquips
    0,                                                  // unk_38
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // unk_3C
    SCENE_LINK_HOME,                                    // savedSceneId
};

static ItemEquips sNewSaveEquips = {
    { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
    { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
    0x1100,                                         // equipment
};

static Inventory sNewSaveInventory = {
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, // items
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },                         // ammo
    // equipment
    (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
     ((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4))),
    0,                                                              // upgrades
    0,                                                              // questItems
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    }, // dungeonKeys
    0, // defenseHearts
    0, // gsTokens
};

static u16 sNewSaveChecksum = 0;

/**
 *  Initialize new save.
 *  This save has an empty inventory with 3 hearts and single magic.
 */
void Sram_InitNewSave(void) {
    SaveContext* temp = &GET_G_SAVE_CONTEXT;

    bzero(&GET_INFO, sizeof(SaveInfo));
    SET_TOTAL_DAYS(0)
    SET_BGS_DAY_COUNT(0)

    SET_PLAYER_DATA(sNewSavePlayerData)
    SET_EQUIPS(sNewSaveEquips)
    SET_INVENTORY(sNewSaveInventory)

    temp->save.info.checksum = sNewSaveChecksum;
    SET_HORSE_DATA_SCENE_ID(SCENE_SPOT00)
    SET_HORSE_DATA_POS_X(-1840)
    SET_HORSE_DATA_POS_Y(72)
    SET_HORSE_DATA_POS_Z(5497)
    SET_HORSE_DATA_ANGLE(-0x6AD9)
    SET_MAGIC_LEVEL(0)
    GET_INF_TABLE[INFTABLE_1DX_INDEX] = 1;
    GET_SCENE_FLAGS[5].swch = 0x40000000;
}

static SavePlayerData sDebugSavePlayerData = {
    { 'Z', 'E', 'L', 'D', 'A', 'Z' },                   // newf
    0,                                                  // deaths
    { 0x15, 0x12, 0x17, 0x14, 0x3E, 0x3E, 0x3E, 0x3E }, // playerName ( "LINK" )
    0,                                                  // n64ddFlag
    0xE0,                                               // healthCapacity
    0xE0,                                               // health
    0,                                                  // magicLevel
    MAGIC_NORMAL_METER,                                 // magic
    150,                                                // rupees
    8,                                                  // swordHealth
    0,                                                  // naviTimer
    true,                                               // isMagicAcquired
    0,                                                  // unk_1F
    false,                                              // isDoubleMagicAcquired
    false,                                              // isDoubleDefenseAcquired
    0,                                                  // bgsFlag
    0,                                                  // ocarinaGameRoundNum
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // childEquips
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // adultEquips
    0,                                                  // unk_38
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // unk_3C
    SCENE_SPOT00,                                       // savedSceneId
};

static ItemEquips sDebugSaveEquips = {
    { ITEM_SWORD_MASTER, ITEM_BOW, ITEM_BOMB, ITEM_OCARINA_FAIRY }, // buttonItems
    { SLOT_BOW, SLOT_BOMB, SLOT_OCARINA },                          // cButtonSlots
    // equipment
    (EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)) | (EQUIP_VALUE_SHIELD_HYLIAN << (EQUIP_TYPE_SHIELD * 4)) |
        (EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4)) | (EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4)),
};

static Inventory sDebugSaveInventory = {
    {
        ITEM_STICK,     ITEM_NUT,           ITEM_BOMB,         ITEM_BOW,         ITEM_ARROW_FIRE,  ITEM_DINS_FIRE,
        ITEM_SLINGSHOT, ITEM_OCARINA_FAIRY, ITEM_BOMBCHU,      ITEM_HOOKSHOT,    ITEM_ARROW_ICE,   ITEM_FARORES_WIND,
        ITEM_BOOMERANG, ITEM_LENS,          ITEM_BEAN,         ITEM_HAMMER,      ITEM_ARROW_LIGHT, ITEM_NAYRUS_LOVE,
        ITEM_BOTTLE,    ITEM_POTION_RED,    ITEM_POTION_GREEN, ITEM_POTION_BLUE, ITEM_POCKET_EGG,  ITEM_WEIRD_EGG,
    },                                                        // items
    { 50, 50, 10, 30, 1, 1, 30, 1, 50, 1, 1, 1, 1, 1, 1, 1 }, // ammo
    // equipment
    ((((1 << EQUIP_INV_SWORD_KOKIRI) << (EQUIP_TYPE_SWORD * 4)) |
      ((1 << EQUIP_INV_SWORD_MASTER) << (EQUIP_TYPE_SWORD * 4)) |
      ((1 << EQUIP_INV_SWORD_BGS) << (EQUIP_TYPE_SWORD * 4))) |
     (((1 << EQUIP_INV_SHIELD_DEKU) << (EQUIP_TYPE_SHIELD * 4)) |
      ((1 << EQUIP_INV_SHIELD_HYLIAN) << (EQUIP_TYPE_SHIELD * 4)) |
      ((1 << EQUIP_INV_SHIELD_MIRROR) << (EQUIP_TYPE_SHIELD * 4))) |
     (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
      ((1 << EQUIP_INV_TUNIC_GORON) << (EQUIP_TYPE_TUNIC * 4)) |
      ((1 << EQUIP_INV_TUNIC_ZORA) << (EQUIP_TYPE_TUNIC * 4))) |
     (((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4)) |
      ((1 << EQUIP_INV_BOOTS_IRON) << (EQUIP_TYPE_BOOTS * 4)) |
      ((1 << EQUIP_INV_BOOTS_HOVER) << (EQUIP_TYPE_BOOTS * 4)))),
    0x125249,                                                       // upgrades
    0x1E3FFFF,                                                      // questItems
    { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
    { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // dungeonKeys
    0,                                                              // defenseHearts
    0,                                                              // gsTokens
};

static u16 sDebugSaveChecksum = 0;

/**
 *  Initialize debug save. This is also used on the Title Screen
 *  This save has a mostly full inventory with 10 hearts and single magic.
 *
 *  Some noteable flags that are set:
 *  Showed Mido sword/shield, met Deku Tree, Deku Tree mouth opened,
 *  used blue warp in Gohmas room, Zelda fled castle, light arrow cutscene watched,
 *  and set water level in Water Temple to lowest level.
 */
void Sram_InitDebugSave(void) {
    SaveContext* temp = &GET_G_SAVE_CONTEXT;

    bzero(&GET_INFO, sizeof(SaveInfo));
    SET_TOTAL_DAYS(0)
    SET_BGS_DAY_COUNT(0)

    SET_PLAYER_DATA(sDebugSavePlayerData)
    SET_EQUIPS(sDebugSaveEquips)
    SET_INVENTORY(sDebugSaveInventory)

    temp->save.info.checksum = sDebugSaveChecksum;
    SET_HORSE_DATA_SCENE_ID(SCENE_SPOT00)
    SET_HORSE_DATA_POS_X(-1840)
    SET_HORSE_DATA_POS_Y(72)
    SET_HORSE_DATA_POS_Z(5497)
    SET_HORSE_DATA_ANGLE(-0x6AD9)
    GET_INF_TABLE[0] |= 0x5009;
    GET_EVENT_CHK_INF[0] |= 0x123F;
    SET_EVENTCHKINF(EVENTCHKINF_80);
    SET_EVENTCHKINF(EVENTCHKINF_C4);

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        GET_EQUIPS_BUTTON_ITEMS[0] = ITEM_SWORD_KOKIRI;
        Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_KOKIRI);
        if (GET_FILE_NUM == 0xFF) {
            GET_EQUIPS_BUTTON_ITEMS[1] = ITEM_SLINGSHOT;
            GET_EQUIPS_C_BUTTON_SLOTS[0] = SLOT_SLINGSHOT;
            Inventory_ChangeEquipment(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_DEKU);
        }
    }

    SET_ENTRANCE_INDEX(ENTR_SPOT00_0)
    SET_MAGIC_LEVEL(0)
    GET_SCENE_FLAGS[5].swch = 0x40000000;
}

static s16 sDungeonEntrances[] = {
    ENTR_YDAN_0,      ENTR_DDAN_0,      ENTR_BDAN_0,         ENTR_BMORI1_0,           ENTR_HIDAN_0, ENTR_MIZUSIN_0,
    ENTR_JYASINZOU_0, ENTR_HAKADAN_0,   ENTR_HAKADANCH_0,    ENTR_ICE_DOUKUTO_0,      ENTR_GANON_0, ENTR_MEN_0,
    ENTR_GERUDOWAY_0, ENTR_GANONTIKA_0, ENTR_GANON_SONOGO_0, ENTR_GANONTIKA_SONOGO_0,
};

/**
 *  Copy save currently on the buffer to Save Context and complete various tasks to open the save.
 *  This includes:
 *  - Set proper entrance depending on where the game was saved
 *  - If health is less than 3 hearts, give 3 hearts
 *  - If either scarecrow song is set, copy them from save context to the proper location
 *  - Handle a case where the player saved and quit after zelda cutscene but didnt get the song
 *  - Give and equip master sword if player is adult and doesnt have kokiri sword (bug?)
 *  - Revert any trade items that spoil
 */
void Sram_OpenSave(SramContext* sramCtx) {
    u16 i;
    u16 j;
    u8* ptr;

    osSyncPrintf("個人Ｆｉｌｅ作成\n"); // "Create personal file"
    i = gSramSlotOffsets[GET_FILE_NUM];
    osSyncPrintf("ぽいんと＝%x(%d)\n", i, GET_FILE_NUM); // "Point="

    MemCpy(&GET_G_SAVE_CONTEXT, sramCtx->readBuff + i, sizeof(Save));

    osSyncPrintf(VT_FGCOL(YELLOW));
    osSyncPrintf("SCENE_DATA_ID = %d   SceneNo = %d\n", GET_SAVED_SCENE_ID,
                 (GET_ENTRANCE_INDEX_VOID0));

    switch (GET_SAVED_SCENE_ID) {
        case SCENE_YDAN:
        case SCENE_DDAN:
        case SCENE_BDAN:
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
            SET_ENTRANCE_INDEX(sDungeonEntrances[gSaveContext.save.info.playerData.savedSceneId])
            break;

        case SCENE_YDAN_BOSS:
            SET_ENTRANCE_INDEX(ENTR_YDAN_0)
            break;

        case SCENE_DDAN_BOSS:
            SET_ENTRANCE_INDEX(ENTR_DDAN_0)
            break;

        case SCENE_BDAN_BOSS:
            SET_ENTRANCE_INDEX(ENTR_BDAN_0)
            break;

        case SCENE_MORIBOSSROOM:
            SET_ENTRANCE_INDEX(ENTR_BMORI1_0)
            break;

        case SCENE_FIRE_BS:
            SET_ENTRANCE_INDEX(ENTR_HIDAN_0)
            break;

        case SCENE_MIZUSIN_BS:
            SET_ENTRANCE_INDEX(ENTR_MIZUSIN_0)
            break;

        case SCENE_JYASINBOSS:
            SET_ENTRANCE_INDEX(ENTR_JYASINZOU_0)
            break;

        case SCENE_HAKADAN_BS:
            SET_ENTRANCE_INDEX(ENTR_HAKADAN_0)
            break;

        case SCENE_GANON_SONOGO:
        case SCENE_GANONTIKA_SONOGO:
        case SCENE_GANON_BOSS:
        case SCENE_GANON_FINAL:
        case SCENE_GANON_DEMO:
            SET_ENTRANCE_INDEX(ENTR_GANON_0)
            break;

        default:
            if (GET_SAVED_SCENE_ID != SCENE_LINK_HOME) {
                SET_ENTRANCE_INDEX((LINK_AGE_IN_YEARS == YEARS_CHILD) ? ENTR_LINK_HOME_0 : ENTR_TOKINOMA_7)
            } else {
                SET_ENTRANCE_INDEX(ENTR_LINK_HOME_0)
            }
            break;
    }

    osSyncPrintf("scene_no = %d\n", GET_ENTRANCE_INDEX);
    osSyncPrintf(VT_RST);

    if (GET_HEALTH < 0x30) {
        SET_HEALTH(0x30)
    }

    if (GET_SCARECROW_LONG_SONG_SET) {
        osSyncPrintf(VT_FGCOL(BLUE));
        osSyncPrintf("\n====================================================================\n");

        MemCpy(gScarecrowLongSongPtr, GET_SCARECROW_LONG_SONG,
               sizeof(GET_SCARECROW_LONG_SONG));

        ptr = (u8*)gScarecrowLongSongPtr;
        for (i = 0; i < ARRAY_COUNT(GET_SCARECROW_LONG_SONG); i++, ptr++) {
            osSyncPrintf("%d, ", *ptr);
        }

        osSyncPrintf("\n====================================================================\n");
        osSyncPrintf(VT_RST);
    }

    if (GET_SCARECROW_SPAWN_SONG_SET) {
        osSyncPrintf(VT_FGCOL(GREEN));
        osSyncPrintf("\n====================================================================\n");

        MemCpy(gScarecrowSpawnSongPtr, GET_SCARECROW_SPAWN_SONG,
               sizeof(GET_SCARECROW_SPAWN_SONG));

        ptr = gScarecrowSpawnSongPtr;
        for (i = 0; i < ARRAY_COUNT(GET_SCARECROW_SPAWN_SONG); i++, ptr++) {
            osSyncPrintf("%d, ", *ptr);
        }

        osSyncPrintf("\n====================================================================\n");
        osSyncPrintf(VT_RST);
    }

    // if zelda cutscene has been watched but lullaby was not obtained, restore cutscene and take away letter
    if (GET_EVENTCHKINF(EVENTCHKINF_40) && !CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
        i = GET_EVENT_CHK_INF[EVENTCHKINF_40_INDEX] & ~EVENTCHKINF_40_MASK;
        GET_EVENT_CHK_INF[EVENTCHKINF_40_INDEX] = i;

        INV_CONTENT(ITEM_LETTER_ZELDA) = ITEM_CHICKEN;

        for (j = 1; j < 4; j++) {
            if (GET_EQUIPS_BUTTON_ITEMS[j] == ITEM_LETTER_ZELDA) {
                GET_EQUIPS_BUTTON_ITEMS[j] = ITEM_CHICKEN;
            }
        }
    }

    if (LINK_AGE_IN_YEARS == YEARS_ADULT && !CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER)) {
        GET_INVENTORY_EQUIPMENT |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
        GET_EQUIPS_BUTTON_ITEMS[0] = ITEM_SWORD_MASTER;
        GET_EQUIPS_EQUIPMENT &= ~(0xF << (EQUIP_TYPE_SWORD * 4));
        GET_EQUIPS_EQUIPMENT |= EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4);
    }

    for (i = 0; i < ARRAY_COUNT(gSpoilingItems); i++) {
        if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[i]) {
            INV_CONTENT(gSpoilingItemReverts[i]) = gSpoilingItemReverts[i];

            for (j = 1; j < 4; j++) {
                if (GET_EQUIPS_BUTTON_ITEMS[j] == gSpoilingItems[i]) {
                    GET_EQUIPS_BUTTON_ITEMS[j] = gSpoilingItemReverts[i];
                }
            }
        }
    }

    SET_MAGIC_LEVEL(0)
}

/**
 *  Write the contents of the Save Context to a main and backup slot in SRAM.
 *  Note: The whole Save Context is written even though only the `save` substruct is read back later
 */
void Sram_WriteSave(SramContext* sramCtx) {
    u16 offset;
    u16 checksum;
    u16 j;
    u16* ptr;

    SET_CHECKSUM(0)

    ptr = (u16*)&GET_G_SAVE_CONTEXT;
    checksum = 0;
    j = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    SET_CHECKSUM(checksum)

    ptr = (u16*)&GET_G_SAVE_CONTEXT;
    checksum = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    offset = gSramSlotOffsets[GET_FILE_NUM];
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);

    ptr = (u16*)&GET_G_SAVE_CONTEXT;
    checksum = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    offset = gSramSlotOffsets[GET_FILE_NUM + 3];
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);
}

/**
 *  For all 3 slots, verify that the checksum is correct. If corrupted, attempt to load a backup save.
 *  If backup is also corrupted, default to a new save (or debug save for slot 0 on debug rom).
 *
 *  After verifying all 3 saves, pass relevant data to File Select to be displayed.
 */
void Sram_VerifyAndLoadAllSaves(FileSelectState* fileSelect, SramContext* sramCtx) {
    u16 i;
    u16 newChecksum;
    u16 slotNum;
    u16 offset;
    u16 j;
    u16 oldChecksum;
    u16* ptr;
    u16 dayTime;

    osSyncPrintf("ＳＲＡＭ ＳＴＡＲＴ─ＬＯＡＤ\n");
    bzero(sramCtx->readBuff, SRAM_SIZE);
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);

    dayTime = (GET_DAY_TIME_VOID0);

    for (slotNum = 0; slotNum < 3; slotNum++) {
        offset = gSramSlotOffsets[slotNum];
        osSyncPrintf("ぽいんと＝%x(%d)    SAVE_MAX=%d\n", offset, GET_FILE_NUM, sizeof(Save));
        MemCpy(&GET_G_SAVE_CONTEXT, sramCtx->readBuff + offset, sizeof(Save));

        oldChecksum = GET_CHECKSUM;
        SET_CHECKSUM(0)
        ptr = (u16*)&GET_G_SAVE_CONTEXT;
        osSyncPrintf("\n＝＝＝＝＝＝＝＝＝＝＝＝＝  Ｓ（%d） ＝＝＝＝＝＝＝＝＝＝＝＝＝\n", slotNum);

        for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++, offset += 2) {
            newChecksum += *ptr++;
        }

        // "SAVE checksum calculation"
        osSyncPrintf("\nＳＡＶＥチェックサム計算  j=%x  mmm=%x  ", newChecksum, oldChecksum);

        if (newChecksum != oldChecksum) {
            // checksum didnt match, try backup save
            osSyncPrintf("ＥＲＲＯＲ！！！ ＝ %x(%d)\n", gSramSlotOffsets[slotNum], slotNum);
            offset = gSramSlotOffsets[slotNum + 3];
            MemCpy(&GET_G_SAVE_CONTEXT, sramCtx->readBuff + offset, sizeof(Save));

            oldChecksum = GET_CHECKSUM;
            SET_CHECKSUM(0)
            ptr = (u16*)&GET_G_SAVE_CONTEXT;
            osSyncPrintf("================= ＢＡＣＫ─ＵＰ ========================\n");

            for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++, offset += 2) {
                newChecksum += *ptr++;
            }
            // "(B) SAVE checksum calculation"
            osSyncPrintf("\n（Ｂ）ＳＡＶＥチェックサム計算  j=%x  mmm=%x  ", newChecksum, oldChecksum);

            if (newChecksum != oldChecksum) {
                // backup save didnt work, make new save
                osSyncPrintf("ＥＲＲＯＲ！！！ ＝ %x(%d+3)\n", gSramSlotOffsets[slotNum + 3], slotNum);
                bzero(&GET_ENTRANCE_INDEX, sizeof(s32));
                bzero(&GET_LINK_AGE, sizeof(s32));
                bzero(&GET_CUTSCENE_INDEX, sizeof(s32));
                // note that GET_DAY_TIME is not actually the sizeof(s32)
                bzero(&GET_DAY_TIME, sizeof(s32));
                bzero(&GET_NIGHT_FLAG, sizeof(s32));
                bzero(&GET_TOTAL_DAYS, sizeof(s32));
                bzero(&GET_BGS_DAY_COUNT, sizeof(s32));

                if (!slotNum) {
                    Sram_InitDebugSave();
                    GET_NEWF[0] = 'Z';
                    GET_NEWF[1] = 'E';
                    GET_NEWF[2] = 'L';
                    GET_NEWF[3] = 'D';
                    GET_NEWF[4] = 'A';
                    GET_NEWF[5] = 'Z';
                    osSyncPrintf("newf=%x,%x,%x,%x,%x,%x\n", GET_NEWF[0],
                                 GET_NEWF[1], GET_NEWF[2],
                                 GET_NEWF[3], GET_NEWF[4],
                                 GET_NEWF[5]);
                } else {
                    Sram_InitNewSave();
                }

                ptr = (u16*)&GET_G_SAVE_CONTEXT;
                osSyncPrintf("\n--------------------------------------------------------------\n");

                for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++) {
                    osSyncPrintf("%x ", *ptr);
                    if (++j == 0x20) {
                        osSyncPrintf("\n");
                        j = 0;
                    }
                    newChecksum += *ptr++;
                }

                SET_CHECKSUM(newChecksum)
                osSyncPrintf("\nCheck_Sum=%x(%x)\n", GET_CHECKSUM, newChecksum);

                i = gSramSlotOffsets[slotNum + 3];
                SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + i, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);

                osSyncPrintf("????#%x,%x,%x,%x,%x,%x\n", GET_NEWF[0],
                             GET_NEWF[1], GET_NEWF[2],
                             GET_NEWF[3], GET_NEWF[4],
                             GET_NEWF[5]);
                osSyncPrintf("\nぽいんと＝%x(%d+3)  check_sum=%x(%x)\n", i, slotNum, GET_CHECKSUM,
                             newChecksum);
            }

            i = gSramSlotOffsets[slotNum];
            SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + i, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);

            osSyncPrintf("ぽいんと＝%x(%d)  check_sum=%x(%x)\n", i, slotNum, GET_CHECKSUM,
                         newChecksum);
        } else {
            osSyncPrintf("\nＳＡＶＥデータ ＯＫ！！！！\n"); // "SAVE data OK! ! ! !"
        }
    }

    bzero(sramCtx->readBuff, SRAM_SIZE);
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);
    SET_DAY_TIME(dayTime)

    osSyncPrintf("SAVECT=%x, NAME=%x, LIFE=%x, ITEM=%x,  64DD=%x,  HEART=%x\n", DEATHS, NAME, HEALTH_CAP, QUEST, N64DD,
                 DEFENSE);

    MemCpy(&fileSelect->deaths[0], sramCtx->readBuff + SLOT_OFFSET(0) + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->deaths[1], sramCtx->readBuff + SLOT_OFFSET(1) + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->deaths[2], sramCtx->readBuff + SLOT_OFFSET(2) + DEATHS, sizeof(fileSelect->deaths[0]));

    MemCpy(&fileSelect->fileNames[0], sramCtx->readBuff + SLOT_OFFSET(0) + NAME, sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->fileNames[1], sramCtx->readBuff + SLOT_OFFSET(1) + NAME, sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->fileNames[2], sramCtx->readBuff + SLOT_OFFSET(2) + NAME, sizeof(fileSelect->fileNames[0]));

    MemCpy(&fileSelect->healthCapacities[0], sramCtx->readBuff + SLOT_OFFSET(0) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->healthCapacities[1], sramCtx->readBuff + SLOT_OFFSET(1) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->healthCapacities[2], sramCtx->readBuff + SLOT_OFFSET(2) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));

    MemCpy(&fileSelect->questItems[0], sramCtx->readBuff + SLOT_OFFSET(0) + QUEST, sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->questItems[1], sramCtx->readBuff + SLOT_OFFSET(1) + QUEST, sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->questItems[2], sramCtx->readBuff + SLOT_OFFSET(2) + QUEST, sizeof(fileSelect->questItems[0]));

    MemCpy(&fileSelect->n64ddFlags[0], sramCtx->readBuff + SLOT_OFFSET(0) + N64DD, sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->n64ddFlags[1], sramCtx->readBuff + SLOT_OFFSET(1) + N64DD, sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->n64ddFlags[2], sramCtx->readBuff + SLOT_OFFSET(2) + N64DD, sizeof(fileSelect->n64ddFlags[0]));

    MemCpy(&fileSelect->defense[0], sramCtx->readBuff + SLOT_OFFSET(0) + DEFENSE, sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->defense[1], sramCtx->readBuff + SLOT_OFFSET(1) + DEFENSE, sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->defense[2], sramCtx->readBuff + SLOT_OFFSET(2) + DEFENSE, sizeof(fileSelect->defense[0]));

    MemCpy(&fileSelect->health[0], sramCtx->readBuff + SLOT_OFFSET(0) + HEALTH, sizeof(fileSelect->health[0]));
    MemCpy(&fileSelect->health[1], sramCtx->readBuff + SLOT_OFFSET(1) + HEALTH, sizeof(fileSelect->health[0]));
    MemCpy(&fileSelect->health[2], sramCtx->readBuff + SLOT_OFFSET(2) + HEALTH, sizeof(fileSelect->health[0]));

    osSyncPrintf("f_64dd=%d, %d, %d\n", fileSelect->n64ddFlags[0], fileSelect->n64ddFlags[1],
                 fileSelect->n64ddFlags[2]);
    osSyncPrintf("heart_status=%d, %d, %d\n", fileSelect->defense[0], fileSelect->defense[1], fileSelect->defense[2]);
    osSyncPrintf("now_life=%d, %d, %d\n", fileSelect->health[0], fileSelect->health[1], fileSelect->health[2]);
}

void Sram_InitSave(FileSelectState* fileSelect, SramContext* sramCtx) {
    u16 offset;
    u16 j;
    u16* ptr;
    u16 checksum;

    if (fileSelect->buttonIndex != 0) {
        Sram_InitNewSave();
    } else {
        Sram_InitDebugSave();
    }

    SET_ENTRANCE_INDEX(ENTR_LINK_HOME_0)
    SET_LINK_AGE(LINK_AGE_CHILD)
    SET_DAY_TIME(CLOCK_TIME(10, 0))
    SET_CUTSCENE_INDEX(0xFFF1)

    if (fileSelect->buttonIndex == 0) {
        SET_CUTSCENE_INDEX(0)
    }

    for (offset = 0; offset < 8; offset++) {
        GET_PLAYER_NAME[offset] = fileSelect->fileNames[fileSelect->buttonIndex][offset];
    }

    GET_NEWF[0] = 'Z';
    GET_NEWF[1] = 'E';
    GET_NEWF[2] = 'L';
    GET_NEWF[3] = 'D';
    GET_NEWF[4] = 'A';
    GET_NEWF[5] = 'Z';

    SET_N64DD_FLAG(fileSelect->n64ddFlag)
    osSyncPrintf("６４ＤＤフラグ=%d\n", fileSelect->n64ddFlag);
    osSyncPrintf("newf=%x,%x,%x,%x,%x,%x\n", GET_NEWF[0],
                 GET_NEWF[1], GET_NEWF[2],
                 GET_NEWF[3], GET_NEWF[4],
                 GET_NEWF[5]);
    osSyncPrintf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

    ptr = (u16*)&GET_G_SAVE_CONTEXT;
    j = 0;
    checksum = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        osSyncPrintf("%x ", *ptr);
        checksum += *ptr++;
        if (++j == 0x20) {
            osSyncPrintf("\n");
            j = 0;
        }
    }

    SET_CHECKSUM(checksum)
    osSyncPrintf("\nチェックサム＝%x\n", GET_CHECKSUM); // "Checksum = %x"

    offset = gSramSlotOffsets[GET_FILE_NUM];
    osSyncPrintf("I=%x no=%d\n", offset, GET_FILE_NUM);
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));

    offset = gSramSlotOffsets[GET_FILE_NUM + 3];
    osSyncPrintf("I=%x no=%d\n", offset, GET_FILE_NUM + 3);
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));

    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);

    osSyncPrintf("ＳＡＶＥ終了\n"); // "SAVE end"
    osSyncPrintf("z_common_data.file_no = %d\n", GET_FILE_NUM);
    osSyncPrintf("SAVECT=%x, NAME=%x, LIFE=%x, ITEM=%x,  SAVE_64DD=%x\n", DEATHS, NAME, HEALTH_CAP, QUEST, N64DD);

    j = gSramSlotOffsets[GET_FILE_NUM];

    MemCpy(&fileSelect->deaths[GET_FILE_NUM], sramCtx->readBuff + j + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->fileNames[GET_FILE_NUM], sramCtx->readBuff + j + NAME,
           sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->healthCapacities[GET_FILE_NUM], sramCtx->readBuff + j + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->questItems[GET_FILE_NUM], sramCtx->readBuff + j + QUEST,
           sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->n64ddFlags[GET_FILE_NUM], sramCtx->readBuff + j + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->defense[GET_FILE_NUM], sramCtx->readBuff + j + DEFENSE, sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->health[GET_FILE_NUM], sramCtx->readBuff + j + HEALTH, sizeof(fileSelect->health[0]));

    osSyncPrintf("f_64dd[%d]=%d\n", GET_FILE_NUM, fileSelect->n64ddFlags[GET_FILE_NUM]);
    osSyncPrintf("heart_status[%d]=%d\n", GET_FILE_NUM, fileSelect->defense[GET_FILE_NUM]);
    osSyncPrintf("now_life[%d]=%d\n", GET_FILE_NUM, fileSelect->health[GET_FILE_NUM]);
}

void Sram_EraseSave(FileSelectState* fileSelect, SramContext* sramCtx) {
    s32 offset;

    Sram_InitNewSave();

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex];
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);

    MemCpy(&fileSelect->n64ddFlags[fileSelect->selectedFileIndex], sramCtx->readBuff + offset + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex + 3];
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &GET_G_SAVE_CONTEXT, SLOT_SIZE, OS_WRITE);

    osSyncPrintf("ＣＬＥＡＲ終了\n");
}

void Sram_CopySave(FileSelectState* fileSelect, SramContext* sramCtx) {
    s32 offset;

    osSyncPrintf("ＲＥＡＤ=%d(%x)  ＣＯＰＹ=%d(%x)\n", fileSelect->selectedFileIndex,
                 gSramSlotOffsets[fileSelect->selectedFileIndex], fileSelect->copyDestFileIndex,
                 gSramSlotOffsets[fileSelect->copyDestFileIndex]);

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex];
    MemCpy(&GET_G_SAVE_CONTEXT, sramCtx->readBuff + offset, sizeof(Save));

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex];
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex + 3];
    MemCpy(sramCtx->readBuff + offset, &GET_G_SAVE_CONTEXT, sizeof(Save));

    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex];

    MemCpy(&fileSelect->deaths[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + DEATHS,
           sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->fileNames[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + NAME,
           sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->healthCapacities[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->questItems[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + QUEST,
           sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->n64ddFlags[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->defense[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + DEFENSE,
           sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->health[fileSelect->copyDestFileIndex], (sramCtx->readBuff + offset) + HEALTH,
           sizeof(fileSelect->health[0]));

    osSyncPrintf("f_64dd[%d]=%d\n", GET_FILE_NUM, fileSelect->n64ddFlags[GET_FILE_NUM]);
    osSyncPrintf("heart_status[%d]=%d\n", GET_FILE_NUM, fileSelect->defense[GET_FILE_NUM]);
    osSyncPrintf("ＣＯＰＹ終了\n"); // "Copy end"
}

/**
 *  Write the first 16 bytes of the read buffer to the SRAM header
 */
void Sram_WriteSramHeader(SramContext* sramCtx) {
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_HEADER_SIZE, OS_WRITE);
}

void Sram_InitSram(GameState* gameState, SramContext* sramCtx) {
    u16 i;

    osSyncPrintf("sram_initialize( Game *game, Sram *sram )\n");
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);

    for (i = 0; i < ARRAY_COUNTU(sZeldaMagic) - 3; i++) {
        if (sZeldaMagic[i + SRAM_HEADER_MAGIC] != sramCtx->readBuff[i + SRAM_HEADER_MAGIC]) {
            osSyncPrintf("ＳＲＡＭ破壊！！！！！！\n"); // "SRAM destruction! ! ! ! ! !"
            SET_LANGUAGE(sramCtx->readBuff[SRAM_HEADER_LANGUAGE])
            MemCpy(sramCtx->readBuff, sZeldaMagic, sizeof(sZeldaMagic));
            sramCtx->readBuff[SRAM_HEADER_LANGUAGE] = GET_LANGUAGE;
            Sram_WriteSramHeader(sramCtx);
        }
    }

    SET_AUDIO_SETTING(sramCtx->readBuff[SRAM_HEADER_SOUND] & 3)
    SET_Z_TARGET_SETTING(sramCtx->readBuff[SRAM_HEADER_ZTARGET] & 1)
    SET_LANGUAGE(sramCtx->readBuff[SRAM_HEADER_LANGUAGE])

    if (GET_LANGUAGE >= LANGUAGE_MAX) {
        SET_LANGUAGE(LANGUAGE_ENG)
        sramCtx->readBuff[SRAM_HEADER_LANGUAGE] = GET_LANGUAGE;
        Sram_WriteSramHeader(sramCtx);
    }

    if (CHECK_BTN_ANY(gameState->input[2].cur.button, BTN_DRIGHT)) {
        bzero(sramCtx->readBuff, SRAM_SIZE);
        for (i = 0; i < CHECKSUM_SIZE; i++) {
            sramCtx->readBuff[i] = i;
        }
        SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);
        osSyncPrintf("ＳＲＡＭ破壊！！！！！！\n"); // "SRAM destruction! ! ! ! ! !"
    }

    // "GOOD! GOOD! Size = %d + %d = %d"
    osSyncPrintf("ＧＯＯＤ！ＧＯＯＤ！ サイズ＝%d + %d ＝ %d\n", sizeof(SaveInfo), 4, sizeof(SaveInfo) + 4);
    osSyncPrintf(VT_FGCOL(BLUE));
    osSyncPrintf("Na_SetSoundOutputMode = %d\n", GET_AUDIO_SETTING);
    osSyncPrintf("Na_SetSoundOutputMode = %d\n", GET_AUDIO_SETTING);
    osSyncPrintf("Na_SetSoundOutputMode = %d\n", GET_AUDIO_SETTING);
    osSyncPrintf(VT_RST);
    func_800F6700(GET_AUDIO_SETTING);
}

void Sram_Alloc(GameState* gameState, SramContext* sramCtx) {
    sramCtx->readBuff = GameState_Alloc(gameState, SRAM_SIZE, "../z_sram.c", 1294);
    ASSERT(sramCtx->readBuff != NULL, "sram->read_buff != NULL", "../z_sram.c", 1295);
}

void Sram_Init(PlayState* play, SramContext* sramCtx) {
}
