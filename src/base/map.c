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

#include <base/map.h>
#include <base/new.h>

struct HashMap *
__new__HashMap(Usize key_size, Usize value_size)
{
    struct HashMap *self = malloc(sizeof(struct HashMap));
    self->keys = NEW(Vec, key_size);
    self->values = NEW(Vec, value_size);
    return self;
}

void *
get__HashMap(struct HashMap self, void *key)
{
    for (Usize i = 0; i < len__Vec(*self.keys); i++)
        if (get__Vec(*self.keys, i) == key)
            return get__Vec(*self.values, i);
    return NULL;
}

struct Tuple *
get_key_value__HashMap(struct HashMap self, void *key)
{
    for (Usize i = 0; i < len__Vec(*self.keys); i++)
        if (get__Vec(*self.keys, i) == key)
            return NEW(Tuple, 2, key, get__Vec(*self.values, i));
    return NULL;
}

void
insert__HashMap(struct HashMap *self, void *key, void *value)
{
    push__Vec(self->keys, key);
    push__Vec(self->values, value);
}

void
__free__HashMap(struct HashMap *self)
{
    FREE(Vec, self->keys);
    FREE(Vec, self->values);
    free(self);
}
