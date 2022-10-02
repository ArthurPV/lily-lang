#ifndef LILY_RUNTIME_UINT16_H
#define LILY_RUNTIME_UINT16_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHECK_U16_RES_OVERFLOW(res, x, y)                                 \
    if (((uint16_t)(uintptr_t)res < x || (uint16_t)(uintptr_t)res < y) || \
        ((uint16_t)(uintptr_t)res > x || (uint16_t)(uintptr_t)res > y)) { \
        RUNTIME_ERROR("Uint16 overflow: 0..65535");                       \
    }

    LILY_API U16 add__Uint16(U16 x, U16 y);
    LILY_API U16 sub__Uint16(U16 x, U16 y);
    LILY_API U16 mul__Uint16(U16 x, U16 y);
    LILY_API U16 div__Uint16(U16 x, U16 y);
    LILY_API U16 mod__Uint16(U16 x, U16 y);
    LILY_API U16 exp__Uint16(U16 x, U16 y);
    LILY_API U16 l_shift__Uint16(U16 x, U16 y);
    LILY_API U16 r_shift__Uint16(U16 x, U16 y);
    LILY_API U16 bit_or__Uint16(U16 x, U16 y);
    LILY_API U16 bit_and__Uint16(U16 x, U16 y);
    LILY_API U16 bit_xor__Uint16(U16 x, U16 y);
    LILY_API U16 bit_not__Uint16(U16 x);
    LILY_API Unit assign__Uint16(MutU16 *var, U16 val);
    LILY_API Unit add_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit sub_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit mul_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit div_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit mod_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit exp_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit l_shift_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit r_shift_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit bit_or_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit bit_and_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Unit bit_xor_assign__Uint16(MutU16 *x, U16 y);
    LILY_API Bool eq__Uint16(U16 x, U16 y);
    LILY_API Bool ne__Uint16(U16 x, U16 y);
    LILY_API U16 neg__Uint16(U16 x);
    LILY_API struct Range range__Uint16(U16 x, U16 y);
    LILY_API Bool lt__Uint16(U16 x, U16 y);
    LILY_API Bool gt__Uint16(U16 x, U16 y);
    LILY_API Bool le__Uint16(U16 x, U16 y);
    LILY_API Bool ge__Uint16(U16 x, U16 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_UINT16_H
