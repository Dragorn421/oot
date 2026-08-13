#include "array_count.h"
#include "ultra64.h"
#include "object.h"

s16 gLinkObjectIds[] = { OBJECT_LINK_BOY, OBJECT_LINK_CHILD };

u32 gObjectTableSize = ARRAY_COUNT(gObjectTable);

// Object Table definition
#define DEFINE_OBJECT(name, _1) name,
#define DEFINE_OBJECT_EMPTY(name, _1) NULL,
#define DEFINE_OBJECT_UNSET(_0) NULL,

const char* gObjectTable[] = {
#include "tables/object_table.h"
};

#undef DEFINE_OBJECT
#undef DEFINE_OBJECT_EMPTY
#undef DEFINE_OBJECT_UNSET
