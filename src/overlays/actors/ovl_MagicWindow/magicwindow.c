#include "magicwindow.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void MagicWindow_Init(Actor* thisx, GlobalContext* globalCtx);
void MagicWindow_Destroy(Actor* thisx, GlobalContext* globalCtx);
void MagicWindow_Update(Actor* thisx, GlobalContext* globalCtx);
void MagicWindow_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit MagicWindow_InitVars = {
    ACTOR_MAGIC_WINDOW,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(MagicWindow),
    (ActorFunc)MagicWindow_Init,
    (ActorFunc)MagicWindow_Destroy,
    (ActorFunc)MagicWindow_Update,
    (ActorFunc)MagicWindow_Draw,
};

#define SCALE 0.03f

void MagicWindow_Init(Actor* thisx, GlobalContext* globalCtx) {
    MagicWindow* this = (MagicWindow*)thisx;

    this->actor.world.pos.y += 30.0f;
    Actor_SetScale(&this->actor, SCALE);
}

void MagicWindow_Destroy(Actor* thisx, GlobalContext* globalCtx) {
}

void MagicWindow_Update(Actor* thisx, GlobalContext* globalCtx) {
    // GET_ACTIVE_CAM(globalCtx)->at.y += 50.0f;
}

#include "gMagicWindowFrameDL.c"
#include "gMagicWindowOutsideDL.c"
#include "gMagicWindowInsideDL.c"

void MagicWindow_Draw(Actor* thisx, GlobalContext* globalCtx) {
    OPEN_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);

    Gfx_DrawDListOpa(globalCtx, gMagicWindowFrameDL);

    Gfx_DrawDListXlu(globalCtx, gMagicWindowOutsideDL);
    //Gfx_DrawDListXlu(globalCtx, gMagicWindowInsideDL);
    Matrix_Translate(-100.0f / SCALE, -30.0f / SCALE, 0.0f, MTXMODE_APPLY);
    Gfx_DrawDListXlu(globalCtx, gTreeStumpDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, __FILE__, __LINE__);
}
