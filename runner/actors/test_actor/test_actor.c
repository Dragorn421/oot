#include "test_actor.h"

#define THIS ((TestActor*)thisx)

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Update(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx);

const ActorInit TestActor_InitVars = {
    ACTOR_TEST_ACTOR,
    ACTORCAT_MISC,
    0x00000030, // always run update and draw
    OBJECT_TEST_OBJECT,
    sizeof(TestActor),
    (ActorFunc)TestActor_Init,
    (ActorFunc)TestActor_Destroy,
    (ActorFunc)TestActor_Update,
    (ActorFunc)TestActor_Draw,
};

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx) {
    osSyncPrintf("Hello Jack, Tharo and Fig!");
    Actor_SetScale(thisx, 1.0f);
    THIS->timer = 1;
}

void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx) {

}

void TestActor_Update(Actor* thisx, GlobalContext* globalCtx) {
    THIS->timer++;
}

void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx) {
    if (THIS->timer % 50 == 0) {
        osSyncPrintf("TestActor_Draw\n");
    }
    Gfx_DrawDListOpa(globalCtx, MODEL);
}
