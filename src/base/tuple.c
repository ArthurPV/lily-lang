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