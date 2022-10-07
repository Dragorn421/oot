#include "global.h"

void GameOver_Init(PlayState* play) {
    play->gameOverCtx.state = GAMEOVER_INACTIVE;
}

void GameOver_FadeInLights(PlayState* play) {
    GameOverContext* gameOverCtx = &play->gameOverCtx;

    if ((gameOverCtx->state >= GAMEOVER_DEATH_WAIT_GROUND && gameOverCtx->state < GAMEOVER_REVIVE_START) ||
        (gameOverCtx->state >= GAMEOVER_REVIVE_RUMBLE && gameOverCtx->state < GAMEOVER_REVIVE_FADE_OUT)) {
        Environment_FadeInGameOverLights(play);
    }
}

// This variable cannot be moved into this file as all of z_message_PAL rodata is in the way
extern s16 gGameOverTimer;

void GameOver_Update(PlayState* play) {
    GameOverContext* gameOverCtx = &play->gameOverCtx;
    s16 i;
    s16 j;
    s32 rumbleStrength;
    s32 rumbleDuration;
    s32 rumbleDecreaseRate;

    switch (gameOverCtx->state) {
        case GAMEOVER_DEATH_START:
            Message_CloseTextbox(play);

            SET_TIMER1STATE(0);
            SET_TIMER2STATE(0);
            CLEAR_EVENTINF(EVENTINF_10);

            // search inventory for spoiling items and revert if necessary
            for (i = 0; i < ARRAY_COUNT(gSpoilingItems); i++) {
                if (INV_CONTENT(ITEM_POCKET_EGG) == gSpoilingItems[i]) {
                    INV_CONTENT(gSpoilingItemReverts[i]) = gSpoilingItemReverts[i];

                    // search c buttons for the found spoiling item and revert if necessary
                    for (j = 1; j < ARRAY_COUNT(GET_EQUIPS_BUTTONITEMS); j++) {
                        if (GET_EQUIPS_BUTTONITEMS_A0(j) == gSpoilingItems[i]) {
                            SET_EQUIPS_BUTTONITEMS_A0(j, gSpoilingItemReverts[i]);
                            Interface_LoadItemIcon1(play, j);
                        }
                    }
                }
            }

            // restore "temporary B" to the B Button if not a sword item
            if (GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KOKIRI &&
                GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_MASTER && GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_BGS &&
                GET_EQUIPS_BUTTONITEMS_A0(0) != ITEM_SWORD_KNIFE) {

                if (GET_BUTTONSTATUS_A0(0) != BTN_ENABLED) {
                    SET_EQUIPS_BUTTONITEMS_A0(0, GET_BUTTONSTATUS_A0(0));
                } else {
                    SET_EQUIPS_BUTTONITEMS_A0(0, ITEM_NONE);
                }
            }

            SET_NAYRUSLOVETIMER(2000);
            SET_NAVITIMER(0);
            SET_SEQID((u8)NA_BGM_DISABLED);
            SET_NATUREAMBIENCEID(NATURE_ID_DISABLED);
            SET_EVENTINF_A0(0, 0);
            SET_EVENTINF_A0(1, 0);
            SET_EVENTINF_A0(2, 0);
            SET_EVENTINF_A0(3, 0);
            SET_BUTTONSTATUS_A0(
                0, SET_BUTTONSTATUS_A0(
                       1, SET_BUTTONSTATUS_A0(2, SET_BUTTONSTATUS_A0(3, SET_BUTTONSTATUS_A0(4, BTN_ENABLED)))));
            SET_UNK_13E7(SET_UNK_13E8(SET_UNK_13EA(SET_UNK_13EC(0))));

            Environment_InitGameOverLights(play);
            gGameOverTimer = 20;

            if (1) {}
            rumbleStrength = R_GAME_OVER_RUMBLE_STRENGTH;
            rumbleDuration = R_GAME_OVER_RUMBLE_DURATION;
            rumbleDecreaseRate = R_GAME_OVER_RUMBLE_DECREASE_RATE;

            Rumble_Request(0.0f, ((rumbleStrength > 100) ? 255 : (rumbleStrength * 255) / 100),
                           (CLAMP_MAX(rumbleDuration * 3, 255)),
                           ((rumbleDecreaseRate > 100) ? 255 : (rumbleDecreaseRate * 255) / 100));

            gameOverCtx->state = GAMEOVER_DEATH_WAIT_GROUND;
            break;

        case GAMEOVER_DEATH_WAIT_GROUND:
            break;

        case GAMEOVER_DEATH_DELAY_MENU:
            gGameOverTimer--;

            if (gGameOverTimer == 0) {
                play->pauseCtx.state = 8;
                gameOverCtx->state++;
                Rumble_Reset();
            }
            break;

        case GAMEOVER_REVIVE_START:
            gameOverCtx->state++;
            gGameOverTimer = 0;
            Environment_InitGameOverLights(play);
            Letterbox_SetSizeTarget(32);
            return;

        case GAMEOVER_REVIVE_RUMBLE:
            gGameOverTimer = 50;
            gameOverCtx->state++;

            if (1) {}
            rumbleStrength = R_GAME_OVER_RUMBLE_STRENGTH;
            rumbleDuration = R_GAME_OVER_RUMBLE_DURATION;
            rumbleDecreaseRate = R_GAME_OVER_RUMBLE_DECREASE_RATE;

            Rumble_Request(0.0f, ((rumbleStrength > 100) ? 255 : (rumbleStrength * 255) / 100),
                           (CLAMP_MAX(rumbleDuration * 3, 255)),
                           ((rumbleDecreaseRate > 100) ? 255 : (rumbleDecreaseRate * 255) / 100));
            break;

        case GAMEOVER_REVIVE_WAIT_GROUND:
            gGameOverTimer--;

            if (gGameOverTimer == 0) {
                gGameOverTimer = 64;
                gameOverCtx->state++;
            }
            break;

        case GAMEOVER_REVIVE_WAIT_FAIRY:
            gGameOverTimer--;

            if (gGameOverTimer == 0) {
                gGameOverTimer = 50;
                gameOverCtx->state++;
            }
            break;

        case GAMEOVER_REVIVE_FADE_OUT:
            Environment_FadeOutGameOverLights(play);
            gGameOverTimer--;

            if (gGameOverTimer == 0) {
                gameOverCtx->state = GAMEOVER_INACTIVE;
            }
            break;
    }
}
