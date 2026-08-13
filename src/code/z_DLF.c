#include "elf_reader.h"
#include "libc64/malloc.h"
#include "libu64/overlay.h"
#include "printf.h"
#include "terminal.h"
#include "translation.h"
#include "ultra64.h"
#include "z_game_dlftbls.h"

void Overlay_LoadGameState(GameStateOverlay* overlayEntry) {
    if (overlayEntry->loadedRamAddr != NULL) {
        PRINTF(T("既にリンクされています\n", "Already linked\n"));
        return;
    }

    if (overlayEntry->file == NULL) {
        overlayEntry->unk_28 = 0;
    } else {
        char dll_name[100];
        int nchar;

        nchar = snprintf(dll_name, sizeof(dll_name), "gamestates/%s", overlayEntry->file);
        assert(nchar < sizeof(dll_name));

        overlayEntry->loadedRamAddr = Overlay_AllocateAndLoad(dll_name);

        if (overlayEntry->loadedRamAddr == NULL) {
            PRINTF(T("ロードに失敗しました\n", "Loading failed\n"));
            return;
        }

        PRINTF_COLOR_GREEN();
        PRINTF("OVL(d): Ram:%08x %s\n", overlayEntry->loadedRamAddr, "");
        PRINTF_RST();

        if (overlayEntry->unk_14 != NULL) {
            overlayEntry->unk_14 =
                (void*)((u32)overlayEntry->unk_14 -
                        (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
        } else {
            overlayEntry->unk_14 = NULL;
        }

        if (overlayEntry->init != NULL) {
            overlayEntry->init = (void*)((u32)overlayEntry->init - (s32)((u32)elf_section_get_dll_vram_start(dll_name) -
                                                                         (u32)overlayEntry->loadedRamAddr));
        } else {
            overlayEntry->init = NULL;
        }

        if (overlayEntry->destroy != NULL) {
            overlayEntry->destroy =
                (void*)((u32)overlayEntry->destroy -
                        (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
        } else {
            overlayEntry->destroy = NULL;
        }

        if (overlayEntry->unk_20 != NULL) {
            overlayEntry->unk_20 =
                (void*)((u32)overlayEntry->unk_20 -
                        (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
        } else {
            overlayEntry->unk_20 = NULL;
        }

        if (overlayEntry->unk_24 != NULL) {
            overlayEntry->unk_24 =
                (void*)((u32)overlayEntry->unk_24 -
                        (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
        } else {
            overlayEntry->unk_24 = NULL;
        }

        overlayEntry->unk_28 = 0;
    }
}

void Overlay_FreeGameState(GameStateOverlay* overlayEntry) {
    if (overlayEntry->loadedRamAddr != NULL) {
        s32 temp = overlayEntry->unk_28 != 0 ? -1 : 0;

        if (temp == 0) {
            char dll_name[100];
            int nchar;

            nchar = snprintf(dll_name, sizeof(dll_name), "gamestates/%s", overlayEntry->file);
            assert(nchar < sizeof(dll_name));

            if (overlayEntry->unk_14 != NULL) {
                overlayEntry->unk_14 =
                    (void*)((u32)overlayEntry->unk_14 +
                            (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
            } else {
                overlayEntry->unk_14 = NULL;
            }

            if (overlayEntry->init != NULL) {
                overlayEntry->init =
                    (void*)((u32)overlayEntry->init +
                            (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
            } else {
                overlayEntry->init = NULL;
            }

            if (overlayEntry->destroy != NULL) {
                overlayEntry->destroy =
                    (void*)((u32)overlayEntry->destroy +
                            (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
            } else {
                overlayEntry->destroy = NULL;
            }

            if (overlayEntry->unk_20 != NULL) {
                overlayEntry->unk_20 =
                    (void*)((u32)overlayEntry->unk_20 +
                            (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
            } else {
                overlayEntry->unk_20 = NULL;
            }

            if (overlayEntry->unk_24 != NULL) {
                overlayEntry->unk_24 =
                    (void*)((u32)overlayEntry->unk_24 +
                            (s32)((u32)elf_section_get_dll_vram_start(dll_name) - (u32)overlayEntry->loadedRamAddr));
            } else {
                overlayEntry->unk_24 = NULL;
            }

            SYSTEM_ARENA_FREE(overlayEntry->loadedRamAddr, "../z_DLF.c", 149);
            overlayEntry->loadedRamAddr = NULL;
        }
    }
}
