#include "ultra64/asm.h"

.set noat
.set noreorder
.set gp=64

.section .text

.balign 16

LEAF(LeoGetAAdr2)
    lui        $at, (0x1FFFE >> 16)
    srl        $t8, $a0, 15
    ori        $at, $at, (0x1FFFE & 0xFFFF)
    and        $v1, $t8, $at
    lui        $at, (0x7EE80 >> 16)
    ori        $at, $at, (0x7EE80 & 0xFFFF)
    srl        $t2, $a0, 8
    add        $v0, $v1, $at
    andi       $t3, $t2, 0xF
    andi       $t5, $a0, 0xFE
    addi       $t4, $t3, 0x1
    sll        $t6, $t5, 24
    andi       $t9, $a0, 0x1
    srl        $v1, $a0, 12
    sw         $t4, 0x0($a2)
    sra        $t7, $t6, 25
    sll        $t8, $t9, 4
    andi       $t1, $v1, 0xF
    sw         $t7, 0x0($a3)
    or         $t2, $t1, $t8
    jr         $ra
     sw        $t2, 0x0($a1)
END(LeoGetAAdr2)
