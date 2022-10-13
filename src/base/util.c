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

#include <base/util.h>
#include <stdbool.h>
#include <string.h>

Int64
atoi_i64(const Str s)
{
    Int64 i = 0;
    Usize pos = 0;
    bool neg = false;

    if (s[pos] == '-') {
        neg = true;
        ++pos;
    }

    while (s[pos] >= '0' && s[pos] <= '9') {
        if (neg)
            i = (10 * i) - (s[pos++] - '0');
        else
            i = (10 * i) + (s[pos++] - '0');
    }

    return i;
}

Int128
atoi_i128(const Str s)
{
    Int128 i = 0;
    Usize pos = 0;
    bool neg = false;

    if (s[pos] == '-') {
        neg = true;
        ++pos;
    }

    while (s[pos] >= '0' && s[pos] <= '9') {
        if (neg)
            i = (10 * i) - (s[pos++] - '0');
        else
            i = (10 * i) + (s[pos++] - '0');
    }

    return i;
}

UInt64
atoi_u64(const Str s)
{
    UInt64 i = 0;
    Usize pos = 0;

    while (s[pos] >= '0' && s[pos] <= '9') {
        i = (10 * i) + (s[pos++] - '0');
    }

    return i;
}
