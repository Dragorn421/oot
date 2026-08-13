#ifndef SEGMENTED_ADDRESS_H
#define SEGMENTED_ADDRESS_H

#include "ultra64.h"
#include "stdint.h"
#include <libdragon.h>

extern uintptr_t gSegments[NUM_SEGMENTS];

#define SEGMENTED_TO_VIRTUAL(addr) (void*)(gSegments[SEGMENT_NUMBER(addr)] + SEGMENT_OFFSET(addr) + (u32)KSEG0_START_ADDR)

#endif
