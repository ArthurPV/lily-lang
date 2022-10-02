/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_RUNTIME_H
#define LILY_RUNTIME_H

#include <assert.h>
#include <lang/runtime/api.h>
#include <stdbool.h>
#if __clang_major__ < 15
#include <stdint.h>
#endif
#include <stdlib.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#if defined(__cplusplus)
extern "C"
{
#endif

#if __clang_major__ < 15
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
#endif

    typedef char *MutStr;
    typedef const char *Str;

#if __clang_major__ >= 15
    typedef unsigned _BitInt(8) MutU8;
    typedef unsigned _BitInt(16) MutU16;
    typedef unsigned _BitInt(32) MutU32;
    typedef unsigned _BitInt(64) MutU64;
    typedef unsigned _BitInt(128) MutU128;

    typedef const unsigned _BitInt(8) U8;
    typedef const unsigned _BitInt(16) U16;
    typedef const unsigned _BitInt(32) U32;
    typedef const unsigned _BitInt(64) U64;
    typedef const unsigned _BitInt(128) U128;

    typedef _BitInt(8) MutI8;
    typedef _BitInt(16) MutI16;
    typedef _BitInt(32) MutI32;
    typedef _BitInt(64) MutI64;
    typedef _BitInt(128) MutI128;

    typedef const _BitInt(8) I8;
    typedef const _BitInt(16) I16;
    typedef const _BitInt(32) I32;
    typedef const _BitInt(64) I64;
    typedef const _BitInt(128) I128;
#else
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
#endif

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
