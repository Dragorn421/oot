#include "test_actor.h"

#include "overlays/actors/ovl_En_Goroiwa/z_en_goroiwa.h"

#include "objects/env_kokiri_forest/env_kokiri_forest.h"
#include "objects/transi_kokiri_to_deku/transi_kokiri_to_deku.h"
#include "objects/env_deku_tree/env_deku_tree.h"

#define THIS ((TestActor*)thisx)

#define ENV(name, dlistOpa, dlistXlu, duration) \
    { ENV_##name, OBJECT_ENV_##name, dlistOpa, dlistXlu, duration, true, false }
#define TRANSI(name, dlistOpa, dlistXlu) \
    { TRANSI_##name, OBJECT_TRANSI_##name, dlistOpa, dlistXlu, 0, false, true }

static CorridorSectionInfo corridorSectionsInfo[] = {
    ENV(KOKIRI_FOREST, DL_ENV_KOKIRI_FOREST_OPA, 0, 250),
    TRANSI(KOKIRI_TO_DEKU, DL_TRANSI_KOKIRI_TO_DEKU_OPA, 0),
    ENV(DEKU_TREE, DL_ENV_DEKU_TREE_OPA, DL_ENV_DEKU_TREE_XLU, 250),
};

#undef ENV
#undef TRANSI

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Update(Actor* thisx, GlobalContext* globalCtx);
void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx);

void TestActor_SetObject(Actor* thisx, GlobalContext* globalCtx, s16 objectId);
void TestActor_EnsureObjectLoaded(GlobalContext* globalCtx, s16 objectId);

void TestActor_UpdateImpl(TestActor* this, GlobalContext* globalCtx);
void TestActor_OnEnable(TestActor* this, GlobalContext* globalCtx);
void TestActor_UpdateEnabled(TestActor* this, GlobalContext* globalCtx);

void TestActor_DrawImpl(TestActor* this, GlobalContext* globalCtx);

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

RunnerObstacle* TestActor_GetObstacleSpot(TestActor* this) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(this->obstacles); i++) {
        if (this->obstacles[i].actor == NULL) {
            return &this->obstacles[i];
        }
    }

    osSyncPrintf("TestActor_GetObstacleSpot no free obstacle spot\n");
    return NULL;
}

void TestActor_RemoveObstacle(RunnerObstacle* obstacle) {
    Actor_Kill(obstacle->actor);
    obstacle->actor = NULL;
}

void TestActor_Init(Actor* thisx, GlobalContext* globalCtx) {
    CollisionHeader* colHeader;
    s32 i;

    osSyncPrintf("Hello Jack, Tharo and Fig!");
    Actor_SetScale(thisx, 0.1f);
    THIS->offset = 0.0f;

    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_GOROIWA);
    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_GOL);

    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_ENV_KOKIRI_FOREST);
    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_TRANSI_KOKIRI_TO_DEKU);
    TestActor_EnsureObjectLoaded(globalCtx, OBJECT_ENV_DEKU_TREE);

    TestActor_SetObject(thisx, globalCtx, OBJECT_ENV_KOKIRI_FOREST);

    DynaPolyActor_Init(&THIS->dyna, DPM_UNK);
    CollisionHeader_GetVirtual(COLL_KOKIRIGROUND, &colHeader);
    THIS->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, thisx, colHeader);

    for (i = 0; i < ARRAY_COUNT(THIS->corridorSections); i++) {
        THIS->corridorSections[i] = &corridorSectionsInfo[ENV_KOKIRI_FOREST];
        THIS->corridorSectionsRand[i] = Rand_Next();
    }
    THIS->curCorridorSectionIdx = 0;
    THIS->curCorridorSectionTotal = 0;
    THIS->nextCorridorSection = &corridorSectionsInfo[ENV_KOKIRI_FOREST];
    THIS->timeBeforeNewCorridorType = corridorSectionsInfo[ENV_KOKIRI_FOREST].duration;

    for (i = 0; i < ARRAY_COUNT(THIS->obstacles); i++) {
        THIS->obstacles[i].actor = NULL;
    }
    THIS->timeBeforeNewObstacle = 0;

    thisx->home.pos.y += 20.0f;
    thisx->world.pos.y += 20.0f;

    PLAYER->runner = &THIS->playerRunner;
}

void TestActor_SetObject(Actor* thisx, GlobalContext* globalCtx, s16 objectId) {
    thisx->objBankIndex = Object_GetIndex(&globalCtx->objectCtx, objectId);
    Actor_SetObjectDependency(globalCtx, thisx);
}

void TestActor_EnsureObjectLoaded(GlobalContext* globalCtx, s16 objectId) {
    if (Object_GetIndex(&globalCtx->objectCtx, objectId) < 0) {
        Object_Spawn(&globalCtx->objectCtx, objectId);
    }
}

void TestActor_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, THIS->dyna.bgId);
}

void TestActor_Update(Actor* thisx, GlobalContext* globalCtx) {
    TestActor_UpdateImpl(THIS, globalCtx);
}

void TestActor_UpdateImpl(TestActor* this, GlobalContext* globalCtx) {
    s32 i;

    gSaveContext.dayTime = 0x8000;

    this->offset += MOVE_SPEED;
    if (this->offset >= LENGTH) {
        s32 lastCorridorSectionIdx;

        this->offset -= LENGTH;

        lastCorridorSectionIdx = this->curCorridorSectionIdx;
        this->curCorridorSectionIdx++;
        this->curCorridorSectionIdx %= ARRAY_COUNT(this->corridorSections);
        this->curCorridorSectionTotal++;

        if (this->timeBeforeNewCorridorType == 0) {
            if (this->nextCorridorSection->idx != (CORRIDOR_SECTION_TYPE_COUNT - 1)) {
                this->nextCorridorSection++;
            }
            this->timeBeforeNewCorridorType = this->nextCorridorSection->duration;
        }

        this->corridorSections[lastCorridorSectionIdx] = this->nextCorridorSection;
        this->corridorSectionsRand[lastCorridorSectionIdx] = Rand_Next();

        if (this->nextCorridorSection->isTransi) {
            this->timeBeforeNewObstacle = 30;
            this->nextCorridorSection++;
            this->timeBeforeNewCorridorType = this->nextCorridorSection->duration;
        }
    }

    for (i = 0; i < ARRAY_COUNT(this->obstacles); i++) {
        if (this->obstacles[i].actor != NULL) {
            if (this->obstacles[i].actor->world.pos.x <= this->dyna.actor.world.pos.x) {
                TestActor_RemoveObstacle(&this->obstacles[i]);
            } else {
                this->obstacles[i].actor->world.pos.x -= MOVE_SPEED;
            }
        }
    }

    if (this->enabled) {
        TestActor_UpdateEnabled(this, globalCtx);
    } else if (!WAIT_DUP_TO_START || (globalCtx->state.input[0].cur.button & BTN_DUP)) {
        TestActor_OnEnable(this, globalCtx);
    }
}

void TestActor_OnEnable(TestActor* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    s32 i;
    f32 zSoftMargin;
    f32 zHardMargin;

    this->enabled = true;
    player->actor.world.pos = this->dyna.actor.world.pos;
    this->playerRunner.running = true;
    zSoftMargin = LINK_IS_ADULT ? 40.0f : 30.0f;
    zHardMargin = zSoftMargin / 2.0f; // only tested as child
    this->playerRunner.softMinZ = this->dyna.actor.home.pos.z - (WIDTH / 2.0f - zSoftMargin);
    this->playerRunner.softMaxZ = this->dyna.actor.home.pos.z + (WIDTH / 2.0f - zSoftMargin);
    this->playerRunner.hardMinZ = this->dyna.actor.home.pos.z - (WIDTH / 2.0f - zHardMargin);
    this->playerRunner.hardMaxZ = this->dyna.actor.home.pos.z + (WIDTH / 2.0f - zHardMargin);
}

void TestActor_ObstacleActorSpawnFail(TestActor* this, GlobalContext* globalCtx, char* str) {
    osSyncPrintf(__FILE__ " failed to spawn obstacle actor: %s\n", str);
    Actor_SetScale(&this->dyna.actor, 0.05f);
}

void TestActor_UpdateEnabled(TestActor* this, GlobalContext* globalCtx) {
    Player* player = PLAYER;
    Camera* camera = ACTIVE_CAM;
    s32 i;

    if (this->timeBeforeNewCorridorType > 0) {
        this->timeBeforeNewCorridorType--;
    }
    if (this->timeBeforeNewCorridorType < 10) {
        this->timeBeforeNewObstacle = 10;
    }
    if (this->timeBeforeNewObstacle == 0) {
        RunnerObstacle* obstacleSpot = TestActor_GetObstacleSpot(this);
        if (obstacleSpot != NULL) {
            if (this->nextCorridorSection->idx == ENV_KOKIRI_FOREST) {
                Vec3f obPos = this->dyna.actor.world.pos;
                obPos.x += LENGTH * N_CORRIDORS;
                obPos.z += (Rand_ZeroOne() * 2.0f - 1.0f) * (WIDTH / 2.0f - GOROIWA_RADIUS);
                obstacleSpot->actor = Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_GOROIWA, obPos.x, obPos.y,
                                                  obPos.z, 0, 0, 0, -1);
                if (obstacleSpot->actor == NULL) {
                    TestActor_ObstacleActorSpawnFail(this, globalCtx, "goroiwa");
                }
                this->timeBeforeNewObstacle = Rand_S16Offset(14, 5);
            } else if (this->nextCorridorSection->idx == ENV_DEKU_TREE) {
                Vec3f obPos = this->dyna.actor.world.pos;
                obPos.x += LENGTH * N_CORRIDORS;
                obPos.z += (Rand_ZeroOne() * 2.0f - 1.0f) * (WIDTH / 2.0f - 50.0f);
                obstacleSpot->actor =
                    Actor_Spawn(&globalCtx->actorCtx, globalCtx, ACTOR_EN_GOMA, obPos.x, obPos.y, obPos.z, 0, 0, 0, 0);
                if (obstacleSpot->actor == NULL) {
                    TestActor_ObstacleActorSpawnFail(this, globalCtx, "goma");
                }
                this->timeBeforeNewObstacle = Rand_S16Offset(11, 3);
            }
        } else {
            this->timeBeforeNewObstacle = 10;
        }
    } else {
        this->timeBeforeNewObstacle--;
        if (this->timeBeforeNewObstacle < 0) {
            this->timeBeforeNewObstacle = 0;
        }
    }

    player->actor.world.pos.x = this->dyna.actor.home.pos.x + LENGTH / 2.0f;
    player->actor.world.pos.z += (f32)globalCtx->state.input[0].cur.stick_x / 60.0f * 2.0f;
    if (player->actor.world.pos.z < this->playerRunner.hardMinZ) {
        Math_SmoothStepToF(&player->actor.world.pos.z, this->playerRunner.hardMinZ, 0.2f, 5.0f, 1.0f);
    }
    if (player->actor.world.pos.z > this->playerRunner.hardMaxZ) {
        Math_SmoothStepToF(&player->actor.world.pos.z, this->playerRunner.hardMaxZ, 0.2f, 5.0f, 1.0f);
    }
    Math_Vec3f_Copy(&camera->at, &player->actor.world.pos);
    camera->at.y += 30.0f;
    camera->at.x += LENGTH / 2.0f;
    Math_Vec3f_Copy(&camera->eye, &camera->at);
    camera->eye.y += 30.0f;
    // values below ~205.0f for camera->dist move the camera very near to link for some reason
    camera->dist = LINK_IS_ADULT ? 250.0f : 210.0f;
    camera->eye.x -= camera->dist;
    Math_Vec3f_Copy(&camera->eyeNext, &camera->eye);
}

void TestActor_Draw(Actor* thisx, GlobalContext* globalCtx) {
    TestActor_DrawImpl(THIS, globalCtx);
}

extern MtxF* sCurrentMatrix;

void TestActor_DrawImpl(TestActor* this, GlobalContext* globalCtx) {
    s32 i;

    Matrix_Translate(-this->offset / this->dyna.actor.scale.x, 0.0f, 0.0f, MTXMODE_APPLY);

    // works in emu, but may need to add pipesync calls somewhere?
    // alpha fog would be nice but idk how it works
    gDPSetFogColor(globalCtx->state.gfxCtx->polyOpa.p++, 150, 200, 200, 255);
    gSPFogPosition(globalCtx->state.gfxCtx->polyOpa.p++, 995, 1000);

    for (i = 0; i < ARRAY_COUNT(this->corridorSections); i++) {
        s32 csIdx = (this->curCorridorSectionIdx + i) % ARRAY_COUNT(this->corridorSections);
        CorridorSectionInfo* csi = this->corridorSections[csIdx];
        u32 csr = this->corridorSectionsRand[csIdx];
        void* objectSegment =
            globalCtx->objectCtx.status[Object_GetIndex(&globalCtx->objectCtx, csi->objectId)].segment;

        if (i == 0) {
            // don't translate first section
        } else {
            // "- 0.1f" fixes visual artifacts (gaps) between sections
            Matrix_Translate((LENGTH - 0.1f) / this->dyna.actor.scale.x, 0.0f, 0.0f, MTXMODE_APPLY);
        }
        if (csi->dlistOpa != 0) {
            gSPSegment(globalCtx->state.gfxCtx->polyOpa.p++, 0x06, objectSegment);
            Gfx_DrawDListOpa(globalCtx, csi->dlistOpa);
        }
        if (csi->dlistXlu != 0) {
            gSPSegment(globalCtx->state.gfxCtx->polyXlu.p++, 0x06, objectSegment);
            if (csi->idx == ENV_DEKU_TREE) {
                MtxF* mf = sCurrentMatrix;
                s32 variant = csr % 8;

                if (variant <= 5) {
                    // do not draw
                } else if (variant % 2 == 0) {
                    Matrix_Push();
                    Matrix_RotateY(DEGTORAD(180), MTXMODE_APPLY);
                    Matrix_Translate(-LENGTH / this->dyna.actor.scale.x, 0, 0, MTXMODE_APPLY);
                    Gfx_DrawDListXlu(globalCtx, csi->dlistXlu);
                    Matrix_Pop();
                } else {
                    Gfx_DrawDListXlu(globalCtx, csi->dlistXlu);
                }
            } else {
                Gfx_DrawDListXlu(globalCtx, csi->dlistXlu);
            }
        }
    }
}
