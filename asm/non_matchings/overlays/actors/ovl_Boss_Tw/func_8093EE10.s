.late_rodata
glabel D_8094AFD0
 .word 0x43CE8000
glabel D_8094AFD4
 .word 0x43838000
glabel D_8094AFD8
 .word 0x4622F983
glabel D_8094AFDC
 .word 0x4622F983

.text
glabel func_8093EE10
/* 06140 8093EE10 27BDFFC8 */  addiu   $sp, $sp, 0xFFC8           ## $sp = FFFFFFC8
/* 06144 8093EE14 AFBF001C */  sw      $ra, 0x001C($sp)           
/* 06148 8093EE18 AFB00018 */  sw      $s0, 0x0018($sp)           
/* 0614C 8093EE1C AFA5003C */  sw      $a1, 0x003C($sp)           
/* 06150 8093EE20 848E0150 */  lh      $t6, 0x0150($a0)           ## 00000150
/* 06154 8093EE24 00808025 */  or      $s0, $a0, $zero            ## $s0 = 00000000
/* 06158 8093EE28 31CF000F */  andi    $t7, $t6, 0x000F           ## $t7 = 00000000
/* 0615C 8093EE2C 15E00003 */  bne     $t7, $zero, .L8093EE3C     
/* 06160 8093EE30 00000000 */  nop
/* 06164 8093EE34 0C00BE0A */  jal     Audio_PlayActorSound2
              
/* 06168 8093EE38 2405391E */  addiu   $a1, $zero, 0x391E         ## $a1 = 0000391E
.L8093EE3C:
/* 0616C 8093EE3C 3C188095 */  lui     $t8, %hi(D_8094C84C)       ## $t8 = 80950000
/* 06170 8093EE40 8F18C84C */  lw      $t8, %lo(D_8094C84C)($t8)  
/* 06174 8093EE44 871905FA */  lh      $t9, 0x05FA($t8)           ## 809505FA
/* 06178 8093EE48 2B210002 */  slti    $at, $t9, 0x0002           
/* 0617C 8093EE4C 50200027 */  beql    $at, $zero, .L8093EEEC     
/* 06180 8093EE50 860D017A */  lh      $t5, 0x017A($s0)           ## 0000017A
/* 06184 8093EE54 86080178 */  lh      $t0, 0x0178($s0)           ## 00000178
/* 06188 8093EE58 24090014 */  addiu   $t1, $zero, 0x0014         ## $t1 = 00000014
/* 0618C 8093EE5C 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 06190 8093EE60 55000019 */  bnel    $t0, $zero, .L8093EEC8     
/* 06194 8093EE64 3C014600 */  lui     $at, 0x4600                ## $at = 46000000
/* 06198 8093EE68 44816000 */  mtc1    $at, $f12                  ## $f12 = 8192.00
/* 0619C 8093EE6C 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 061A0 8093EE70 A6090178 */  sh      $t1, 0x0178($s0)           ## 00000178
/* 061A4 8093EE74 3C0A8095 */  lui     $t2, %hi(D_8094C84C)       ## $t2 = 80950000
/* 061A8 8093EE78 8D4AC84C */  lw      $t2, %lo(D_8094C84C)($t2)  
/* 061AC 8093EE7C 3C014248 */  lui     $at, 0x4248                ## $at = 42480000
/* 061B0 8093EE80 44816000 */  mtc1    $at, $f12                  ## $f12 = 50.00
/* 061B4 8093EE84 C5440024 */  lwc1    $f4, 0x0024($t2)           ## 80950024
/* 061B8 8093EE88 46040180 */  add.s   $f6, $f0, $f4              
/* 061BC 8093EE8C 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 061C0 8093EE90 E60604B0 */  swc1    $f6, 0x04B0($s0)           ## 000004B0
/* 061C4 8093EE94 3C0143C8 */  lui     $at, 0x43C8                ## $at = 43C80000
/* 061C8 8093EE98 44814000 */  mtc1    $at, $f8                   ## $f8 = 400.00
/* 061CC 8093EE9C 3C0142C8 */  lui     $at, 0x42C8                ## $at = 42C80000
/* 061D0 8093EEA0 44816000 */  mtc1    $at, $f12                  ## $f12 = 100.00
/* 061D4 8093EEA4 46080280 */  add.s   $f10, $f0, $f8             
/* 061D8 8093EEA8 0C00CFC8 */  jal     Rand_CenteredFloat
              
/* 061DC 8093EEAC E60A04B4 */  swc1    $f10, 0x04B4($s0)          ## 000004B4
/* 061E0 8093EEB0 3C0B8095 */  lui     $t3, %hi(D_8094C84C)       ## $t3 = 80950000
/* 061E4 8093EEB4 8D6BC84C */  lw      $t3, %lo(D_8094C84C)($t3)  
/* 061E8 8093EEB8 C572002C */  lwc1    $f18, 0x002C($t3)          ## 8095002C
/* 061EC 8093EEBC 46120100 */  add.s   $f4, $f0, $f18             
/* 061F0 8093EEC0 E60404B8 */  swc1    $f4, 0x04B8($s0)           ## 000004B8
/* 061F4 8093EEC4 3C014600 */  lui     $at, 0x4600                ## $at = 46000000
.L8093EEC8:
/* 061F8 8093EEC8 44813000 */  mtc1    $at, $f6                   ## $f6 = 8192.00
/* 061FC 8093EECC 3C0140A0 */  lui     $at, 0x40A0                ## $at = 40A00000
/* 06200 8093EED0 44814000 */  mtc1    $at, $f8                   ## $f8 = 5.00
/* 06204 8093EED4 240C000A */  addiu   $t4, $zero, 0x000A         ## $t4 = 0000000A
/* 06208 8093EED8 A60C017A */  sh      $t4, 0x017A($s0)           ## 0000017A
/* 0620C 8093EEDC E60604C8 */  swc1    $f6, 0x04C8($s0)           ## 000004C8
/* 06210 8093EEE0 1000004A */  beq     $zero, $zero, .L8093F00C   
/* 06214 8093EEE4 E6080068 */  swc1    $f8, 0x0068($s0)           ## 00000068
/* 06218 8093EEE8 860D017A */  lh      $t5, 0x017A($s0)           ## 0000017A
.L8093EEEC:
/* 0621C 8093EEEC 24010009 */  addiu   $at, $zero, 0x0009         ## $at = 00000009
/* 06220 8093EEF0 00001025 */  or      $v0, $zero, $zero          ## $v0 = 00000000
/* 06224 8093EEF4 15A10017 */  bne     $t5, $at, .L8093EF54       
/* 06228 8093EEF8 26040024 */  addiu   $a0, $s0, 0x0024           ## $a0 = 00000024
/* 0622C 8093EEFC 3C018095 */  lui     $at, %hi(D_8094AFD0)       ## $at = 80950000
/* 06230 8093EF00 C42AAFD0 */  lwc1    $f10, %lo(D_8094AFD0)($at) 
/* 06234 8093EF04 44809000 */  mtc1    $zero, $f18                ## $f18 = 0.00
/* 06238 8093EF08 44802000 */  mtc1    $zero, $f4                 ## $f4 = 0.00
/* 0623C 8093EF0C 2403000C */  addiu   $v1, $zero, 0x000C         ## $v1 = 0000000C
/* 06240 8093EF10 E60A04B4 */  swc1    $f10, 0x04B4($s0)          ## 000004B4
/* 06244 8093EF14 E612002C */  swc1    $f18, 0x002C($s0)          ## 0000002C
/* 06248 8093EF18 E6040024 */  swc1    $f4, 0x0024($s0)           ## 00000024
.L8093EF1C:
/* 0624C 8093EF1C 00430019 */  multu   $v0, $v1                   
/* 06250 8093EF20 8C990000 */  lw      $t9, 0x0000($a0)           ## 00000024
/* 06254 8093EF24 24420001 */  addiu   $v0, $v0, 0x0001           ## $v0 = 00000001
/* 06258 8093EF28 00021400 */  sll     $v0, $v0, 16               
/* 0625C 8093EF2C 00021403 */  sra     $v0, $v0, 16               
/* 06260 8093EF30 28410032 */  slti    $at, $v0, 0x0032           
/* 06264 8093EF34 00007012 */  mflo    $t6                        
/* 06268 8093EF38 020E7821 */  addu    $t7, $s0, $t6              
/* 0626C 8093EF3C ADF901E8 */  sw      $t9, 0x01E8($t7)           ## 000001E8
/* 06270 8093EF40 8C980004 */  lw      $t8, 0x0004($a0)           ## 00000028
/* 06274 8093EF44 ADF801EC */  sw      $t8, 0x01EC($t7)           ## 000001EC
/* 06278 8093EF48 8C990008 */  lw      $t9, 0x0008($a0)           ## 0000002C
/* 0627C 8093EF4C 1420FFF3 */  bne     $at, $zero, .L8093EF1C     
/* 06280 8093EF50 ADF901F0 */  sw      $t9, 0x01F0($t7)           ## 000001F0
.L8093EF54:
/* 06284 8093EF54 8608001C */  lh      $t0, 0x001C($s0)           ## 0000001C
/* 06288 8093EF58 24010069 */  addiu   $at, $zero, 0x0069         ## $at = 00000069
/* 0628C 8093EF5C 3C028095 */  lui     $v0, %hi(D_8094C848)       ## $v0 = 80950000
/* 06290 8093EF60 15010008 */  bne     $t0, $at, .L8093EF84       
/* 06294 8093EF64 2442C848 */  addiu   $v0, $v0, %lo(D_8094C848)  ## $v0 = 8094C848
/* 06298 8093EF68 8C490000 */  lw      $t1, 0x0000($v0)           ## 8094C848
/* 0629C 8093EF6C C5260024 */  lwc1    $f6, 0x0024($t1)           ## 00000024
/* 062A0 8093EF70 E60604B0 */  swc1    $f6, 0x04B0($s0)           ## 000004B0
/* 062A4 8093EF74 8C4A0000 */  lw      $t2, 0x0000($v0)           ## 8094C848
/* 062A8 8093EF78 C548002C */  lwc1    $f8, 0x002C($t2)           ## 0000002C
/* 062AC 8093EF7C 10000009 */  beq     $zero, $zero, .L8093EFA4   
/* 062B0 8093EF80 E60804B8 */  swc1    $f8, 0x04B8($s0)           ## 000004B8
.L8093EF84:
/* 062B4 8093EF84 3C028095 */  lui     $v0, %hi(D_8094C844)       ## $v0 = 80950000
/* 062B8 8093EF88 2442C844 */  addiu   $v0, $v0, %lo(D_8094C844)  ## $v0 = 8094C844
/* 062BC 8093EF8C 8C4B0000 */  lw      $t3, 0x0000($v0)           ## 8094C844
/* 062C0 8093EF90 C56A0024 */  lwc1    $f10, 0x0024($t3)          ## 80950024
/* 062C4 8093EF94 E60A04B0 */  swc1    $f10, 0x04B0($s0)          ## 000004B0
/* 062C8 8093EF98 8C4C0000 */  lw      $t4, 0x0000($v0)           ## 8094C844
/* 062CC 8093EF9C C592002C */  lwc1    $f18, 0x002C($t4)          ## 00000036
/* 062D0 8093EFA0 E61204B8 */  swc1    $f18, 0x04B8($s0)          ## 000004B8
.L8093EFA4:
/* 062D4 8093EFA4 3C054383 */  lui     $a1, 0x4383                ## $a1 = 43830000
/* 062D8 8093EFA8 34A58000 */  ori     $a1, $a1, 0x8000           ## $a1 = 43838000
/* 062DC 8093EFAC 260404B4 */  addiu   $a0, $s0, 0x04B4           ## $a0 = 000004B4
/* 062E0 8093EFB0 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 062E4 8093EFB4 0C01E107 */  jal     Math_ApproachF
              
/* 062E8 8093EFB8 3C074000 */  lui     $a3, 0x4000                ## $a3 = 40000000
/* 062EC 8093EFBC 3C018095 */  lui     $at, %hi(D_8094AFD4)       ## $at = 80950000
/* 062F0 8093EFC0 C424AFD4 */  lwc1    $f4, %lo(D_8094AFD4)($at)  
/* 062F4 8093EFC4 C60604B4 */  lwc1    $f6, 0x04B4($s0)           ## 000004B4
/* 062F8 8093EFC8 26040068 */  addiu   $a0, $s0, 0x0068           ## $a0 = 00000068
/* 062FC 8093EFCC 24050000 */  addiu   $a1, $zero, 0x0000         ## $a1 = 00000000
/* 06300 8093EFD0 46062032 */  c.eq.s  $f4, $f6                   
/* 06304 8093EFD4 3C063F80 */  lui     $a2, 0x3F80                ## $a2 = 3F800000
/* 06308 8093EFD8 3C073E4C */  lui     $a3, 0x3E4C                ## $a3 = 3E4C0000
/* 0630C 8093EFDC 4502000C */  bc1fl   .L8093F010                 
/* 06310 8093EFE0 C61204B4 */  lwc1    $f18, 0x04B4($s0)          ## 000004B4
/* 06314 8093EFE4 0C01E107 */  jal     Math_ApproachF
              
/* 06318 8093EFE8 34E7CCCD */  ori     $a3, $a3, 0xCCCD           ## $a3 = 3E4CCCCD
/* 0631C 8093EFEC 3C0D8095 */  lui     $t5, %hi(D_8094C84C)       ## $t5 = 80950000
/* 06320 8093EFF0 8DADC84C */  lw      $t5, %lo(D_8094C84C)($t5)  
/* 06324 8093EFF4 24010003 */  addiu   $at, $zero, 0x0003         ## $at = 00000003
/* 06328 8093EFF8 85AE05FA */  lh      $t6, 0x05FA($t5)           ## 809505FA
/* 0632C 8093EFFC 55C10004 */  bnel    $t6, $at, .L8093F010       
/* 06330 8093F000 C61204B4 */  lwc1    $f18, 0x04B4($s0)          ## 000004B4
/* 06334 8093F004 0C00B55C */  jal     Actor_Kill
              
/* 06338 8093F008 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
.L8093F00C:
/* 0633C 8093F00C C61204B4 */  lwc1    $f18, 0x04B4($s0)          ## 000004B4
.L8093F010:
/* 06340 8093F010 C6040028 */  lwc1    $f4, 0x0028($s0)           ## 00000028
/* 06344 8093F014 C60804B0 */  lwc1    $f8, 0x04B0($s0)           ## 000004B0
/* 06348 8093F018 C60A0024 */  lwc1    $f10, 0x0024($s0)          ## 00000024
/* 0634C 8093F01C 46049181 */  sub.s   $f6, $f18, $f4             
/* 06350 8093F020 460A4301 */  sub.s   $f12, $f8, $f10            
/* 06354 8093F024 E7A60030 */  swc1    $f6, 0x0030($sp)           
/* 06358 8093F028 C60A002C */  lwc1    $f10, 0x002C($s0)          ## 0000002C
/* 0635C 8093F02C C60804B8 */  lwc1    $f8, 0x04B8($s0)           ## 000004B8
/* 06360 8093F030 E7AC0034 */  swc1    $f12, 0x0034($sp)          
/* 06364 8093F034 460A4381 */  sub.s   $f14, $f8, $f10            
/* 06368 8093F038 0C03F494 */  jal     Math_FAtan2F              
/* 0636C 8093F03C E7AE002C */  swc1    $f14, 0x002C($sp)          
/* 06370 8093F040 3C018095 */  lui     $at, %hi(D_8094AFD8)       ## $at = 80950000
/* 06374 8093F044 C432AFD8 */  lwc1    $f18, %lo(D_8094AFD8)($at) 
/* 06378 8093F048 C7A20034 */  lwc1    $f2, 0x0034($sp)           
/* 0637C 8093F04C C7B0002C */  lwc1    $f16, 0x002C($sp)          
/* 06380 8093F050 46120102 */  mul.s   $f4, $f0, $f18             
/* 06384 8093F054 C7AC0030 */  lwc1    $f12, 0x0030($sp)          
/* 06388 8093F058 46021202 */  mul.s   $f8, $f2, $f2              
/* 0638C 8093F05C 00000000 */  nop
/* 06390 8093F060 46108282 */  mul.s   $f10, $f16, $f16           
/* 06394 8093F064 4600218D */  trunc.w.s $f6, $f4                   
/* 06398 8093F068 460A4000 */  add.s   $f0, $f8, $f10             
/* 0639C 8093F06C 44183000 */  mfc1    $t8, $f6                   
/* 063A0 8093F070 46000384 */  sqrt.s  $f14, $f0                  
/* 063A4 8093F074 0C03F494 */  jal     Math_FAtan2F              
/* 063A8 8093F078 A7B80024 */  sh      $t8, 0x0024($sp)           
/* 063AC 8093F07C 3C018095 */  lui     $at, %hi(D_8094AFDC)       ## $at = 80950000
/* 063B0 8093F080 C432AFDC */  lwc1    $f18, %lo(D_8094AFDC)($at) 
/* 063B4 8093F084 C60804C8 */  lwc1    $f8, 0x04C8($s0)           ## 000004C8
/* 063B8 8093F088 26040030 */  addiu   $a0, $s0, 0x0030           ## $a0 = 00000030
/* 063BC 8093F08C 46120102 */  mul.s   $f4, $f0, $f18             
/* 063C0 8093F090 4600428D */  trunc.w.s $f10, $f8                  
/* 063C4 8093F094 24060005 */  addiu   $a2, $zero, 0x0005         ## $a2 = 00000005
/* 063C8 8093F098 44075000 */  mfc1    $a3, $f10                  
/* 063CC 8093F09C 4600218D */  trunc.w.s $f6, $f4                   
/* 063D0 8093F0A0 00073C00 */  sll     $a3, $a3, 16               
/* 063D4 8093F0A4 00073C03 */  sra     $a3, $a3, 16               
/* 063D8 8093F0A8 44053000 */  mfc1    $a1, $f6                   
/* 063DC 8093F0AC 00000000 */  nop
/* 063E0 8093F0B0 00052C00 */  sll     $a1, $a1, 16               
/* 063E4 8093F0B4 0C01E1EF */  jal     Math_ApproachS
              
/* 063E8 8093F0B8 00052C03 */  sra     $a1, $a1, 16               
/* 063EC 8093F0BC C61204C8 */  lwc1    $f18, 0x04C8($s0)          ## 000004C8
/* 063F0 8093F0C0 26040032 */  addiu   $a0, $s0, 0x0032           ## $a0 = 00000032
/* 063F4 8093F0C4 87A50024 */  lh      $a1, 0x0024($sp)           
/* 063F8 8093F0C8 4600910D */  trunc.w.s $f4, $f18                  
/* 063FC 8093F0CC 24060005 */  addiu   $a2, $zero, 0x0005         ## $a2 = 00000005
/* 06400 8093F0D0 44072000 */  mfc1    $a3, $f4                   
/* 06404 8093F0D4 00000000 */  nop
/* 06408 8093F0D8 00073C00 */  sll     $a3, $a3, 16               
/* 0640C 8093F0DC 0C01E1EF */  jal     Math_ApproachS
              
/* 06410 8093F0E0 00073C03 */  sra     $a3, $a3, 16               
/* 06414 8093F0E4 0C00B642 */  jal     func_8002D908              
/* 06418 8093F0E8 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 0641C 8093F0EC 0C00B5FB */  jal     Actor_UpdatePosition              
/* 06420 8093F0F0 02002025 */  or      $a0, $s0, $zero            ## $a0 = 00000000
/* 06424 8093F0F4 8FBF001C */  lw      $ra, 0x001C($sp)           
/* 06428 8093F0F8 8FB00018 */  lw      $s0, 0x0018($sp)           
/* 0642C 8093F0FC 27BD0038 */  addiu   $sp, $sp, 0x0038           ## $sp = 00000000
/* 06430 8093F100 03E00008 */  jr      $ra                        
/* 06434 8093F104 00000000 */  nop
