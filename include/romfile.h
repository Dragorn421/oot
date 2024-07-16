#ifndef ROMFILE_H
#define ROMFILE_H

#include "ultra64.h"

typedef struct {
    // TODO remove vromStart, end
    /* 0x00 */ uintptr_t vromStart;
    /* 0x04 */ uintptr_t vromEnd;
    char* path;
} RomFile; // size = 0x8

// TODO
#define ROM_FILE(name) \
    { 0, 0, "ROM_FILE:/" #name }
#define ROM_FILE_EMPTY(name) \
    { 0, 0, NULL }
#define ROM_FILE_UNSET \
    { 0, 0, NULL }

#endif
