#include <ultra64.h>
#include <global.h>
#include <vt.h>

static Vtx sVertices[5] = {
    VTX(-32, -32, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(32, 32, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(-32, 32, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    VTX(32, -32, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0xFF),
};

// original name: "EffectShieldParticle_ct"
void EffectShieldParticle_Init(void* thisx, void* initParamsx) {
    EffectShieldParticle* this = (EffectShieldParticle*)thisx;
    EffectShieldParticleInit* initParams = (EffectShieldParticleInit*)initParamsx;
    EffectShieldParticleElement* elem;

    if ((this != NULL) && (initParams != NULL)) {
        this->numElements = initParams->numElements;
        if (this->numElements > ARRAY_COUNT(this->elements)) {
            osSyncPrintf(VT_FGCOL(RED));
            osSyncPrintf("EffectShieldParticle_ct():パーティクル数がオーバしてます。\n");
            osSyncPrintf(VT_RST);
            return;
        }

        this->pos = initParams->pos;
        this->primColor0 = initParams->primColor0;
        this->envColor0 = initParams->envColor0;
        this->primColor1 = initParams->primColor1;
        this->envColor1 = initParams->envColor1;
        this->primColor2 = initParams->primColor2;
        this->envColor2 = initParams->envColor2;
        this->deceleration = initParams->deceleration;
        this->unk1A8 = initParams->unk24;
        this->unk1AC = initParams->unk28;
        this->unk1B0 = initParams->unk2C;
        this->unk1B1 = 0;

        for (elem = &this->elements[0]; elem < &this->elements[this->numElements]; elem++) {
            elem->unk00 = (Math_Rand_ZeroOne() * (this->unk1A8 * 0.5f)) + (this->unk1A8 * 0.5f);
            elem->endX = 0.0f;
            elem->startXChange = 0.0f;
            elem->startX = 0.0f;
            elem->endXChange = elem->unk00;
            elem->yaw = Math_Rand_ZeroOne() * 65534.0f;
            elem->pitch = Math_Rand_ZeroOne() * 65534.0f;
        }

        this->lightDecay = initParams->lightDecay;
        if (this->lightDecay == true) {
            this->lightInfo.type = LIGHT_POINT_NOGLOW;
            this->lightInfo.params.point = initParams->lightPoint;
            this->lightNode =
                LightContext_InsertLight(Effect_GetGlobalCtx(), &Effect_GetGlobalCtx()->lightCtx, &this->lightInfo);
        } else {
            this->lightNode = NULL;
        }
    }
}

void EffectShieldParticle_Destroy(void* thisx) {
    EffectShieldParticle* this = (EffectShieldParticle*)thisx;

    if ((this != NULL) && (this->lightDecay == true)) {
        if (this->lightNode == Effect_GetGlobalCtx()->lightCtx.listHead) {
            Effect_GetGlobalCtx()->lightCtx.listHead = this->lightNode->next;
        }
        LightContext_RemoveLight(Effect_GetGlobalCtx(), &Effect_GetGlobalCtx()->lightCtx, this->lightNode);
    }
}

s32 EffectShieldParticle_Update(void* thisx) {
    EffectShieldParticle* this = (EffectShieldParticle*)thisx;
    EffectShieldParticleElement* elem;

    if (this == NULL) {
        return 0;
    }

    for (elem = &this->elements[0]; elem < &this->elements[this->numElements]; elem++) {
        elem->endXChange -= this->deceleration;
        if (elem->endXChange < 0.0f) {
            elem->endXChange = 0.0f;
        }

        if (elem->startXChange > 0.0f) {
            elem->startXChange -= this->deceleration;
            if (elem->startXChange < 0.0f) {
                elem->startXChange = 0.0f;
            }
        }

        elem->endX += elem->endXChange;
        elem->startX += elem->startXChange;

        if ((elem->startXChange == 0.0f) && (this->unk1AC < (elem->endX - elem->startX))) {
            elem->startXChange = elem->unk00;
        }
    }

    if (this->lightDecay == true) {
        this->lightInfo.params.point.radius /= 2;
    }

    this->unk1B1++;

    if (this->unk1B0 < this->unk1B1) {
        return 1;
    }

    return 0;
}

void EffectShieldParticle_GetColors(EffectShieldParticle* this, Color_RGBA8* primColor, Color_RGBA8* envColor) {
    s32 halfDuration;
    f32 ratio;

    halfDuration = this->unk1B0 * 0.5f;
    if (halfDuration == 0) {
        primColor->r = this->primColor0.r;
        primColor->g = this->primColor0.g;
        primColor->b = this->primColor0.b;
        primColor->a = this->primColor0.a;
        envColor->r = this->envColor0.r;
        envColor->g = this->envColor0.g;
        envColor->b = this->envColor0.b;
        envColor->a = this->envColor0.a;
    } else if (this->unk1B1 < halfDuration) {
        ratio = this->unk1B1 / (f32)halfDuration;
        primColor->r = this->primColor0.r + (this->primColor1.r - this->primColor0.r) * ratio;
        primColor->g = this->primColor0.g + (this->primColor1.g - this->primColor0.g) * ratio;
        primColor->b = this->primColor0.b + (this->primColor1.b - this->primColor0.b) * ratio;
        primColor->a = this->primColor0.a + (this->primColor1.a - this->primColor0.a) * ratio;
        envColor->r = this->envColor0.r + (this->envColor1.r - this->envColor0.r) * ratio;
        envColor->g = this->envColor0.g + (this->envColor1.g - this->envColor0.g) * ratio;
        envColor->b = this->envColor0.b + (this->envColor1.b - this->envColor0.b) * ratio;
        envColor->a = this->envColor0.a + (this->envColor1.a - this->envColor0.a) * ratio;
    } else {
        ratio = (this->unk1B1 - halfDuration) / (f32)halfDuration;
        primColor->r = this->primColor1.r + (this->primColor2.r - this->primColor1.r) * ratio;
        primColor->g = this->primColor1.g + (this->primColor2.g - this->primColor1.g) * ratio;
        primColor->b = this->primColor1.b + (this->primColor2.b - this->primColor1.b) * ratio;
        primColor->a = this->primColor1.a + (this->primColor2.a - this->primColor1.a) * ratio;
        envColor->r = this->envColor1.r + (this->envColor2.r - this->envColor1.r) * ratio;
        envColor->g = this->envColor1.g + (this->envColor2.g - this->envColor1.g) * ratio;
        envColor->b = this->envColor1.b + (this->envColor2.b - this->envColor1.b) * ratio;
        envColor->a = this->envColor1.a + (this->envColor2.a - this->envColor1.a) * ratio;
    }
}

void EffectShieldParticle_Draw(void* thisx, GraphicsContext* gfxCtx) {
    EffectShieldParticle* this = (EffectShieldParticle*)thisx;
    EffectShieldParticleElement* elem;
    Color_RGBA8 primColor;
    Color_RGBA8 envColor;

    OPEN_DISPS(gfxCtx, "../z_eff_shield_particle.c", 272);

    if (this != NULL) {
        oGfxCtx->polyXlu.p = Gfx_CallSetupDL(oGfxCtx->polyXlu.p, 0x26);

        gDPSetCycleType(oGfxCtx->polyXlu.p++, G_CYC_2CYCLE);
        gDPPipeSync(oGfxCtx->polyXlu.p++);
        gSPTexture(oGfxCtx->polyXlu.p++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

        gDPLoadTextureBlock(oGfxCtx->polyXlu.p++, D_04038FB0, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

        if (1) {} // Necessary to match

        gDPSetCombineLERP(oGfxCtx->polyXlu.p++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0, 0,
                          0, 0, COMBINED, 0, 0, 0, COMBINED);
        gDPSetRenderMode(oGfxCtx->polyXlu.p++, G_RM_PASS, G_RM_ZB_CLD_SURF2);
        gSPClearGeometryMode(oGfxCtx->polyXlu.p++,
                             G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
        gSPSetGeometryMode(oGfxCtx->polyXlu.p++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH);

        EffectShieldParticle_GetColors(this, &primColor, &envColor);

        gDPSetPrimColor(oGfxCtx->polyXlu.p++, 0, 0, primColor.r, primColor.g, primColor.b, primColor.a);
        gDPSetEnvColor(oGfxCtx->polyXlu.p++, envColor.r, envColor.g, envColor.b, envColor.a);
        gDPPipeSync(oGfxCtx->polyXlu.p++);

        for (elem = &this->elements[0]; elem < &this->elements[this->numElements]; elem++) {
            Mtx* mtx;
            MtxF sp104;
            MtxF spC4;
            MtxF sp84;
            f32 temp1;
            f32 temp2;
            f32 temp3;

            temp1 = (s16)((elem->endX + elem->startX) * 0.5f);

            temp2 = elem->endX - elem->startX;
            temp3 = (s16)((temp2 * (1.0f / 64.0f)) / 0.02f);
            if (temp3 < 1.0f) {
                temp3 = 1.0f;
            }

            SkinMatrix_SetTranslate(&spC4, this->pos.x, this->pos.y, this->pos.z);
            SkinMatrix_SetRotateRPY(&sp104, 0, elem->yaw, 0);
            SkinMatrix_MtxFMtxFMult(&spC4, &sp104, &sp84);
            SkinMatrix_SetRotateRPY(&sp104, 0, 0, elem->pitch);
            SkinMatrix_MtxFMtxFMult(&sp84, &sp104, &spC4);
            SkinMatrix_SetTranslate(&sp104, temp1, 0.0f, 0.0f);
            SkinMatrix_MtxFMtxFMult(&spC4, &sp104, &sp84);
            SkinMatrix_SetScale(&sp104, temp3 * 0.02f, 0.02f, 0.02f);
            SkinMatrix_MtxFMtxFMult(&sp84, &sp104, &spC4);

            mtx = SkinMatrix_MtxFToNewMtx(gfxCtx, &spC4);
            if (mtx == NULL) {
                break;
            }

            gSPMatrix(oGfxCtx->polyXlu.p++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPVertex(oGfxCtx->polyXlu.p++, sVertices, 4, 0);
            gSP2Triangles(oGfxCtx->polyXlu.p++, 0, 1, 2, 0, 0, 3, 1, 0);
        }
    }

    CLOSE_DISPS(gfxCtx, "../z_eff_shield_particle.c", 359);
}
