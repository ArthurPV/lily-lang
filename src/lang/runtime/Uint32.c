#include <lang/runtime/Uint32.h>
#include <stdio.h>

LILY_API U32
add__Uint32(U32 x, U32 y)
{
    if (y == 0)
        return x;

    return add__Uint32(x ^ y, (x & y) << 1);
}

LILY_API U32
sub__Uint32(U32 x, U32 y)
{
    if (y == 0)
        return x;

    return sub__Uint32(x ^ y, ((~x) & y) << 1);
}

#define abs(x) -x

LILY_API U32
mul__Uint32(U32 x, U32 y)
{
    MutU32 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API U32
div__Uint32(U32 x, U32 y)
{
    return 0;
}

LILY_API U32
mod__Uint32(U32 x, U32 y)
{
    return (div__Uint32(add__Uint32(x, x), y)) & y;
}

LILY_API U32
exp__Uint32(U32 x, U32 y)
{
    if (y < 0)
        return 0;

    MutU32 res = x;

    for (MutU32 i = y; i--;)
        res *= x;

    return res;
}

LILY_API U32
l_shift__Uint32(U32 x, U32 y)
{
    return x << y;
}

LILY_API U32
r_shift__Uint32(U32 x, U32 y)
{
    return x >> y;
}

LILY_API U32
bit_or__Uint32(U32 x, U32 y)
{
    return x | y;
}

LILY_API U32
bit_and__Uint32(U32 x, U32 y)
{
    return x & y;
}

LILY_API U32
bit_xor__Uint32(U32 x, U32 y)
{
    return x ^ y;
}

LILY_API U32
bit_not__Uint32(U32 x)
{
    return ~x;
}

LILY_API Unit
assign__Uint32(MutU32 *var, U32 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Uint32(MutU32 *x, U32 y)
{
    *x = add__Uint32(*x, y);
}

LILY_API Unit
sub_assign__Uint32(MutU32 *x, U32 y)
{
    *x = sub__Uint32(*x, y);
}

LILY_API Unit
mul_assign__Uint32(MutU32 *x, U32 y)
{
    *x = mul__Uint32(*x, y);
}

LILY_API Unit
div_assign__Uint32(MutU32 *x, U32 y)
{
    *x = div__Uint32(*x, y);
}

LILY_API Unit
mod_assign__Uint32(MutU32 *x, U32 y)
{
    *x = mod__Uint32(*x, y);
}

LILY_API Unit
exp_assign__Uint32(MutU32 *x, U32 y)
{
    *x = exp__Uint32(*x, y);
}

LILY_API Unit
l_shift_assign__Uint32(MutU32 *x, U32 y)
{
    *x = l_shift__Uint32(*x, y);
}

LILY_API Unit
r_shift_assign__Uint32(MutU32 *x, U32 y)
{
    *x = r_shift__Uint32(*x, y);
}

LILY_API Unit
bit_or_assign__Uint32(MutU32 *x, U32 y)
{
    *x = bit_or__Uint32(*x, y);
}

LILY_API Unit
bit_and_assign__Uint32(MutU32 *x, U32 y)
{
    *x = bit_and__Uint32(*x, y);
}

LILY_API Unit
bit_xor_assign__Uint32(MutU32 *x, U32 y)
{
    *x = bit_xor__Uint32(*x, y);
}

LILY_API Bool
eq__Uint32(U32 x, U32 y)
{
    return x == y;
}

LILY_API Bool
ne__Uint32(U32 x, U32 y)
{
    return x != y;
}

LILY_API U32
neg__Uint32(U32 x)
{
    return -x;
}

LILY_API struct Range
range__Uint32(MutU32 x, MutU32 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Uint32(U32 x, U32 y)
{
    return x < y;
}

LILY_API Bool
gt__Uint32(U32 x, U32 y)
{
    return x > y;
}

LILY_API Bool
le__Uint32(U32 x, U32 y)
{
    return x <= y;
}

LILY_API Bool
ge__Uint32(U32 x, U32 y)
{
    return x >= y;
}