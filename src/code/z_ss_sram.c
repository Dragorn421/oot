#include "ultra64.h"
#include "printf.h"
#include "ss_sram.h"

// TODO-ootdragon implement saving (the below functions have been stubbed out)

typedef struct SsSramContext {
    int unused;
} SsSramContext; // size = 0xA4

SsSramContext sSsSramContext = { 0 };

void SsSram_Init(s32 addr, u8 handleType, u8 handleDomain, u8 handleLatency, u8 handlePageSize, u8 handleRelDuration,
                 u8 handlePulse, u32 handleSpeed) {
}

void SsSram_Dma(void* dramAddr, size_t size, s32 direction) {
}

void SsSram_ReadWrite(s32 addr, void* dramAddr, size_t size, s32 direction) {
    PRINTF("ssSRAMReadWrite:%08x %08x %08x %d\n", addr, dramAddr, size, direction);
    if (direction == OS_READ) {
        memset(dramAddr, 0, size);
    }
}
