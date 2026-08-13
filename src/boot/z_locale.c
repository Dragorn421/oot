#include "joypad.h"
#include "libu64/debug.h"
#include "alignment.h"
#include "line_numbers.h"
#include "padmgr.h"
#include "printf.h"
#include "region.h"
#include "terminal.h"
#include "translation.h"
#include "versions.h"
#include "z_locale.h"

s32 gCurrentRegion = 0;

void Locale_Init(void) {
    if (OOT_PAL) {
        gCurrentRegion = REGION_EU;
    } else {
        assert(false);
    }
}

void Locale_ResetRegion(void) {
    gCurrentRegion = REGION_NULL;
}

#if DEBUG_FEATURES
u32 func_80001F48(void) {
    if (gCurrentRegion == OOT_REGION) {
        return 0;
    }

    if (joypad_is_connected(JOYPAD_PORT_3)) {
        return 0;
    }

    return 1;
}

u32 func_80001F8C(void) {
    if (gCurrentRegion == OOT_REGION) {
        return 0;
    }

    if (joypad_is_connected(JOYPAD_PORT_3)) {
        return 1;
    }

    return 0;
}

// This function appears to be unused?
u32 Locale_IsRegionNative(void) {
    return gCurrentRegion == OOT_REGION;
}
#endif
