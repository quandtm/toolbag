#pragma once
#include <xmmintrin.h>
#include "commonmath.h"
#include "float2.h"
#include "float3.h"

struct vec4
{
    float x, y, z, w;
};

struct float4
{
    INLINE float4() {}
    INLINE explicit float4(float x, float y, float z, float w) { m = _mm_set_ps(w, z, y, x); }
    INLINE explicit float4(float v) { m = _mm_set1_ps(v); }
    INLINE explicit float4(const vec4 v) { m = _mm_set_ps(v.w, v.z, v.y, v.x); }
    INLINE explicit float4(const vec3 v) { m = _mm_set_ps(v.z, v.z, v.y, v.x); }
    INLINE explicit float4(const vec2 v) { m = _mm_set_ps(v.y, v.y, v.y, v.x); }
    INLINE explicit float4(const float *p) { m = _mm_set_ps(p[3], p[2], p[1], p[0]); }
    INLINE explicit float4(__m128 v) { m = v; }

    INLINE float4 float4i(const int x, const int y, const int z, const int w) { return float4((float)x, (float)y, (float)z, (float)w); }

    INLINE float x() const { return _mm_cvtss_f32(m); }
    INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    INLINE float z() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    INLINE float w() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

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
    INLINE void setW(const float z)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(z));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
        m = _mm_move_ss(t, m);
    }

    INLINE float4 xxxx() const { return float4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(0, 0, 0, 0))); }
    INLINE float4 yyyy() const { return float4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    INLINE float4 zzzz() const { return float4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
    INLINE float4 wwww() const { return float4(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }

    INLINE void store(float *p) const { p[0] = x(); p[1] = y(); p[2] = z(); p[3] = w(); }

    INLINE float operator[] (const uint32_t i) const { return m.m128_f32[i]; }
    INLINE float& operator[] (const uint32_t i) { return m.m128_f32[i]; }

    INLINE vec4 toVec4() { vec4 v; v.x = x(); v.y = y(); v.z = z(); v.w = w(); return v; }

    __m128 m;
};

typedef float4 bool4;

INLINE float4 operator+(float4 a, float4 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
INLINE float4 operator-(float4 a, float4 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
INLINE float4 operator*(float4 a, float4 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
INLINE float4 operator/(float4 a, float4 b) { a.m = _mm_div_ps(a.m, b.m); return a; }

INLINE float4 operator+(float4 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float4 operator-(float4 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float4 operator*(float4 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float4 operator/(float4 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }

INLINE float4 operator+(float a, float4 b) { b.m = _mm_add_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float4 operator-(float a, float4 b) { b.m = _mm_sub_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float4 operator*(float a, float4 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float4 operator/(float a, float4 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

INLINE float4& operator+=(float4 &a, float4 b) { a = a + b; return a; }
INLINE float4& operator-=(float4 &a, float4 b) { a = a - b; return a; }
INLINE float4& operator*=(float4 &a, float4 b) { a = a * b; return a; }
INLINE float4& operator/=(float4 &a, float4 b) { a = a / b; return a; }

INLINE float4& operator+=(float4 &a, float b) { a = a + b; return a; }
INLINE float4& operator-=(float4 &a, float b) { a = a - b; return a; }
INLINE float4& operator*=(float4 &a, float b) { a = a * b; return a; }
INLINE float4& operator/=(float4 &a, float b) { a = a / b; return a; }
INLINE float4 operator-(float4 a) { return float4(_mm_setzero_ps()) - a; }

INLINE bool4 operator==(float4 a, float4 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
INLINE bool4 operator!=(float4 a, float4 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
INLINE bool4 operator<=(float4 a, float4 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
INLINE bool4 operator>=(float4 a, float4 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
INLINE bool4 operator<(float4 a, float4 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
INLINE bool4 operator>(float4 a, float4 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }

INLINE unsigned mask(float4 a) { return _mm_movemask_ps(a.m) & 15; }
INLINE bool any(float4 a) { return mask(a) != 0; }
INLINE bool all(float4 a) { return mask(a) == 15; }

// Windows headers (or others) are #defining these and that breaks all overloads
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
float4 min(float4 a, float4 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
float4 max(float4 a, float4 b) { a.m = _mm_max_ps(a.m, b.m); return a; }

INLINE float4 abs(float4 a) { a.m = _mm_andnot_ps(vecsignbits, a.m); return a; }
INLINE float sum(float4 a) { return a.x() + a.y() + a.z(); }
INLINE float dot(float4 a, float4 b) { return sum(a * b); }
INLINE float4 clamp(float4 v, float l, float h) { return min(max(v, float4(l)), float4(h)); }
INLINE float4 saturate(float4 v) { return clamp(v, 0.0f, 1.0f); }
INLINE float lengthSq(float4 v) { return dot(v, v); }
INLINE float length(float4 v) { return sqrtf(lengthSq(v)); }
INLINE float4 normalize(float4 v) { return v / length(v); }
INLINE float4 lerp(float4 a, float4 b, float t) { return a + (b - a) * t; }