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

#ifndef LILY_RUNTIME_ARRAY_H
#define LILY_RUNTIME_ARRAY_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct Array
    {
        struct Generic **items;
        MutUsize len;
        MutUsize capacity;
    } Array;

    LILY_API struct Array new__Array();
    LILY_API struct Array init__Array(Usize n, ...);
    LILY_API Unit push__Array(struct Array *arr, struct Generic *item);
    LILY_API struct Array *copy__Array(struct Array arr);
    LILY_API Usize len__Array(struct Array arr);
    LILY_API Usize capacity__Array(struct Array arr);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ARRAY_H
