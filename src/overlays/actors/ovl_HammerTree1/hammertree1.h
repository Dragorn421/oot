#ifndef HAMMERTREE1_H
#define HAMMERTREE1_H

#include "ultra64.h"
#include "global.h"

typedef struct HammerTree1 {
    DynaPolyActor dyna;
    ColliderTris colTris;
    ColliderTrisElement colTrisItems[2];
} HammerTree1;

#endif
