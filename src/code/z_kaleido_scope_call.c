#include "global.h"
#include "vt.h"

void (*sKaleidoScopeUpdateFunc)(PlayState* play);
void (*sKaleidoScopeDrawFunc)(PlayState* play);
f32 gBossMarkScale;
u32 D_8016139C;
PauseMapMarksData* gLoadedPauseMarkDataTable;

extern void KaleidoScope_Update(PlayState* play);
extern void KaleidoScope_Draw(PlayState* play);

void KaleidoScopeCall_LoadPlayer(void) {
    KaleidoMgrOverlay* playerActorOvl = &gKaleidoMgrOverlayTable[KALEIDO_OVL_PLAYER_ACTOR];

    if (gKaleidoMgrCurOvl != playerActorOvl) {
        if (gKaleidoMgrCurOvl != NULL) {
            osSyncPrintf(VT_FGCOL(GREEN));
            osSyncPrintf("カレイド領域 強制排除\n"); // "Kaleido area forced exclusion"
            osSyncPrintf(VT_RST);

            KaleidoManager_ClearOvl(gKaleidoMgrCurOvl);
        }

        osSyncPrintf(VT_FGCOL(GREEN));
        osSyncPrintf("プレイヤーアクター搬入\n"); // "Player actor import"
        osSyncPrintf(VT_RST);

        KaleidoManager_LoadOvl(playerActorOvl);
    }
}

void KaleidoScopeCall_Init(PlayState* play) {
    // "Kaleidoscope replacement construction"
    osSyncPrintf("カレイド・スコープ入れ替え コンストラクト \n");

    sKaleidoScopeUpdateFunc = KaleidoManager_GetRamAddr(KaleidoScope_Update);
    sKaleidoScopeDrawFunc = KaleidoManager_GetRamAddr(KaleidoScope_Draw);

    LOG_ADDRESS("kaleido_scope_move", KaleidoScope_Update, "../z_kaleido_scope_call.c", 98);
    LOG_ADDRESS("kaleido_scope_move_func", sKaleidoScopeUpdateFunc, "../z_kaleido_scope_call.c", 99);
    LOG_ADDRESS("kaleido_scope_draw", KaleidoScope_Draw, "../z_kaleido_scope_call.c", 100);
    LOG_ADDRESS("kaleido_scope_draw_func", sKaleidoScopeDrawFunc, "../z_kaleido_scope_call.c", 101);

    KaleidoSetup_Init(play);
}

void KaleidoScopeCall_Destroy(PlayState* play) {
    // "Kaleidoscope replacement destruction"
    osSyncPrintf("カレイド・スコープ入れ替え デストラクト \n");

    KaleidoSetup_Destroy(play);
}

void KaleidoScopeCall_Update(PlayState* play) {
    KaleidoMgrOverlay* kaleidoScopeOvl = &gKaleidoMgrOverlayTable[KALEIDO_OVL_KALEIDO_SCOPE];
    PauseContext* pauseCtx = &play->pauseCtx;

    if ((pauseCtx->state != PAUSE_STATE_OFF) || (pauseCtx->debugState != 0)) {
        if (pauseCtx->state == PAUSE_STATE_WAIT_LETTERBOX) {
            if (Letterbox_GetSize() == 0) {
                HREG(80) = 7;
                HREG(82) = 3;
                R_PAUSE_BG_PRERENDER_STATE = PAUSE_BG_PRERENDER_DRAW;
                pauseCtx->mainState = PAUSE_MAIN_STATE_IDLE;
                pauseCtx->savePromptSubState = PAUSE_SAVE_PROMPT_STATE_APPEARING;
                pauseCtx->state = (pauseCtx->state & 0xFFFF) + 1; // PAUSE_STATE_WAIT_BG_PRERENDER
            }
        } else if (pauseCtx->state == PAUSE_STATE_8) {
            HREG(80) = 7;
            HREG(82) = 3;
            R_PAUSE_BG_PRERENDER_STATE = PAUSE_BG_PRERENDER_DRAW;
            pauseCtx->mainState = PAUSE_MAIN_STATE_IDLE;
            pauseCtx->savePromptSubState = PAUSE_SAVE_PROMPT_STATE_APPEARING;
            pauseCtx->state = (pauseCtx->state & 0xFFFF) + 1; // PAUSE_STATE_9
        } else if ((pauseCtx->state == PAUSE_STATE_WAIT_BG_PRERENDER) || (pauseCtx->state == PAUSE_STATE_9)) {
            osSyncPrintf("PR_KAREIDOSCOPE_MODE=%d\n", R_PAUSE_BG_PRERENDER_STATE);

            if (R_PAUSE_BG_PRERENDER_STATE >= PAUSE_BG_PRERENDER_DONE) {
                pauseCtx->state++; // PAUSE_STATE_INIT or PAUSE_STATE_10
            }
        } else if (pauseCtx->state != PAUSE_STATE_OFF) {
            if (gKaleidoMgrCurOvl != kaleidoScopeOvl) {
                if (gKaleidoMgrCurOvl != NULL) {
                    osSyncPrintf(VT_FGCOL(GREEN));
                    // "Kaleido area Player Forced Elimination"
                    osSyncPrintf("カレイド領域 プレイヤー 強制排除\n");
                    osSyncPrintf(VT_RST);

                    KaleidoManager_ClearOvl(gKaleidoMgrCurOvl);
                }

                osSyncPrintf(VT_FGCOL(GREEN));
                // "Kaleido area Kaleidoscope loading"
                osSyncPrintf("カレイド領域 カレイドスコープ搬入\n");
                osSyncPrintf(VT_RST);

                KaleidoManager_LoadOvl(kaleidoScopeOvl);
            }

            if (gKaleidoMgrCurOvl == kaleidoScopeOvl) {
                sKaleidoScopeUpdateFunc(play);

                if ((play->pauseCtx.state == PAUSE_STATE_OFF) && (play->pauseCtx.debugState == 0)) {
                    osSyncPrintf(VT_FGCOL(GREEN));
                    // "Kaleido area Kaleidoscope Emission"
                    osSyncPrintf("カレイド領域 カレイドスコープ排出\n");
                    osSyncPrintf(VT_RST);

                    KaleidoManager_ClearOvl(kaleidoScopeOvl);
                    KaleidoScopeCall_LoadPlayer();
                }
            }
        }
    }
}

void KaleidoScopeCall_Draw(PlayState* play) {
    KaleidoMgrOverlay* kaleidoScopeOvl = &gKaleidoMgrOverlayTable[KALEIDO_OVL_KALEIDO_SCOPE];

    if (R_PAUSE_BG_PRERENDER_STATE >= PAUSE_BG_PRERENDER_DONE) {
        if (((play->pauseCtx.state >= PAUSE_STATE_OPENING_1) && (play->pauseCtx.state <= PAUSE_STATE_SAVE_PROMPT)
             /* PAUSE_STATE_OPENING_1, PAUSE_STATE_OPENING_2, PAUSE_STATE_MAIN, PAUSE_STATE_SAVE_PROMPT */) ||
            ((play->pauseCtx.state >= PAUSE_STATE_11) && (play->pauseCtx.state <= PAUSE_STATE_CLOSING)
             /* PAUSE_STATE_11, PAUSE_STATE_12, PAUSE_STATE_13, PAUSE_STATE_14, PAUSE_STATE_15,
                PAUSE_STATE_16, PAUSE_STATE_17, PAUSE_STATE_CLOSING */
             )) {
            if (gKaleidoMgrCurOvl == kaleidoScopeOvl) {
                sKaleidoScopeDrawFunc(play);
            }
        }
    }
}
