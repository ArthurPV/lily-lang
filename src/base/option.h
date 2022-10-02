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

#ifndef LILY_OPTION_H
#define LILY_OPTION_H

#include <stdbool.h>

typedef struct Option
{
    enum
    {
        Some,
        None
    } kind;
    union
    {
        void *some;
    };
} Option;

#define Some(some) __new__Some__Option(some)
#define None() __new__None__Option()

/**
 * @brief Construct Some option.
 * @param some
 * @return struct Option*
 */
struct Option *
__new__Some__Option(void *some);

/**
 * @brief Construct None option;
 * @return struct Option*
 */
struct Option *
__new__None__Option();

/**
 * @brief Returns true if the Option kind is Some else returns false.
 * @param self
 * @return bool
 */
bool
is_Some__Option(struct Option *self);

/**
 * @brief Return true if the Option kind is None else returns false.
 * @param self
 * @return bool
 */
bool
is_None__Option(struct Option *self);

/**
 *
 * @return if true if the value is equal.
 */
bool
eq__Option(struct Option self, void *ptr);

/**
 * @brief Get the value of Some.
 * @param self
 * @return void*
 */
void *
get__Option(struct Option *self);

/**
 *
 * @brief Free Option type.
 */
void
__free__Option(struct Option *self);

#endif // LILY_OPTION_H
