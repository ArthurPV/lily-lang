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

#ifndef LILY_STR_H
#define LILY_STR_H

#include <base/types.h>
#include <stdbool.h>

/**
 *
 * @brief Convert bool in Str.
 */
Str
of_bool__Str(bool b);

/**
 *
 * @brief Convert int in Str.
 */
Str
of_int__Str(int i);

/**
 *
 * @brief Convert u8 in Str.
 */
Str
of_u8__Str(UInt8 u);

/**
 *
 * @brief Convert u16 in Str.
 */
Str
of_u16__Str(UInt16 u);

/**
 *
 * @brief Convert u32 in Str.
 */
Str
of_u32__Str(UInt32 u);

/**
 *
 * @brief Convert u64 in Str.
 */
Str
of_u64__Str(UInt64 u);

/**
 *
 * @brief Convert i64 in Str.
 */
Str
of_i64__Str(Int64 i);

/**
 *
 * @brief Convert i128 in Str.
 */
Str
of_i128__Str(Int128 i);

/**
 *
 * @brief Convert double in Str.
 */
Str
of_double__Str(double d);

/**
 * @brief Convert char in Str.
 *
 */
Str
of_char__Str(char c);

#endif // LILY_STR_H
