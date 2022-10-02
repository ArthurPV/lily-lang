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

#include <lang/runtime/Int128.h>
#include <stdio.h>

LILY_API I128
add__Int128(I128 x, I128 y)
{
    if (y == 0)
        return x;

    return add__Int128(x ^ y, (x & y) << 1);
}

LILY_API I128
sub__Int128(I128 x, I128 y)
{
    if (y == 0)
        return x;

    return sub__Int128(x ^ y, ((~x) & y) << 1);
}

#define abs(x) -x

LILY_API I128
mul__Int128(I128 x, I128 y)
{
    MutI128 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API I128
div__Int128(I128 x, I128 y)
{
    return 0;
}

LILY_API I128
mod__Int128(I128 x, I128 y)
{
    return (div__Int128(add__Int128(x, x), y)) & y;
}

LILY_API I128
exp__Int128(I128 x, I128 y)
{
    if (y < 0)
        return 0;

    MutI128 res = x;

    for (MutI128 i = y; i--;)
        res *= x;

    return res;
}

LILY_API I128
l_shift__Int128(I128 x, I128 y)
{
    return x << y;
}

LILY_API I128
r_shift__Int128(I128 x, I128 y)
{
    return x >> y;
}

LILY_API I128
bit_or__Int128(I128 x, I128 y)
{
    return x | y;
}

LILY_API I128
bit_and__Int128(I128 x, I128 y)
{
    return x & y;
}

LILY_API I128
bit_xor__Int128(I128 x, I128 y)
{
    return x ^ y;
}

LILY_API I128
bit_not__Int128(I128 x)
{
    return ~x;
}

LILY_API Unit
assign__Int128(MutI128 *var, I128 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Int128(MutI128 *x, I128 y)
{
    *x = add__Int128(*x, y);
}

LILY_API Unit
sub_assign__Int128(MutI128 *x, I128 y)
{
    *x = sub__Int128(*x, y);
}

LILY_API Unit
mul_assign__Int128(MutI128 *x, I128 y)
{
    *x = mul__Int128(*x, y);
}

LILY_API Unit
div_assign__Int128(MutI128 *x, I128 y)
{
    *x = div__Int128(*x, y);
}

LILY_API Unit
mod_assign__Int128(MutI128 *x, I128 y)
{
    *x = mod__Int128(*x, y);
}

LILY_API Unit
exp_assign__Int128(MutI128 *x, I128 y)
{
    *x = exp__Int128(*x, y);
}

LILY_API Unit
l_shift_assign__Int128(MutI128 *x, I128 y)
{
    *x = l_shift__Int128(*x, y);
}

LILY_API Unit
r_shift_assign__Int128(MutI128 *x, I128 y)
{
    *x = r_shift__Int128(*x, y);
}

LILY_API Unit
bit_or_assign__Int128(MutI128 *x, I128 y)
{
    *x = bit_or__Int128(*x, y);
}

LILY_API Unit
bit_and_assign__Int128(MutI128 *x, I128 y)
{
    *x = bit_and__Int128(*x, y);
}

LILY_API Unit
bit_xor_assign__Int128(MutI128 *x, I128 y)
{
    *x = bit_xor__Int128(*x, y);
}

LILY_API struct Range
range__Int128(MutI128 x, MutI128 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Int128(I128 x, I128 y)
{
    return x < y;
}

LILY_API Bool
gt__Int128(I128 x, I128 y)
{
    return x > y;
}

LILY_API Bool
le__Int128(I128 x, I128 y)
{
    return x <= y;
}

LILY_API Bool
ge__Int128(I128 x, I128 y)
{
    return x >= y;
}
