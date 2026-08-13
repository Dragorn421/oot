#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include "game_audio.h"

typedef enum AudioMgrActivityLevel {
    /* 0 */ AUDIOMGR_ACTIVITY_LEVEL_ALL,
    /* 1 */ AUDIOMGR_ACTIVITY_LEVEL_NO_RSP,
    /* 2 */ AUDIOMGR_ACTIVITY_LEVEL_NO_UPDATE
} AudioMgrActivityLevel;

typedef struct AudioMgr {
    /* 0x0008 */ OSTask audioTask;
    /* 0x0070 */ AudioTask* rspTask;
} AudioMgr; // size = 0x298

void AudioMgr_Init(AudioMgr* audioMgr);

void AudioMgr_WaitForInit(AudioMgr* audioMgr);

void AudioMgr_StopAllSfx(void);

#endif
