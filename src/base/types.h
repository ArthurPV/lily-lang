#ifndef LILY_TYPES_H
#define LILY_TYPES_H

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;
typedef __int128_t Int128;

typedef float Float32;
typedef double Float64;

typedef size_t Usize;
typedef char *Str;

typedef uintptr_t UPtr;

#endif // LILY_TYPES_H