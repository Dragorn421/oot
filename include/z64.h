#ifndef Z64_H
#define Z64_H

#include "ultra64.h"
#include "ultra64/gs2dex.h"
#include "attributes.h"
#include "audiomgr.h"
#include "controller.h"
#include "versions.h"
#include "z64player.h"
#include "z64audio.h"
#include "z64ocarina.h"
#include "z64effect.h"
#include "z64animation.h"
#include "z64animation_legacy.h"
#include "letterbox.h"
#include "z64math.h"
#include "z64map_mark.h"
#include "one_point_cutscene.h"
#include "z64play.h"
#include "z64skin.h"
#include "z64skin_matrix.h"
#include "zelda_arena.h"
#include "alignment.h"
#include "audiothread_cmd.h"
#include "sfx.h"
#include "color.h"
#include "libu64/gfxprint.h"
#include "z_lib.h"
#include "regs.h"
#include "irqmgr.h"
#include "padmgr.h"
#include "sched.h"
#include "rumble.h"
#include "map.h"
#include "mempak.h"
#include "tha.h"
#include "thga.h"
#include "speed_meter.h"
#include "gfx.h"
#include "gfx_setupdl.h"
#include "gfxalloc.h"
#include "prerender.h"
#include "rand.h"
#include "sys_math.h"
#include "sys_math3d.h"
#include "libc64/math64.h"
#include "sys_matrix.h"
#include "main.h"
#include "segmented_address.h"
#include "stackcheck.h"
#include "kaleido_manager.h"
#include "libc64/aprintf.h"
#include "libc64/malloc.h"
#include "libc64/sleep.h"
#include "libc64/sprintf.h"
#include "libu64/debug.h"

// TODO: include all files listed above into the right place, and then delete this file.

// Vis...
typedef struct struct_801664F0 {
    /* 0x00 */ u32 type;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 color;
    /* 0x0C */ Color_RGBA8_u32 envColor;
} struct_801664F0; // size = 0x10

typedef struct VisMono {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 primColor;
    /* 0x0C */ Color_RGBA8_u32 envColor;
    /* 0x10 */ u16* tlut;
    /* 0x14 */ Gfx* dList;
} VisMono; // size = 0x18

// Vis...
typedef struct struct_80166500 {
    /* 0x00 */ u32 useRgba;
    /* 0x04 */ u32 setScissor;
    /* 0x08 */ Color_RGBA8_u32 primColor;
    /* 0x08 */ Color_RGBA8_u32 envColor;
} struct_80166500; // size = 0x10

#endif
