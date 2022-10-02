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

#include <base/str.h>
#include <stdio.h>

Str
of_bool__Str(bool b)
{
    if (b)
        return "true";
    else
        return "false";
}

Str
of_int__Str(int i)
{
    Usize size = snprintf(NULL, 0, "%d", i) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%d", i);

    return s;
}

Str
of_u8__Str(UInt8 u)
{
    Usize size = snprintf(NULL, 0, "%d", u) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%d", u);

    return s;
}

Str
of_u16__Str(UInt16 u)
{
    Usize size = snprintf(NULL, 0, "%d", u) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%d", u);

    return s;
}

Str
of_u32__Str(UInt32 u)
{
    Usize size = snprintf(NULL, 0, "%d", u) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%d", u);

    return s;
}

Str
of_u64__Str(UInt64 u)
{
    Usize size = snprintf(NULL, 0, "%lu", u) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%lu", u);

    return s;
}

Str
of_i64__Str(Int64 i)
{
    Usize size = snprintf(NULL, 0, "%ld", i) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%ld", i);

    return s;
}

#include <base/macros.h>
#include <stdint.h>

#define P10_UINT64 10000000000000000000ULL
#ifdef UINT64_MAX
#else
#define UINT64_MAX 18446744073709551615ULL
#endif

Str
of_i128__Str(Int128 i)
{
    Str res = "";

    if (i > UINT64_MAX) {
        Int128 leading = i / P10_UINT64;
        Int64 trailing = i % P10_UINT64;
        res = of_i128__Str(leading);
    } else {
        Int64 i2 = i;
        res = of_i64__Str(i2);
    }

    TODO("finish");

    return res;
}

Str
of_double__Str(double d)
{
    Usize size = snprintf(NULL, 0, "%f", d) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%f", d);

    return s;
}

Str
of_char__Str(char c)
{
    Usize size = snprintf(NULL, 0, "%c", c) + 1;
    Str s = malloc(size);

    snprintf(s, size, "%c", c);

    return s;
}
