#include <lang/runtime/Int32.h>
#include <stdio.h>

LILY_API I32
add__Int32(I32 x, I32 y)
{
    if (y == 0)
        return x;

    return add__Int32(x ^ y, (x & y) << 1);
}

LILY_API I32
sub__Int32(I32 x, I32 y)
{
    if (y == 0)
        return x;

    return sub__Int32(x ^ y, ((~x) & y) << 1);
}

LILY_API I32
mul__Int32(I32 x, I32 y)
{
    MutI32 n1 = abs(x), n2 = abs(y), res = 0;
    bool neg = MIN(x, y) < 0 && MAX(x, y) >= 0 ? true : false;

    while (n2 > 0) {
        if ((n2 & 1) == 1)
            res += n1;
        n2 >>= 1;
        n1 <<= 1;
    }

    return neg ? (~(res) + 1) : res;
}

LILY_API I32
div__Int32(I32 x, I32 y)
{
    return 0;
}

LILY_API I32
mod__Int32(I32 x, I32 y)
{
    return (div__Int32(add__Int32(x, x), y)) & y;
}

LILY_API I32
exp__Int32(I32 x, I32 y)
{
    if (y < 0)
        return 0;

    MutI32 res = x;

    for (MutI32 i = y; i--;)
        res *= x;

    return res;
}

LILY_API I32
l_shift__Int32(I32 x, I32 y)
{
    return x << y;
}

LILY_API I32
r_shift__Int32(I32 x, I32 y)
{
    return x >> y;
}

LILY_API I32
bit_or__Int32(I32 x, I32 y)
{
    return x | y;
}

LILY_API I32
bit_and__Int32(I32 x, I32 y)
{
    return x & y;
}

LILY_API I32
bit_xor__Int32(I32 x, I32 y)
{
    return x ^ y;
}

LILY_API I32
bit_not__Int32(I32 x)
{
    return ~x;
}

LILY_API Unit
assign__Int32(MutI32 *var, I32 val)
{
    *var = val;
}

LILY_API Unit
add_assign__Int32(MutI32 *x, I32 y)
{
    *x = add__Int32(*x, y);
}

LILY_API Unit
sub_assign__Int32(MutI32 *x, I32 y)
{
    *x = sub__Int32(*x, y);
}

LILY_API Unit
mul_assign__Int32(MutI32 *x, I32 y)
{
    *x = mul__Int32(*x, y);
}

LILY_API Unit
div_assign__Int32(MutI32 *x, I32 y)
{
    *x = div__Int32(*x, y);
}

LILY_API Unit
mod_assign__Int32(MutI32 *x, I32 y)
{
    *x = mod__Int32(*x, y);
}

LILY_API Unit
exp_assign__Int32(MutI32 *x, I32 y)
{
    *x = exp__Int32(*x, y);
}

LILY_API Unit
l_shift_assign__Int32(MutI32 *x, I32 y)
{
    *x = l_shift__Int32(*x, y);
}

LILY_API Unit
r_shift_assign__Int32(MutI32 *x, I32 y)
{
    *x = r_shift__Int32(*x, y);
}

LILY_API Unit
bit_or_assign__Int32(MutI32 *x, I32 y)
{
    *x = bit_or__Int32(*x, y);
}

LILY_API Unit
bit_and_assign__Int32(MutI32 *x, I32 y)
{
    *x = bit_and__Int32(*x, y);
}

LILY_API Unit
bit_xor_assign__Int32(MutI32 *x, I32 y)
{
    *x = bit_xor__Int32(*x, y);
}

LILY_API struct Range
range__Int32(MutI32 x, MutI32 y)
{
    return __new__Range(&x, &y);
}

LILY_API Bool
lt__Int32(I32 x, I32 y)
{
    return x < y;
}

LILY_API Bool
gt__Int32(I32 x, I32 y)
{
    return x > y;
}

LILY_API Bool
le__Int32(I32 x, I32 y)
{
    return x <= y;
}

LILY_API Bool
ge__Int32(I32 x, I32 y)
{
    return x >= y;
}
