glabel func_8092E930
/* 02360 8092E930 27BDFFD8 */  addiu   $sp, $sp, 0xFFD8           ## $sp = FFFFFFD8
/* 02364 8092E934 AFB00020 */  sw      $s0, 0x0020($sp)
/* 02368 8092E938 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 0236C 8092E93C AFBF0024 */  sw      $ra, 0x0024($sp)
/* 02370 8092E940 3C050601 */  lui     $a1, 0x0601                ## $a1 = 06010000
/* 02374 8092E944 24A5CC6C */  addiu   $a1, $a1, 0xCC6C           ## $a1 = 0600CC6C
/* 02378 8092E948 2484014C */  addiu   $a0, $a0, 0x014C           ## $a0 = 0000014C
/* 0237C 8092E94C 0C029490 */  jal     Animation_MorphToPlayOnce
/* 02380 8092E950 3C06C040 */  lui     $a2, 0xC040                ## $a2 = C0400000
/* 02384 8092E954 3C040601 */  lui     $a0, 0x0601                ## $a0 = 06010000
/* 02388 8092E958 0C028800 */  jal     Animation_GetLastFrame

/* 0238C 8092E95C 2484CC6C */  addiu   $a0, $a0, 0xCC6C           ## $a0 = 0600CC6C
/* 02390 8092E960 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02394 8092E964 24054000 */  addiu   $a1, $zero, 0x4000         ## $a1 = 00004000
/* 02398 8092E968 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 0239C 8092E96C 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 023A0 8092E970 0C00D09B */  jal     Actor_SetColorFilter
/* 023A4 8092E974 AFA20010 */  sw      $v0, 0x0010($sp)
/* 023A8 8092E978 3C040601 */  lui     $a0, 0x0601                ## $a0 = 06010000
/* 023AC 8092E97C 0C028800 */  jal     Animation_GetLastFrame

/* 023B0 8092E980 2484CC6C */  addiu   $a0, $a0, 0xCC6C           ## $a0 = 0600CC6C
/* 023B4 8092E984 3C048094 */  lui     $a0, %hi(D_80938C98)       ## $a0 = 80940000
/* 023B8 8092E988 8C848C98 */  lw      $a0, %lo(D_80938C98)($a0)
/* 023BC 8092E98C 24054000 */  addiu   $a1, $zero, 0x4000         ## $a1 = 00004000
/* 023C0 8092E990 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 023C4 8092E994 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 023C8 8092E998 0C00D09B */  jal     Actor_SetColorFilter
/* 023CC 8092E99C AFA20010 */  sw      $v0, 0x0010($sp)
/* 023D0 8092E9A0 3C040601 */  lui     $a0, 0x0601                ## $a0 = 06010000
/* 023D4 8092E9A4 0C028800 */  jal     Animation_GetLastFrame

/* 023D8 8092E9A8 2484CC6C */  addiu   $a0, $a0, 0xCC6C           ## $a0 = 0600CC6C
/* 023DC 8092E9AC 3C048094 */  lui     $a0, %hi(D_80938C9C)       ## $a0 = 80940000
/* 023E0 8092E9B0 8C848C9C */  lw      $a0, %lo(D_80938C9C)($a0)
/* 023E4 8092E9B4 24054000 */  addiu   $a1, $zero, 0x4000         ## $a1 = 00004000
/* 023E8 8092E9B8 240600FF */  addiu   $a2, $zero, 0x00FF         ## $a2 = 000000FF
/* 023EC 8092E9BC 00003825 */  or      $a3, $zero, $zero          ## $a3 = 00000000
/* 023F0 8092E9C0 0C00D09B */  jal     Actor_SetColorFilter
/* 023F4 8092E9C4 AFA20010 */  sw      $v0, 0x0010($sp)
/* 023F8 8092E9C8 920E06C5 */  lbu     $t6, 0x06C5($s0)           ## 000006C5
/* 023FC 8092E9CC 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 02400 8092E9D0 24053969 */  addiu   $a1, $zero, 0x3969         ## $a1 = 00003969
/* 02404 8092E9D4 31CFFFFE */  andi    $t7, $t6, 0xFFFE           ## $t7 = 00000000
/* 02408 8092E9D8 0C24CF4B */  jal     func_80933D2C
/* 0240C 8092E9DC A20F06C5 */  sb      $t7, 0x06C5($s0)           ## 000006C5
/* 02410 8092E9E0 3C188093 */  lui     $t8, %hi(func_8092EA00)    ## $t8 = 80930000
/* 02414 8092E9E4 2718EA00 */  addiu   $t8, $t8, %lo(func_8092EA00) ## $t8 = 8092EA00
/* 02418 8092E9E8 AE180190 */  sw      $t8, 0x0190($s0)           ## 00000190
/* 0241C 8092E9EC 8FBF0024 */  lw      $ra, 0x0024($sp)
/* 02420 8092E9F0 8FB00020 */  lw      $s0, 0x0020($sp)
/* 02424 8092E9F4 27BD0028 */  addiu   $sp, $sp, 0x0028           ## $sp = 00000000
/* 02428 8092E9F8 03E00008 */  jr      $ra
/* 0242C 8092E9FC 00000000 */  nop
