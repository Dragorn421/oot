#include "item_dragorn.h"

#include "z64play.h"

#include "assets/objects/gameplay_keep/gDragornDL.h"

#define FLAGS 0

void ItemDragorn_Init(Actor* thisx, PlayState* play);
void ItemDragorn_Destroy(Actor* thisx, PlayState* play);
void ItemDragorn_Update(Actor* thisx, PlayState* play);
void ItemDragorn_Draw(Actor* thisx, PlayState* play);

ActorProfile item_dragorn_Profile = {
    /**/ ACTOR_ITEM_DRAGORN,
    /**/ ACTORCAT_MISC,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(struct item_dragorn),
    /**/ ItemDragorn_Init,
    /**/ ItemDragorn_Destroy,
    /**/ ItemDragorn_Update,
    /**/ ItemDragorn_Draw,
};

void ItemDragorn_Init(Actor* thisx, PlayState* play) {
    Actor_SetScale(thisx, 1.0f);
}

void ItemDragorn_Destroy(Actor* thisx, PlayState* play) {
}

void ItemDragorn_Update(Actor* thisx, PlayState* play) {
}

void ItemDragorn_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, gDragornDL);
}
