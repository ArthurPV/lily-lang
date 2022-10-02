#ifndef LILY_RUNTIME_H
#define LILY_RUNTIME_H

#include <assert.h>
#include <lang/runtime/api.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct u128_t
    {
        uint64_t x, y;
    } u128_t;

#define MAX_U128 (1 << 128) - 1

    typedef const struct u128_t U128;
    typedef struct u128_t MutU128;

    LILY_API struct u128_t __new__u128_t(uint64_t x, uint64_t y);
    LILY_API U128 add__u128_t(U128 self, U128 v);
    LILY_API U128 sub__u128_t(U128 self, U128 v);
    LILY_API U128 mul__u128_t(U128 self, U128 v);

    typedef char *MutStr;
    typedef const char *Str;

    typedef uint8_t MutU8;
    typedef uint16_t MutU16;
    typedef uint32_t MutU32;
    typedef uint64_t MutU64;

    typedef const uint8_t U8;
    typedef const uint16_t U16;
    typedef const uint32_t U32;
    typedef const uint64_t U64;

    typedef int8_t MutI8;
    typedef int16_t MutI16;
    typedef int32_t MutI32;
    typedef int64_t MutI64;
    typedef __int128_t MutI128;

    typedef const int8_t I8;
    typedef const int16_t I16;
    typedef const int32_t I32;
    typedef const int64_t I64;
    typedef const __int128_t I128;

    typedef char MutChar;
    typedef const char Char;

    typedef float MutF32;
    typedef const float F32;

    typedef double MutF64;
    typedef const double F64;

    typedef bool MutBool;
    typedef const bool Bool;

    typedef size_t MutUsize;
    typedef const size_t Usize;

    typedef long long MutIsize;
    typedef const long long Isize;

    typedef void Unit;

#define RUNTIME_ASSERT(expr) \
    if (!(expr))             \
        assert(0 && "runtime error");
#define RUNTIME_ERROR(msg)            \
    printf("Runtime Error: %s", msg); \
    exit(1)

    typedef struct Exception
    {
        void *val;
        void *ext;
    } Exception;

    LILY_API struct Exception __new__Exception(void *val, void *ext);

    typedef Exception MutExt;
    typedef const Exception Ext;

    typedef struct Optional
    {
        void *val;
    } Optional;

    LILY_API struct Optional __new__Optional(void *val);

    LILY_API void *get__Optional(struct Optional self);

    typedef Optional MutOp;
    typedef const Optional Op;

    typedef struct Range
    {
        void *start;
        void *end;
    } Range;

    LILY_API struct Range __new__Range(void *start, void *end);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_H
