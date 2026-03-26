#ifndef _Z_OBJ_TIMEBLOCK_H_
#define _Z_OBJ_TIMEBLOCK_H_

#include "ultra64.h"
#include "global.h"

struct ObjTimeblock;

typedef void (*ObjTimeblockActionFunc)(struct ObjTimeblock*, GlobalContext*);
typedef s32 (*ObjTimeblockUnk168Func)(struct ObjTimeblock*, GlobalContext*);

typedef struct ObjTimeblock {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ ObjTimeblockActionFunc unk164;
    /* 0x168 */ ObjTimeblockUnk168Func unk168;
    /* 0x16C */ s16 unk16C;
    /* 0x16E */ s16 unk16E;
    /* 0x170 */ s16 unk170;
    /* 0x172 */ u16 unk172;
    /* 0x174 */ u8 unk174;
    /* 0x175 */ u8 unk175;
    /* 0x176 */ u8 unk176;
    /* 0x177 */ u8 unk177;
    /* 0x178 */ u8 unk178;
    /* 0x179 */ char pad179[3]; /* maybe part of unk178[4]? */
} ObjTimeblock;                 // size = 0x017C

extern const ActorInit Obj_Timeblock_InitVars;

#endif
