#include "mycustomactor.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void MyActor_Init(Actor* thisx, PlayState* play);
void MyActor_Destroy(Actor* thisx, PlayState* play);
void MyActor_Update(Actor* thisx, PlayState* play);
void MyActor_Draw(Actor* thisx, PlayState* play);

const ActorInit MyCustomActor_InitVars = {
    ACTOR_MYCUSTOM,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MyActor),
    (ActorFunc)MyActor_Init,
    (ActorFunc)MyActor_Destroy,
    (ActorFunc)MyActor_Update,
    (ActorFunc)MyActor_Draw,
};

void MyActor_Init(Actor* thisx, PlayState* play) {

}

void MyActor_Destroy(Actor* thisx, PlayState* play) {

}

void MyActor_Update(Actor* thisx, PlayState* play) {

}

void MyActor_Draw(Actor* thisx, PlayState* play) {
    GfxPrint printer;
    Gfx* gfx;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    gfx = POLY_OPA_DISP + 1;
    gSPDisplayList(OVERLAY_DISP++, gfx);

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);

    GfxPrint_SetColor(&printer, 255, 0, 255, 255);
    GfxPrint_SetPos(&printer, 10, 10);
    GfxPrint_Printf(&printer, "Hello from MyActor_Draw");

    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);

    gSPEndDisplayList(gfx++);
    gSPBranchList(POLY_OPA_DISP, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}
