#include "ultra64.h"
#include "gfx.h"
#include "tex_len.h"

#define sTex_WIDTH 32
#define sTex_HEIGHT 32
static u64 sTex[TEX_LEN(u64, sTex_WIDTH, sTex_HEIGHT, 8)] = {
#include "assets/overlays/ovl_Oceff_Spot/sTex.i8.inc.c"
};

static Vtx sCylinderVtx[27] = {
#include "assets/overlays/ovl_Oceff_Spot/sCylinderVtx.inc.c"
};

static Gfx sCylinderMaterialDL[19] = {
#include "assets/overlays/ovl_Oceff_Spot/sCylinderMaterialDL.inc.c"
};

static Gfx sCylinderModelDL[18] = {
#include "assets/overlays/ovl_Oceff_Spot/sCylinderModelDL.inc.c"
};
