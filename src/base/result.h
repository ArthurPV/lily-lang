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

#ifndef LILY_RESULT_H
#define LILY_RESULT_H

#include <stdbool.h>

typedef struct Result
{
    enum
    {
        Ok,
        Err
    } kind;
    union
    {
        void *ok;
        void *err;
    };
} Result;

#define Ok(ok) __new__ResultOk(ok)
#define Err(err) __new__ResultErr(err)

/**
 *
 * @return new Result instance (Ok).
 */
struct Result *
__new__ResultOk(void *ok);

/**
 *
 * @return new Result instance (Err).
 */
struct Result *
__new__ResultErr(void *err);

/**
 *
 * @brief Returns true if the result kind is an Err otherwise returns false.
 */
bool
is_err__Result(struct Result self);

/**
 *
 * @brief Returns true if the result kind is Ok otherwise returns false.
 */
bool
is_ok__Result(struct Result self);

/**
 *
 * @brief Get the ok result value.
 */
void *
get_ok__Result(struct Result self);

/**
 *
 * @brief Get the err result value.
 */
void *
get_err__Result(struct Result self);

/**
 *
 * @brief Free the Result type.
 */
void
__free__Result(struct Result *self);

#endif // LILY_RESULT_H
