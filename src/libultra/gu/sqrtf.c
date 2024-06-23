#include "global.h"

#ifndef __GNUC__
#define __builtin_sqrtf sqrtf
#endif

f32 sqrtf_libultra(f32 f) {
    return __builtin_sqrtf(f);
}
