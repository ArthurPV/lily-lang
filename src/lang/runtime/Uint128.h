#ifndef LILY_RUNTIME_UINT128_H
#define LILY_RUNTIME_UINT128_H

#include "api.h"
#include "runtime.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHECK_U64_RES_OVERFLOW(res, x, y)                                   \
    if (((__int128_t)(uintptr_t)res < x || (uint64_t)(uintptr_t)res < y) || \
        ((__int128_t)(uintptr_t)res > x || (uint64_t)(uintptr_t)res > y)) { \
        RUNTIME_ERROR(                                                      \
          "Uint128 overflow: 0..680564733841876926926749214863536422911");  \
    }

    LILY_API U64 add__Uint64(U64 x, U64 y);
    LILY_API U64 sub__Uint64(U64 x, U64 y);
    LILY_API U64 mul__Uint64(U64 x, U64 y);
    LILY_API U64 div__Uint64(U64 x, U64 y);
    LILY_API U64 mod__Uint64(U64 x, U64 y);
    LILY_API U64 exp__Uint64(U64 x, U64 y);
    LILY_API U64 l_shift__Uint64(U64 x, U64 y);
    LILY_API U64 r_shift__Uint64(U64 x, U64 y);
    LILY_API U64 bit_or__Uint64(U64 x, U64 y);
    LILY_API U64 bit_and__Uint64(U64 x, U64 y);
    LILY_API U64 bit_xor__Uint64(U64 x, U64 y);
    LILY_API U64 bit_not__Uint64(U64 x);
    LILY_API Unit assign__Uint64(MutU64 *var, U64 val);
    LILY_API Unit add_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit sub_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit mul_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit div_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit mod_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit exp_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit l_shift_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit r_shift_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit bit_or_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit bit_and_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Unit bit_xor_assign__Uint64(MutU64 *x, U64 y);
    LILY_API Bool eq__Uint64(U64 x, U64 y);
    LILY_API Bool ne__Uint64(U64 x, U64 y);
    LILY_API U64 neg__Uint64(U64 x);
    LILY_API struct Range range__Uint64(U64 x, U64 y);
    LILY_API Bool lt__Uint64(U64 x, U64 y);
    LILY_API Bool gt__Uint64(U64 x, U64 y);
    LILY_API Bool le__Uint64(U64 x, U64 y);
    LILY_API Bool ge__Uint64(U64 x, U64 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_UINT128_H
