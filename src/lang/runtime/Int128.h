#ifndef LILY_RUNTIME_INT128_H
#define LILY_RUNTIME_INT128_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define MIN_I128 1 << 127
#define MAX_I128 ~MIN_I128

#define CHECK_I128_RES_OVERFLOW(res, x, y)                         \
    if ((x > 0 && y > 0 && (__int128_t)(uintptr_t)res < 0) ||      \
        (x < 0 && y < 0 && (__int128_t)(uintptr_t)res > 0)) {      \
        RUNTIME_ERROR("Int128 overflow: "                          \
                      "-170141183460469231731687303715884105729.." \
                      "170141183460469231731687303715884105728");  \
    }

    LILY_API I128 add__Int128(I128 x, I128 y);
    LILY_API I128 sub__Int128(I128 x, I128 y);
    LILY_API I128 mul__Int128(I128 x, I128 y);
    LILY_API I128 div__Int128(I128 x, I128 y);
    LILY_API I128 mod__Int128(I128 x, I128 y);
    LILY_API I128 exp__Int128(I128 x, I128 y);
    LILY_API I128 l_shift__Int128(I128 x, I128 y);
    LILY_API I128 r_shift__Int128(I128 x, I128 y);
    LILY_API I128 bit_or__Int128(I128 x, I128 y);
    LILY_API I128 bit_and__Int128(I128 x, I128 y);
    LILY_API I128 bit_xor__Int128(I128 x, I128 y);
    LILY_API I128 bit_not__Int128(I128 x);
    LILY_API Unit assign__Int128(MutI128 *var, I128 val);
    LILY_API Unit add_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit sub_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit mul_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit div_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit mod_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit exp_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit l_shift_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit r_shift_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit bit_or_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit bit_and_assign__Int128(MutI128 *x, I128 y);
    LILY_API Unit bit_xor_assign__Int128(MutI128 *x, I128 y);
    LILY_API struct Range range__Int128(I128 x, I128 y);
    LILY_API Bool lt__Int128(I128 x, I128 y);
    LILY_API Bool gt__Int128(I128 x, I128 y);
    LILY_API Bool le__Int128(I128 x, I128 y);
    LILY_API Bool ge__Int128(I128 x, I128 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_INT128_H
