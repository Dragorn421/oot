#include "idle.h"
#if PLATFORM_N64
#include "cic6105.h"
#endif
#include "z_locale.h"
#include "thread.h"

#pragma increment_block_number "gc-eu:0 gc-eu-mq:0 gc-jp:0 gc-jp-ce:0 gc-jp-mq:0 gc-us:0 gc-us-mq:0 ntsc-1.2:128"

int main(void) {
#if PLATFORM_N64
    CIC6105_SaveBootMagicValues();
#endif

    Locale_Init();

    Idle_ThreadEntry(NULL);

    return 0;
}
