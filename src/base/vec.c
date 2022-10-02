#include <assert.h>
#include <base/assert.h>
#include <base/new.h>
#include <base/vec.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Vec *
__new__Vec(Usize item_size)
{
    struct Vec *self = malloc(sizeof(struct Vec));
    self->items = malloc(item_size * 8);
    self->len = 0;
    self->capacity = 8;
    self->default_capacity = 8;
    self->item_size = item_size;
    return self;
}

struct Vec *
from__Vec(void **items, Usize item_size, Usize len)
{
    struct Vec *self = NEW(Vec, item_size);

    for (Usize i = 0; i <= len; i++)
        push__Vec(self, items[i]);

    return self;
}


struct Vec* create__Vec(void *item, Usize item_size, Usize len) {
    struct Vec *self = NEW(Vec, item_size);

    for (Usize i = 0; i < len; i++)
        push__Vec(self, item);

    return self;
}

void resize__Vec(struct Vec *self) {
    Usize old_capacity = self->capacity;
    Usize new_capacity;
     
    if (self->len >= self->default_capacity)
        new_capacity = (ceil(self->len / self->default_capacity) + 1) * self->default_capacity;
    else
        new_capacity = self->default_capacity;

    if (old_capacity != new_capacity) {
        self->capacity = new_capacity; 
        self->items = realloc(self->items, self->capacity * self->item_size);
    }
}

void
push__Vec(struct Vec *self, void *item)
{
    if (self->len == self->capacity) {
        self->capacity *= 2;
        self->items = realloc(self->items, self->item_size * self->capacity);
    }
    self->items[self->len++] = item;
}

void *
get__Vec(struct Vec self, Usize idx)
{
    assert(idx < self.len || idx < 0 && "index out of bounds");
    return self.items[idx];
}

void
remove__Vec(struct Vec *self, Usize idx)
{
    for (; idx < self->len; idx++)
        self->items[idx] = self->items[idx + 1];

    self->items[self->len--] = NULL;
    resize__Vec(self);
}

void *
pop__Vec(struct Vec *self)
{
    assert(self->len > 0 && "index out of bounds");
    void *last = self->items[--self->len];
    resize__Vec(self);

    return last;
}

void *
shift__Vec(struct Vec *self)
{
    assert(self->len > 0 && "index out of bounds");
    void *result = get__Vec(*self, 0);
    remove__Vec(self, 0);
    return result;
}

void
modify_item__Vec(struct Vec *self, void *value, Usize idx)
{
    assert(self->len > 0 || idx < self->len ||
           idx > 0 && "index out of bounds");
    self->items[idx] = value;
}

struct Vec *
concat__Vec(struct Vec *self, struct Vec *self2)
{
    for (Usize i = 0; i < self2->len; i++)
        push__Vec(self, get__Vec(*self2, i));

    return self;
}

struct Vec
fill__Vec(struct Vec self, void *value, const Usize *start, const Usize *end)
{
    if (start == NULL && end == NULL)
        for (Usize i = 0; i < self.len; i++)
            modify_item__Vec(&self, value, i);
    else if (start == NULL)
        for (Usize i = 0; i < *end; i++)
            modify_item__Vec(&self, value, i);
    else
        for (Usize i = *start; i < self.len; i++)
            modify_item__Vec(&self, value, i);

    return self;
}

struct Vec
filter__Vec(struct Vec self, bool(f)(void *))
{
loop : {
    for (Usize i = 0; i < self.len; i++)
        if (!f(get__Vec(self, i))) {
            remove__Vec(&self, i);
            goto loop;
        }
}
    return self;
}

struct Vec
map__Vec(struct Vec self, void *(f)(void *))
{
    for (Usize i = 0; i < self.len; i++)
        modify_item__Vec(&self, f(self.items[i]), i);

    return self;
}

void *
find__Vec(struct Vec self, bool(f)(void *))
{
    void *res = NULL;
    for (Usize i = 0; i < self.len; i++)
        if (f(get__Vec(self, i))) {
            res = get__Vec(self, i);
            break;
        }
    return res;
}

Usize *
find_index__Vec(struct Vec self, bool(f)(void *))
{
    Usize *res = NULL;
    for (Usize i = 0; i < self.len; i++)
        if (f(get__Vec(self, i))) {
            *res = i;
            break;
        }
    return res;
}

void
for_each__Vec(struct Vec self, void(f)(void *))
{
    for (Usize i = 0; i < self.len; i++)
        f(get__Vec(self, i));
}

bool
includes__Vec(struct Vec self, void *item)
{
    for (Usize i = 0; i < self.len; i++)
        if (item == get__Vec(self, i))
            return true;
    return false;
}

Usize *
index_of__Vec(struct Vec self, void *value, const Usize *start)
{
    for (Usize i = start == NULL ? 0 : *start; i < self.len; i++)
        if (value == get__Vec(self, i))
            return (Usize *)i;
    return NULL;
}

void
insert__Vec(struct Vec *self, void *item, Usize idx)
{
    assert(idx < self->len || idx < 0 && "index out of bounds");
    void *last = get__Vec(*self, self->len - 1);

    modify_item__Vec(self, item, idx);
    push__Vec(self, last);
}

Str
join__Vec(struct Vec *self, Str separator)
{
    Str result = malloc(sizeof(char));
    Usize total_size = 0;

    for (Usize i = 0; i < self->len; i++) {
        Str item = (Str)get__Vec(*self, i);

        for (Usize j = 0; j < strlen(item); j++) {
            result = realloc(result, ++total_size);
            result[total_size - 1] = item[j];
        }

        if (self->len - 1 != i)
            for (Usize k = 0; k < strlen(separator); k++) {
                result = realloc(result, ++total_size);
                result[total_size - 1] = separator[k];
            }
    }

    result = realloc(result, total_size + 1);
    result[total_size] = '\0';

    return result;
}

struct Vec *
reverse__Vec(struct Vec self)
{
    struct Vec *self2 = NEW(Vec, self.item_size);
    Usize idx = self.len;

    for (Usize i = idx; i > 0; i--) {
        void *value = get__Vec(self, i - 1);
        push__Vec(self2, value);
    }

    return self2;
}

struct Vec *
sort__Vec(struct Vec self, bool reverse)
{
    struct Vec *self2 = NEW(Vec, self.item_size);

loop : {
    for (Usize i = 0; i < self.len; i++) {
        void *min_value = NULL;
        void *i_value = NULL;
        void *j_value = NULL;
        Usize j = i + 1;

        for (; j < self.len; j++) {
            i_value = get__Vec(self, i);
            j_value = get__Vec(self, j);

            if (min_value == NULL || i_value < j_value)
                min_value = i_value;
            else
                min_value = j_value;
        }

        if (i_value == NULL && j_value == NULL) {
            i_value = get__Vec(self, i);
            push__Vec(self2, i_value);
            break;
        }

        push__Vec(self2, min_value);

        if (i_value < j_value)
            remove__Vec(&self, i);
        else
            remove__Vec(&self, j);

        goto loop;
    }
};

    if (reverse)
        return reverse__Vec(*self2);
    else
        return self2;
}

void *
reduce__Vec(struct Vec self, void *(f)(void *, void *, void *))
{
    void *reduce = NULL;

    for (Usize i = 0; i < self.len && i + 1 < self.len; i++)
        f(reduce, get__Vec(self, i), get__Vec(self, i + 1));

    return reduce;
}

void
truncate__Vec(struct Vec *self, Usize len)
{
    if (len < self->len) {
        Usize count = self->len - 1;
        for (Usize i = 0; i <= len - self->len && count >= len; i++) {
            self->items[count--] = NULL;
            self->len -= 1;
            resize__Vec(self);
        }
    }
}

void
clear__Vec(struct Vec *self)
{
    truncate__Vec(self, 0);
}

bool
is_clear__Vec(struct Vec self)
{
    return self.len == 0;
}

Usize
len__Vec(struct Vec self)
{
    return self.len;
}

Usize
capacity__Vec(struct Vec self)
{
    return self.capacity;
}

void
__free__Vec(struct Vec *self)
{
    free(self->items);
    free(self);
}