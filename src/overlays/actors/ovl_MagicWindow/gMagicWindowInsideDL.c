#include "ultra64.h"
#include "global.h"

Vtx gMagicWindowInsideDL_inside_mesh_layer_Opaque_vtx_cull[8] = {
    { { { 100, -700, -700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, -700, 700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, 700, 700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, 700, -700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, -700, -700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, -700, 700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, 700, 700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
    { { { 100, 700, -700 }, 0, { -16, -16 }, { 0x0, 0x0, 0x0, 0x0 } } },
};

Vtx gMagicWindowInsideDL_inside_mesh_layer_Opaque_vtx_0[4] = {
    { { { 100, -700, -700 }, 0, { -16, 1008 }, { 0x7F, 0x0, 0x0, 0xFF } } },
    { { { 100, 700, -700 }, 0, { -16, 1008 }, { 0x7F, 0x0, 0x0, 0xFF } } },
    { { { 100, 700, 700 }, 0, { -16, 1008 }, { 0x7F, 0x0, 0x0, 0xFF } } },
    { { { 100, -700, 700 }, 0, { -16, 1008 }, { 0x7F, 0x0, 0x0, 0xFF } } },
};

Gfx gMagicWindowInsideDL_inside_mesh_layer_Opaque_tri_0[] = {
    gsSPVertex(gMagicWindowInsideDL_inside_mesh_layer_Opaque_vtx_0 + 0, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

Gfx mat_gMagicWindowInsideDL_inside_layerOpaque[] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1),
    gsSPSetGeometryMode(G_ZBUFFER | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPClearGeometryMode(G_SHADE | G_CULL_FRONT | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                     G_AC_THRESHOLD | G_ZS_PRIM | Z_UPD | IM_RD | CVG_DST_SAVE | ZMODE_OPA | FORCE_BL |
                         GBL_c1(G_BL_CLR_BL, G_BL_0, G_BL_CLR_MEM, G_BL_1MA) |
                         GBL_c2(G_BL_CLR_BL, G_BL_0, G_BL_CLR_MEM, G_BL_1MA)),
    gsDPSetPrimDepth(-1, 0),
    gsSPTexture(65535, 65535, 0, 0, 1),
    gsDPSetPrimColor(0, 0, 254, 254, 254, 255),
    gsSPEndDisplayList(),
};

Gfx mat_revert_gMagicWindowInsideDL_inside_layerOpaque[] = {
    gsDPPipeSync(),
    gsDPSetPrimDepth(0, 0),
    gsSPEndDisplayList(),
};

Gfx gMagicWindowInsideDL[] = {
    gsSPClearGeometryMode(G_LIGHTING),
    gsSPVertex(gMagicWindowInsideDL_inside_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
    gsSPCullDisplayList(0, 7),
    gsSPDisplayList(mat_gMagicWindowInsideDL_inside_layerOpaque),
    gsSPDisplayList(gMagicWindowInsideDL_inside_mesh_layer_Opaque_tri_0),
    gsSPDisplayList(mat_revert_gMagicWindowInsideDL_inside_layerOpaque),
    gsSPEndDisplayList(),
};
