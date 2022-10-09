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

#include <assert.h>
#include <base/tuple.h>
#include <stdarg.h>

struct Tuple *
__new__Tuple(Usize size, ...)
{
    struct Tuple *self = malloc(sizeof(struct Tuple));
    self->size = size;
    self->items = malloc(sizeof(void *) * size);

    va_list vl;
    va_start(vl, size);

    for (Usize i = 0; i < size; i++)
        self->items[i] = va_arg(vl, void *);

    va_end(vl);

    return self;
}

void *
get__Tuple(struct Tuple self, Usize idx)
{
    assert(idx < self.size && "index out of bounds");
    return self.items[idx];
}

void
__free__Tuple(struct Tuple *self)
{
    free(self->items);
    free(self);
}
