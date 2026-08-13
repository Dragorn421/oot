#include <libdragon.h>
#include "ultra64.h"

void Sleep_Cycles(u32 cycles) {
    wait_ticks(cycles);
}

void Sleep_Nsec(u32 nsec) {
    Sleep_Cycles(TICKS_FROM_US(nsec / 1000));
}

void Sleep_Usec(u32 usec) {
    Sleep_Cycles(TICKS_FROM_US(usec));
}

// originally "msleep"
void Sleep_Msec(u32 ms) {
    Sleep_Cycles(TICKS_FROM_MS(ms));
}

void Sleep_Sec(u32 sec) {
    Sleep_Cycles(TICKS_FROM_MS(sec * 1000));
}
