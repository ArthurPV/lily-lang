#ifndef LILY_RUNTIME_INT8_H
#define LILY_RUNTIME_INT8_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define VERIFY_I8(res, x, y)                              \
    if ((x > 0 && y > 0 && (int8_t)(uintptr_t)res < 0) || \
        (x < 0 && y < 0 && (int8_t)(uintptr_t)res > 0)) { \
        RUNTIME_ERROR("Int8 overflow: -128..127");        \
    }

    LILY_API I8 add__Int8(I8 x, I8 y);
    LILY_API I8 sub__Int8(I8 x, I8 y);
    LILY_API I8 mul__Int8(I8 x, I8 y);
    LILY_API I8 div__Int8(I8 x, I8 y);
    LILY_API I8 mod__Int8(I8 x, I8 y);
    LILY_API I8 exp__Int8(I8 x, I8 y);
    LILY_API I8 l_shift__Int8(I8 x, I8 y);
    LILY_API I8 r_shift__Int8(I8 x, I8 y);
    LILY_API I8 bit_or__Int8(I8 x, I8 y);
    LILY_API I8 bit_and__Int8(I8 x, I8 y);
    LILY_API I8 bit_xor__Int8(I8 x, I8 y);
    LILY_API I8 bit_not__Int8(I8 x);
    LILY_API Unit assign__Int8(MutI8 *var, I8 val);
    LILY_API Unit add_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit sub_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit mul_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit div_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit mod_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit exp_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit l_shift_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit r_shift_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit bit_or_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit bit_and_assign__Int8(MutI8 *x, I8 y);
    LILY_API Unit bit_xor_assign__Int8(MutI8 *x, I8 y);
    LILY_API Bool eq__Int8(I8 x, I8 y);
    LILY_API Bool ne__Int8(I8 x, I8 y);
    LILY_API I8 neg__Int8(I8 x);
    LILY_API struct Range range__Int8(I8 x, I8 y);
    LILY_API Bool lt__Int8(I8 x, I8 y);
    LILY_API Bool gt__Int8(I8 x, I8 y);
    LILY_API Bool le__Int8(I8 x, I8 y);
    LILY_API Bool ge__Int8(I8 x, I8 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_INT8_H
