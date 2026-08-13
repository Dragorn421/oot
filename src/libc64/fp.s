#include "ultra64/asm.h"
#include "ultra64/regdef.h"

.data

#if !PLATFORM_N64

DATA(gPositiveInfinity)
    .word 0x7F800000
ENDDATA(gPositiveInfinity)

DATA(gNegativeInfinity)
    .word 0xFF800000
ENDDATA(gNegativeInfinity)

DATA(gPositiveZero)
    .word 0x00000000
ENDDATA(gPositiveZero)

DATA(gNegativeZero)
    .word 0x80000000
ENDDATA(gNegativeZero)

DATA(qNaN0x3FFFFF)
    .word 0x7FBFFFFF
ENDDATA(qNaN0x3FFFFF)

DATA(qNaN0x10000)
    .word 0x7F810000
ENDDATA(qNaN0x10000)

DATA(sNaN0x3FFFFF)
    .word 0x7FFFFFFF
ENDDATA(sNaN0x3FFFFF)

#endif
