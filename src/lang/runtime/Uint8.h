/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_RUNTIME_UINT8_H
#define LILY_RUNTIME_UINT8_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHECK_U8_RES_OVERFLOW(res, x, y)                                \
    if (((uint8_t)(uintptr_t)res < x || (uint8_t)(uintptr_t)res < y) || \
        ((uint8_t)(uintptr_t)res > x || (uint8_t)(uintptr_t)res > y)) { \
        RUNTIME_ERROR("Uint8 overflow: 0..255");                        \
    }

    LILY_API U8 add__Uint8(U8 x, U8 y);
    LILY_API U8 sub__Uint8(U8 x, U8 y);
    LILY_API U8 mul__Uint8(U8 x, U8 y);
    LILY_API U8 div__Uint8(U8 x, U8 y);
    LILY_API U8 mod__Uint8(U8 x, U8 y);
    LILY_API U8 exp__Uint8(U8 x, U8 y);
    LILY_API U8 l_shift__Uint8(U8 x, U8 y);
    LILY_API U8 r_shift__Uint8(U8 x, U8 y);
    LILY_API U8 bit_or__Uint8(U8 x, U8 y);
    LILY_API U8 bit_and__Uint8(U8 x, U8 y);
    LILY_API U8 bit_xor__Uint8(U8 x, U8 y);
    LILY_API U8 bit_not__Uint8(U8 x);
    LILY_API Unit assign__Uint8(MutU8 *var, U8 val);
    LILY_API Unit add_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit sub_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit mul_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit div_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit mod_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit exp_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit l_shift_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit r_shift_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit bit_or_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit bit_and_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Unit bit_xor_assign__Uint8(MutU8 *x, U8 y);
    LILY_API Bool eq__Uint8(U8 x, U8 y);
    LILY_API Bool ne__Uint8(U8 x, U8 y);
    LILY_API U8 neg__Uint8(U8 x);
    LILY_API struct Range range__Uint8(U8 x, U8 y);
    LILY_API Bool lt__Uint8(U8 x, U8 y);
    LILY_API Bool gt__Uint8(U8 x, U8 y);
    LILY_API Bool le__Uint8(U8 x, U8 y);
    LILY_API Bool ge__Uint8(U8 x, U8 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_UINT8_H
