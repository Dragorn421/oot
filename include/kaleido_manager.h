#ifndef KALEIDO_MANAGER_H
#define KALEIDO_MANAGER_H

#include "ultra64.h"

struct PlayState;

typedef struct KaleidoMgrOverlay {
    const char* dso_path;
    void* dso_handle;
} KaleidoMgrOverlay; // size = 0x1C

typedef enum KaleidoOverlayType {
    /* 0 */ KALEIDO_OVL_KALEIDO_SCOPE,
    /* 1 */ KALEIDO_OVL_PLAYER_ACTOR,
    /* 2 */ KALEIDO_OVL_MAX
} KaleidoOverlayType;

void KaleidoManager_LoadOvl(KaleidoMgrOverlay* ovl);
void KaleidoManager_ClearOvl(KaleidoMgrOverlay* ovl);
void KaleidoManager_Init(struct PlayState* play);
void KaleidoManager_Destroy(void);
void* KaleidoManager_GetRamAddr(const char* sym);

extern KaleidoMgrOverlay gKaleidoMgrOverlayTable[KALEIDO_OVL_MAX];
extern KaleidoMgrOverlay* gKaleidoMgrCurOvl;

void KaleidoScopeCall_LoadPlayer(void);
void KaleidoScopeCall_InitFuncPtrs(void);
void KaleidoScopeCall_ClearFuncPtrs(void);
void KaleidoScopeCall_Init(struct PlayState* play);
void KaleidoScopeCall_Destroy(struct PlayState* play);
void KaleidoScopeCall_Update(struct PlayState* play);
void KaleidoScopeCall_Draw(struct PlayState* play);

void PlayerCall_InitFuncPtrs(void);
void PlayerCall_ClearFuncPtrs(void);

#endif
