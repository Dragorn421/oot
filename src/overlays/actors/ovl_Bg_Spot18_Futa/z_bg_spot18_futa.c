/*
 * File: z_bg_spot18_futa.c
 * Overlay: ovl_Bg_Spot18_Futa
 * Description: The lid to the spinning goron vase.
 */

#include "z_bg_spot18_futa.h"
#include "assets/objects/object_spot18_obj/object_spot18_obj.h"

#define FLAGS 0

void BgSpot18Futa_Init(Actor* thisx, PlayState* play);
void BgSpot18Futa_Destroy(Actor* thisx, PlayState* play);
void BgSpot18Futa_Update(Actor* thisx, PlayState* play);
void BgSpot18Futa_Draw(Actor* thisx, PlayState* play);

ActorInit Bg_Spot18_Futa_InitVars = {
    /**/ ACTOR_BG_SPOT18_FUTA,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_SPOT18_OBJ,
    /**/ sizeof(BgSpot18Futa),
    /**/ BgSpot18Futa_Init,
    /**/ BgSpot18Futa_Destroy,
    /**/ BgSpot18Futa_Update,
    /**/ BgSpot18Futa_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void BgSpot18Futa_Init(Actor* thisx, PlayState* play) {
    BgSpot18Futa* this = (BgSpot18Futa*)thisx;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->actor, 0);
    CollisionHeader_GetVirtual(&gGoronCityVaseLidCol, &colHeader);
    this->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->actor, colHeader);
    Actor_ProcessInitChain(&this->actor, sInitChain);
}

void BgSpot18Futa_Destroy(Actor* thisx, PlayState* play) {
    BgSpot18Futa* this = (BgSpot18Futa*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->bgId);
}

void BgSpot18Futa_Update(Actor* thisx, PlayState* play) {
    BgSpot18Futa* this = (BgSpot18Futa*)thisx;
    s32 iVar1;

    if (this->actor.parent == NULL) {
        iVar1 = Math_StepToF(&this->actor.scale.x, 0, 0.005);

        if (iVar1 != 0) {
            Actor_Kill(&this->actor);
        } else {
            this->actor.scale.z = this->actor.scale.x;
            this->actor.scale.y = this->actor.scale.x;
        }
    }
}

void BgSpot18Futa_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, gGoronCityVaseLidDL);
}
