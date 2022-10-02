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