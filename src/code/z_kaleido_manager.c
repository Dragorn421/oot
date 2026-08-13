#include "elf_reader.h"
#include "libu64/debug.h"
#include "libu64/overlay.h"
#include "array_count.h"
#include "kaleido_manager.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "play_state.h"

#define KALEIDO_OVERLAY(dll_name, nameString) \
    {                                         \
        NULL,                                 \
        dll_name,                             \
        0,                                    \
        nameString,                           \
    }

KaleidoMgrOverlay gKaleidoMgrOverlayTable[] = {
    KALEIDO_OVERLAY("misc/ovl_kaleido_scope", "kaleido_scope"),
    KALEIDO_OVERLAY("actors/ovl_player_actor", "player_actor"),
};

void* sKaleidoAreaPtr = NULL;
KaleidoMgrOverlay* gKaleidoMgrCurOvl = NULL;
u8 gBossMarkState = 0;

void KaleidoManager_LoadOvl(KaleidoMgrOverlay* ovl) {
    LOG_UTILS_CHECK_NULL_POINTER("KaleidoArea_allocp", sKaleidoAreaPtr, "../z_kaleido_manager.c", 99);

    ovl->loadedRamAddr = sKaleidoAreaPtr;
    elf_section_load_dll(ovl->dll_name, ovl->loadedRamAddr);

    PRINTF_COLOR_GREEN();
    PRINTF("OVL(k): Ram:%08x %s\n", ovl->loadedRamAddr, ovl->name);
    PRINTF_RST();

    ovl->offset = (uintptr_t)ovl->loadedRamAddr - (uintptr_t)elf_section_get_dll_vram_start(ovl->dll_name);
    gKaleidoMgrCurOvl = ovl;
}

void KaleidoManager_ClearOvl(KaleidoMgrOverlay* ovl) {
    if (ovl->loadedRamAddr != NULL) {
        ovl->offset = 0;
        bzero(ovl->loadedRamAddr, elf_section_get_dll_ramsize(ovl->dll_name));
        ovl->loadedRamAddr = NULL;
        gKaleidoMgrCurOvl = NULL;
    }
}

void KaleidoManager_Init(PlayState* play) {
    s32 largestSize = 0;
    s32 size;
    u32 i;

    for (i = 0; i < ARRAY_COUNT(gKaleidoMgrOverlayTable); i++) {
        size = elf_section_get_dll_ramsize(gKaleidoMgrOverlayTable[i].dll_name);
        if (size > largestSize) {
            largestSize = size;
        }
    }

    PRINTF_COLOR_GREEN();
    PRINTF(T("KaleidoArea の最大サイズは %d バイトを確保します\n", "The maximum size of KaleidoArea is %d bytes\n"),
           largestSize);
    PRINTF_RST();

    sKaleidoAreaPtr = GAME_STATE_ALLOC(&play->state, largestSize, "../z_kaleido_manager.c", 150);
    LOG_UTILS_CHECK_NULL_POINTER("KaleidoArea_allocp", sKaleidoAreaPtr, "../z_kaleido_manager.c", 151);

    PRINTF_COLOR_GREEN();
    PRINTF("KaleidoArea %08x - %08x\n", sKaleidoAreaPtr, (uintptr_t)sKaleidoAreaPtr + largestSize);
    PRINTF_RST();

    gKaleidoMgrCurOvl = NULL;
}

void KaleidoManager_Destroy(void) {
    if (gKaleidoMgrCurOvl != NULL) {
        KaleidoManager_ClearOvl(gKaleidoMgrCurOvl);
        gKaleidoMgrCurOvl = NULL;
    }

    sKaleidoAreaPtr = NULL;
}

// NOTE: this function looks messed up and probably doesn't work how it was intended to
void* KaleidoManager_GetRamAddr(void* vram) {
    KaleidoMgrOverlay* iter = gKaleidoMgrCurOvl;
    KaleidoMgrOverlay* ovl = iter;

    if (ovl == NULL) {
        u32 i;

        iter = &gKaleidoMgrOverlayTable[0];
        for (i = 0; i < ARRAY_COUNT(gKaleidoMgrOverlayTable); i++) {
            if (((uintptr_t)vram >= (uintptr_t)elf_section_get_dll_vram_start(iter->dll_name)) &&
                ((uintptr_t)(elf_section_get_dll_vram_start(iter->dll_name) +
                             elf_section_get_dll_ramsize(iter->dll_name)) >= (uintptr_t)vram)) {
                KaleidoManager_LoadOvl(iter);
                ovl = iter;
                goto KaleidoManager_GetRamAddr_end;
            }
            //! @bug Probably missing iter++ here
        }

        PRINTF(T("異常\n", "Abnormal\n"));
        return NULL;
    }

KaleidoManager_GetRamAddr_end:
    if ((ovl == NULL) || ((uintptr_t)vram < (uintptr_t)elf_section_get_dll_vram_start(ovl->dll_name)) ||
        ((uintptr_t)vram >=
         (uintptr_t)(elf_section_get_dll_vram_start(ovl->dll_name) + elf_section_get_dll_ramsize(ovl->dll_name)))) {
        return NULL;
    }

    return (void*)((uintptr_t)vram + ovl->offset);
}
