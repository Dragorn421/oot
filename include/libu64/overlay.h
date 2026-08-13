#ifndef LIBU64_OVERLAY_H
#define LIBU64_OVERLAY_H

#include "ultra64.h"

extern s32 gOverlayLogSeverity;

void* Overlay_AllocateAndLoad(const char* dll_name);

#endif
