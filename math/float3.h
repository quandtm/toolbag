#pragma once
#include <xmmintrin.h>
#include "commonmath.h"
#include "float2.h"

// Standard data type for storage
struct vec3
{
    float x, y, z;
};

struct float3
{
    INLINE float3() {}
    INLINE explicit float3(float x, float y, float z) { m = _mm_set_ps(z, z, y, x); }
    INLINE explicit float3(float v) { m = _mm_set_ps(v, v, v, v); }
    INLINE explicit float3(const vec3 v) { m = _mm_set_ps(v.z, v.z, v.y, v.x); }
    INLINE explicit float3(const vec2 v) { m = _mm_set_ps(v.y, v.y, v.y, v.x); }
    INLINE explicit float3(const float *p) { m = _mm_set_ps(p[2], p[2], p[1], p[0]); }
    INLINE explicit float3(__m128 v) { m = v; }

    INLINE float3 float3i(const int x, const int y, const int z) { return float3((float)x, (float)y, (float)z); }

    INLINE float x() const { return _mm_cvtss_f32(m); }
    INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }

    INLINE void setX(const float x) { m = _mm_move_ss(m, _mm_set_ss(x)); }
    INLINE void setY(const float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }
    INLINE void setZ(const float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
        m = _mm_move_ss(t, m);
    }

    INLINE float3 xxx() const { return float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 0, 0))); }
    INLINE float3 xzx() const { return float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 2, 0))); }
    INLINE float3 yzx() const { return float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 2, 1))); }
    INLINE float3 zxy() const { return float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 2, 0, 1))); }
    INLINE float3 zyx() const { return float3(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 1, 2))); }

    INLINE float2 xy() const { return float2(m); }

    INLINE void store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); }

    INLINE float operator[] (const uint32_t i) const { return m.m128_f32[i]; }
    INLINE float& operator[] (const uint32_t i) { return m.m128_f32[i]; }

    INLINE vec3 toVec3() { vec3 v; v.x = x(); v.y = y(); v.z = z(); return v; }

    __m128 m;
};

typedef float3 bool3;

INLINE float3 operator+(float3 a, float3 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
INLINE float3 operator-(float3 a, float3 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
INLINE float3 operator*(float3 a, float3 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
INLINE float3 operator/(float3 a, float3 b) { a.m = _mm_div_ps(a.m, b.m); return a; }

INLINE float3 operator+(float3 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float3 operator-(float3 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float3 operator*(float3 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float3 operator/(float3 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }

INLINE float3 operator+(float a, float3 b) { b.m = _mm_add_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float3 operator-(float a, float3 b) { b.m = _mm_sub_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float3 operator*(float a, float3 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float3 operator/(float a, float3 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

INLINE float3& operator+=(float3 &a, float3 b) { a = a + b; return a; }
INLINE float3& operator-=(float3 &a, float3 b) { a = a - b; return a; }
INLINE float3& operator*=(float3 &a, float3 b) { a = a * b; return a; }
INLINE float3& operator/=(float3 &a, float3 b) { a = a / b; return a; }

INLINE float3& operator+=(float3 &a, float b) { a = a + b; return a; }
INLINE float3& operator-=(float3 &a, float b) { a = a - b; return a; }
INLINE float3& operator*=(float3 &a, float b) { a = a * b; return a; }
INLINE float3& operator/=(float3 &a, float b) { a = a / b; return a; }

INLINE float3 operator-(float3 a) { return float3(_mm_setzero_ps()) - a; }

INLINE bool3 operator==(float3 a, float3 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
INLINE bool3 operator!=(float3 a, float3 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
INLINE bool3 operator<=(float3 a, float3 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
INLINE bool3 operator>=(float3 a, float3 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
INLINE bool3 operator<(float3 a, float3 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
INLINE bool3 operator>(float3 a, float3 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }

INLINE unsigned mask(float3 a) { return _mm_movemask_ps(a.m) & 7; }
INLINE bool any(float3 a) { return mask(a) != 0; }
INLINE bool all(float3 a) { return mask(a) == 7; }

// Windows headers (or others) are #defining these and that breaks all overloads
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
float3 min(float3 a, float3 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
float3 max(float3 a, float3 b) { a.m = _mm_max_ps(a.m, b.m); return a; }

INLINE float3 abs(float3 a) { a.m = _mm_andnot_ps(vecsignbits, a.m); return a; }
INLINE float3 cross(float3 a, float3 b) { return (a.zxy() * b - a * b.zxy()).zxy(); }
INLINE float sum(float3 a) { return a.x() + a.y() + a.z(); }
INLINE float dot(float3 a, float3 b) { return sum(a * b); }
INLINE float3 clamp(float3 v, float l, float h) { return min(max(v, float3(l)), float3(h)); }
INLINE float3 saturate(float3 v) { return clamp(v, 0.0f, 1.0f); }
INLINE float lengthSq(float3 v) { return dot(v, v); }
INLINE float length(float3 v) { return sqrtf(lengthSq(v)); }
INLINE float3 normalize(float3 v) { return v / length(v); }
INLINE float3 lerp(float3 a, float3 b, float t) { return a + (b - a) * t; }