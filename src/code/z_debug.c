#include "global.h"

GameInfo* gGameInfo;

typedef struct struct_8015FA98_sub {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3[0x15];
} struct_8015FA98_sub;
typedef struct struct_8015FA98 {
    struct_8015FA98_sub unk0[0x16];
} struct_8015FA98;

struct_8015FA98 D_8015FA98;

s16 D_8011E0B0 = 0;
Color_RGBA8 D_8011E0B4[] = {
    { 0xFF, 0xFF, 0x20, 0xC0 }, { 0xFF, 0x96, 0x80, 0xC0 }, { 0x80, 0x60, 0x00, 0x40 }, { 0xC0, 0x80, 0x10, 0x80 },
    { 0xFF, 0xC0, 0x20, 0x80 }, { 0xE6, 0xE6, 0xDC, 0x40 }, { 0x80, 0x96, 0xFF, 0x80 }, { 0x80, 0xFF, 0x20, 0x80 },
};

typedef struct struct_8011E0D4 {
    u16 unk0;
    u16 unk2;
} struct_8011E0D4;
struct_8011E0D4 D_8011E0D4[] = {
    { 0x0020, 0x0008 }, { 0x0020, 0x0002 }, { 0x0020, 0x0004 }, { 0x0020, 0x8000 }, { 0x0010, 0x0004 },
    { 0x0020, 0x0001 }, { 0x0020, 0x0010 }, { 0x0020, 0x0200 }, { 0x0020, 0x0100 }, { 0x0020, 0x0800 },
    { 0x0020, 0x4000 }, { 0x0020, 0x2000 }, { 0x0020, 0x0400 }, { 0x0010, 0x8000 }, { 0x0010, 0x4000 },
    { 0x0010, 0x2000 }, { 0x0010, 0x0020 }, { 0x0010, 0x0008 }, { 0x0010, 0x0001 }, { 0x0010, 0x0200 },
    { 0x0010, 0x0002 }, { 0x0010, 0x1000 }, { 0x0020, 0x1000 }, { 0x0010, 0x0100 }, { 0x0010, 0x0800 },
    { 0x1000, 0x0010 }, { 0x1000, 0x8000 }, { 0x1000, 0x4000 }, { 0x1000, 0x0001 },
};

u8 D_8011E148[] = {
    0x20, 0x53, 0x4F, 0x50, 0x51, 0x4D, 0x59, 0x44, 0x55, 0x49, 0x5A, 0x43, 0x4E, 0x4B,
    0x58, 0x63, 0x73, 0x69, 0x57, 0x41, 0x56, 0x48, 0x47, 0x6D, 0x6E, 0x42, 0x64, 0x6B,
    0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void func_800636C0_cc0(void) {
    s32 i;

    gGameInfo = SystemArena_MallocDebug(sizeof(GameInfo), "../z_debug.c", 260);
    gGameInfo->unk0 = 0;
    gGameInfo->unk4 = 0;
    gGameInfo->unk8 = 0;
    gGameInfo->unkC = 0;
    gGameInfo->unk10 = 0;
    for (i = 0; i < ARRAY_COUNT(gGameInfo->data); i++) {
        gGameInfo->data[i] = 0;
    }
}

void func_8006375C_cc0(s32 arg0, s32 arg1, s32 arg2) {
}

#ifdef NON_MATCHING
// Note: matched by Roman in 64b5536ab
void func_8006376C_cc0(s32 arg0, s32 arg1, s32 arg2, u8* arg3) {
    s16 var_v1;
    s8* var_t0;
    struct_8015FA98_sub* temp_v0;
    u8* var_s0;
    u8 temp_a3;
    u8 temp_a3_2;

    temp_v0 = &D_8015FA98.unk0[D_8011E0B0];
    if (D_8011E0B0 < 0x16) {
        temp_v0->unk0 = arg0 & 0xFF;
        temp_v0->unk1 = arg1 & 0xFF;
        temp_v0->unk2 = arg2 & 0xFF;
        temp_a3 = *arg3;
        var_v1 = 0;
        var_t0 = &temp_v0->unk3[1];
        var_s0 = arg3 + 1;
        temp_v0->unk3[0] = (s8)temp_a3;
        if (temp_a3 != 0) {
        loop_2:
            var_v1 += 1;
            if (((var_v1 < 0x15) ^ 1) == 0) {
                temp_a3_2 = *var_s0;
                *var_t0 = temp_a3_2;
                var_t0 += 1;
                var_s0 += 1;
                if (temp_a3_2 != 0) {
                    goto loop_2;
                }
            }
        }
        *var_t0 = 0;
        D_8011E0B0 += 1;
    }
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_debug/func_8006376C.s")
#endif

void func_80063828_cc0(GfxPrint* arg0) {
    Color_RGBA8* temp_v0;
    s32 i;

    for (i = 0; D_8011E0B0 > i; i++) {
        temp_v0 = &D_8011E0B4[(u8)D_8015FA98.unk0[i].unk2];
        GfxPrint_SetColor(arg0, (u32)temp_v0->r, (u32)temp_v0->g, (u32)temp_v0->b, (u32)temp_v0->a);
        GfxPrint_SetPos(arg0, (s32)(u8)D_8015FA98.unk0[i].unk0, (s32)(u8)D_8015FA98.unk0[i].unk1);
        GfxPrint_Printf(arg0, "%s", D_8015FA98.unk0[i].unk3);
    }
}

void func_8006390C_cc0(struct_8006390C_arg0* arg0) {
    s32 var_a3;
    s32 var_v1;
    s32 var_v1_2;
    s32 var = (((((gGameInfo->unk4 * 6) + gGameInfo->unk0) * 0x10) - 0x10));

    var_a3 = arg0->unk00 & 0xF00;
    if ((~(arg0->unk00 | ~0x20) == 0) || (~(arg0->unk00 | ~0x10) == 0) || (~(arg0->unk00 | ~0x1000) == 0)) {
        for (var_v1 = 0; var_v1 < 0x1D; var_v1++) {
            if (!((~(~D_8011E0D4[var_v1].unk0 | arg0->unk00) != 0) ||
                  (~(~D_8011E0D4[var_v1].unk2 | arg0->unk0C) != 0))) {
                break;
            }
        }
        if (var_v1 < 0x1D) {
            if (var_v1 == gGameInfo->unk4) {
                gGameInfo->unk0 = (s32)(gGameInfo->unk0 + 1) % 7;
                return;
            }
            gGameInfo->unk4 = var_v1;
            gGameInfo->unk0 = 0;
        }
    } else {
        switch (gGameInfo->unk0) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                if (var_a3 == gGameInfo->unkC) {
                    gGameInfo->unk10 -= 1;
                    if (gGameInfo->unk10 < 0) {
                        gGameInfo->unk10 = 1;
                    } else {
                        var_a3 ^= gGameInfo->unkC;
                    }
                } else {
                    gGameInfo->unk10 = 0x10;
                    gGameInfo->unkC = var_a3;
                }

                var_v1_2 = (var_a3 & 0x100)   ? ((~(arg0->unk00 | ~0xC000) == 0)   ? 0x3E8
                                                 : (~(arg0->unk00 | ~0x8000) == 0) ? 0x64
                                                 : (~(arg0->unk00 | ~0x4000) == 0) ? 0xA
                                                                                   : 1)
                           : (var_a3 & 0x200) ? ((~(arg0->unk00 | ~0xC000) == 0)   ? -0x3E8
                                                 : (~(arg0->unk00 | ~0x8000) == 0) ? -0x64
                                                 : (~(arg0->unk00 | ~0x4000) == 0) ? -0xA
                                                                                   : -1)
                                              : 0;

                gGameInfo->data[gGameInfo->unk8 + var] += var_v1_2;
                if (var_a3 & 0x800) {
                    gGameInfo->unk8 -= 1;
                    if (gGameInfo->unk8 < 0) {
                        gGameInfo->unk8 = 0xF;
                    }
                } else if (var_a3 & 0x400) {
                    gGameInfo->unk8 += 1;
                    if (gGameInfo->unk8 >= 0x10) {
                        gGameInfo->unk8 = 0;
                    }
                }
                if (gGameInfo->data[0x660] != 0) {
                    gGameInfo->data[0x660] = 0;
                    func_800AA000(0.0f, gGameInfo->data[(0xCD6 - 0x14) / 2], gGameInfo->data[(0xCD8 - 0x14) / 2],
                                  gGameInfo->data[(0xCDA - 0x14) / 2]);
                }
                break;
        }
    }
}

void func_80063C04_cc0(GfxPrint* arg0) {
    s32 temp_s4;
    s32 temp_s5;
    s32 var_s0;
    s32 var_s2;
    char sp4C[3];

    temp_s4 = (gGameInfo->unk0 * 0x10) - 0x10;
    temp_s5 = (((gGameInfo->unk4 * 6) + gGameInfo->unk0) * 0x10) - 0x10;
    sp4C[0] = 0x52;
    sp4C[1] = D_8011E148[gGameInfo->unk4];
    sp4C[2] = 0;
    GfxPrint_SetColor(arg0, 0U, 0x80U, 0x80U, 0x80U);
    var_s2 = temp_s4;
    for (var_s0 = 0; var_s0 != 0x10; var_s0++) {
        if (var_s0 == gGameInfo->unk8) {
            GfxPrint_SetColor(arg0, 0U, 0xFFU, 0xFFU, 0xFFU);
        }
        GfxPrint_SetPos(arg0, 3, var_s0 + 5);
        GfxPrint_Printf(arg0, "%s%02d%6d", &sp4C, var_s2, (s32)(gGameInfo->data[var_s0 + temp_s5]));
        if (var_s0 == gGameInfo->unk8) {
            GfxPrint_SetColor(arg0, 0U, 0x80U, 0x80U, 0x80U);
        }
        var_s2 += 1;
    }
}

void func_80063D7C_cc0(GraphicsContext* arg0) {
    Gfx* temp_v0;
    Gfx* sp78;
    s32 pad1[7];
    GfxPrint sp48;
    s32 pad2[2];
    Gfx* sp30[4];

    func_800C6AC4(sp30, arg0, "../z_debug.c", 0x274);
    GfxPrint_Ctor(&sp48);
    sp78 = arg0->polyOpa.p;
    temp_v0 = func_800C6C20(sp78);
    gSPDisplayList(arg0->overlay.p++, temp_v0);
    GfxPrint_Open(&sp48, temp_v0);
    if ((gGameInfo->data[0xC0] == 1) || (gGameInfo->data[0xC0] == 8)) {
        func_80063828_cc0(&sp48);
    }
    if (gGameInfo->unk0 != 0) {
        func_80063C04_cc0(&sp48);
    }
    D_8011E0B0 = 0;
    temp_v0 = GfxPrint_Close(&sp48);
    gSPEndDisplayList(temp_v0++);
    func_800C6C28(sp78, temp_v0);
    arg0->polyOpa.p = temp_v0;
    if (1) {}
    func_800C6B54(sp30, arg0, "../z_debug.c", 0x298);
    GfxPrint_Dtor(&sp48);
}
