/**
 * @file cic6105.c
 *
 * This file implements routines relating to the CIC X105 anti-piracy measures present in N64 releases.
 *
 * The "authentication" chain begins in IPL3, which deposits specific expected values into RAM and runs an RSP task in
 * parallel with loading the boot segment into RAM. This RSP task leaves data in the RSP's registers which rspboot and
 * the CIC6105 RSP task later read according to routines in this file. Their security model relied on CICs being
 * uncloneable with scarcely many donor CIC options from other games available at the time, in which case IPL3 would be
 * unmodifiable. The rest of the chain is designed with a "security through obscurity" mindset, storing later antipiracy
 * checks intermixed with other game code or RSP code that was expected to be non-trivial to analyze in a timely
 * fashion. Notably this effort did little to curb emulation, almost wholly due to early emulators being insufficiently
 * accurate to run RSP code or even IPL3 at a low level, sidestepping much of the early setup in favor of providing a
 * known-good post-boot state to begin emulation from.
 */
#include "libultra_ucode.h"
#include <stdint.h>
#pragma increment_block_number "ntsc-1.0:132 ntsc-1.1:132 ntsc-1.2:132 pal-1.0:132 pal-1.1:132"
#include "audiomgr.h"
#include "build.h"
#include "cic6105.h"
#include "regs.h"

s32 CIC6105_Stub(void);

OSTask sCIC6105Task = {
    // clang-format off
    4,
    0,
    rspbootTextStart, 0x3E8,
    cic6105TextStart, 0x20,
    (u64*)gBuildCreator, 8,
    NULL, 0,
    NULL, NULL,
    NULL, 0,
    NULL, 0,
    // clang-format on
};

u32 gCICBootMagic0;
u32 gCICBootMagic1;
u32 sCICTaskResult0;
u32 sCICTaskResult1;

void CIC6105_DisableAudio(void) {
    R_AUDIOMGR_ACTIVITY_LEVEL = AUDIOMGR_ACTIVITY_LEVEL_NO_RSP;
}

void CIC6105_EnableAudio(void) {
    R_AUDIOMGR_ACTIVITY_LEVEL = AUDIOMGR_ACTIVITY_LEVEL_ALL;
}

void CIC6105_AddFaultClient(void) {
}

void CIC6105_RemoveFaultClient(void) {
}

void CIC6105_RunBootTask(void) {
    struct libultra_ucode_task_handle task_handle;

    libultra_ucode_run(&sCIC6105Task, false, &task_handle);
    libultra_ucode_wait(&task_handle);

    // Retrieve results from RSP DMEM, it is assumed no other RSP task is running
    sCICTaskResult0 = *(vu32*)((uintptr_t)SP_DMEM + 0xFF4);
    sCICTaskResult1 = *(vu32*)((uintptr_t)SP_DMEM + 0xFFC);
    CIC6105_Stub();
}

s32 CIC6105_Stub(void) {
    return 0;
}

void CIC6105_SaveBootMagicValues(void) {
    // IPL3 writes two magic values into RDRAM during the boot process into fixed locations.
    // These must be retrieved early before memory is claimed by game memory management systems.
    gCICBootMagic0 = *(vu32*)0x002FB1F4;
    gCICBootMagic1 = *(vu32*)0x002FE1C0;
}
