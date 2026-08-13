#include "effect.h"

// Profile declarations (also used in the table below)
#define DEFINE_EFFECT_SS(name, _1) extern EffectSsProfile name##_Profile;
#define DEFINE_EFFECT_SS_UNSET(_0)

#include "tables/effect_ss_table.h"

#undef DEFINE_EFFECT_SS
#undef DEFINE_EFFECT_SS_UNSET

// Effect SS Overlay Table definition
#define DEFINE_EFFECT_SS(name, _1) \
    {                              \
        "ovl_" #name,              \
        NULL,                      \
        &name##_Profile,           \
        1,                         \
    },

#define DEFINE_EFFECT_SS_UNSET(_0) \
    {                              \
        NULL,                      \
        NULL,                      \
        NULL,                      \
        0,                         \
    },

EffectSsOverlay gEffectSsOverlayTable[] = {
#include "tables/effect_ss_table.h"
};

#undef DEFINE_EFFECT_SS
#undef DEFINE_EFFECT_SS_UNSET
