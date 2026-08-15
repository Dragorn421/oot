#include <dlfcn.h>
#include "elf_reader.h"
#include "libc64/malloc.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "ultra64.h"
#include "z_game_dlftbls.h"

void Overlay_LoadGameState(GameStateOverlay* overlayEntry) {
    if (overlayEntry->dso_handle != NULL) {
        PRINTF(T("既にリンクされています\n", "Already linked\n"));
        return;
    }

    if (overlayEntry->dso_path == NULL) {
        // Nothing to do
    } else {
        overlayEntry->dso_handle = dlopen(overlayEntry->dso_path, 0);
        assert(overlayEntry->dso_handle != NULL);
        overlayEntry->init = dlsym(overlayEntry->dso_handle, overlayEntry->init_sym_name);
        assert(overlayEntry->init != NULL);
    }
}

void Overlay_FreeGameState(GameStateOverlay* overlayEntry) {
    if (overlayEntry->dso_handle != NULL) {
        overlayEntry->init = NULL;
        dlclose(overlayEntry->dso_handle);
        overlayEntry->dso_handle = NULL;
    }
}
