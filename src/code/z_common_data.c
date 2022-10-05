#include "global.h"

// The use of ALIGNED8 here is just a temporary solution until the SaveContext is re-structured
ALIGNED8 SaveContext GET_G_SAVE_CONTEXT;
u32 D_8015FA88;
u32 D_8015FA8C;

void SaveContext_Init(void) {
    bzero(&GET_G_SAVE_CONTEXT, sizeof(GET_G_SAVE_CONTEXT));
    D_8015FA88 = 0;
    D_8015FA8C = 0;
    SET_SEQ_ID((u8)NA_BGM_DISABLED)
    SET_NATURE_AMBIENCE_ID(NATURE_ID_DISABLED)
    SET_FORCED_SEQ_ID(NA_BGM_GENERAL_SFX)
    SET_NEXT_CUTSCENE_INDEX(0xFFEF)
    SET_CUTSCENE_TRIGGER(0)
    SET_CHAMBER_CUTSCENE_NUM(0)
    SET_NEXT_DAY_TIME(NEXT_TIME_NONE)
    SET_SKYBOX_TIME(0)
    SET_DOG_IS_LOST(true)
    SET_NEXT_TRANSITION_TYPE(TRANS_NEXT_TYPE_DEFAULT)
    SET_UNK_13_E_E(50)
}
