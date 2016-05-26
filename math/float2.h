#pragma once
#include <xmmintrin.h>
#include "commonmath.h"

struct vec2
{
    float x, y;
};

struct float2
{
    INLINE float2() {}
    INLINE explicit float2(const float x, const float y) { m = _mm_set_ps(y, y, y, x); }
    INLINE explicit float2(const float v) { m = _mm_set_ps(v, v, v, v); }
    INLINE explicit float2(const vec2 v) { m = _mm_set_ps(v.y, v.y, v.y, v.x); }
    INLINE explicit float2(const float *p) { m = _mm_set_ps(p[1], p[1], p[1], p[0]); }
    INLINE explicit float2(__m128 v) { m = v; }

    INLINE float2 float2i(const int x, const int y) { return float2((float)x, (float)y); }

    INLINE float x() const { return _mm_cvtss_f32(m); }
    INLINE float y() const { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }

    INLINE void setx(const float x) { m = _mm_move_ss(m, _mm_set_ss(x)); }
    INLINE void sety(const float y)
    {
        __m128 t = _mm_move_ss(m, _mm_set_ss(y));
        t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
        m = _mm_move_ss(t, m);
    }

    INLINE float2 xx() const { return float2(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 0, 0))); }
    INLINE float2 yy() const { return float2(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
    INLINE float2 yx() const { return float2(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 0, 1))); }

    INLINE void store(float *p) { p[0] = x(); p[1] = y(); }

    INLINE float operator[](const uint32_t i) const { return m.m128_f32[i]; }
    INLINE float operator[](const uint32_t i) { return m.m128_f32[i]; }

    INLINE vec2 toVec2() const { vec2 v; v.x = x(); v.y = y(); return v; }

    __m128 m;
};

typedef float2 bool2;

INLINE float2 operator+(float2 a, float2 b) { a.m = _mm_add_ps(a.m, b.m); return a; }
INLINE float2 operator-(float2 a, float2 b) { a.m = _mm_sub_ps(a.m, b.m); return a; }
INLINE float2 operator*(float2 a, float2 b) { a.m = _mm_mul_ps(a.m, b.m); return a; }
INLINE float2 operator/(float2 a, float2 b) { a.m = _mm_div_ps(a.m, b.m); return a; }

INLINE float2 operator+(float2 a, float b) { a.m = _mm_add_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float2 operator-(float2 a, float b) { a.m = _mm_sub_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float2 operator*(float2 a, float b) { a.m = _mm_mul_ps(a.m, _mm_set1_ps(b)); return a; }
INLINE float2 operator/(float2 a, float b) { a.m = _mm_div_ps(a.m, _mm_set1_ps(b)); return a; }

INLINE float2 operator+(float a, float2 b) { b.m = _mm_add_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float2 operator-(float a, float2 b) { b.m = _mm_sub_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float2 operator*(float a, float2 b) { b.m = _mm_mul_ps(_mm_set1_ps(a), b.m); return b; }
INLINE float2 operator/(float a, float2 b) { b.m = _mm_div_ps(_mm_set1_ps(a), b.m); return b; }

INLINE float2& operator+=(float2 &a, float2 b) { a = a + b; return a; }
INLINE float2& operator-=(float2 &a, float2 b) { a = a - b; return a; }
INLINE float2& operator*=(float2 &a, float2 b) { a = a * b; return a; }
INLINE float2& operator/=(float2 &a, float2 b) { a = a / b; return a; }

INLINE float2& operator+=(float2 &a, float b) { a = a + b; return a; }
INLINE float2& operator-=(float2 &a, float b) { a = a - b; return a; }
INLINE float2& operator*=(float2 &a, float b) { a = a * b; return a; }
INLINE float2& operator/=(float2 &a, float b) { a = a / b; return a; }

INLINE float2 operator-(float2 a) { return float2(_mm_setzero_ps()) - a; }

INLINE bool2 operator==(float2 a, float2 b) { a.m = _mm_cmpeq_ps(a.m, b.m); return a; }
INLINE bool2 operator!=(float2 a, float2 b) { a.m = _mm_cmpneq_ps(a.m, b.m); return a; }
INLINE bool2 operator<=(float2 a, float2 b) { a.m = _mm_cmple_ps(a.m, b.m); return a; }
INLINE bool2 operator>=(float2 a, float2 b) { a.m = _mm_cmpge_ps(a.m, b.m); return a; }
INLINE bool2 operator<(float2 a, float2 b) { a.m = _mm_cmplt_ps(a.m, b.m); return a; }
INLINE bool2 operator>(float2 a, float2 b) { a.m = _mm_cmpgt_ps(a.m, b.m); return a; }

INLINE unsigned mask(float2 a) { return _mm_movemask_ps(a.m) & 3; }
INLINE bool any(float2 a) { return mask(a) != 0; }
INLINE bool all(float2 a) { return mask(a) == 3; }

// Windows headers (or others) are #defining these and that breaks all overloads
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
float2 min(float2 a, float2 b) { a.m = _mm_min_ps(a.m, b.m); return a; }
float2 max(float2 a, float2 b) { a.m = _mm_max_ps(a.m, b.m); return a; }

INLINE float2 abs(float2 a) { a.m = _mm_andnot_ps(vecsignbits, a.m); return a; }
INLINE float sum(float2 a) { return a.x() + a.y(); }
INLINE float dot(float2 a, float2 b) { return sum(a * b); }
INLINE float2 clamp(float2 v, float l, float h) { return min(max(v, float2(l)), float2(h)); }
INLINE float2 saturate(float2 v) { return clamp(v, 0.0f, 1.0f); }
INLINE float lengthSq(float2 v) { return dot(v, v); }
INLINE float length(float2 v) { return sqrtf(lengthSq(v)); }
INLINE float2 normalize(float2 v) { return v / length(v); }
INLINE float2 lerp(float2 a, float2 b, float t) { return a + (b - a) * t; }