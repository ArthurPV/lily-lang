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