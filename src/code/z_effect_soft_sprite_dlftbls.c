#include "effect.h"

// Effect SS Overlay Table definition
#define DEFINE_EFFECT_SS(name, _1)        \
    {                                     \
        "rom:/effects/ovl_" #name ".dso", \
        NULL,                             \
        #name "_Profile",                 \
        NULL,                             \
    },

#define DEFINE_EFFECT_SS_UNSET(_0) \
    {                              \
        NULL,                      \
        NULL,                      \
        NULL,                      \
        NULL,                      \
    },

EffectSsOverlay gEffectSsOverlayTable[] = {
#include "tables/effect_ss_table.h"
};

#undef DEFINE_EFFECT_SS
#undef DEFINE_EFFECT_SS_UNSET
