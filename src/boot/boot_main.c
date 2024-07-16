#include "global.h"
#include "boot.h"

StackEntry sBootThreadInfo;
OSThread sIdleThread;
STACK(sIdleThreadStack, 0x400);
StackEntry sIdleThreadInfo;
STACK(sBootThreadStack, BOOT_STACK_SIZE);

void cleararena(void) {
    // ootdragon-TODO Not sure what this is for in the game. Not sure if libdragon zeros the whole ram on boot already.
    // bzero(_dmadataSegmentStart, osMemSize - OS_K0_TO_PHYSICAL(_dmadataSegmentStart));
}

int main() {
    StackCheck_Init(&sBootThreadInfo, sBootThreadStack, STACK_TOP(sBootThreadStack), 0, -1, "boot");

    osMemSize = osGetMemSize();
    cleararena();
    __osInitialize_common();
    __osInitialize_autodetect();

    gCartHandle = osCartRomInit();
    osDriveRomInit();
#if OOT_DEBUG
    isPrintfInit();
#endif
    Locale_Init();

    StackCheck_Init(&sIdleThreadInfo, sIdleThreadStack, STACK_TOP(sIdleThreadStack), 0, 256, "idle");
    osCreateThread(&sIdleThread, THREAD_ID_IDLE, Idle_ThreadEntry, NULL, STACK_TOP(sIdleThreadStack),
                   THREAD_PRI_IDLE_INIT);
    osStartThread(&sIdleThread);
}
