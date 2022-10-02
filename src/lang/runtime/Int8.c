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

#include <lang/runtime/Int8.h>
#include <stdio.h>

LILY_API I8
add__Int8(I8 x, I8 y)
{
    if (y == 0)
        return x;

    return add__Int8(x ^ y, (x & y) << 1);
}

LILY_API I8
sub__Int8(I8 x, I8 y)
{
    if (y == 0)
        return x;

    return sub__Int8(x ^ y, ((~x) & y) << 1);
}

LILY_API I8
mul__Int8(I8 x, I8 y)
{
    MutI8 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API I8
div__Int8(I8 x, I8 y)
{
    return 0;
}

LILY_API I8
mod__Int8(I8 x, I8 y)
{
    return (div__Int8(add__Int8(x, x), y)) & y;
}

LILY_API I8
exp__Int8(I8 x, I8 y)
{
    if (y < 0)
        return 0;

    MutI8 res = x;

    for (MutI8 i = y; i--;)
        res *= x;

    return res;
}

LILY_API I8
l_shift__Int8(I8 x, I8 y)
{
    return x << y;
}

LILY_API I8
r_shift__Int8(I8 x, I8 y)
{
    return x >> y;
}

LILY_API I8
bit_or__Int8(I8 x, I8 y)
{
    return x | y;
}

LILY_API I8
bit_and__Int8(I8 x, I8 y)
{
    return x & y;
}

LILY_API I8
bit_xor__Int8(I8 x, I8 y)
{
    return x ^ y;
}

LILY_API I8
bit_not__Int8(I8 x)
{
    return ~x;
}

LILY_API Unit
assign__Int8(MutI8 *var, I8 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Int8(MutI8 *x, I8 y)
{
    *x = add__Int8(*x, y);
}

LILY_API Unit
sub_assign__Int8(MutI8 *x, I8 y)
{
    *x = sub__Int8(*x, y);
}

LILY_API Unit
mul_assign__Int8(MutI8 *x, I8 y)
{
    *x = mul__Int8(*x, y);
}

LILY_API Unit
div_assign__Int8(MutI8 *x, I8 y)
{
    *x = div__Int8(*x, y);
}

LILY_API Unit
mod_assign__Int8(MutI8 *x, I8 y)
{
    *x = mod__Int8(*x, y);
}

LILY_API Unit
exp_assign__Int8(MutI8 *x, I8 y)
{
    *x = exp__Int8(*x, y);
}

LILY_API Unit
l_shift_assign__Int8(MutI8 *x, I8 y)
{
    *x = l_shift__Int8(*x, y);
}

LILY_API Unit
r_shift_assign__Int8(MutI8 *x, I8 y)
{
    *x = r_shift__Int8(*x, y);
}

LILY_API Unit
bit_or_assign__Int8(MutI8 *x, I8 y)
{
    *x = bit_or__Int8(*x, y);
}

LILY_API Unit
bit_and_assign__Int8(MutI8 *x, I8 y)
{
    *x = bit_and__Int8(*x, y);
}

LILY_API Unit
bit_xor_assign__Int8(MutI8 *x, I8 y)
{
    *x = bit_xor__Int8(*x, y);
}

LILY_API Bool
eq__Int8(I8 x, I8 y)
{
    return x == y;
}

LILY_API Bool
ne__Int8(I8 x, I8 y)
{
    return x != y;
}

LILY_API I8
neg__Int8(I8 x)
{
    return -x;
}

LILY_API struct Range
range__Int8(MutI8 x, MutI8 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Int8(I8 x, I8 y)
{
    return x < y;
}

LILY_API Bool
gt__Int8(I8 x, I8 y)
{
    return x > y;
}

LILY_API Bool
le__Int8(I8 x, I8 y)
{
    return x <= y;
}

LILY_API Bool
ge__Int8(I8 x, I8 y)
{
    return x >= y;
}
