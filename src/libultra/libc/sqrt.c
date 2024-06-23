#include "global.h"

double sqrt_libultra(double f) {
#ifndef __GNUC__
    return sqrt(f);
#else
    return __builtin_sqrt(f);
#endif
}
