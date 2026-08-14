#include "debug.h"
#include "display.h"
#include "dma_queue.h"
#include "elf_reader.h"
#include "gfx.h"
#include "idle.h"
#include "libultra_ucode.h"
#if PLATFORM_N64
#include "cic6105.h"
#endif
#include "z_locale.h"
#include "thread.h"

#pragma increment_block_number "gc-eu:0 gc-eu-mq:0 gc-jp:0 gc-jp-ce:0 gc-jp-mq:0 gc-us:0 gc-us-mq:0 ntsc-1.2:128"

int main(void) {
    debug_init_emulog();
    display_init((resolution_t){ SCREEN_WIDTH, SCREEN_HEIGHT, false }, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    dma_queue_init();
    read_elf_metadata();
    libultra_ucode_init();

#if PLATFORM_N64
    CIC6105_SaveBootMagicValues();
#endif

    Locale_Init();

    Idle_ThreadEntry(NULL);

    return 0;
}
