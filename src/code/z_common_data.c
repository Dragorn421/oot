#include "global.h"

// The use of ALIGNED8 here is just a temporary solution until the SaveContext is re-structured
ALIGNED8 SaveContext gSaveContext;
u32 D_8015FA88;
u32 D_8015FA8C;

void SaveContext_Init(void) {
    bzero(&gSaveContext, sizeof(gSaveContext));
    D_8015FA88 = 0;
    D_8015FA8C = 0;
    SET_SEQID((u8)NA_BGM_DISABLED);
    SET_NATUREAMBIENCEID(NATURE_ID_DISABLED);
    SET_FORCEDSEQID(NA_BGM_GENERAL_SFX);
    SET_NEXTCUTSCENEINDEX(0xFFEF);
    SET_CUTSCENETRIGGER(0);
    SET_CHAMBERCUTSCENENUM(0);
    SET_NEXTDAYTIME(NEXT_TIME_NONE);
    SET_SKYBOXTIME(0);
    SET_DOGISLOST(true);
    SET_NEXTTRANSITIONTYPE(TRANS_NEXT_TYPE_DEFAULT);
    SET_UNK_13EE(50);
}
