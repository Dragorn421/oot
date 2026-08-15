#include <dlfcn.h>
#include "elf_reader.h"
#include "libu64/debug.h"
#include "array_count.h"
#include "kaleido_manager.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "play_state.h"
#include <stdint.h>

#define KALEIDO_OVERLAY(dso_path) \
    { dso_path, NULL }

KaleidoMgrOverlay gKaleidoMgrOverlayTable[] = {
    KALEIDO_OVERLAY("rom:/misc/ovl_kaleido_scope.dso"),
    KALEIDO_OVERLAY("rom:/actors/ovl_player_actor.dso"),
};

KaleidoMgrOverlay* gKaleidoMgrCurOvl = NULL;
u8 gBossMarkState = 0;

void KaleidoManager_LoadOvl(KaleidoMgrOverlay* ovl) {
    assert(ovl->dso_handle == NULL);
    ovl->dso_handle = dlopen(ovl->dso_path, 0);
    assert(ovl->dso_handle != NULL);

    PRINTF_COLOR_GREEN();
    PRINTF("OVL(k): %s\n", ovl->dso_path);
    PRINTF_RST();

    gKaleidoMgrCurOvl = ovl;
}

void KaleidoManager_ClearOvl(KaleidoMgrOverlay* ovl) {
    if (ovl->dso_handle != NULL) {
        dlclose(ovl->dso_handle);
        ovl->dso_handle = NULL;
        gKaleidoMgrCurOvl = NULL;
    }
}

void KaleidoManager_Init(PlayState* play) {
    gKaleidoMgrCurOvl = NULL;
}

void KaleidoManager_Destroy(void) {
    if (gKaleidoMgrCurOvl != NULL) {
        KaleidoManager_ClearOvl(gKaleidoMgrCurOvl);
        gKaleidoMgrCurOvl = NULL;
    }
}

void* KaleidoManager_GetRamAddr(const char* sym) {
    void* ram = dlsym(gKaleidoMgrCurOvl->dso_handle, sym);
    assertf(ram != NULL, "%s not found", sym);
    return ram;
}
