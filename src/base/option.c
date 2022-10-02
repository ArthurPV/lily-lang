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
#include <base/option.h>
#include <stdlib.h>

struct Option *
__new__Some__Option(void *some)
{
    struct Option *self = malloc(sizeof(struct Option));
    self->kind = Some;
    self->some = some;
    return self;
}

struct Option *
__new__None__Option()
{
    struct Option *self = malloc(sizeof(struct Option));
    self->kind = None;
    return self;
}

bool
is_Some__Option(struct Option *self)
{
    return self->kind == Some;
}

bool
is_None__Option(struct Option *self)
{
    return self->kind == None;
}

bool
eq__Option(struct Option self, void *ptr)
{
    if (ptr == NULL && self.kind == None)
        return true;
    else if (ptr != NULL && self.some == ptr && self.kind == Some)
        return true;
    else
        return false;
}

void *
get__Option(struct Option *self)
{
    assert(self->kind != None && "The Option Kind is None.");
    return self->some;
}

void
__free__Option(struct Option *self)
{
    free(self);
}
