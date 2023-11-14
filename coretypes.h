#pragma once
#include <cstdint>
#include <cstring>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

inline u64 operator""_GB(u64 GBVal)
{
    return GBVal * 1024 * 1024 * 1024;
}
inline u64 operator""_MB(u64 MBVal)
{
    return MBVal * 1024 * 1024;
}
inline u64 operator""_KB(u64 KBVal)
{
    return KBVal * 1024;
}

template <typename T> inline void ZeroMem(T& Dest)
{
    memset(&Dest, 0, sizeof(Dest));
}

template <typename T> void Swap(T& A, T& B)
{
    T tmp = A;
    A = B;
    B = tmp;
}
