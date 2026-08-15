#ifndef Z_GAME_DLFTBLS_H
#define Z_GAME_DLFTBLS_H

#include "ultra64.h"

typedef struct GameStateOverlay {
    void* dso_handle;
    const char* dso_path;      // if a dso
    const char* init_sym_name; // if a dso
    void* init;
    u32 instanceSize;
} GameStateOverlay; // size = 0x30

// Used in Graph_GetNextGameState in graph.c
#define DEFINE_GAMESTATE_INTERNAL(typeName, enumName) enumName,
#define DEFINE_GAMESTATE(typeName, enumName, name) DEFINE_GAMESTATE_INTERNAL(typeName, enumName)
typedef enum GameStateId {
#include "tables/gamestate_table.h"
    GAMESTATE_ID_MAX
} GameStateId;
#undef DEFINE_GAMESTATE
#undef DEFINE_GAMESTATE_INTERNAL

extern GameStateOverlay gGameStateOverlayTable[GAMESTATE_ID_MAX];

void Overlay_LoadGameState(GameStateOverlay* overlayEntry);
void Overlay_FreeGameState(GameStateOverlay* overlayEntry);

#endif
