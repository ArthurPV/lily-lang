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

#ifndef LILY_TYPES_H
#define LILY_TYPES_H

#if __clang_major__ < 15
#include <stdint.h>
#endif
#include <stdlib.h>

#if __clang_major__ >= 15
typedef unsigned _BitInt(8) UInt8;
typedef unsigned _BitInt(16) UInt16;
typedef unsigned _BitInt(32) UInt32;
typedef unsigned _BitInt(64) UInt64;
typedef unsigned _BitInt(128) UInt128;

typedef _BitInt(8) Int8;
typedef _BitInt(16) Int16;
typedef _BitInt(32) Int32;
typedef _BitInt(64) Int64;
typedef _BitInt(128) Int128;
#else
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
    UInt64 *x, *y;
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
    Int64 *x, *y;
} Int128;
#endif
#endif

typedef float Float32;
typedef double Float64;

typedef size_t Usize;
typedef ssize_t Isize;
typedef char *Str;

typedef uintptr_t UPtr;

#endif // LILY_TYPES_H
