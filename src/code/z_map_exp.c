#include "global.h"
#include "vt.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "textures/parameter_static/parameter_static.h"

MapData* gMapData;

s16 sPlayerInitialPosX = 0;
s16 sPlayerInitialPosZ = 0;
s16 sPlayerInitialDirection = 0;
s16 sEntranceIconMapIndex = 0;

void Map_SavePlayerInitialInfo(GlobalContext* globalCtx) {
    Player* player = PLAYER;

    sPlayerInitialPosX = player->actor.world.pos.x;
    sPlayerInitialPosZ = player->actor.world.pos.z;
    sPlayerInitialDirection = (s16)((0x7FFF - player->actor.shape.rot.y) / 0x400);
}

void Map_SetPaletteData(GlobalContext* globalCtx, s16 room) {
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;
    s16 paletteIndex = gMapData->roomPalette[mapIndex][room];

    if (interfaceCtx->mapRoomNum == room) {
        interfaceCtx->mapPaletteIndex = paletteIndex;
    }

    osSyncPrintf(VT_FGCOL(YELLOW));
    // Translates to: "PALETE Set"
    osSyncPrintf("ＰＡＬＥＴＥセット 【 i=%x : room=%x 】Room_Inf[%d][4]=%x  ( map_palete_no = %d )\n", paletteIndex,
                 room, mapIndex, gSaveContext.sceneFlags[mapIndex].rooms, interfaceCtx->mapPaletteIndex);
    osSyncPrintf(VT_RST);

    interfaceCtx->mapPalette[paletteIndex * 2] = 2;
    interfaceCtx->mapPalette[paletteIndex * 2 + 1] = 0xBF;
}

void Map_SetFloorPalettesData(GlobalContext* globalCtx, s16 floor) {
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;
    s16 room;
    s16 i;

    for (i = 0; i < 16; i++) {
        interfaceCtx->mapPalette[i] = 0;
        interfaceCtx->mapPalette[i + 16] = 0;
    }

    if (CHECK_DUNGEON_ITEM(DUNGEON_MAP, mapIndex)) {
        interfaceCtx->mapPalette[30] = 0;
        interfaceCtx->mapPalette[31] = 1;
    }

    switch (globalCtx->sceneNum) {
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
        case SCENE_YDAN_BOSS:
        case SCENE_DDAN_BOSS:
        case SCENE_BDAN_BOSS:
        case SCENE_MORIBOSSROOM:
        case SCENE_FIRE_BS:
        case SCENE_MIZUSIN_BS:
        case SCENE_JYASINBOSS:
        case SCENE_HAKADAN_BS:
            for (i = 0; i < gMapData->maxPaletteCount[mapIndex]; i++) {
                room = gMapData->paletteRoom[mapIndex][floor][i];
                if ((room != 0xFF) && (gSaveContext.sceneFlags[mapIndex].rooms & gBitFlags[room])) {
                    Map_SetPaletteData(globalCtx, room);
                }
            }
            break;
    }
}

void Map_InitData(GlobalContext* globalCtx, s16 room) {
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;
    s16 extendedMapIndex;

    switch (globalCtx->sceneNum) {
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
            extendedMapIndex = mapIndex;
            if (globalCtx->sceneNum == SCENE_SPOT02) {
                if (CHECK_QUEST_ITEM(QUEST_SONG_NOCTURNE)) {
                    extendedMapIndex = 0x14;
                }
            } else if (globalCtx->sceneNum == SCENE_SPOT06) {
                if ((LINK_AGE_IN_YEARS == YEARS_ADULT) && !CHECK_QUEST_ITEM(QUEST_MEDALLION_WATER)) {
                    extendedMapIndex = 0x15;
                }
            } else if (globalCtx->sceneNum == SCENE_SPOT09) {
                if ((LINK_AGE_IN_YEARS == YEARS_ADULT) && !((gSaveContext.eventChkInf[9] & 0xF) == 0xF)) {
                    extendedMapIndex = 0x16;
                }
            } else if (globalCtx->sceneNum == SCENE_SPOT12) {
                if ((gSaveContext.eventChkInf[9] & 0xF) == 0xF) {
                    extendedMapIndex = 0x17;
                }
            }
            osSyncPrintf(VT_FGCOL(BLUE));
            osSyncPrintf("ＫＫＫ＝%d\n", extendedMapIndex);
            osSyncPrintf(VT_RST);
            sEntranceIconMapIndex = extendedMapIndex;
            DmaMgr_SendRequest1(interfaceCtx->mapSegment,
                                (u32)_map_grand_staticSegmentRomStart + gMapData->owMinimapTexOffset[extendedMapIndex],
                                gMapData->owMinimapTexSize[mapIndex], "../z_map_exp.c", 309);
            interfaceCtx->unk_258 = mapIndex;
            break;
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
        case SCENE_YDAN_BOSS:
        case SCENE_DDAN_BOSS:
        case SCENE_BDAN_BOSS:
        case SCENE_MORIBOSSROOM:
        case SCENE_FIRE_BS:
        case SCENE_MIZUSIN_BS:
        case SCENE_JYASINBOSS:
        case SCENE_HAKADAN_BS:
            osSyncPrintf(VT_FGCOL(YELLOW));
            // Translates to: "Deku Tree Dungeon MAP Texture DMA"
            osSyncPrintf("デクの樹ダンジョンＭＡＰ テクスチャＤＭＡ(%x) scene_id_offset=%d  VREG(30)=%d\n", room,
                         mapIndex, VREG(30));
            osSyncPrintf(VT_RST);
            DmaMgr_SendRequest1(globalCtx->interfaceCtx.mapSegment,
                                (u32)_map_i_staticSegmentRomStart +
                                    ((gMapData->dgnMinimapTexIndexOffset[mapIndex] + room) * 0xFF0),
                                0xFF0, "../z_map_exp.c", 346);
            R_COMPASS_OFFSET_X = gMapData->roomCompassOffsetX[mapIndex][room];
            R_COMPASS_OFFSET_Y = gMapData->roomCompassOffsetY[mapIndex][room];
            Map_SetFloorPalettesData(globalCtx, VREG(30));
            // Translates to: "MAP Individual Floor ON Check"
            osSyncPrintf("ＭＡＰ 各階ＯＮチェック\n");
            break;
    }
}

void Map_InitRoomData(GlobalContext* globalCtx, s16 room) {
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;

    osSyncPrintf("＊＊＊＊＊＊＊\n＊＊＊＊＊＊＊\nroom_no=%d (%d)(%d)\n＊＊＊＊＊＊＊\n＊＊＊＊＊＊＊\n", room,
                 mapIndex, globalCtx->sceneNum);

    if (room >= 0) {
        switch (globalCtx->sceneNum) {
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
            case SCENE_YDAN_BOSS:
            case SCENE_DDAN_BOSS:
            case SCENE_BDAN_BOSS:
            case SCENE_MORIBOSSROOM:
            case SCENE_FIRE_BS:
            case SCENE_MIZUSIN_BS:
            case SCENE_JYASINBOSS:
            case SCENE_HAKADAN_BS:
                gSaveContext.sceneFlags[mapIndex].rooms |= gBitFlags[room];
                osSyncPrintf("ＲＯＯＭ＿ＩＮＦ＝%d\n", gSaveContext.sceneFlags[mapIndex].rooms);
                interfaceCtx->mapRoomNum = room;
                interfaceCtx->unk_25A = mapIndex;
                Map_SetPaletteData(globalCtx, room);
                osSyncPrintf(VT_FGCOL(YELLOW));
                osSyncPrintf("部屋部屋＝%d\n", room); // "Room Room = %d"
                osSyncPrintf(VT_RST);
                Map_InitData(globalCtx, room);
                break;
        }
    } else {
        interfaceCtx->mapRoomNum = 0;
    }

    if (gSaveContext.unk_1422 != 2) {
        gSaveContext.unk_1422 = 0;
    }
}

void Map_Destroy(GlobalContext* globalCtx) {
    MapMark_ClearPointers(globalCtx);
    gMapData = NULL;
}

void Map_Init(GlobalContext* globalCtx) {
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;

    gMapData = &gMapDataTable;

    interfaceCtx->unk_258 = -1;
    interfaceCtx->unk_25A = -1;

    interfaceCtx->mapSegment = GameState_Alloc(&globalCtx->state, 0x1000, "../z_map_exp.c", 457);
    // Translates to "ＭＡＰ TEXTURE INITIALIZATION scene_data_ID=%d mapSegment=%x"
    osSyncPrintf("\n\n\nＭＡＰ テクスチャ初期化   scene_data_ID=%d\nmapSegment=%x\n\n", globalCtx->sceneNum,
                 interfaceCtx->mapSegment, globalCtx);
    ASSERT(interfaceCtx->mapSegment != NULL, "parameter->mapSegment != NULL", "../z_map_exp.c", 459);

    switch (globalCtx->sceneNum) {
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
            mapIndex = globalCtx->sceneNum - SCENE_SPOT00;
            R_MAP_INDEX = gSaveContext.mapIndex = mapIndex;
            R_COMPASS_SCALE_X = gMapData->owCompassInfo[mapIndex][0];
            R_COMPASS_SCALE_Y = gMapData->owCompassInfo[mapIndex][1];
            R_COMPASS_OFFSET_X = gMapData->owCompassInfo[mapIndex][2];
            R_COMPASS_OFFSET_Y = gMapData->owCompassInfo[mapIndex][3];
            Map_InitData(globalCtx, mapIndex);
            R_OW_MINIMAP_X = gMapData->owMinimapPosX[mapIndex];
            R_OW_MINIMAP_Y = gMapData->owMinimapPosY[mapIndex];
            break;
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
        case SCENE_GANON_SONOGO:
        case SCENE_GANONTIKA_SONOGO:
        case SCENE_TAKARAYA:
        case SCENE_YDAN_BOSS:
        case SCENE_DDAN_BOSS:
        case SCENE_BDAN_BOSS:
        case SCENE_MORIBOSSROOM:
        case SCENE_FIRE_BS:
        case SCENE_MIZUSIN_BS:
        case SCENE_JYASINBOSS:
        case SCENE_HAKADAN_BS:
            mapIndex =
                (globalCtx->sceneNum >= SCENE_YDAN_BOSS) ? globalCtx->sceneNum - SCENE_YDAN_BOSS : globalCtx->sceneNum;
            R_MAP_INDEX = gSaveContext.mapIndex = mapIndex;
            if ((globalCtx->sceneNum <= SCENE_ICE_DOUKUTO) || (globalCtx->sceneNum >= SCENE_YDAN_BOSS)) {
                R_COMPASS_SCALE_X = gMapData->dgnCompassInfo[mapIndex][0];
                R_COMPASS_SCALE_Y = gMapData->dgnCompassInfo[mapIndex][1];
                R_COMPASS_OFFSET_X = gMapData->dgnCompassInfo[mapIndex][2];
                R_COMPASS_OFFSET_Y = gMapData->dgnCompassInfo[mapIndex][3];
                R_MAP_TEX_INDEX = R_MAP_TEX_INDEX_BASE = gMapData->dgnMinimapTexIndexBase[mapIndex];
                Map_InitRoomData(globalCtx, globalCtx->roomCtx.curRoom.num);
                MapMark_Init(globalCtx);
            }
            break;
    }
}

void Minimap_DrawCompassIcons(GlobalContext* globalCtx) {
    s32 pad;
    Player* player = PLAYER;
    s16 tempX, tempZ;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_map_exp.c", 565);

    if (globalCtx->interfaceCtx.minimapAlpha >= 0xAA) {
        func_80094A14(globalCtx->state.gfxCtx);

        gSPMatrix(OVERLAY_DISP++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                          PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);
        gDPSetCombineMode(OVERLAY_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);

        tempX = player->actor.world.pos.x;
        tempZ = player->actor.world.pos.z;
        tempX /= R_COMPASS_SCALE_X;
        tempZ /= R_COMPASS_SCALE_Y;
        Matrix_Translate((R_COMPASS_OFFSET_X + tempX) / 10.0f, (R_COMPASS_OFFSET_Y - tempZ) / 10.0f, 0.0f, MTXMODE_NEW);
        Matrix_Scale(0.4f, 0.4f, 0.4f, MTXMODE_APPLY);
        Matrix_RotateX(-1.6f, MTXMODE_APPLY);
        tempX = (0x7FFF - player->actor.shape.rot.y) / 0x400;
        Matrix_RotateY(tempX / 10.0f, MTXMODE_APPLY);
        gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_map_exp.c", 585),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 255, 0, 255);
        gSPDisplayList(OVERLAY_DISP++, gCompassArrowDL);

        tempX = sPlayerInitialPosX;
        tempZ = sPlayerInitialPosZ;
        tempX /= R_COMPASS_SCALE_X;
        tempZ /= R_COMPASS_SCALE_Y;
        Matrix_Translate((R_COMPASS_OFFSET_X + tempX) / 10.0f, (R_COMPASS_OFFSET_Y - tempZ) / 10.0f, 0.0f, MTXMODE_NEW);
        Matrix_Scale(VREG(9) / 100.0f, VREG(9) / 100.0f, VREG(9) / 100.0f, MTXMODE_APPLY);
        Matrix_RotateX(VREG(52) / 10.0f, MTXMODE_APPLY);
        Matrix_RotateY(sPlayerInitialDirection / 10.0f, MTXMODE_APPLY);
        gSPMatrix(OVERLAY_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_map_exp.c", 603),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0xFF, 200, 0, 0, 255);
        gSPDisplayList(OVERLAY_DISP++, gCompassArrowDL);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_map_exp.c", 607);
}

void Minimap_Draw(GlobalContext* globalCtx) {}

s16 Map_GetFloorTextIndexOffset(s32 mapIndex, s32 floor) {
    return gMapData->floorTexIndexOffset[mapIndex][floor];
}

void Map_Update(GlobalContext* globalCtx) {
    static s16 sLastRoomNum = 99;
    Player* player = PLAYER;
    s32 mapIndex = gSaveContext.mapIndex;
    InterfaceContext* interfaceCtx = &globalCtx->interfaceCtx;
    s16 floor;
    s16 i;

    if ((globalCtx->pauseCtx.state == 0) && (globalCtx->pauseCtx.debugState == 0)) {
        switch (globalCtx->sceneNum) {
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
                interfaceCtx->mapPalette[30] = 0;
                if (CHECK_DUNGEON_ITEM(DUNGEON_MAP, mapIndex)) {
                    interfaceCtx->mapPalette[31] = 1;
                } else {
                    interfaceCtx->mapPalette[31] = 0;
                }

                for (floor = 0; floor < 8; floor++) {
                    if (player->actor.world.pos.y > gMapData->floorCoordY[mapIndex][floor]) {
                        break;
                    }
                }

                gSaveContext.sceneFlags[mapIndex].floors |= gBitFlags[floor];
                VREG(30) = floor;
                if (R_MAP_TEX_INDEX != (R_MAP_TEX_INDEX_BASE + Map_GetFloorTextIndexOffset(mapIndex, floor))) {
                    R_MAP_TEX_INDEX = R_MAP_TEX_INDEX_BASE + Map_GetFloorTextIndexOffset(mapIndex, floor);
                }
                if (1) {} // Appears to be necessary to match

                if (interfaceCtx->mapRoomNum != sLastRoomNum) {
                    // Translates to "Current floor = %d Current room = %x Number of rooms = %d"
                    osSyncPrintf("現在階＝%d  現在部屋＝%x  部屋数＝%d\n", floor, interfaceCtx->mapRoomNum,
                                 gMapData->switchEntryCount[mapIndex]);
                    sLastRoomNum = interfaceCtx->mapRoomNum;
                }

                for (i = 0; i < gMapData->switchEntryCount[mapIndex]; i++) {
                    if ((interfaceCtx->mapRoomNum == gMapData->switchFromRoom[mapIndex][i]) &&
                        (floor == gMapData->switchFromFloor[mapIndex][i])) {
                        interfaceCtx->mapRoomNum = gMapData->switchToRoom[mapIndex][i];
                        osSyncPrintf(VT_FGCOL(YELLOW));
                        osSyncPrintf("階層切替＝%x\n", interfaceCtx->mapRoomNum); // "Layer switching = %x"
                        osSyncPrintf(VT_RST);
                        Map_InitData(globalCtx, interfaceCtx->mapRoomNum);
                        gSaveContext.unk_1422 = 0;
                        Map_SavePlayerInitialInfo(globalCtx);
                    }
                }

                VREG(10) = interfaceCtx->mapRoomNum;
                break;
            case SCENE_YDAN_BOSS:
            case SCENE_DDAN_BOSS:
            case SCENE_BDAN_BOSS:
            case SCENE_MORIBOSSROOM:
            case SCENE_FIRE_BS:
            case SCENE_MIZUSIN_BS:
            case SCENE_JYASINBOSS:
            case SCENE_HAKADAN_BS:
                VREG(30) = gMapData->bossFloor[globalCtx->sceneNum - SCENE_YDAN_BOSS];
                R_MAP_TEX_INDEX = R_MAP_TEX_INDEX_BASE +
                                  gMapData->floorTexIndexOffset[globalCtx->sceneNum - SCENE_YDAN_BOSS][VREG(30)];
                break;
        }
    }
}
