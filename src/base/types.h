#ifndef LILY_TYPES_H
#define LILY_TYPES_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8) || \
  defined(__clang__) || defined(_MSVC_LANG)
typedef __int128_t UInt128;
#else
typedef struct UInt128
{
    UInt64 x, y;
} UInt128;
#endif

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8) || \
  defined(__clang__) || defined(_MSVC_LANG)
typedef __int128_t Int128;
#else
typedef struct Int128
{
    Int64 x, y;
} Int128;
#endif

typedef float Float32;
typedef double Float64;

typedef size_t Usize;
typedef char *Str;

typedef uintptr_t UPtr;

#endif // LILY_TYPES_H
