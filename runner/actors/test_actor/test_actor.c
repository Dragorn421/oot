#include "test_actor.h"

#define THIS ((TestActor*)thisx)

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Update(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx);

void TestActor_EnsureObjectLoaded(GlobalContext* globalCtx, s16 objectId);

void TestActor_UpdateMainTA(TestActor* this, GlobalContext* globalCtx);
void TestActor_OnEnable(TestActor* this, GlobalContext* globalCtx);
void TestActor_UpdateEnabled(TestActor* this, GlobalContext* globalCtx);
void TestActor_UpdateOtherTA(TestActor* this, GlobalContext* globalCtx);

void TestActor_DrawMainTA(TestActor* this, GlobalContext* globalCtx);
void TestActor_DrawOtherTA(TestActor* this, GlobalContext* globalCtx);

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

TestActor* mainTA = NULL;
TestActor* otherTA = NULL;
s32 otherOthersTA = 0; // just in case, for debug

#define LENGTH 300.0f
#define WIDTH 300.0f
#define MOVE_SPEED 30.0f
#define N_FAKE_CORRIDORS 3

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;
    osSyncPrintf("Hello Jack, Tharo and Fig!");
    Actor_SetScale(thisx, 1.0f);
    THIS->timer = 1;
    THIS->offset = 0.0f;
    DynaPolyActor_Init(&THIS->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(COLLISION, &colHeader);
    THIS->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);

    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_BOMBIWA);
    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_GOROIWA);

    THIS->nextBombiwaIdx = 0;

    if (mainTA == NULL) {
        mainTA = THIS;
        thisx->home.pos.y += 20.0f;
        thisx->world.pos.y += 20.0f;
    } else if (otherTA == NULL) {
        otherTA = THIS;
        thisx->home.pos.x += LENGTH;
    } else {
        otherOthersTA++;
    }
}

void TestActor_EnsureObjectLoaded(GlobalContext* globalCtx, s16 objectId) {
    if (Object_GetIndex(&globalCtx->objectCtx, objectId) < 0) {
        Object_Spawn(&globalCtx->objectCtx, objectId);
    }
}

void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    if (THIS == mainTA) {
        mainTA = NULL;
    } else if (THIS == otherTA) {
        otherTA = NULL;
    } else {
        otherOthersTA--;
    }
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, THIS->dyna.bgId);
}

Actor* TestActor_NextBombiwa(TestActor* this, GlobalContext* globalCtx) {
    Actor* nextBombiwa = this->bombiwas[this->nextBombiwaIdx];
    this->nextBombiwaIdx++;
    this->nextBombiwaIdx %= ARRAY_COUNT(this->bombiwas);
    return nextBombiwa;
}

void TestActor_Update(Actor* thisx, GlobalContext* globalCtx) {
    THIS->timer++;

    if (THIS == mainTA) {
        TestActor_UpdateMainTA(THIS, globalCtx);
    } else if (THIS == otherTA) {
        TestActor_UpdateOtherTA(THIS, globalCtx);
    }
}

void TestActor_UpdateMainTA(TestActor* this, GlobalContext* globalCtx) {
    this->offset += MOVE_SPEED;
    if (this->offset >= LENGTH) {
        this->offset -= LENGTH;
    }
    this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + this->offset;

    if (this->enabled) {
        TestActor_UpdateEnabled(this, globalCtx);
    } else if (globalCtx->state.input[0].cur.button & BTN_DUP) {
        TestActor_OnEnable(this, globalCtx);
    }
}

void TestActor_OnEnable(TestActor* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    s32 i;

    this->enabled = true;
    player->runnerRunning = true;
    player->runnerMinZ = this->dyna.actor.home.pos.z - (WIDTH / 2.0f - 40.0f);
    player->runnerMaxZ = this->dyna.actor.home.pos.z + (WIDTH / 2.0f - 40.0f);
    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_TEST_ACTOR, this->dyna.actor.home.pos.x,
                this->dyna.actor.home.pos.y, this->dyna.actor.home.pos.z, 0, 0, 0, 0);
    for (i = 0; i < ARRAY_COUNT(this->bombiwas); i++) {
        this->bombiwas[i] = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_OBJ_BOMBIWA, this->dyna.actor.home.pos.x,
                                        this->dyna.actor.home.pos.y, this->dyna.actor.home.pos.z, 0, 0, 0, 0);
    }
}

void TestActor_UpdateEnabled(TestActor* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    Camera* camera = ACTIVE_CAM;
    s32 i;

    player->actor.world.pos.x = this->dyna.actor.home.pos.x + LENGTH / 3.0f;
    player->actor.world.pos.z -= (f32)globalCtx->state.input[0].cur.stick_x / 60.0f * 2.0f;
    Math_Vec3f_Copy(&camera->at, &player->actor.world.pos);
    camera->at.y += 30.0f;
    camera->at.x -= LENGTH / 2.0f;
    Math_Vec3f_Copy(&camera->eye, &camera->at);
    camera->eye.y += 30.0f;
    camera->dist = 250.0f;
    camera->eye.x += camera->dist;
    Math_Vec3f_Copy(&camera->eyeNext, &camera->eye);
    otherTA->dyna.actor.world.pos.x = this->dyna.actor.world.pos.x - LENGTH;

    if (this->offset < LENGTH / 10.0f && this->timer > 35) {
        for (i = 2 * Rand_ZeroOne(); i >= 0; i--) {
            Actor* bombiwa = TestActor_NextBombiwa(this, globalCtx);
            Math_Vec3f_Copy(&bombiwa->world.pos, &this->dyna.actor.world.pos);
            bombiwa->world.pos.x -= ((2.0f + (f32)N_FAKE_CORRIDORS) * LENGTH - this->offset);
            bombiwa->world.pos.z += (Rand_ZeroOne() * 2.0f - 1.0f) * (LENGTH / 2.0f - 50.0f);
        }
        if (Rand_ZeroOne() > 0.2f) {
            Actor* bombiwa = TestActor_NextBombiwa(this, globalCtx);
            Math_Vec3f_Copy(&bombiwa->world.pos, &this->dyna.actor.world.pos);
            bombiwa->world.pos.x -= ((2.0f + (f32)N_FAKE_CORRIDORS) * LENGTH - this->offset);
            bombiwa->world.pos.z = player->actor.world.pos.z;
        }
        this->timer = (s32)(Rand_ZeroOne() * 15.0f);
    }

    for (i = 0; i < ARRAY_COUNT(this->bombiwas); i++) {
        this->bombiwas[i]->world.pos.x += MOVE_SPEED;
        if (Actor_WorldDistXZToActor(&player->actor, this->bombiwas[i]) < 40.0f) {
            // gSaveContext.health = 0;
        }
    }
}

void TestActor_UpdateOtherTA(TestActor* this, GlobalContext* globalCtx) {
}

void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx) {
    if (THIS == mainTA) {
        TestActor_DrawMainTA(THIS, globalCtx);
    } else if (THIS == otherTA) {
        TestActor_DrawOtherTA(THIS, globalCtx);
    }
}

void TestActor_DrawMainTA(TestActor* this, GlobalContext* globalCtx) {
    s32 i;

    Gfx_DrawDListOpa(globalCtx, MODEL);
    Matrix_Translate(-LENGTH, 0.0f, 0.0f, MTXMODE_APPLY);
    Gfx_DrawDListOpa(globalCtx, MODEL);
    for (i = 0; i < N_FAKE_CORRIDORS; i++) {
        Matrix_Translate(-LENGTH, 0.0f, 0.0f, MTXMODE_APPLY);
        Gfx_DrawDListOpa(globalCtx, MODEL);
    }
}

void TestActor_DrawOtherTA(TestActor* this, GlobalContext* globalCtx) {
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Gfx_DrawDListOpa(globalCtx, MODEL);
}
