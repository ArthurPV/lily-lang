#include <lang/runtime/Int16.h>
#include <stdio.h>

LILY_API I16
add__Int16(I16 x, I16 y)
{
    if (y == 0)
        return x;

    return add__Int16(x ^ y, (x & y) << 1);
}

LILY_API I16
sub__Int16(I16 x, I16 y)
{
    if (y == 0)
        return x;

    return sub__Int16(x ^ y, ((~x) & y) << 1);
}

LILY_API I16
mul__Int16(I16 x, I16 y)
{
    MutI16 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API I16
div__Int16(I16 x, I16 y)
{
    return 0;
}

LILY_API I16
mod__Int16(I16 x, I16 y)
{
    return (div__Int16(add__Int16(x, x), y)) & y;
}

LILY_API I16
exp__Int16(I16 x, I16 y)
{
    if (y < 0)
        return 0;

    MutI16 res = x;

    for (MutI16 i = y; i--;)
        res *= x;

    return res;
}

LILY_API I16
l_shift__Int16(I16 x, I16 y)
{
    return x << y;
}

LILY_API I16
r_shift__Int16(I16 x, I16 y)
{
    return x >> y;
}

LILY_API I16
bit_or__Int16(I16 x, I16 y)
{
    return x | y;
}

LILY_API I16
bit_and__Int16(I16 x, I16 y)
{
    return x & y;
}

LILY_API I16
bit_xor__Int16(I16 x, I16 y)
{
    return x ^ y;
}

LILY_API I16
bit_not__Int16(I16 x)
{
    return ~x;
}

LILY_API Unit
assign__Int16(MutI16 *var, I16 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Int16(MutI16 *x, I16 y)
{
    *x = add__Int16(*x, y);
}

LILY_API Unit
sub_assign__Int16(MutI16 *x, I16 y)
{
    *x = sub__Int16(*x, y);
}

LILY_API Unit
mul_assign__Int16(MutI16 *x, I16 y)
{
    *x = mul__Int16(*x, y);
}

LILY_API Unit
div_assign__Int16(MutI16 *x, I16 y)
{
    *x = div__Int16(*x, y);
}

LILY_API Unit
mod_assign__Int16(MutI16 *x, I16 y)
{
    *x = mod__Int16(*x, y);
}

LILY_API Unit
exp_assign__Int16(MutI16 *x, I16 y)
{
    *x = exp__Int16(*x, y);
}

LILY_API Unit
l_shift_assign__Int16(MutI16 *x, I16 y)
{
    *x = l_shift__Int16(*x, y);
}

LILY_API Unit
r_shift_assign__Int16(MutI16 *x, I16 y)
{
    *x = r_shift__Int16(*x, y);
}

LILY_API Unit
bit_or_assign__Int16(MutI16 *x, I16 y)
{
    *x = bit_or__Int16(*x, y);
}

LILY_API Unit
bit_and_assign__Int16(MutI16 *x, I16 y)
{
    *x = bit_and__Int16(*x, y);
}

LILY_API Unit
bit_xor_assign__Int16(MutI16 *x, I16 y)
{
    *x = bit_xor__Int16(*x, y);
}

LILY_API struct Range range__Int16(MutI16 x, MutI16 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool lt__Int16(I16 x, I16 y)
{
    return x < y;
}

LILY_API Bool gt__Int16(I16 x, I16 y)
{
    return x > y;
}

LILY_API Bool le__Int16(I16 x, I16 y)
{
    return x <= y;
}

LILY_API Bool ge__Int16(I16 x, I16 y)
{
    return x >= y;
}
