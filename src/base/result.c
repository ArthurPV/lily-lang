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
#include <base/result.h>
#include <stdlib.h>

struct Result *
__new__ResultOk(void *ok)
{
    struct Result *self = malloc(sizeof(struct Result));
    self->kind = Ok;
    self->ok = ok;
    return self;
}

struct Result *
__new__ResultErr(void *err)
{
    struct Result *self = malloc(sizeof(struct Result));
    self->kind = Err;
    self->err = err;
    return self;
}

bool
is_err__Result(struct Result self)
{
    return self.kind == Err;
}

bool
is_ok__Result(struct Result self)
{
    return self.kind == Ok;
}

void *
get_ok__Result(struct Result self)
{
    assert(self.kind == Ok && "only Ok");
    return self.ok;
}

void *
get_err__Result(struct Result self)
{
    assert(self.kind == Err && "only Err");
    return self.err;
}

void
__free__Result(struct Result *self)
{
    free(self);
}
