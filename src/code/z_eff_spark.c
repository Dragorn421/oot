#include "global.h"

// original name: "spark"
void EffectSpark_Init(void* thisx, void* initParamsx) {
    EffectSpark* this = (EffectSpark*)thisx;
    EffectSparkInit* initParams = (EffectSparkInit*)initParamsx;
    EffectSparkElement* elem;
    f32 velocityNorm;
    s32 i;

    if ((this != NULL) && (initParams != NULL)) {
        if ((initParams->uDiv == 0) || (initParams->vDiv == 0)) {
            // Translates to: "u_div,v_div 0 is not good."
            osSyncPrintf("spark():u_div,v_div 0では困る。\n");
            return;
        }

        this->pos = initParams->pos;
        this->unk48C = initParams->unk48C;
        this->unk490 = initParams->unk490;
        this->uDiv = initParams->uDiv;
        this->vDiv = initParams->vDiv;
        this->unk49C[0].r = initParams->unk49C[0].r;
        this->unk49C[0].g = initParams->unk49C[0].g;
        this->unk49C[0].b = initParams->unk49C[0].b;
        this->unk49C[0].a = initParams->unk49C[0].a;
        this->unk49C[1].r = initParams->unk49C[1].r;
        this->unk49C[1].g = initParams->unk49C[1].g;
        this->unk49C[1].b = initParams->unk49C[1].b;
        this->unk49C[1].a = initParams->unk49C[1].a;
        this->unk49C[2].r = initParams->unk49C[2].r;
        this->unk49C[2].g = initParams->unk49C[2].g;
        this->unk49C[2].b = initParams->unk49C[2].b;
        this->unk49C[2].a = initParams->unk49C[2].a;
        this->unk49C[3].r = initParams->unk49C[3].r;
        this->unk49C[3].g = initParams->unk49C[3].g;
        this->unk49C[3].b = initParams->unk49C[3].b;
        this->unk49C[3].a = initParams->unk49C[3].a;
        this->unk4AC[0].r = initParams->unk4AC[0].r;
        this->unk4AC[0].g = initParams->unk4AC[0].g;
        this->unk4AC[0].b = initParams->unk4AC[0].b;
        this->unk4AC[0].a = initParams->unk4AC[0].a;
        this->unk4AC[1].r = initParams->unk4AC[1].r;
        this->unk4AC[1].g = initParams->unk4AC[1].g;
        this->unk4AC[1].b = initParams->unk4AC[1].b;
        this->unk4AC[1].a = initParams->unk4AC[1].a;
        this->unk4AC[2].r = initParams->unk4AC[2].r;
        this->unk4AC[2].g = initParams->unk4AC[2].g;
        this->unk4AC[2].b = initParams->unk4AC[2].b;
        this->unk4AC[2].a = initParams->unk4AC[2].a;
        this->unk4AC[3].r = initParams->unk4AC[3].r;
        this->unk4AC[3].g = initParams->unk4AC[3].g;
        this->unk4AC[3].b = initParams->unk4AC[3].b;
        this->unk4AC[3].a = initParams->unk4AC[3].a;
        this->unk4C0 = initParams->unk4C0;

        this->numElements = (this->uDiv * this->vDiv) + 2;
        if (this->numElements > ARRAY_COUNT(this->elements)) {
            osSyncPrintf("table_sizeオーバー\n"); // "over table_size"
            return;
        }

        for (i = 0; i < this->numElements; i++) {
            elem = &this->elements[i];

            elem->pos.x = this->pos.x;
            elem->pos.y = this->pos.y;
            elem->pos.z = this->pos.z;
            elem->velocity.x = Math_Rand_ZeroOne() - 0.5f;
            elem->velocity.y = Math_Rand_ZeroOne() - 0.5f;
            elem->velocity.z = Math_Rand_ZeroOne() - 0.5f;

            velocityNorm = sqrtf(SQ(elem->velocity.x) + SQ(elem->velocity.y) + SQ(elem->velocity.z));

            if (!(fabsf(velocityNorm) < 0.008f)) {
                elem->velocity.x *= this->unk48C * (1.0f / velocityNorm);
                elem->velocity.y *= this->unk48C * (1.0f / velocityNorm);
                elem->velocity.z *= this->unk48C * (1.0f / velocityNorm);
            } else {
                elem->velocity.x = elem->velocity.z = 0.0f;
                elem->velocity.y = this->unk48C;
            }

            elem->unk18.x = 30000.0f - Math_Rand_ZeroOne() * 15000.0f;
            elem->unk18.y = 30000.0f - Math_Rand_ZeroOne() * 15000.0f;
            elem->unk18.z = 30000.0f - Math_Rand_ZeroOne() * 15000.0f;
            elem->unk1E.x = Math_Rand_ZeroOne() * 65534.0f;
            elem->unk1E.y = Math_Rand_ZeroOne() * 65534.0f;
            elem->unk1E.z = Math_Rand_ZeroOne() * 65534.0f;
        }

        this->timer = 0;
    }
}

void EffectSpark_Destroy(void* thisx) {
}

// original name: "EffectSparkInfo_proc"
s32 EffectSpark_Update(void* thisx) {
    EffectSpark* this = (EffectSpark*)thisx;
    EffectSparkElement* elem;
    s32 i;

    if (this == NULL) {
        osSyncPrintf("EffectSparkInfo_proc():Spark Pointer is NULL\n");
    }

    for (i = 0; i < this->numElements; i++) {
        elem = &this->elements[i];

        elem->pos.x += elem->velocity.x;
        elem->pos.y += elem->velocity.y;
        elem->pos.z += elem->velocity.z;
        elem->velocity.y += this->unk490;
        elem->unk1E.x += elem->unk18.x;
        elem->unk1E.y += elem->unk18.y;
        elem->unk1E.z += elem->unk18.z;
    }

    this->timer++;

    if (this->unk4C0 < this->timer) {
        return 1;
    } else {
        return 0;
    }
}

// original name: "EffectSparkInfo_disp"
void EffectSpark_Draw(void* thisx, GraphicsContext* gfxCtx) {
    Vtx* vertices;
    EffectSpark* this = (EffectSpark*)thisx;
    GlobalContext* globalCtx;
    s32 i;
    s32 j;
    u8 sp1D3;
    u8 sp1D2;
    u8 sp1D1;
    u8 sp1D0;
    u8 sp1CF;
    u8 sp1CE;
    u8 sp1CD;
    u8 sp1CC;
    u8 sp1CB;
    u8 sp1CA;
    u8 sp1C9;
    u8 sp1C8;
    u8 sp1C7;
    u8 sp1C6;
    u8 sp1C5;
    u8 sp1C4;
    f32 ratio;

    globalCtx = Effect_GetGlobalCtx();

    if (1) {}

    OPEN_DISPS(gfxCtx, "../z_eff_spark.c", 293);

    if (this != NULL) {
        gSPMatrix(oGfxCtx->polyXlu.p++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        oGfxCtx->polyXlu.p = Gfx_CallSetupDL(oGfxCtx->polyXlu.p, 0x26);
        gDPSetCycleType(oGfxCtx->polyXlu.p++, G_CYC_2CYCLE);
        gDPPipeSync(oGfxCtx->polyXlu.p++);

        gSPTexture(oGfxCtx->polyXlu.p++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gDPLoadTextureBlock(oGfxCtx->polyXlu.p++, D_04038FB0, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

        gDPSetCombineMode(oGfxCtx->polyXlu.p++, G_CC_SHADEDECALA, G_CC_PASS2);
        gDPSetRenderMode(oGfxCtx->polyXlu.p++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
        gSPClearGeometryMode(oGfxCtx->polyXlu.p++,
                             G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
        gSPSetGeometryMode(oGfxCtx->polyXlu.p++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);
        gDPPipeSync(oGfxCtx->polyXlu.p++);

        vertices = Graph_Alloc(gfxCtx, this->numElements * sizeof(Vtx[4]));
        if (vertices == NULL) {
            // Translates to: "Memory Allocation Failure graph_malloc"
            osSyncPrintf("EffectSparkInfo_disp():メモリー確保失敗 graph_malloc\n");
            goto end;
        }

        j = 0;

        ratio = (f32)this->timer / (f32)this->unk4C0;
        sp1D3 = this->unk49C[0].r + ((f32)this->unk4AC[0].r - (f32)this->unk49C[0].r) * ratio;
        sp1D2 = this->unk49C[0].g + ((f32)this->unk4AC[0].g - (f32)this->unk49C[0].g) * ratio;
        sp1D1 = this->unk49C[0].b + ((f32)this->unk4AC[0].b - (f32)this->unk49C[0].b) * ratio;
        sp1D0 = this->unk49C[0].a + ((f32)this->unk4AC[0].a - (f32)this->unk49C[0].a) * ratio;
        sp1CF = this->unk49C[1].r + ((f32)this->unk4AC[1].r - (f32)this->unk49C[1].r) * ratio;
        sp1CE = this->unk49C[1].g + ((f32)this->unk4AC[1].g - (f32)this->unk49C[1].g) * ratio;
        sp1CD = this->unk49C[1].b + ((f32)this->unk4AC[1].b - (f32)this->unk49C[1].b) * ratio;
        sp1CC = this->unk49C[1].a + ((f32)this->unk4AC[1].a - (f32)this->unk49C[1].a) * ratio;
        sp1CB = this->unk49C[2].r + ((f32)this->unk4AC[2].r - (f32)this->unk49C[2].r) * ratio;
        sp1CA = this->unk49C[2].g + ((f32)this->unk4AC[2].g - (f32)this->unk49C[2].g) * ratio;
        sp1C9 = this->unk49C[2].b + ((f32)this->unk4AC[2].b - (f32)this->unk49C[2].b) * ratio;
        sp1C8 = this->unk49C[2].a + ((f32)this->unk4AC[2].a - (f32)this->unk49C[2].a) * ratio;
        sp1C7 = this->unk49C[3].r + ((f32)this->unk4AC[3].r - (f32)this->unk49C[3].r) * ratio;
        sp1C6 = this->unk49C[3].g + ((f32)this->unk4AC[3].g - (f32)this->unk49C[3].g) * ratio;
        sp1C5 = this->unk49C[3].b + ((f32)this->unk4AC[3].b - (f32)this->unk49C[3].b) * ratio;
        sp1C4 = this->unk49C[3].a + ((f32)this->unk4AC[3].a - (f32)this->unk49C[3].a) * ratio;

        for (i = 0; i < this->numElements; i++) {
            MtxF sp12C;
            MtxF spEC;
            MtxF spAC;
            MtxF sp6C;
            EffectSparkElement* elem;
            Mtx* mtx;
            f32 temp;

            elem = &this->elements[i];

            SkinMatrix_SetTranslate(&spEC, elem->pos.x, elem->pos.y, elem->pos.z);
            temp = ((Math_Rand_ZeroOne() * 2.5f) + 1.5f) * 0.015625f;
            SkinMatrix_SetScale(&spAC, temp, temp, 1.0f);
            SkinMatrix_MtxFMtxFMult(&spEC, &globalCtx->mf_11DA0, &sp6C);
            SkinMatrix_MtxFMtxFMult(&sp6C, &spAC, &sp12C);

            vertices[j].v.ob[0] = -32;
            vertices[j].v.ob[1] = -32;
            vertices[j].v.ob[2] = 0;
            vertices[j].v.cn[0] = sp1D3;
            vertices[j].v.cn[1] = sp1D2;
            vertices[j].v.cn[2] = sp1D1;
            vertices[j].v.cn[3] = sp1D0;
            vertices[j].v.tc[0] = 0;
            vertices[j].v.tc[1] = 1024;
            vertices[j].v.flag = 0;

            vertices[j + 1].v.ob[0] = 32;
            vertices[j + 1].v.ob[1] = 32;
            vertices[j + 1].v.ob[2] = 0;
            vertices[j + 1].v.cn[0] = sp1CF;
            vertices[j + 1].v.cn[1] = sp1CE;
            vertices[j + 1].v.cn[2] = sp1CD;
            vertices[j + 1].v.cn[3] = sp1CC;
            vertices[j + 1].v.tc[0] = 1024;
            vertices[j + 1].v.tc[1] = 0;
            vertices[j + 1].v.flag = 0;

            vertices[j + 2].v.ob[0] = -32;
            vertices[j + 2].v.ob[1] = 32;
            vertices[j + 2].v.ob[2] = 0;
            vertices[j + 2].v.cn[0] = sp1CB;
            vertices[j + 2].v.cn[1] = sp1CA;
            vertices[j + 2].v.cn[2] = sp1C9;
            vertices[j + 2].v.cn[3] = sp1C8;
            vertices[j + 2].v.tc[0] = 0;
            vertices[j + 2].v.tc[1] = 0;
            vertices[j + 2].v.flag = 0;

            vertices[j + 3].v.ob[0] = 32;
            vertices[j + 3].v.ob[1] = -32;
            vertices[j + 3].v.ob[2] = 0;
            vertices[j + 3].v.cn[0] = sp1C7;
            vertices[j + 3].v.cn[1] = sp1C6;
            vertices[j + 3].v.cn[2] = sp1C5;
            vertices[j + 3].v.cn[3] = sp1C4;
            vertices[j + 3].v.tc[0] = 1024;
            vertices[j + 3].v.tc[1] = 1024;
            vertices[j + 3].v.flag = 0;

            j += 4;

            mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &sp12C);
            if (mtx == NULL) {
                goto end;
            }

            gSPMatrix(oGfxCtx->polyXlu.p++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPVertex(oGfxCtx->polyXlu.p++, &vertices[4 * i], 4, 0);
            gSP2Triangles(oGfxCtx->polyXlu.p++, 2, 0, 3, 0, 2, 3, 1, 0);
        }

        gDPPipeSync(oGfxCtx->polyXlu.p++);
    }

end:
    CLOSE_DISPS(gfxCtx, "../z_eff_spark.c", 498);
}
