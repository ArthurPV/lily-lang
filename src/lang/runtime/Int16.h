#ifndef LILY_RUNTIME_INT16_H
#define LILY_RUNTIME_INT16_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    LILY_API I16 add__Int16(MutI16 x, MutI16 y);
    LILY_API I16 sub__Int16(MutI16 x, MutI16 y);
    LILY_API I16 mul__Int16(I16 x, I16 y);
    LILY_API I16 div__Int16(I16 x, I16 y);
    LILY_API I16 mod__Int16(I16 x, I16 y);
    LILY_API I16 exp__Int16(I16 x, I16 y);
    LILY_API I16 l_shift__Int16(I16 x, I16 y);
    LILY_API I16 r_shift__Int16(I16 x, I16 y);
    LILY_API I16 bit_or__Int16(I16 x, I16 y);
    LILY_API I16 bit_and__Int16(I16 x, I16 y);
    LILY_API I16 bit_xor__Int16(I16 x, I16 y);
    LILY_API I16 bit_not__Int16(I16 x);
    LILY_API Unit assign__Int16(MutI16 *var, I16 val);
    LILY_API Unit add_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit sub_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit mul_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit div_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit mod_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit exp_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit l_shift_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit r_shift_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit bit_or_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit bit_and_assign__Int16(MutI16 *x, I16 y);
    LILY_API Unit bit_xor_assign__Int16(MutI16 *x, I16 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_INT8_H
