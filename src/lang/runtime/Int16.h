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

#ifndef LILY_RUNTIME_INT16_H
#define LILY_RUNTIME_INT16_H

#include "api.h"
#include "runtime.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#define CHECK_I16_RES_OVERFLOW(res, x, y)                  \
    if ((x > 0 && y > 0 && (int16_t)(uintptr_t)res < 0) || \
        (x < 0 && y < 0 && (int16_t)(uintptr_t)res > 0)) { \
        RUNTIME_ERROR("Int16 overflow: -32768..32767");    \
    }

    LILY_API I16 add__Int16(I16 x, I16 y);
    LILY_API I16 sub__Int16(I16 x, I16 y);
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
    LILY_API struct Range range__Int16(I16 x, I16 y);
    LILY_API Bool lt__Int16(I16 x, I16 y);
    LILY_API Bool gt__Int16(I16 x, I16 y);
    LILY_API Bool le__Int16(I16 x, I16 y);
    LILY_API Bool ge__Int16(I16 x, I16 y);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_INT16_H
