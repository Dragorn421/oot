#include <libdragon.h>
#include <stdint.h>
#include "actor_profile.h"
#include "dragonfs.h"
#include "elf_reader.h"
#include "libdragon/include/backtrace.h"
#include "printf.h"
#include "z_actor_dlftbls.h"

// Profile declarations (also used in the table below)
#define DEFINE_ACTOR(name, _1, _2, _3) extern ActorProfile name##_Profile;
#define DEFINE_ACTOR_INTERNAL(name, _1, _2, _3) extern ActorProfile name##_Profile;
#define DEFINE_ACTOR_UNSET(_0)

#include "tables/actor_table.h"

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

// Actor Overlay Table definition
#if DEBUG_FEATURES

#define DEFINE_ACTOR(name, _1, allocType, nameString)                  \
    {                                                                  \
        "ovl_" #name, NULL, &name##_Profile, nameString, allocType, 0, \
    },

#define DEFINE_ACTOR_INTERNAL(name, _1, allocType, nameString) \
    {                                                          \
        NULL, NULL, &name##_Profile, nameString, allocType, 0, \
    },

#else

// Actor name is set to NULL in retail builds
#define DEFINE_ACTOR(name, _1, allocType, _3)                    \
    {                                                            \
        "ovl_" #name, NULL, &name##_Profile, NULL, allocType, 0, \
    },

#define DEFINE_ACTOR_INTERNAL(name, _1, allocType, _3)   \
    {                                                    \
        NULL, NULL, &name##_Profile, NULL, allocType, 0, \
    },

#endif

#define DEFINE_ACTOR_UNSET(_0) { 0 },

ActorOverlay gActorOverlayTable[] = {
#include "tables/actor_table.h"
};

#undef DEFINE_ACTOR
#undef DEFINE_ACTOR_INTERNAL
#undef DEFINE_ACTOR_UNSET

s32 gMaxActorId = 0;

void ActorOverlayTable_LogPrint(void) {
#if DEBUG_FEATURES
    ActorOverlay* overlayEntry;
    u32 i;

    PRINTF("actor_dlftbls %u\n", gMaxActorId);
    PRINTF("allocp   profile  segname\n");

    for (i = 0, overlayEntry = &gActorOverlayTable[0]; i < (u32)gMaxActorId; i++, overlayEntry++) {
        PRINTF("%08x %08x %s\n", overlayEntry->loadedRamAddr, &overlayEntry->profile->id,
               overlayEntry->name != NULL ? overlayEntry->name : "?");
    }
#endif
}

bool actor_dll_syms_callback(void* ram, struct custom_module* mod) {
    ActorOverlay* entry;
    char dll_name[100];
    int nchars;
    size_t size;
    uintptr_t ramint = (uintptr_t)ram;
    uintptr_t dll_start_int;

    for (int i = 0; i < ACTOR_ID_MAX; i++) {
        entry = &gActorOverlayTable[i];
        if (entry->loadedRamAddr == NULL) {
            continue;
        }
        nchars = snprintf(dll_name, sizeof(dll_name), "actors/%s", entry->ovl_name);
        // note: we should be more resilient here as this runs in the crash inspector context.
        // but the dll already having loaded successfully should mean this is safe
        assert(nchars < sizeof(dll_name));
        size = elf_section_get_dll_ramsize(dll_name);
        dll_start_int = (uintptr_t)entry->loadedRamAddr;
        if (ramint >= dll_start_int && ramint < (dll_start_int + size)) {
            char path[200];
            snprintf(path, sizeof(path), "src/overlays/actors/%s/dll.sym", entry->ovl_name);
            mod->symt_rom = dfs_rom_addr(path);
            mod->addrtable_base = dll_start_int;
            if (mod->symt_rom == 0) {
                debugf("bad path %s\n", path);
            }
            return mod->symt_rom != 0;
        }
    }

    return false;
}

void ActorOverlayTable_Init(void) {
    gMaxActorId = ACTOR_ID_MAX;
    register_custom_module_callback(actor_dll_syms_callback);
}

void ActorOverlayTable_Cleanup(void) {
    unregister_custom_module_callback(actor_dll_syms_callback);
    gMaxActorId = 0;
}
