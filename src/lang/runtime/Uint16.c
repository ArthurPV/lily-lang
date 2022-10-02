#include <lang/runtime/Uint16.h>
#include <stdio.h>

LILY_API U16
add__Uint16(U16 x, U16 y)
{
    if (y == 0)
        return x;

    return add__Uint16(x ^ y, (x & y) << 1);
}

LILY_API U16
sub__Uint16(U16 x, U16 y)
{
    if (y == 0)
        return x;

    return sub__Uint16(x ^ y, ((~x) & y) << 1);
}

#define abs(x) -x

LILY_API U16
mul__Uint16(U16 x, U16 y)
{
    MutU16 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API U16
div__Uint16(U16 x, U16 y)
{
    return 0;
}

LILY_API U16
mod__Uint16(U16 x, U16 y)
{
    return (div__Uint16(add__Uint16(x, x), y)) & y;
}

LILY_API U16
exp__Uint16(U16 x, U16 y)
{
    if (y < 0)
        return 0;

    MutU16 res = x;

    for (MutU16 i = y; i--;)
        res *= x;

    return res;
}

LILY_API U16
l_shift__Uint16(U16 x, U16 y)
{
    return x << y;
}

LILY_API U16
r_shift__Uint16(U16 x, U16 y)
{
    return x >> y;
}

LILY_API U16
bit_or__Uint16(U16 x, U16 y)
{
    return x | y;
}

LILY_API U16
bit_and__Uint16(U16 x, U16 y)
{
    return x & y;
}

LILY_API U16
bit_xor__Uint16(U16 x, U16 y)
{
    return x ^ y;
}

LILY_API U16
bit_not__Uint16(U16 x)
{
    return ~x;
}

LILY_API Unit
assign__Uint16(MutU16 *var, U16 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Uint16(MutU16 *x, U16 y)
{
    *x = add__Uint16(*x, y);
}

LILY_API Unit
sub_assign__Uint16(MutU16 *x, U16 y)
{
    *x = sub__Uint16(*x, y);
}

LILY_API Unit
mul_assign__Uint16(MutU16 *x, U16 y)
{
    *x = mul__Uint16(*x, y);
}

LILY_API Unit
div_assign__Uint16(MutU16 *x, U16 y)
{
    *x = div__Uint16(*x, y);
}

LILY_API Unit
mod_assign__Uint16(MutU16 *x, U16 y)
{
    *x = mod__Uint16(*x, y);
}

LILY_API Unit
exp_assign__Uint16(MutU16 *x, U16 y)
{
    *x = exp__Uint16(*x, y);
}

LILY_API Unit
l_shift_assign__Uint16(MutU16 *x, U16 y)
{
    *x = l_shift__Uint16(*x, y);
}

LILY_API Unit
r_shift_assign__Uint16(MutU16 *x, U16 y)
{
    *x = r_shift__Uint16(*x, y);
}

LILY_API Unit
bit_or_assign__Uint16(MutU16 *x, U16 y)
{
    *x = bit_or__Uint16(*x, y);
}

LILY_API Unit
bit_and_assign__Uint16(MutU16 *x, U16 y)
{
    *x = bit_and__Uint16(*x, y);
}

LILY_API Unit
bit_xor_assign__Uint16(MutU16 *x, U16 y)
{
    *x = bit_xor__Uint16(*x, y);
}

LILY_API Bool
eq__Uint16(U16 x, U16 y)
{
    return x == y;
}

LILY_API Bool
ne__Uint16(U16 x, U16 y)
{
    return x != y;
}

LILY_API U16
neg__Uint16(U16 x)
{
    return -x;
}

LILY_API struct Range
range__Uint16(MutU16 x, MutU16 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Uint16(U16 x, U16 y)
{
    return x < y;
}

LILY_API Bool
gt__Uint16(U16 x, U16 y)
{
    return x > y;
}

LILY_API Bool
le__Uint16(U16 x, U16 y)
{
    return x <= y;
}

LILY_API Bool
ge__Uint16(U16 x, U16 y)
{
    return x >= y;
}
