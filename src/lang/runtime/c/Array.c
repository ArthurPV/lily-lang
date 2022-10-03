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

#include "Array.h"
#include "generic.h"
#include <stdarg.h>
#include <string.h>

LILY_API struct Array
new__Array()
{
    struct Array arr = { .items = malloc(sizeof(struct Generic) * 8),
                         .len = 0,
                         .capacity = 8 };

    return arr;
}

LILY_API struct Array
init__Array(Usize n, ...)
{
    struct Array arr = new__Array();
    va_list vl;

    va_start(vl, n);

    for (MutUsize i = 0; i < n; i++)
        push__Array(&arr, va_arg(vl, struct Generic *));

    va_end(vl);

    return arr;
}

LILY_API Unit
push__Array(struct Array *arr, struct Generic *item)
{
    if (arr->len == arr->capacity) {
        arr->capacity *= 2;
        arr->items =
          realloc(arr->items, sizeof(struct Generic *) * arr->capacity);
    }

    arr->items[arr->len++] = item;
}

LILY_API struct Array *
copy__Array(struct Array arr)
{
    struct Array *copy = malloc(sizeof(struct Array));

    memcpy(&arr, copy, sizeof(struct Array));

    return copy;
}

LILY_API Usize
len__Array(struct Array arr)
{
    return arr.len;
}

LILY_API Usize
capacity__Array(struct Array arr)
{
    return arr.capacity;
}
