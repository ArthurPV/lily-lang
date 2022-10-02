#ifndef LILY_RUNTIME_INT64_H
#define LILY_RUNTIME_INT64_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define VERIFY_I64(res, x, y)                                     \
    if ((x > 0 && y > 0 && (int64_t)(uintptr_t)res < 0) ||        \
        (x < 0 && y < 0 && (int64_t)(uintptr_t)res > 0)) {        \
        RUNTIME_ERROR("Int64 overflow: -9223372036854775808..9223372036854775807"); \
    }

    LILY_API I64 add__Int64(I64 x, I64 y);
    LILY_API I64 sub__Int64(I64 x, I64 y);
    LILY_API I64 mul__Int64(I64 x, I64 y);
    LILY_API I64 div__Int64(I64 x, I64 y);
    LILY_API I64 mod__Int64(I64 x, I64 y);
    LILY_API I64 exp__Int64(I64 x, I64 y);
    LILY_API I64 l_shift__Int64(I64 x, I64 y);
    LILY_API I64 r_shift__Int64(I64 x, I64 y);
    LILY_API I64 bit_or__Int64(I64 x, I64 y);
    LILY_API I64 bit_and__Int64(I64 x, I64 y);
    LILY_API I64 bit_xor__Int64(I64 x, I64 y);
    LILY_API I64 bit_not__Int64(I64 x);
    LILY_API Unit assign__Int64(MutI64 *var, I64 val);
    LILY_API Unit add_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit sub_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit mul_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit div_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit mod_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit exp_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit l_shift_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit r_shift_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit bit_or_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit bit_and_assign__Int64(MutI64 *x, I64 y);
    LILY_API Unit bit_xor_assign__Int64(MutI64 *x, I64 y);
    LILY_API struct Range range__Int64(I64 x, I64 y);
    LILY_API Bool lt__Int64(I64 x, I64 y);
    LILY_API Bool gt__Int64(I64 x, I64 y);
    LILY_API Bool le__Int64(I64 x, I64 y);
    LILY_API Bool ge__Int64(I64 x, I64 y);

#if defined(__cplusplus)
}
#endif


#endif // LILY_RUNTIME_INT64_H
