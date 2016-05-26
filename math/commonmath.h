#pragma once
#include <stdint.h>
#include <math.h>

// From codersnotes.com, pretty standard definitions plus a shuffle helper for later code
#define INLINE __forceinline
#define DEG2RAD(_a) ((_a)*M_PI/180.0f)
#define RAD2DEG(_a) ((_a)*180.0f/M_PI)

#define EXTCONST extern const __declspec(selectany)
struct vconstu
{
    union { uint32_t u[4]; __m128 v; };
    INLINE operator __m128() const { return v; }
};
EXTCONST vconstu vecsignbits = { 0x80000000, 0x80000000, 0x80000000, 0x80000000 };