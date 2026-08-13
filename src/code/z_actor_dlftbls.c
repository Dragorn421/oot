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

void ActorOverlayTable_Init(void) {
    gMaxActorId = ACTOR_ID_MAX;
}

void ActorOverlayTable_Cleanup(void) {
    gMaxActorId = 0;
}
