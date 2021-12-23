#include "actor.h"
#include "../object/cube.h"

// always draw and update
#define FLAGS 0x00000030

#define THIS ((ModActor*)thisx)

void ModActor_Init(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Update(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Draw(Actor* thisx, GlobalContext* globalCtx);

void ModActor_VecSub(Vec3f* a, Vec3f* b, Vec3f* dest);
void ModActor_VecMul(Vec3f* vec, f32 fac, Vec3f* dest);
void ModActor_VecNormalize(Vec3f* vec, Vec3f* dest);
f32 ModActor_VecDot(Vec3f* a, Vec3f* b);
void ModActor_Project(Vec3f* vec, Vec3f* along, Vec3f* dest);

const ActorInit mod_actor_InitVars = {
    ACTOR_MOD_ACTOR,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_MOD_CUBE,
    sizeof(ModActor),
    (ActorFunc)ModActor_Init,
    (ActorFunc)ModActor_Destroy,
    (ActorFunc)ModActor_Update,
    (ActorFunc)ModActor_Draw,
};

#define SCALE 10.0f
#define ROT_SPEED_FACTOR (SCALE / 100.0f)

s32 DEBUGON = 1;
Vec3f SOL;
char* SOLSTR;
u32 passedEdgesMaskLast;
MtxF systemLast;
MtxF systemInvLast;
Vec3f vec0toPosLast;

LinkedFace faces[] = {
#include "../object/cube_linked_faces.c"
};

void ModActor_Init(Actor* thisx, GlobalContext* globalCtx) {
    ModActor* this = THIS;
    Vec3f dest;
    Vec3f up = { 0.0f, 1.0f, 0.0f };
    Vec3f axis;

    R_MINIMAP_DISABLED = true;
    // leave all transforms up to the actor
    Actor_SetScale(thisx, 1.0f);
    thisx->world.pos.x = 0.0f;
    thisx->world.pos.y = 0.0f;
    thisx->world.pos.z = 0.0f;
    thisx->shape.rot.x = 0;
    thisx->shape.rot.y = 0;
    thisx->shape.rot.z = 0;
    this->curFace = &faces[30];
    // start at center of curFace
    Math_Vec3f_Sum(&this->curFace->verts[0], &this->curFace->verts[1], &dest);
    Math_Vec3f_Sum(&this->curFace->verts[2], &dest, &dest);
    ModActor_VecMul(&dest, 1.0f / 3.0f, &dest);
    this->positionModel = dest;
    // init rotating members
    this->rotating = false;
    this->rotatingTo = NULL;
    VEC_SET(this->rotatingAxis, 0.0f, 0.0f, 0.0f);
    this->rotatingAngle = 0.0f;
    this->rotatingAngleTarget = 0.0f;
    // init transform as a rotation matrix so that the curFace normal points up in world space
    SkinMatrix_Clear(&this->transform);
    Matrix_Push();
    Matrix_Put(&this->transform);
    Matrix_Translate(this->positionModel.x, this->positionModel.y, this->positionModel.z, MTXMODE_APPLY);
    Math3D_Vec3f_Cross(&this->curFace->norm, &up, &axis);
    if (!IS_ZERO(Math3D_Vec3fMagnitude(&axis))) {
        ModActor_VecNormalize(&axis, &axis);
        Matrix_RotateAxis(Math_FAcosF(Math3D_Cos(&up, &this->curFace->norm)), &axis, MTXMODE_APPLY);
    } else if (this->curFace->norm.y < 0.0f) {
        // normal is -y
        Matrix_RotateX(M_PI, MTXMODE_APPLY);
    } else {
        // normal is +y, no need to rotate
    }
    Matrix_Translate(-this->positionModel.x, -this->positionModel.y, -this->positionModel.z, MTXMODE_APPLY);
    Matrix_Get(&this->transform);
    Matrix_CheckFloats(&this->transform, __FILE__, __LINE__);
    Matrix_Pop();
    VEC_SET(GET_PLAYER(globalCtx)->actor.world.pos, 0.0f, 0.0f, 0.0f);
}

void ModActor_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

/* dest = a - b*/
void ModActor_VecSub(Vec3f* a, Vec3f* b, Vec3f* dest) {
    dest->x = a->x - b->x;
    dest->y = a->y - b->y;
    dest->z = a->z - b->z;
}

/* dest = fac * vec */
void ModActor_VecMul(Vec3f* vec, f32 fac, Vec3f* dest) {
    dest->x = fac * vec->x;
    dest->y = fac * vec->y;
    dest->z = fac * vec->z;
}

void ModActor_VecNormalize(Vec3f* vec, Vec3f* dest) {
    f32 magnitude = Math3D_Vec3fMagnitude(vec);
    if (IS_ZERO(magnitude)) {
        osSyncPrintf("Not normalizing vector with tiny magnitude\n");
        Fault_AddHungupAndCrash(__FILE__, __LINE__);
    }
    ModActor_VecMul(vec, 1.0f / magnitude, dest);
}

/* a.b */
f32 ModActor_VecDot(Vec3f* a, Vec3f* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* project vec onto the unit vector along */
void ModActor_Project(Vec3f* vec, Vec3f* along, Vec3f* dest) {
    ModActor_VecMul(along, ModActor_VecDot(vec, along), dest);
}

s32 ModActor_GetPassedEdges(LinkedFace* face, Vec3f* pos) {
    Vec3f vec0toPos;
    Vec3f vec0to1;
    Vec3f vec0to2;
    MtxF system;
    MtxF systemInv;
    Vec3f solution;
    u32 passedEdgesMask = 0;

    ModActor_VecSub(pos, &face->verts[0], &vec0toPos);
    ModActor_VecSub(&face->verts[1], &face->verts[0], &vec0to1);
    ModActor_VecSub(&face->verts[2], &face->verts[0], &vec0to2);

    /*
    find a, b, c such that a * vec0to1 + b * vec0to2 + c * norm = vec0toPos
    aka system * (a, b, c) = vec0toPos
    with system = [
        vec0to1.x, vec0to2.x, norm.x
        vec0to1.y, vec0to2.y, norm.y
        vec0to1.z, vec0to2.z, norm.z
    ]
    */
    system.xx = vec0to1.x;
    system.yx = vec0to1.y;
    system.zx = vec0to1.z;
    system.xy = vec0to2.x;
    system.yy = vec0to2.y;
    system.zy = vec0to2.z;
    system.xz = face->norm.x;
    system.yz = face->norm.y;
    system.zz = face->norm.z;
    system.wx = system.wy = system.wz = system.xw = system.yw = system.zw = 0.0f;
    system.ww = 1.0f;
    if (SkinMatrix_Invert(&system, &systemInv)) {
        osSyncPrintf("Couldn't invert system\n");
        // failed to invert
        Fault_AddHungupAndCrash(__FILE__, __LINE__);
    }
    SkinMatrix_Vec3fMtxFMultXYZ(&systemInv, &vec0toPos, &solution);
    SOL = solution;
    if (solution.x < 0.0f && solution.x < solution.y && (solution.x + solution.y) < (1.0f - solution.x)) {
        SOLSTR = "x < 0 e2";
        passedEdgesMask |= 2 << 8;
    } else if (solution.y < 0.0f && (solution.x + solution.y) < (1.0f - solution.y)) {
        SOLSTR = "y < 0 e0";
        passedEdgesMask |= 0 << 8;
    } else if (solution.x + solution.y > 1.0f) {
        SOLSTR = "x + y > 1 e1";
        passedEdgesMask |= 1 << 8;
    } else {
        SOLSTR = "in tri";
    }

    if (solution.x < 0.0f) {
        passedEdgesMask |= 1 << 2;
    }
    if (solution.y < 0.0f) {
        passedEdgesMask |= 1 << 0;
    }
    if (solution.x + solution.y > 1.0f) {
        passedEdgesMask |= 1 << 1;
    }

    passedEdgesMaskLast = passedEdgesMask;
    systemLast = system;
    systemInvLast = systemInv;
    vec0toPosLast = vec0toPos;
    return passedEdgesMask;
}

void ModActor_Update(Actor* thisx, GlobalContext* globalCtx) {
    ModActor* this = THIS;
    Player* player = GET_PLAYER(globalCtx);
    s32 i;
    LinkedFace* face;
    Vec3f playerWorld = player->actor.world.pos;
    Vec3f zero = { 0.0f, 0.0f, 0.0f };
    MtxF invertTransform;
    Vec3f playerModel;
    Vec3f zeroModel;
    Vec3f deltaModel;
    Vec3f newPositionModel;
    s32 passedEdgesMask;
    Vec3f deltaModelFinal;

    // toggle debug display
    if (globalCtx->state.input[0].press.button & BTN_DDOWN) {
        DEBUGON = !DEBUGON;
    }

    VEC_SET(player->actor.world.pos, 0.0f, 0.0f, 0.0f);

    ModActor_VecMul(&playerWorld, 1.0f / SCALE, &playerWorld);

    SkinMatrix_Invert(&this->transform, &invertTransform);
    SkinMatrix_Vec3fMtxFMultXYZ(&invertTransform, &playerWorld, &playerModel);
    SkinMatrix_Vec3fMtxFMultXYZ(&invertTransform, &zero, &zeroModel);
    ModActor_VecSub(&playerModel, &zeroModel, &deltaModel);

    Math_Vec3f_Sum(&this->positionModel, &deltaModel, &newPositionModel);
    // todo ? project positionModel onto curFace
    passedEdgesMask = ModActor_GetPassedEdges(this->curFace, &newPositionModel);

    if (passedEdgesMask == 0) {
        // inside curFace, nothing to do
    } else {
        // on the edge of curFace
        for (i = 0; i < 3; i++) {
            if (passedEdgesMask & (1 << i)) {
                Vec3f vA = this->curFace->verts[i];
                Vec3f vB = this->curFace->verts[(i + 1) % 3];
                InfiniteLine line;

                // clip position to edge i
                ModActor_VecSub(&vA, &vB, &line.dir);
                line.point = vA;
                Math3D_LineClosestToPoint(&line, &newPositionModel, &newPositionModel);
            }
        }
    }
    ModActor_VecSub(&newPositionModel, &this->positionModel, &deltaModelFinal);
    if (passedEdgesMask != 0) {
        Vec3f discardedDelta;

        i = passedEdgesMask >> 8 & 0xFF;
        if (!this->rotating && this->curFace->neighbours[i] != -1) {
            // start rotation around edge i
            this->rotating = true;
            this->rotatingTo = &faces[this->curFace->neighbours[i]];
            Math3D_Vec3f_Cross(&this->rotatingTo->norm, &this->curFace->norm, &this->rotatingAxis);
            if (IS_ZERO(Math3D_Vec3fMagnitude(&this->rotatingAxis))) {
                // fixme pick better, not that it matters
                // or set rotatingAngleTarget to 0 here idk
                this->rotatingAxis = this->curFace->norm;
            } else {
                ModActor_VecNormalize(&this->rotatingAxis, &this->rotatingAxis);
            }
            this->rotatingAngle = 0.0f;
            this->rotatingAngleTarget = Math_FAcosF(Math3D_Cos(&this->rotatingTo->norm, &this->curFace->norm));
        }
        // todo discardedDelta should only be the delta discarded due to the i edge
        // but other edges could also discard part of deltaModel (if in tri corner)
        ModActor_VecSub(&deltaModel, &deltaModelFinal, &discardedDelta);
        if (this->rotating) {
            // fixme allow going back, currently once rotating is started it is
            // expected to keep going until the next face takes over as curFace
            this->rotatingAngle += Math3D_Vec3fMagnitude(&discardedDelta) * ROT_SPEED_FACTOR;
            if (this->rotatingAngle >= this->rotatingAngleTarget) {
                this->curFace = this->rotatingTo;
                Matrix_Push();
                Matrix_Put(&this->transform);
                Matrix_RotateAxis(this->rotatingAngleTarget, &this->rotatingAxis, MTXMODE_APPLY);
                Matrix_Get(&this->transform);
                Matrix_Pop();
                this->rotating = false;
                this->rotatingTo = NULL;
                VEC_SET(this->rotatingAxis, 0.0f, 0.0f, 0.0f);
                this->rotatingAngle = 0.0f;
                this->rotatingAngleTarget = 0.0f;
            }
        }
    }
    this->positionModel = newPositionModel;
}

void ModActor_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ModActor* this = THIS;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;
    Vec3f positionWorld;

    // scale
    Matrix_Scale(SCALE, SCALE, SCALE, MTXMODE_APPLY);
    // position
    Matrix_Push();
    Matrix_Put(&this->transform);
    if (this->rotating) {
        Matrix_RotateAxis(this->rotatingAngle, &this->rotatingAxis, MTXMODE_APPLY);
    }
    Matrix_MultVec3f(&this->positionModel, &positionWorld);
    Matrix_Pop();
    Matrix_Translate(-positionWorld.x, -positionWorld.y, -positionWorld.z, MTXMODE_APPLY);
    // rotations
    Matrix_Mult(&this->transform, MTXMODE_APPLY);
    if (this->rotating) {
        Matrix_RotateAxis(this->rotatingAngle, &this->rotatingAxis, MTXMODE_APPLY);
    }

    gSPMatrix(gfxCtx->polyOpa.p++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(gfxCtx->polyOpa.p++, DL_CUBE);

    if (DEBUGON) {
        GfxPrint printer;
        Gfx* gfx = globalCtx->state.gfxCtx->polyOpa.p + 1;
        s32 y = 5;

        gSPDisplayList(globalCtx->state.gfxCtx->overlay.p++, gfx);

        GfxPrint_Init(&printer);
        GfxPrint_Open(&printer, gfx);

        GfxPrint_SetColor(&printer, 255, 255, 255, 255);
        GfxPrint_SetPos(&printer, 10, y + 1);
        GfxPrint_Printf(&printer, "On face %d", ((u32)this->curFace - (u32)faces) / sizeof(LinkedFace));
        GfxPrint_SetPos(&printer, 1, y + 2);
        GfxPrint_Printf(&printer, "v0 = %f %f %f", this->curFace->verts[0].x, this->curFace->verts[0].y,
                        this->curFace->verts[0].z);
        GfxPrint_SetPos(&printer, 1, y + 3);
        GfxPrint_Printf(&printer, "v1 = %f %f %f", this->curFace->verts[1].x, this->curFace->verts[1].y,
                        this->curFace->verts[1].z);
        GfxPrint_SetPos(&printer, 1, y + 4);
        GfxPrint_Printf(&printer, "v2 = %f %f %f", this->curFace->verts[2].x, this->curFace->verts[2].y,
                        this->curFace->verts[2].z);
        GfxPrint_SetPos(&printer, 1, y + 5);
        GfxPrint_Printf(&printer, "norm = %f %f %f", this->curFace->norm.x, this->curFace->norm.y,
                        this->curFace->norm.z);
        GfxPrint_SetPos(&printer, 1, y + 7);
        GfxPrint_Printf(&printer, "pos (model) = %f %f %f", this->positionModel.x, this->positionModel.y,
                        this->positionModel.z);
        GfxPrint_SetPos(&printer, 1, y + 8);
        GfxPrint_Printf(&printer, "pos (world) = %f %f %f", positionWorld.x, positionWorld.y, positionWorld.z);
        GfxPrint_SetPos(&printer, 1, y + 9);
        GfxPrint_Printf(&printer, "SOL = %f %f %f", SOL.x, SOL.y, SOL.z);
        GfxPrint_SetPos(&printer, 1, y + 10);
        if (SOLSTR != NULL) {
            GfxPrint_Printf(&printer, "%s", SOLSTR);
            SOLSTR = NULL;
        }
        GfxPrint_SetPos(&printer, 1, y + 11);
        GfxPrint_Printf(&printer, "passedEdgesMaskLast = %08X", passedEdgesMaskLast);
        if (this->rotating) {
            GfxPrint_SetPos(&printer, 1, y + 12);
            GfxPrint_Printf(&printer, "rotaxis %f %f %f", this->rotatingAxis.x, this->rotatingAxis.y,
                            this->rotatingAxis.z);
            GfxPrint_SetPos(&printer, 1, y + 13);
            GfxPrint_Printf(&printer, "rotangle = %f -> %f rotto %d", this->rotatingAngle, this->rotatingAngleTarget,
                            ((u32)this->rotatingTo - (u32)faces) / sizeof(LinkedFace));
            GfxPrint_SetPos(&printer, 1, y + 14);
            GfxPrint_Printf(&printer, "rottonorm %f %f %f", this->rotatingTo->norm.x, this->rotatingTo->norm.y,
                            this->rotatingTo->norm.z);
        } else {
            // MtxF mtxF = this->transform;
            // MtxF mtxF = systemLast;
            MtxF mtxF = systemInvLast;

            GfxPrint_SetPos(&printer, 1, y + 12);
            GfxPrint_Printf(&printer, "% 4.2f % 4.2f % 4.2f % 4.2f", mtxF.xx, mtxF.xy, mtxF.xz, mtxF.xw);
            GfxPrint_SetPos(&printer, 1, y + 13);
            GfxPrint_Printf(&printer, "% 4.2f % 4.2f % 4.2f % 4.2f", mtxF.yx, mtxF.yy, mtxF.yz, mtxF.yw);
            GfxPrint_SetPos(&printer, 1, y + 14);
            GfxPrint_Printf(&printer, "% 4.2f % 4.2f % 4.2f % 4.2f", mtxF.zx, mtxF.zy, mtxF.zz, mtxF.zw);
            GfxPrint_SetPos(&printer, 1, y + 15);
            GfxPrint_Printf(&printer, "% 4.2f % 4.2f % 4.2f % 4.2f", mtxF.wx, mtxF.wy, mtxF.wz, mtxF.ww);
            GfxPrint_SetPos(&printer, 1, y + 16);
            GfxPrint_Printf(&printer, "vec0toPosLast = %f %f %f", vec0toPosLast.x, vec0toPosLast.y, vec0toPosLast.z);
        }

        gfx = GfxPrint_Close(&printer);
        GfxPrint_Destroy(&printer);

        gSPEndDisplayList(gfx++);
        gSPBranchList(globalCtx->state.gfxCtx->polyOpa.p, gfx);
        globalCtx->state.gfxCtx->polyOpa.p = gfx;
    }
}
