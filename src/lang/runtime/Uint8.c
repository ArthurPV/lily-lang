#include <lang/runtime/Uint8.h>
#include <stdio.h>

LILY_API U8
add__Uint8(U8 x, U8 y)
{
    if (y == 0)
        return x;

    return add__Uint8(x ^ y, (x & y) << 1);
}

LILY_API U8
sub__Uint8(U8 x, U8 y)
{
    if (y == 0)
        return x;

    return sub__Uint8(x ^ y, ((~x) & y) << 1);
}

#define abs(x) -x

LILY_API U8
mul__Uint8(U8 x, U8 y)
{
    MutU8 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API U8
div__Uint8(U8 x, U8 y)
{
    return 0;
}

LILY_API U8
mod__Uint8(U8 x, U8 y)
{
    return (div__Uint8(add__Uint8(x, x), y)) & y;
}

LILY_API U8
exp__Uint8(U8 x, U8 y)
{
    if (y < 0)
        return 0;

    MutU8 res = x;

    for (MutU8 i = y; i--;)
        res *= x;

    return res;
}

LILY_API U8
l_shift__Uint8(U8 x, U8 y)
{
    return x << y;
}

LILY_API U8
r_shift__Uint8(U8 x, U8 y)
{
    return x >> y;
}

LILY_API U8
bit_or__Uint8(U8 x, U8 y)
{
    return x | y;
}

LILY_API U8
bit_and__Uint8(U8 x, U8 y)
{
    return x & y;
}

LILY_API U8
bit_xor__Uint8(U8 x, U8 y)
{
    return x ^ y;
}

LILY_API U8
bit_not__Uint8(U8 x)
{
    return ~x;
}

LILY_API Unit
assign__Uint8(MutU8 *var, U8 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Uint8(MutU8 *x, U8 y)
{
    *x = add__Uint8(*x, y);
}

LILY_API Unit
sub_assign__Uint8(MutU8 *x, U8 y)
{
    *x = sub__Uint8(*x, y);
}

LILY_API Unit
mul_assign__Uint8(MutU8 *x, U8 y)
{
    *x = mul__Uint8(*x, y);
}

LILY_API Unit
div_assign__Uint8(MutU8 *x, U8 y)
{
    *x = div__Uint8(*x, y);
}

LILY_API Unit
mod_assign__Uint8(MutU8 *x, U8 y)
{
    *x = mod__Uint8(*x, y);
}

LILY_API Unit
exp_assign__Uint8(MutU8 *x, U8 y)
{
    *x = exp__Uint8(*x, y);
}

LILY_API Unit
l_shift_assign__Uint8(MutU8 *x, U8 y)
{
    *x = l_shift__Uint8(*x, y);
}

LILY_API Unit
r_shift_assign__Uint8(MutU8 *x, U8 y)
{
    *x = r_shift__Uint8(*x, y);
}

LILY_API Unit
bit_or_assign__Uint8(MutU8 *x, U8 y)
{
    *x = bit_or__Uint8(*x, y);
}

LILY_API Unit
bit_and_assign__Uint8(MutU8 *x, U8 y)
{
    *x = bit_and__Uint8(*x, y);
}

LILY_API Unit
bit_xor_assign__Uint8(MutU8 *x, U8 y)
{
    *x = bit_xor__Uint8(*x, y);
}

LILY_API Bool
eq__Uint8(U8 x, U8 y)
{
    return x == y;
}

LILY_API Bool
ne__Uint8(U8 x, U8 y)
{
    return x != y;
}

LILY_API U8
neg__Uint8(U8 x)
{
    return -x;
}

LILY_API struct Range
range__Uint8(MutU8 x, MutU8 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Uint8(U8 x, U8 y)
{
    return x < y;
}

LILY_API Bool
gt__Uint8(U8 x, U8 y)
{
    return x > y;
}

LILY_API Bool
le__Uint8(U8 x, U8 y)
{
    return x <= y;
}

LILY_API Bool
ge__Uint8(U8 x, U8 y)
{
    return x >= y;
}
