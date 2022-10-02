#ifndef LILY_RUNTIME_UINT32_H
#define LILY_RUNTIME_UINT32_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHECK_U32_RES_OVERFLOW(res, x, y)                                 \
    if (((uint32_t)(uintptr_t)res < x || (uint32_t)(uintptr_t)res < y) || \
        ((uint32_t)(uintptr_t)res > x || (uint32_t)(uintptr_t)res > y)) { \
        RUNTIME_ERROR("Uint32 overflow: 0..4294967295");                  \
    }

    LILY_API U32 add__Uint32(U32 x, U32 y);
    LILY_API U32 sub__Uint32(U32 x, U32 y);
    LILY_API U32 mul__Uint32(U32 x, U32 y);
    LILY_API U32 div__Uint32(U32 x, U32 y);
    LILY_API U32 mod__Uint32(U32 x, U32 y);
    LILY_API U32 exp__Uint32(U32 x, U32 y);
    LILY_API U32 l_shift__Uint32(U32 x, U32 y);
    LILY_API U32 r_shift__Uint32(U32 x, U32 y);
    LILY_API U32 bit_or__Uint32(U32 x, U32 y);
    LILY_API U32 bit_and__Uint32(U32 x, U32 y);
    LILY_API U32 bit_xor__Uint32(U32 x, U32 y);
    LILY_API U32 bit_not__Uint32(U32 x);
    LILY_API Unit assign__Uint32(MutU32 *var, U32 val);
    LILY_API Unit add_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit sub_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit mul_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit div_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit mod_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit exp_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit l_shift_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit r_shift_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit bit_or_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit bit_and_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Unit bit_xor_assign__Uint32(MutU32 *x, U32 y);
    LILY_API Bool eq__Uint32(U32 x, U32 y);
    LILY_API Bool ne__Uint32(U32 x, U32 y);
    LILY_API U32 neg__Uint32(U32 x);
    LILY_API struct Range range__Uint32(U32 x, U32 y);
    LILY_API Bool lt__Uint32(U32 x, U32 y);
    LILY_API Bool gt__Uint32(U32 x, U32 y);
    LILY_API Bool le__Uint32(U32 x, U32 y);
    LILY_API Bool ge__Uint32(U32 x, U32 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_UINT32_H
