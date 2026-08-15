#include "ultra64.h"
#include "versions.h"

#pragma increment_block_number "gc-eu:0 gc-eu-mq:0 gc-jp:0 gc-jp-ce:0 gc-jp-mq:0 gc-us:0 gc-us-mq:0 ique-cn:0" \
                               "ntsc-1.0:0 ntsc-1.1:0 ntsc-1.2:0 pal-1.0:0 pal-1.1:0"

// Declared before including other headers for BSS ordering
extern uintptr_t gSegments[NUM_SEGMENTS];

#pragma increment_block_number "gc-eu:252 gc-eu-mq:252 gc-jp:252 gc-jp-ce:252 gc-jp-mq:252 gc-us:252 gc-us-mq:252" \
                               "ique-cn:252 ntsc-1.0:192 ntsc-1.1:192 ntsc-1.2:192 pal-1.0:192 pal-1.1:192"

extern struct PreNmiBuff* gAppNmiBufferPtr;
extern struct Scheduler gScheduler;
extern struct PadMgr gPadMgr;
extern struct IrqMgr gIrqMgr;

#include <stdlib.h>
#include "libc64/malloc.h"
#include "libu64/runtime.h"
#include "array_count.h"
#include "audiomgr.h"
#include "debug_arena.h"
#include "gfx.h"
#include "idle.h"
#include "padmgr.h"
#include "printf.h"
#include "regs.h"
#include "segmented_address.h"
#include "terminal.h"
#include "translation.h"
#include "versions.h"
#if PLATFORM_N64
#include "cic6105.h"
#include "n64dd.h"
#endif
#include "game_debug.h"
#include "thread.h"

#pragma increment_block_number "gc-eu:128 gc-eu-mq:128 gc-jp:128 gc-jp-ce:128 gc-jp-mq:128 gc-us:128 gc-us-mq:128" \
                               "ique-cn:128 ntsc-1.0:82 ntsc-1.1:82 ntsc-1.2:82 pal-1.0:80 pal-1.1:80"

s32 gScreenWidth = SCREEN_WIDTH;
s32 gScreenHeight = SCREEN_HEIGHT;
u32 gSystemHeapSize = 0;

PadMgr gPadMgr;
uintptr_t gSegments[NUM_SEGMENTS];

AudioMgr sAudioMgr;

void Main(void* arg) {
    uintptr_t systemHeapStart;
    uintptr_t systemHeapEnd;

    PRINTF(T("mainproc 実行開始\n", "mainproc Start running\n"));
    gScreenWidth = SCREEN_WIDTH;
    gScreenHeight = SCREEN_HEIGHT;
    gSystemHeapSize = 3 * 1024 * 1024; // TODO-ootdragon this is probably much more than needed
    systemHeapStart = (uintptr_t)aligned_alloc(16, gSystemHeapSize);
    assert(systemHeapStart != 0);
    systemHeapEnd = systemHeapStart + gSystemHeapSize;
    PRINTF(T("システムヒープ初期化 %08x-%08x %08x\n", "System heap initialization %08x-%08x %08x\n"), systemHeapStart,
           systemHeapEnd, gSystemHeapSize);
    Runtime_Init((void*)systemHeapStart, gSystemHeapSize);

#if DEBUG_FEATURES
    {
        void* debugHeapStart;
        u32 debugHeapSize;

        debugHeapSize = 0x400;
        debugHeapStart = SYSTEM_ARENA_MALLOC(debugHeapSize, "../main.c", 565);

        PRINTF("debug_InitArena(%08x, %08x)\n", debugHeapStart, debugHeapSize);
        DebugArena_Init(debugHeapStart, debugHeapSize);
    }
#endif

    Regs_Init();

    R_ENABLE_ARENA_DBG = 0;

#if PLATFORM_N64
    CIC6105_AddFaultClient();
    CIC6105_RunBootTask();
#endif

    // TODO-ootdragon audio
    // AudioMgr_Init(&sAudioMgr, STACK_TOP(sAudioStack), THREAD_PRI_AUDIOMGR, THREAD_ID_AUDIOMGR, &gScheduler,
    // &gIrqMgr);

    PadMgr_Init(&gPadMgr);

    // TODO-ootdragon audio
    // AudioMgr_WaitForInit(&sAudioMgr);

    Graph_ThreadEntry(arg);

    PRINTF(T("mainproc 後始末\n", "mainproc Cleanup\n"));
#if PLATFORM_N64
    CIC6105_RemoveFaultClient();
#endif
    PRINTF(T("mainproc 実行終了\n", "mainproc End of execution\n"));
}
