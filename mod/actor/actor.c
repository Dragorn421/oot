#include "actor.h"
#include "../object/cube.h"

// always draw and update
#define FLAGS 0x00000030

#define THIS ((ModActor*)thisx)

void ModActor_Init(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Destroy(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Update(Actor* thisx, GlobalContext* globalCtx);
void ModActor_Draw(Actor* thisx, GlobalContext* globalCtx);

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

#define SCALE 100.0f

void ModActor_Init(Actor* thisx, GlobalContext* globalCtx) {
    R_MINIMAP_DISABLED = true;
    Actor_SetScale(thisx, 1.0f);
    thisx->world.pos.x = 0.0f;
    thisx->world.pos.z = 0.0f;
    thisx->world.pos.y = -SCALE;
    VEC_SET(PLAYER->actor.world.pos, 0.0f, 0.0f, 0.0f);
}

void ModActor_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void ModActor_Update(Actor* thisx, GlobalContext* globalCtx) {
    ModActor* this = THIS;
    Player* player = PLAYER;
    Vec3f delta = player->actor.world.pos;

    VEC_SET(PLAYER->actor.world.pos, 0.0f, 0.0f, 0.0f);
    if (this->curRotationAxis == ROT_AXIS_NONE) {
        this->curTranslation.x -= delta.x;
        this->curTranslation.y -= delta.y;
        this->curTranslation.z -= delta.z;
    }
    // player towards -x
    if (this->curTranslation.x >= SCALE) {
        this->curTranslation.x = SCALE;
        if (this->curRotationAxis == ROT_AXIS_NONE) {
            this->curRotationAxis = ROT_AXIS_Z_NEG;
            this->curRotationRad = 0.0f;
        }
        if (this->curRotationAxis == ROT_AXIS_Z_NEG) {
            this->curRotationRad += M_PI / 2.0f * (-delta.x) / 100.0f;
            if (this->curRotationRad <= 0.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.x = SCALE;
            }
            if (this->curRotationRad >= M_PI / 2.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.x = -SCALE;
            }
        }
    }
    // player towards +x
    if (this->curTranslation.x <= -SCALE) {
        this->curTranslation.x = -SCALE;
        if (this->curRotationAxis == ROT_AXIS_NONE) {
            this->curRotationAxis = ROT_AXIS_Z_POSI;
            this->curRotationRad = 0.0f;
        }
        if (this->curRotationAxis == ROT_AXIS_Z_POSI) {
            this->curRotationRad += M_PI / 2.0f * delta.x / 100.0f;
            if (this->curRotationRad <= 0.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.x = -SCALE;
            }
            if (this->curRotationRad >= M_PI / 2.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.x = SCALE;
            }
        }
    }
    // player towards -z
    if (this->curTranslation.z >= SCALE) {
        this->curTranslation.z = SCALE;
        if (this->curRotationAxis == ROT_AXIS_NONE) {
            this->curRotationAxis = ROT_AXIS_X_POSI;
            this->curRotationRad = 0.0f;
        }
        if (this->curRotationAxis == ROT_AXIS_X_POSI) {
            this->curRotationRad += M_PI / 2.0f * (-delta.z) / 100.0f;
            if (this->curRotationRad <= 0.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.z = SCALE;
            }
            if (this->curRotationRad >= M_PI / 2.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.z = -SCALE;
            }
        }
    }
    // player towards +z
    if (this->curTranslation.z <= -SCALE) {
        this->curTranslation.z = -SCALE;
        if (this->curRotationAxis == ROT_AXIS_NONE) {
            this->curRotationAxis = ROT_AXIS_X_NEG;
            this->curRotationRad = 0.0f;
        }
        if (this->curRotationAxis == ROT_AXIS_X_NEG) {
            this->curRotationRad += M_PI / 2.0f * delta.z / 100.0f;
            if (this->curRotationRad <= 0.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.z = -SCALE;
            }
            if (this->curRotationRad >= M_PI / 2.0f) {
                this->curRotationAxis = ROT_AXIS_NONE;
                this->curRotationRad = 0.0f;
                this->curTranslation.z = SCALE;
            }
        }
    }
}

void ModActor_Draw(Actor* thisx, GlobalContext* globalCtx) {
    ModActor* this = THIS;
    GraphicsContext* gfxCtx = globalCtx->state.gfxCtx;

    Matrix_Translate(this->curTranslation.x, this->curTranslation.y, this->curTranslation.z, MTXMODE_APPLY);
    Matrix_Scale(SCALE, SCALE, SCALE, MTXMODE_APPLY);
    switch (this->curRotationAxis) {
        case ROT_AXIS_Z_NEG:
            Matrix_Translate(-1.0f, 1.0f, 0.0f, MTXMODE_APPLY);
            Matrix_RotateZ(-this->curRotationRad, MTXMODE_APPLY);
            Matrix_Translate(1.0f, -1.0f, 0.0f, MTXMODE_APPLY);
            break;
        case ROT_AXIS_Z_POSI:
            Matrix_Translate(1.0f, 1.0f, 0.0f, MTXMODE_APPLY);
            Matrix_RotateZ(this->curRotationRad, MTXMODE_APPLY);
            Matrix_Translate(-1.0f, -1.0f, 0.0f, MTXMODE_APPLY);
            break;
        case ROT_AXIS_X_POSI:
            Matrix_Translate(0.0f, 1.0f, -1.0f, MTXMODE_APPLY);
            Matrix_RotateX(this->curRotationRad, MTXMODE_APPLY);
            Matrix_Translate(0.0f, -1.0f, 1.0f, MTXMODE_APPLY);
            break;
        case ROT_AXIS_X_NEG:
            Matrix_Translate(0.0f, 1.0f, 1.0f, MTXMODE_APPLY);
            Matrix_RotateX(-this->curRotationRad, MTXMODE_APPLY);
            Matrix_Translate(0.0f, -1.0f, -1.0f, MTXMODE_APPLY);
            break;
    }
    gSPMatrix(gfxCtx->polyOpa.p++, Matrix_NewMtx(gfxCtx, __FILE__, __LINE__), G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(gfxCtx->polyOpa.p++, DL_CUBE);
}
