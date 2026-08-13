#ifndef SPEEDMETER_H
#define SPEEDMETER_H

#include "ultra64.h"

struct GraphicsContext;
struct GameState;

typedef struct SpeedMeter {
    /* 0x00 */ char unk_00[0x18];
    /* 0x18 */ s32 x; // Unused
    /* 0x1C */ s32 y;
} SpeedMeter; // size = 0x20

extern volatile u64 gAudioThreadUpdateTimeTotalPerGfxTask;
extern volatile u64 gGfxTaskSentToNextReadyMinusAudioThreadUpdateTime;
extern volatile u64 gRSPAudioTimeTotal;
extern volatile u64 gRSPGfxTimeTotal;
extern volatile u64 gRDPTimeTotal;
extern volatile u64 gGraphUpdatePeriod;
extern volatile u64 gAudioThreadUpdateTimeStart;
extern volatile u64 gAudioThreadUpdateTimeAcc;
extern volatile u64 gRSPAudioTimeAcc;
extern volatile u64 gRSPGfxTimeAcc;
extern volatile u64 gRSPOtherTimeAcc;
extern volatile u64 D_8016A578;
extern volatile u64 gRDPTimeAcc;

void SpeedMeter_Init(SpeedMeter* this);
void SpeedMeter_Destroy(SpeedMeter* this);
void SpeedMeter_DrawTimeEntries(SpeedMeter* this, struct GraphicsContext* gfxCtx);
void SpeedMeter_DrawAllocEntries(SpeedMeter* meter, struct GraphicsContext* gfxCtx, struct GameState* state);

#endif
