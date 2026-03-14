#ifndef _Z64EFFECT_H_
#define _Z64EFFECT_H_
#include <color.h>

// gz has trail related structs but they seem to be different in dbg
typedef struct {
    /* 0x0000 */ char        unk_00[0x184];
    /* 0x0184 */ Color_RGBA8 p1Start;
    /* 0x0188 */ Color_RGBA8 p2Start;
    /* 0x018C */ Color_RGBA8 p1End;
    /* 0x0190 */ Color_RGBA8 p2End;
    /* 0x0194 */ u32         unk_194; // these are proably bytes
    /* 0x0198 */ u32         unk_198;
    /* 0x019C */ u32         unk_19C;
    /* 0x01A0 */
} TrailEffect; // size = unk

// effect id 0
typedef struct struct_8015D8A0 {
    /* 0x000 */ Vec3s unk0;
    /* 0x006 */ char unk_6[0x486];
    /* 0x48C */ f32 unk48C;
    /* 0x490 */ f32 unk490;
    /* 0x494 */ s32 unk494;
    /* 0x498 */ s32 unk498;
    Color_RGBA8 unk49C[4];
    Color_RGBA8 unk4AC[4];
    s32 unk4BC;
    s32 unk4C0;
} struct_8015D8A0;

// effect id 3
typedef struct struct_8011E068 {
    /* 0x00 */ char unk_0[2];
    /* 0x02 */ Vec3s unk2;
    /* 0x08 */ char unk_8[0x26];
    /* 0x2E */ Vec3s unk2E;
    /* 0x34 */ char unk_34[0xC];
} struct_8011E068;

#endif
