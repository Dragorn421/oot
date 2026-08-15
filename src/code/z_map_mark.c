#include "dlfcn.h"
#include "elf_reader.h"
#include "libu64/debug.h"
#include "libu64/overlay.h"
#include "map.h"
#include "printf.h"
#if PLATFORM_N64
#include "n64dd.h"
#endif
#include "regs.h"
#include "terminal.h"
#include "translation.h"
#include "map_mark.h"
#include "play_state.h"
#include "save.h"

#include "assets/textures/parameter_static/parameter_static.h"

typedef struct MapMarkInfo {
    /* 0x00 */ void* texture;
    /* 0x04 */ u32 imageFormat;
    /* 0x08 */ u32 imageSize;
    /* 0x0C */ u32 textureWidth;
    /* 0x10 */ u32 textureHeight;
    /* 0x14 */ u32 rectWidth;
    /* 0x18 */ u32 rectHeight;
    /* 0x1C */ u32 dsdx;
    /* 0x20 */ u32 dtdy;
} MapMarkInfo; // size = 0x24

typedef struct MapMarkDataOverlay {
    void* dso_handle;
    const char* dso_path;
    const char* table_sym_name;
} MapMarkDataOverlay; // size = 0x18

#define GDP_LOADTEXTUREBLOCK_RUNTIME_QUALIFIERS
#include "src/code/gDPLoadTextureBlock_Runtime.inc.c"

MapMarkInfo sMapMarkInfoTable[] = {
    { gMapChestIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 32, 32, 1 << 10, 1 << 10 }, // Chest Icon
    { gMapBossIconTex, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 8, 32, 32, 1 << 10, 1 << 10 },     // Boss Skull Icon
};

static MapMarkDataOverlay sMapMarkDataOvl = {
    NULL,
    "rom:/misc/ovl_map_mark_data.dso",
    "gMapMarkDataTable",
};

static MapMarkData** sLoadedMarkDataTable;

void MapMark_Init(PlayState* play) {
    MapMarkDataOverlay* overlay = &sMapMarkDataOvl;

    assert(overlay->dso_handle == NULL);
    overlay->dso_handle = dlopen(overlay->dso_path, 0);
    assert(overlay->dso_handle != NULL);

    sLoadedMarkDataTable = dlsym(overlay->dso_handle, overlay->table_sym_name);
    assert(sLoadedMarkDataTable != NULL);
}

void MapMark_ClearPointers(PlayState* play) {
    sLoadedMarkDataTable = NULL;
    if (sMapMarkDataOvl.dso_handle != NULL) {
        dlclose(sMapMarkDataOvl.dso_handle);
        sMapMarkDataOvl.dso_handle = NULL;
    }
}

void MapMark_DrawForDungeon(PlayState* play) {
    InterfaceContext* interfaceCtx;
    MapMarkIconData* mapMarkIconData;
    MapMarkPoint* markPoint;
    MapMarkInfo* markInfo;
    u16 dungeon = gSaveContext.mapIndex;
    s32 i;
    s32 rectLeft;
    s32 rectTop;

    interfaceCtx = &play->interfaceCtx;

    if ((gMapData != NULL) && (play->interfaceCtx.mapRoomNum >= gMapData->dgnMinimapCount[dungeon])) {
        PRINTF(VT_COL(RED, WHITE) T("部屋番号がオーバーしてるで,ヤバイで %d/%d  \nMapMarkDraw の処理を中断します\n",
                                    "Room number exceeded, yikes %d/%d  \nMapMarkDraw processing interrupted\n"),
               VT_RST, play->interfaceCtx.mapRoomNum, gMapData->dgnMinimapCount[dungeon]);
        return;
    }

    mapMarkIconData = &sLoadedMarkDataTable[dungeon][interfaceCtx->mapRoomNum][0];

    OPEN_DISPS(play->state.gfxCtx, "../z_map_mark.c", 303);

    while (true) {
        if (mapMarkIconData->markType == MAP_MARK_NONE) {
            break;
        }

        gDPPipeSync(OVERLAY_DISP++);
        gDPSetTextureLUT(OVERLAY_DISP++, G_TT_NONE);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, interfaceCtx->minimapAlpha);
        gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, interfaceCtx->minimapAlpha);

        markPoint = &mapMarkIconData->points[0];
        for (i = 0; i < mapMarkIconData->count; i++) {
            if ((mapMarkIconData->markType != MAP_MARK_CHEST) || !Flags_GetTreasure(play, markPoint->chestFlag)) {
                markInfo = &sMapMarkInfoTable[mapMarkIconData->markType];

                gDPPipeSync(OVERLAY_DISP++);
                gDPLoadTextureBlock_Runtime(OVERLAY_DISP++, markInfo->texture, markInfo->imageFormat,
                                            markInfo->imageSize, markInfo->textureWidth, markInfo->textureHeight, 0,
                                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK,
                                            G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

                rectLeft = ((DEBUG_FEATURES ? GREG(94) : 0) + markPoint->x + 204) << 2;
                rectTop = ((DEBUG_FEATURES ? GREG(95) : 0) + markPoint->y + 140) << 2;
                gSPTextureRectangle(OVERLAY_DISP++, rectLeft, rectTop, markInfo->rectWidth + rectLeft,
                                    rectTop + markInfo->rectHeight, G_TX_RENDERTILE, 0, 0, markInfo->dsdx,
                                    markInfo->dtdy);
            }
            markPoint++;
        }
        mapMarkIconData++;
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_map_mark.c", 339);
}

void MapMark_Draw(PlayState* play) {
    switch (play->sceneId) {
        case SCENE_DEKU_TREE:
        case SCENE_DODONGOS_CAVERN:
        case SCENE_JABU_JABU:
        case SCENE_FOREST_TEMPLE:
        case SCENE_FIRE_TEMPLE:
        case SCENE_WATER_TEMPLE:
        case SCENE_SPIRIT_TEMPLE:
        case SCENE_SHADOW_TEMPLE:
        case SCENE_BOTTOM_OF_THE_WELL:
        case SCENE_ICE_CAVERN:
        case SCENE_DEKU_TREE_BOSS:
        case SCENE_DODONGOS_CAVERN_BOSS:
        case SCENE_JABU_JABU_BOSS:
        case SCENE_FOREST_TEMPLE_BOSS:
        case SCENE_FIRE_TEMPLE_BOSS:
            MapMark_DrawForDungeon(play);
            break;
    }
}
