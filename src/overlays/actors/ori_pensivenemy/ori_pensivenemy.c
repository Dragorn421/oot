// SPDX-License-Identifier: CC0-1.0

#include "ori_pensivenemy.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

/*
from in game logging:
(player pos = down on the ground)
player torso y = player pos + ~35
player top of hat = player pos + ~60

visually:
player torso is 20-30 units wide
with arms and hands the player model is 30-40 units wide

see assets/original/pensivenemy/measurements.odp
*/

#define LINK_ADULT_HEIGHT 60.0f
#define LINK_ADULT_WIDTH 40.0f
#define LINK_ADULT_TORSO_Y 35.0f
#define LINK_ADULT_TORSO_WIDTH 30.0f

#define OVERLAP_HEIGHT_FOR_PUSH 20.0f

void Pensivenemy_Init(Actor* thisx, PlayState* play);
void Pensivenemy_Destroy(Actor* thisx, PlayState* play);
void Pensivenemy_Update(Actor* thisx, PlayState* play);
void Pensivenemy_Draw(Actor* thisx, PlayState* play);

const ActorInit ori_pensivenemy_InitVars = {
    ACTOR_ORI_PENSIVENEMY,
    ACTORCAT_MISC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(Pensivenemy),
    (ActorFunc)Pensivenemy_Init,
    (ActorFunc)Pensivenemy_Destroy,
    (ActorFunc)Pensivenemy_Update,
    (ActorFunc)Pensivenemy_Draw,
};

static Pensivenemy* sGfxPrintOwner = NULL;
static int sGfxPrintOwnerNextDirection = 1; // 1=next -1=prev
static bool sGfxPrintActive = true;
struct {
    s16 view;
    f32 pushEffectiveness;
    f32 speedXZ, speedZboost;
    Vec3f playerPosLocal;
    Vec3f playerDisplacementLocal;
    Vec3f playerDisplacementWorld;
} sGfxPrintDebugData;

PENSIVENEMY_ACTIONFUNC(Pensivenemy_FaceInit);
PENSIVENEMY_ACTIONFUNC(Pensivenemy_Face);
PENSIVENEMY_ACTIONFUNC(Pensivenemy_FaceDraw);
void Pensivenemy_FaceGfxPrint(struct Pensivenemy* this, PlayState* play, GfxPrint* printer);

PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProjInit);
PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProj);
PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProjDraw);
void Pensivenemy_HandGfxPrint(struct Pensivenemy* this, PlayState* play, GfxPrint* printer);

static struct {
    int width_in_units;
    int height_in_units;
    f32 newHandProjsSpeed;
    f32 newHandProjsCooldownSeconds;
} sSettingsProfiles[] = {
    /* 0 */ { -1, -1, -1, -1 }, // code
    /* 1 */ { 40, 40, 5, 5 },
    /* 2 */ { 200, 200, 30, 3 },
};

void Pensivenemy_Init(Actor* thisx, PlayState* play) {
    Pensivenemy* this = (Pensivenemy*)thisx;
    f32 displaceActiveRadius;
    s32 profileId;

    profileId = PENSIVEENEMY_PROFILEID(this->actor.params);

    switch (profileId) {
        case 0:
            this->width_in_units = 30.0f + Rand_ZeroOne() * 100.0f;
            this->height_in_units = this->width_in_units + (Rand_ZeroOne() * 2.0f - 1.0f) * 50.0f;
            this->height_in_units = CLAMP_MIN(this->height_in_units, 30.0f);

            this->newHandProjsSpeed = 5.0f + Rand_ZeroOne() * 30.0f;
            this->newHandProjsCooldownSeconds = 1.0f + Rand_ZeroOne() * 5.0f;
            break;

        default:
            if (profileId >= ARRAY_COUNT(sSettingsProfiles) || sSettingsProfiles[profileId].width_in_units < 0) {
                osSyncPrintf("Pensivenemy_Init: bad params=%hx profileId=%d\n", (u16)this->actor.params, profileId);
                Actor_Kill(&this->actor);
                return;
            } else {
                this->width_in_units = sSettingsProfiles[profileId].width_in_units;
                this->height_in_units = sSettingsProfiles[profileId].height_in_units;
                this->newHandProjsSpeed = sSettingsProfiles[profileId].newHandProjsSpeed;
                this->newHandProjsCooldownSeconds = sSettingsProfiles[profileId].newHandProjsCooldownSeconds;
            }
            break;
    }

    displaceActiveRadius = (MAX(this->width_in_units, this->height_in_units) + LINK_ADULT_WIDTH +
                            Math3D_Vec3fMagnitude(&this->actor.velocity) + 20.0f) /
                           2.0f * 1.05f;
    this->displaceActiveRadiusSq = SQ(displaceActiveRadius);

    // the model is 2 blender units wide/high
    // the scale used on export is 1000 (idk where that comes from)
    this->actor.scale.x = this->width_in_units * 0.001f / 2;
    this->actor.scale.y = this->height_in_units * 0.001f / 2;
    this->actor.scale.z = sqrt(this->actor.scale.x * this->actor.scale.y);

    if (sGfxPrintOwner == NULL) {
        sGfxPrintOwner = this;
    }
    // always debug latest spawned actor
    sGfxPrintOwner = this;

    this->instType = PENSIVEENEMY_INSTTYPE(this->actor.params);
    switch (this->instType) {
        case PENSIVENEMY_FACE:
            Pensivenemy_FaceInit(this, play);
            break;
        case PENSIVENEMY_HAND_PROJ:
            Pensivenemy_HandProjInit(this, play);
            break;
        default:
            osSyncPrintf("Pensivenemy_Init: bad params=%hx instType=%d\n", (u16)this->actor.params, this->instType);
            Actor_Kill(&this->actor);
            break;
    }
}

void Pensivenemy_Destroy(Actor* thisx, PlayState* play) {
    Pensivenemy* this = (Pensivenemy*)thisx;

    if (sGfxPrintOwner == this) {
        sGfxPrintOwner = NULL;
    }
}

void Pensivenemy_Update(Actor* thisx, PlayState* play) {
    Pensivenemy* this = (Pensivenemy*)thisx;

    if (sGfxPrintOwner == NULL) {
        sGfxPrintOwner = this;
    } else {
        if (this->actor.xyzDistToPlayerSq < sGfxPrintOwner->actor.xyzDistToPlayerSq) {
            sGfxPrintOwner = this;
        }
    }

    if (sGfxPrintOwner == this) {
        if (play->state.input[0].press.button & BTN_DUP) {
            bool noNextPrev = false, noNextAfter = false;

            sGfxPrintOwner = NULL;
            while (sGfxPrintOwner == NULL && !(noNextPrev && noNextAfter)) {
                if (sGfxPrintOwnerNextDirection == 1) {
                    Actor* actorIter;

                    actorIter = this->actor.next;
                    while (actorIter != NULL && actorIter->id != ACTOR_ORI_PENSIVENEMY) {
                        actorIter = actorIter->next;
                    }
                    if (actorIter == NULL) {
                        sGfxPrintOwnerNextDirection = -1;
                        noNextAfter = true;
                    } else {
                        sGfxPrintOwner = (Pensivenemy*)actorIter;
                    }
                }
                if (sGfxPrintOwnerNextDirection == -1) {
                    Actor* actorIter;

                    actorIter = this->actor.prev;
                    while (actorIter != NULL && actorIter->id != ACTOR_ORI_PENSIVENEMY) {
                        actorIter = actorIter->prev;
                    }
                    if (actorIter == NULL) {
                        sGfxPrintOwnerNextDirection = 1;
                        noNextPrev = true;
                    } else {
                        sGfxPrintOwner = (Pensivenemy*)actorIter;
                    }
                }
            }
            if (sGfxPrintOwner == NULL) {
                sGfxPrintOwner = this;
            }
        }

        if (play->state.input[0].press.button & BTN_DDOWN) {
            sGfxPrintActive = !sGfxPrintActive;
        }
    }

    this->func_action(this, play);
}

void Pensivenemy_Draw(Actor* thisx, PlayState* play) {
    Pensivenemy* this = (Pensivenemy*)thisx;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    if (sGfxPrintOwner == this && sGfxPrintActive) {
        GfxPrint printer;
        Gfx* gfx;

        OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

        gfx = POLY_OPA_DISP + 1;
        gSPDisplayList(OVERLAY_DISP++, gfx);

        GfxPrint_Init(&printer);
        GfxPrint_Open(&printer, gfx);

        GfxPrint_SetColor(&printer, 255, 0, 255, 255);
        GfxPrint_SetPos(&printer, 10, 6);
        GfxPrint_Printf(&printer, "%x %hd\n", (u32)sGfxPrintOwner, this->actor.params);

        if (this->func_gfxprint != NULL)
            this->func_gfxprint(this, play, &printer);

        gfx = GfxPrint_Close(&printer);
        GfxPrint_Destroy(&printer);

        gSPEndDisplayList(gfx++);
        gSPBranchList(POLY_OPA_DISP, gfx);
        POLY_OPA_DISP = gfx;

        CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    }

    this->func_draw(this, play);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

s16 Pensivenemy_PitchAdjustForView(Pensivenemy* this, PlayState* play, s16 basePitch) {
    s16 pitchTowardsViewEye, pitchDiff, adjustedPitch;
    f32 t;
    f32 t_threshold = 0.5f;

    // - (minus) because using for shape.rot.x which is oriented opposite of world.rot.x
    pitchTowardsViewEye = Actor_WorldPitchTowardPoint(&this->actor, &play->view.eye);

    pitchDiff = (s16)(basePitch - pitchTowardsViewEye);
    t = (f32)ABS(pitchDiff) / (f32)0x4000;
    // t = 0: actor/viewEye/base aligned
    // t = 1: at a straight turn
    adjustedPitch = LERP16(basePitch, pitchTowardsViewEye, CLAMP_MIN(t - t_threshold, 0.0f) / (1.0f - t_threshold) / 2);

    return adjustedPitch;
}

s16 Pensivenemy_YawAdjustForView(Pensivenemy* this, PlayState* play, s16 baseYaw) {
    s16 yawTowardsViewEye, yawDiff, yawAlignedWithViewEye, adjustedYaw;
    f32 t;
    f32 t_threshold = 0.8f;

    yawTowardsViewEye = Actor_WorldYawTowardPoint(&this->actor, &play->view.eye);

    yawDiff = (s16)(baseYaw - yawTowardsViewEye);
    if (ABS(yawDiff) < 0x4000) {
        yawAlignedWithViewEye = yawTowardsViewEye;
    } else {
        yawAlignedWithViewEye = yawTowardsViewEye + 0x8000;
    }

    yawDiff = (s16)(baseYaw - yawAlignedWithViewEye);
    t = (f32)ABS(yawDiff) / (f32)0x4000;
    // t = 0: actor/viewEye/base aligned
    // t = 1: at a straight turn
    adjustedYaw = LERP16(baseYaw, yawAlignedWithViewEye, CLAMP_MIN(t - t_threshold, 0.0f) / (1.0f - t_threshold) / 6);

    return adjustedYaw;
}

void Pensivenemy_SetShapePitchYawAdjustForView(Pensivenemy* this, PlayState* play, s16 basePitch, s16 baseYaw) {
    s16 pitch, yaw, yawTowardsViewEye, yawDiff;

    pitch = Pensivenemy_PitchAdjustForView(this, play, basePitch);
    yaw = Pensivenemy_YawAdjustForView(this, play, baseYaw);

    yawTowardsViewEye = Actor_WorldYawTowardPoint(&this->actor, &play->view.eye);
    yawDiff = (s16)(yaw - yawTowardsViewEye);
    if (ABS(yawDiff) > 0x4000) {
        // reverse the pitch orientation
        // TODO this probably only works when basePitch==0
        pitch = -pitch;
    }

    Math_StepToBinang(&this->actor.shape.rot.x, pitch, 0x800);

    // since the model has central symmetry,
    // we can make the yaw jump by 0x8000 if it brings us closer to the target
    // (and it doesn't look weird visually)
    yawDiff = (s16)(this->actor.shape.rot.y - yaw);
    if (ABS(yawDiff) > 0x4000) {
        // this->actor.shape.rot.y += 0x8000;

        // (TODO: what about lighting? the normals aren't centrally symmetric?)
        // to solve this TODO, turn around the target instead of the current yaw
        // (but it means the side of the actor visually facing the player will change)
        yaw += 0x8000;
    }

    Math_StepToBinang(&this->actor.shape.rot.y, yaw, 0x800);
}

/*
 * face
 */

PENSIVENEMY_ACTIONFUNC(Pensivenemy_FaceInit) {
    this->next_attack_cooldown = SECONDS_TO_FRAMES(0);
    this->func_action = Pensivenemy_Face;
    this->func_draw = Pensivenemy_FaceDraw;
    this->func_gfxprint = Pensivenemy_FaceGfxPrint;
}

PENSIVENEMY_ACTIONFUNC(Pensivenemy_Face) {
    Pensivenemy_SetShapePitchYawAdjustForView(this, play, 0, this->actor.yawTowardsPlayer);

    if (this->actor.xyzDistToPlayerSq < SQ(100.0f)) {
        // TODO hand crush
    } else if (this->actor.xyzDistToPlayerSq < SQ(1000.0f)) {
        this->next_attack_cooldown--;
        if (this->next_attack_cooldown <= 0) {
            Pensivenemy* hand;

            this->next_attack_cooldown = SECONDS_TO_FRAMES(this->newHandProjsCooldownSeconds);

            hand = (Pensivenemy*)Actor_Spawn(
                &play->actorCtx, play, ACTOR_ORI_PENSIVENEMY, XYZ(this->actor.world.pos),
                // rot
                0, this->actor.yawTowardsPlayer, 0,
                // params
                PENSIVEENEMY_PARAMS(PENSIVENEMY_HAND_PROJ, PENSIVEENEMY_PROFILEID(this->actor.params)));

            if (hand != NULL) {
                Player* player = GET_PLAYER(play);
                Vec3f faceToPlayer;
                f32 faceToPlayerDist;

                Math_Vec3f_Diff(&player->bodyPartsPos[PLAYER_BODYPART_TORSO], &this->actor.world.pos, &faceToPlayer);
                faceToPlayerDist = Math3D_Vec3fMagnitude(&faceToPlayer);
                // avoid division by zero (but the xyzDistToPlayerSq logic should prevent that anyway)
                faceToPlayerDist = CLAMP_MIN(faceToPlayerDist, 0.001f);
                Math_Vec3f_Scale(&faceToPlayer, this->newHandProjsSpeed / faceToPlayerDist);
                hand->actor.velocity = faceToPlayer;
            }
        }
    }
}

#include "assets/original/pensivenemy/exported/gPensivenemyPensiveFaceDL.c"

PENSIVENEMY_ACTIONFUNC(Pensivenemy_FaceDraw) {
    Gfx_DrawDListOpa(play, gPensivenemyPensiveFaceDL);
}

void Pensivenemy_FaceGfxPrint(struct Pensivenemy* this, PlayState* play, GfxPrint* printer) {
    Player* player = GET_PLAYER(play);
    Vec3f* playerPosWorld = &player->bodyPartsPos[PLAYER_BODYPART_TORSO];

    GfxPrint_Printf(printer, " face\n");
    GfxPrint_Printf(printer, " pitch=%hx yaw=%hx\n", this->actor.shape.rot.x, this->actor.shape.rot.y);
    GfxPrint_Printf(printer, " yawTowardsPlayer=%hx\n", this->actor.yawTowardsPlayer);
    GfxPrint_Printf(printer, " this pos    =%.1f %.1f %.1f\n", XYZ(this->actor.world.pos));
    GfxPrint_Printf(printer, " player pos  =%.1f %.1f %.1f\n", XYZ(player->actor.world.pos));
    GfxPrint_Printf(printer, " player torso=%.1f %.1f %.1f\n", XYZ(player->bodyPartsPos[PLAYER_BODYPART_TORSO]));
}

/*
 * hand proj
 */

PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProjInit) {
    VEC_SET(this->actor.velocity, 0, 0, 0);
    this->func_action = Pensivenemy_HandProj;
    this->func_draw = Pensivenemy_HandProjDraw;
    this->func_gfxprint = Pensivenemy_HandGfxPrint;
}

PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProj) {
    Pensivenemy_SetShapePitchYawAdjustForView(this, play, 0, this->actor.world.rot.y);
    Math_Vec3f_Sum(&this->actor.world.pos, &this->actor.velocity, &this->actor.world.pos);

    if (this->actor.xyzDistToPlayerSq > SQ(2000.0f)) {
        Actor_Kill(&this->actor);
    }

    if (sGfxPrintOwner == this) {
        sGfxPrintDebugData.pushEffectiveness = -1.0f;
    }

    if (this->actor.xyzDistToPlayerSq < this->displaceActiveRadiusSq) { // for performance
        Player* player = GET_PLAYER(play);
        Vec3f playerPosWorld = player->bodyPartsPos[PLAYER_BODYPART_TORSO];
        Vec3f playerPosWorldFromActor, playerPosLocal;
        f32 distSide, speedXZ;

        playerPosWorld.y = CLAMP_MIN(playerPosWorld.y, player->actor.world.pos.y + LINK_ADULT_HEIGHT / 3.0f);

        Math_Vec3f_Diff(&playerPosWorld, &this->actor.world.pos, &playerPosWorldFromActor);

        Matrix_Push();
        Matrix_RotateY(BINANG_TO_RAD(-this->actor.world.rot.y), MTXMODE_NEW);
        Matrix_MultVec3f(&playerPosWorldFromActor, &playerPosLocal);
        Matrix_Pop();

        if (sGfxPrintOwner == this) {
            sGfxPrintDebugData.playerPosLocal = playerPosLocal;
        }

        distSide = ABS(playerPosLocal.x);

        speedXZ = sqrt(SQXZ(this->actor.velocity));

        // between x units below and y units above
        if (-(this->height_in_units / 2.0f + LINK_ADULT_HEIGHT - LINK_ADULT_TORSO_Y - OVERLAP_HEIGHT_FOR_PUSH) <
                playerPosLocal.y &&
            playerPosLocal.y < (LINK_ADULT_TORSO_Y + this->height_in_units / 2.0f - OVERLAP_HEIGHT_FOR_PUSH)) {
            f32 distSideMax = this->width_in_units * 0.75f / 2.0f + LINK_ADULT_TORSO_WIDTH / 2.0f;

            // between x units right and x units left
            // looks OK with: width_in_units=40 x=30
            if (distSide < distSideMax) {
                // between x units behind and y units in front
                // have these scale with speed so the player can't "clip" through
                if (-(LINK_ADULT_WIDTH / 2 + speedXZ) < playerPosLocal.z &&
                    playerPosLocal.z < (LINK_ADULT_WIDTH / 2 + speedXZ)) {
                    f32 distSide_threshold, f, pushEffectiveness;
                    Vec3f playerDisplacementLocal, playerDisplacementWorld;

                    distSide_threshold = 0.2f * distSideMax;
                    f = CLAMP_MIN(distSide - distSide_threshold, 0.0f) / (distSideMax - distSide_threshold);
                    pushEffectiveness = 1.0f - f / 2.0f;

                    playerDisplacementLocal.x =
                        (playerPosLocal.x < 0 ? -1.0f : 1.0f) * ((1.0f - pushEffectiveness) * speedXZ * 0.2f);
                    playerDisplacementLocal.y = 0.0f;
                    playerDisplacementLocal.z = pushEffectiveness * speedXZ;

                    {
                        f32 targetLocalZ = 13.0f;
                        f32 zDiff = playerPosLocal.z + playerDisplacementLocal.z - targetLocalZ;

                        playerDisplacementLocal.z -= pushEffectiveness * CLAMP(zDiff, -speedXZ, speedXZ);
                    };

                    Matrix_Push();
                    Matrix_RotateY(BINANG_TO_RAD(this->actor.world.rot.y), MTXMODE_NEW);
                    Matrix_MultVec3f(&playerDisplacementLocal, &playerDisplacementWorld);
                    Matrix_Pop();

                    Math_Vec3f_Sum(&player->actor.world.pos, &playerDisplacementWorld, &player->actor.world.pos);

                    Math_StepToF(&this->actor.velocity.y, playerPosLocal.y / 10.0f * speedXZ / 5.0f,
                                 0.2f * speedXZ / 5.0f);

                    if (sGfxPrintOwner == this) {
                        sGfxPrintDebugData.pushEffectiveness = pushEffectiveness;
                        sGfxPrintDebugData.speedXZ = speedXZ;
                        sGfxPrintDebugData.playerDisplacementLocal = playerDisplacementLocal;
                        sGfxPrintDebugData.playerDisplacementWorld = playerDisplacementWorld;
                    }
                }
            }
        }
    }
}

#include "assets/original/pensivenemy/exported/gPensivenemyRaisedHandDL.c"

PENSIVENEMY_ACTIONFUNC(Pensivenemy_HandProjDraw) {
    Gfx_DrawDListOpa(play, gPensivenemyRaisedHandDL);
}

void Pensivenemy_HandGfxPrint(struct Pensivenemy* this, PlayState* play, GfxPrint* printer) {
    GfxPrint_Printf(printer, " hand\n");
    GfxPrint_Printf(printer, " pitch=%hx yaw=%hx\n", this->actor.shape.rot.x, this->actor.shape.rot.y);
    GfxPrint_Printf(printer, " yawTowardsPlayer=%hx\n", this->actor.yawTowardsPlayer);
    GfxPrint_Printf(printer, " pushEffectiveness=%f\n", sGfxPrintDebugData.pushEffectiveness);
    GfxPrint_Printf(printer, " speedXZ=%f\n", sGfxPrintDebugData.speedXZ);
    GfxPrint_Printf(printer, " speedZboost=%f\n", sGfxPrintDebugData.speedZboost);
    GfxPrint_Printf(printer, " velocity = %.1f %.1f %.1f\n", XYZ(this->actor.velocity));
    GfxPrint_Printf(printer, " playerPosLocal = %.1f %.1f %.1f\n", XYZ(sGfxPrintDebugData.playerPosLocal));
    GfxPrint_Printf(printer, " playerDisplacementLocal = %.1f %.1f %.1f\n",
                    XYZ(sGfxPrintDebugData.playerDisplacementLocal));
    GfxPrint_Printf(printer, " playerDisplacementWorld = %.1f %.1f %.1f\n",
                    XYZ(sGfxPrintDebugData.playerDisplacementWorld));
}
