#include "console_logo_state.h"
#include "file_select_state.h"
#include "map_select_state.h"
#include "setup_state.h"
#include "title_setup_state.h"
#include "z_game_dlftbls.h"
#include "play_state.h"

// Gamestate Overlay Table definition
#define DEFINE_GAMESTATE_INTERNAL(typeName, enumName)                                                  \
    {                                                                                                  \
        NULL, NULL, NULL, typeName##_Init, typeName##_Destroy, NULL, NULL, 0, sizeof(typeName##State), \
    },

#define DEFINE_GAMESTATE(typeName, enumName, name)                                                             \
    {                                                                                                          \
        NULL, "ovl_" #name, NULL, typeName##_Init, typeName##_Destroy, NULL, NULL, 0, sizeof(typeName##State), \
    },

GameStateOverlay gGameStateOverlayTable[] = {
#include "tables/gamestate_table.h"
};

#undef DEFINE_GAMESTATE
#undef DEFINE_GAMESTATE_INTERNAL
