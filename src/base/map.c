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
