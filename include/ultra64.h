#ifndef ULTRA64_H
#define ULTRA64_H

#include <libdragon.h>
#include "ultra64/abi.h"
#include "ultra64/controller.h"
#include "ultra64/gbi.h"
#include "ultra64/sptask.h"
#include "ultra64/ucode.h"
#include "ultra64/ultratypes.h"

#define OS_READ 0
#define OS_WRITE 1

#define osSyncPrintf debugf

void guLookAt(Mtx*, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp);
void guLookAtHilite(Mtx* m, LookAt* l, Hilite* h, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp,
                    f32 yUp, f32 zUp, f32 xl1, f32 yl1, f32 zl1, f32 xl2, f32 yl2, f32 zl2, s32 hiliteWidth,
                    s32 hiliteHeight);
void guMtxF2L(f32 mf[4][4], Mtx* m);
void guMtxIdent(Mtx*);
void guMtxIdentF(f32 mf[4][4]);
void guMtxL2F(f32 mf[4][4], Mtx* m);
void guNormalize(f32* x, f32* y, f32* z);
void guOrtho(Mtx*, f32, f32, f32, f32, f32, f32, f32);
void guPerspective(Mtx* m, u16* perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void guPosition(Mtx*, f32, f32, f32, f32, f32, f32, f32);
void guRotate(Mtx*, f32 a, f32 x, f32 y, f32 z);
void guScale(Mtx* m, f32 x, f32 y, f32 z);
void guTranslate(Mtx* m, f32 x, f32 y, f32 z);

s16 sins(u16 angle);
s16 coss(u16 angle);

#endif
