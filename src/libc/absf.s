#include "ultra64/asm.h"

#if OOT_DEBUG
.set noreorder
#endif

.section .text

.balign 16

LEAF(absf)
    abs.s       $f0, $f12
    jr          $ra
     nop
END(absf)
