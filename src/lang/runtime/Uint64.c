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

#include "Uint64.h"
#include <stdio.h>

LILY_API U64
add__Uint64(U64 x, U64 y)
{
    if (y == 0)
        return x;

    return add__Uint64(x ^ y, (x & y) << 1);
}

LILY_API U64
sub__Uint64(U64 x, U64 y)
{
    if (y == 0)
        return x;

    return sub__Uint64(x ^ y, ((~x) & y) << 1);
}

#define abs(x) -x

LILY_API U64
mul__Uint64(U64 x, U64 y)
{
    MutU64 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API U64
div__Uint64(U64 x, U64 y)
{
    return 0;
}

LILY_API U64
mod__Uint64(U64 x, U64 y)
{
    return (div__Uint64(add__Uint64(x, x), y)) & y;
}

LILY_API U64
exp__Uint64(U64 x, U64 y)
{
    if (y < 0)
        return 0;

    MutU64 res = x;

    for (MutU64 i = y; i--;)
        res *= x;

    return res;
}

LILY_API U64
l_shift__Uint64(U64 x, U64 y)
{
    return x << y;
}

LILY_API U64
r_shift__Uint64(U64 x, U64 y)
{
    return x >> y;
}

LILY_API U64
bit_or__Uint64(U64 x, U64 y)
{
    return x | y;
}

LILY_API U64
bit_and__Uint64(U64 x, U64 y)
{
    return x & y;
}

LILY_API U64
bit_xor__Uint64(U64 x, U64 y)
{
    return x ^ y;
}

LILY_API U64
bit_not__Uint64(U64 x)
{
    return ~x;
}

LILY_API Unit
assign__Uint64(MutU64 *var, U64 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Uint64(MutU64 *x, U64 y)
{
    *x = add__Uint64(*x, y);
}

LILY_API Unit
sub_assign__Uint64(MutU64 *x, U64 y)
{
    *x = sub__Uint64(*x, y);
}

LILY_API Unit
mul_assign__Uint64(MutU64 *x, U64 y)
{
    *x = mul__Uint64(*x, y);
}

LILY_API Unit
div_assign__Uint64(MutU64 *x, U64 y)
{
    *x = div__Uint64(*x, y);
}

LILY_API Unit
mod_assign__Uint64(MutU64 *x, U64 y)
{
    *x = mod__Uint64(*x, y);
}

LILY_API Unit
exp_assign__Uint64(MutU64 *x, U64 y)
{
    *x = exp__Uint64(*x, y);
}

LILY_API Unit
l_shift_assign__Uint64(MutU64 *x, U64 y)
{
    *x = l_shift__Uint64(*x, y);
}

LILY_API Unit
r_shift_assign__Uint64(MutU64 *x, U64 y)
{
    *x = r_shift__Uint64(*x, y);
}

LILY_API Unit
bit_or_assign__Uint64(MutU64 *x, U64 y)
{
    *x = bit_or__Uint64(*x, y);
}

LILY_API Unit
bit_and_assign__Uint64(MutU64 *x, U64 y)
{
    *x = bit_and__Uint64(*x, y);
}

LILY_API Unit
bit_xor_assign__Uint64(MutU64 *x, U64 y)
{
    *x = bit_xor__Uint64(*x, y);
}

LILY_API Bool
eq__Uint64(U64 x, U64 y)
{
    return x == y;
}

LILY_API Bool
ne__Uint64(U64 x, U64 y)
{
    return x != y;
}

LILY_API U64
neg__Uint64(U64 x)
{
    return -x;
}

LILY_API struct Range
range__Uint64(MutU64 x, MutU64 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Uint64(U64 x, U64 y)
{
    return x < y;
}

LILY_API Bool
gt__Uint64(U64 x, U64 y)
{
    return x > y;
}

LILY_API Bool
le__Uint64(U64 x, U64 y)
{
    return x <= y;
}

LILY_API Bool
ge__Uint64(U64 x, U64 y)
{
    return x >= y;
}
