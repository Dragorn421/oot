#include "hammertree1.h"
#include "build/mods/hammertree1.h"

#define FLAGS 0

#define SCALE 0.1f

void HammerTree1_Init(Actor* thisx, GlobalContext* globalCtx);
void HammerTree1_Destroy(Actor* thisx, GlobalContext* globalCtx);
void HammerTree1_Update(Actor* thisx, GlobalContext* globalCtx);
void HammerTree1_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit HammerTree1_InitVars = {
    ACTOR_HAMMERTREE1,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_HAMMERTREE1,
    sizeof(HammerTree1),
    (ActorFunc)HammerTree1_Init,
    (ActorFunc)HammerTree1_Destroy,
    (ActorFunc)HammerTree1_Update,
    (ActorFunc)HammerTree1_Draw,
};

#define V0 \
    { 2.7208f, 0.0f, -2.2314f }
#define V1 \
    { -2.7208f, 0.0f, -2.2314f }
#define V2 \
    { -0.83108f, 77.883f, 0.074209f }
#define V3 \
    { 0.83108f, 77.883f, 0.074209f }

#define TRI(v0, v1, v2)                 \
    {                                   \
        {                               \
            ELEMTYPE_UNK0,              \
            { 0x00000000, 0x00, 0x00 }, \
            { 0x40000040, 0x00, 0x00 }, \
            TOUCH_NONE,                 \
            BUMP_ON,                    \
            OCELEM_ON,                  \
        },                              \
        { { v0, v1, v2 } },             \
    },

static ColliderTrisElementInit sTrunkTrisElemInit[] = { TRI(V0, V1, V2) TRI(V2, V3, V0) };

static ColliderTrisInit sTrunkTrisInit = {
    {
        COLTYPE_TREE,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_TRIS,
    },
    ARRAY_COUNT(sTrunkTrisElemInit),
    sTrunkTrisElemInit,
};

void HammerTree1_InitDynapoly(HammerTree1* this, GlobalContext* globalCtx) {
    s32 pad;
    s32 pad2;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(COLL_TRUNK, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &this->dyna.actor, colHeader);

    if (this->dyna.bgId == BG_ACTOR_MAX) {
        osSyncPrintf("Warning: could not initialize HammerTree1 dynapoly (params 0x%04x)\n", this->dyna.actor.params);
    }
}

void HammerTree1_InitTrisCollider(HammerTree1* this, GlobalContext* globalCtx, ColliderTrisInit* colliderTrisInit) {
    ColliderTris* colliderTris = &this->colTris;

    Collider_InitTris(globalCtx, colliderTris);
    Collider_SetTris(globalCtx, colliderTris, &this->dyna.actor, colliderTrisInit, this->colTrisItems);
}

void HammerTree1_UpdateTrisCollider(HammerTree1* this, GlobalContext* globalCtx, ColliderTrisInit* colliderTrisInit,
                                    MtxF* mf) {
    ColliderTris* colliderTris = &this->colTris;
    s32 i;
    s32 j;
    Vec3f pos[3];
    f32 w;

    for (i = 0; i < colliderTrisInit->count; i++) {
        for (j = 0; j < 3; j++) {
            SkinMatrix_Vec3fMtxFMultXYZW(mf, &colliderTrisInit->elements[i].dim.vtx[j], &pos[j], &w);
            Math_Vec3f_Scale(&pos[j], 100 * SCALE / w);
            Math_Vec3f_Sum(&pos[j], &this->dyna.actor.world.pos, &pos[j]);
        }

        Collider_SetTrisVertices(colliderTris, i, &pos[0], &pos[1], &pos[2]);
    }
}

void HammerTree1_Init(Actor* thisx, GlobalContext* globalCtx) {
    HammerTree1* this = (HammerTree1*)thisx;

    Actor_SetScale(&this->dyna.actor, SCALE);

    HammerTree1_InitDynapoly(this, globalCtx);

    HammerTree1_InitTrisCollider(this, globalCtx, &sTrunkTrisInit);
}

void HammerTree1_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    HammerTree1* this = (HammerTree1*)thisx;

    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, this->dyna.bgId);

    Collider_DestroyTris(globalCtx, &this->colTris);
}

void HammerTree1_Update(Actor* thisx, GlobalContext* globalCtx) {
    HammerTree1* this = (HammerTree1*)thisx;
    Player* player = GET_PLAYER(globalCtx);
    MtxF mf;
    s32 i;
    f32 yawTowardsPlayerModel;
    Vec3f upWorld = { 0, 1, 0 };
    Vec3f treeUpModel = { 0, 1, 0 };
    Vec3f treeUpWorld;
    f32 cosUpToTreeUpAngle;

    Matrix_Push();

    /*
    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
    {
        GfxPrint printer;
        Gfx* gfx;

        gfx = POLY_OPA_DISP + 1;
        gSPDisplayList(OVERLAY_DISP++, gfx);

        GfxPrint_Init(&printer);
        GfxPrint_Open(&printer, gfx);

        GfxPrint_SetColor(&printer, 255, 0, 0, 255);
        GfxPrint_SetPos(&printer, 0, 20);
        GfxPrint_Printf(&printer, " HammerTree1\n");
        GfxPrint_Printf(&printer, " acFlags = 0x%02X\n", this->colTris.base.acFlags);

        gfx = GfxPrint_Close(&printer);
        GfxPrint_Destroy(&printer);

        gSPEndDisplayList(gfx++);
        gSPBranchList(POLY_OPA_DISP, gfx);
        POLY_OPA_DISP = gfx;
    }
    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
    */

    if (this->colTris.base.acFlags & AC_BOUNCED) {
        for (i = 0; i < this->colTris.count; i++) {
            ColliderTrisElement elem = this->colTris.elements[i];

            if (elem.info.acHitInfo != NULL && elem.info.acHitInfo->toucher.dmgFlags & 0x40) {
                s16 dirRotY = player->actor.shape.rot.y;
                Vec3f axis;

                axis.x = Math_CosS(dirRotY);
                axis.y = 0.0f;
                axis.z = -Math_SinS(dirRotY);

                Matrix_SetTranslateRotateYXZ(0, 0, 0, &this->dyna.actor.world.rot);

                Matrix_MultVec3f(&treeUpModel, &treeUpWorld);
                Math3D_CosOut(&treeUpWorld, &upWorld, &cosUpToTreeUpAngle);

                Matrix_RotateAxis(CLAMP((1 - cosUpToTreeUpAngle) * M_PI / 3 + M_PI / 17, M_PI / 20, M_PI / 10), &axis,
                                  MTXMODE_APPLY);

                Matrix_Get(&mf);

                Matrix_MtxFToYXZRotS(&mf, &this->dyna.actor.world.rot, 0);

                break;
            }
        }
    }

    Matrix_SetTranslateRotateYXZ(0, 0, 0, &this->dyna.actor.world.rot);
    Matrix_MultVec3f(&treeUpModel, &treeUpWorld);
    Math3D_CosOut(&treeUpWorld, &upWorld, &cosUpToTreeUpAngle);
    if (cosUpToTreeUpAngle <= 0.0f) {
        if (cosUpToTreeUpAngle < 0.0f) {
            f32 upToTreeUpAngle = Math_FAcosF(cosUpToTreeUpAngle);
            Vec3f axis;
            f32 axisNorm;

            Math3D_Vec3f_Cross(&upWorld, &treeUpWorld, &axis);
            axisNorm = sqrt(SQXYZ(axis));
            if (axisNorm < 0.001f) {
                axis.x = 1;
                axis.y = 0;
                axis.z = 0;
                Matrix_RotateAxis(M_PI / 2, &axis, MTXMODE_APPLY);
            } else {
                Math_Vec3f_Scale(&axis, 1.0f / axisNorm);

                Matrix_RotateAxis(M_PI / 2 - upToTreeUpAngle, &axis, MTXMODE_APPLY);
            }

            Matrix_Get(&mf);

            Matrix_MtxFToYXZRotS(&mf, &this->dyna.actor.world.rot, 0);
        }
    } else if (cosUpToTreeUpAngle < 0.8f) {
        Vec3f axis;

        Math3D_Vec3f_Cross(&upWorld, &treeUpWorld, &axis);
        // 0 < cos < 0.8 guarantees the norm is not zero
        Math_Vec3f_Scale(&axis, 1.0f / sqrt(SQXYZ(axis)));

        Matrix_RotateAxis(this->dyna.actor.speedXZ, &axis, MTXMODE_APPLY);

        this->dyna.actor.speedXZ = CLAMP_MAX(this->dyna.actor.speedXZ + M_PI / 100, M_PI / 20);

        Matrix_Get(&mf);

        Matrix_MtxFToYXZRotS(&mf, &this->dyna.actor.world.rot, 0);
    } else {
        this->dyna.actor.speedXZ = 0;
    }

    {
        MtxF modelToWorld;
        MtxF worldToModel;
        Vec3f treeToPlayerWorld;
        Vec3f treeToPlayerModel;

        Matrix_SetTranslateRotateYXZ(0, 0, 0, &this->dyna.actor.world.rot);
        Matrix_Get(&modelToWorld);

        SkinMatrix_Invert(&modelToWorld, &worldToModel);

        Math_Vec3f_Diff(&player->actor.world.pos, &this->dyna.actor.world.pos, &treeToPlayerWorld);
        treeToPlayerWorld.y += 50.0f;

        Matrix_MultVec3fExt(&treeToPlayerWorld, &treeToPlayerModel, &worldToModel);

        yawTowardsPlayerModel = Math_Atan2F(treeToPlayerModel.z, treeToPlayerModel.x);
    }

    Matrix_SetTranslateRotateYXZ(0, 0, 0, &this->dyna.actor.world.rot);
    Matrix_RotateY(yawTowardsPlayerModel + M_PI, MTXMODE_APPLY);

    Matrix_Get(&mf);

    // shape.rot is used for the dynapoly rotation
    Matrix_MtxFToYXZRotS(&mf, &this->dyna.actor.shape.rot, 0);

    HammerTree1_UpdateTrisCollider(this, globalCtx, &sTrunkTrisInit, &mf);

    CollisionCheck_SetAC(globalCtx, &globalCtx->colChkCtx, &this->colTris.base);
    CollisionCheck_SetOC(globalCtx, &globalCtx->colChkCtx, &this->colTris.base);

    Matrix_Pop();
}

void HammerTree1_Draw(Actor* thisx, GlobalContext* globalCtx) {
    HammerTree1* this = (HammerTree1*)thisx;
    MtxF* mf;
    Mtx* m;

    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);

    Matrix_SetTranslateRotateYXZ(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                                 this->dyna.actor.world.pos.z, &this->dyna.actor.world.rot);
    Matrix_Scale(SCALE, SCALE, SCALE, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, __FILE__, __LINE__),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, DL_TRUNK);
    gSPDisplayList(POLY_OPA_DISP++, DL_LEAVES1);
    gSPDisplayList(POLY_OPA_DISP++, DL_LEAVES2);
    gSPDisplayList(POLY_OPA_DISP++, DL_LEAVES3);
    gSPDisplayList(POLY_OPA_DISP++, DL_LEAVES4);
    gSPDisplayList(POLY_OPA_DISP++, DL_LEAVES5);

   /*
    Collider_Draw(globalCtx, &this->colTris.base);

    {
        GfxPrint printer;
        Gfx* gfx;
        s32 i;
        Vec3f p;

        gfx = POLY_OPA_DISP + 1;
        gSPDisplayList(OVERLAY_DISP++, gfx);

        GfxPrint_Init(&printer);
        GfxPrint_Open(&printer, gfx);

        GfxPrint_SetColor(&printer, 255, 0, 255, 255);
        GfxPrint_SetPos(&printer, 0, 10);
        GfxPrint_Printf(&printer, " HammerTree1\n");
        p = GET_PLAYER(globalCtx)->actor.world.pos;
        GfxPrint_Printf(&printer, " %f %f %f\n", p.x, p.y, p.z);
        for (i = 0; i < 6; i++) {
            s16* v = sDebugTrisVerts[i].ob;

            GfxPrint_Printf(&printer, " %i %hi %hi %hi\n", i, v[0], v[1], v[2]);
        }

        gfx = GfxPrint_Close(&printer);
        GfxPrint_Destroy(&printer);

        gSPEndDisplayList(gfx++);
        gSPBranchList(POLY_OPA_DISP, gfx);
        POLY_OPA_DISP = gfx;
    }
    */
    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
}
