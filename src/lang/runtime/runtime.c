#include <lang/runtime/runtime.h>

struct Exception
__new__Exception(void *val, void *ext) {
    RUNTIME_ASSERT(val != NULL || ext != NULL);

    struct Exception self = {
        .ext = ext,
        .val = val
    };

    return self;
}

struct Optional 
__new__Optional(void *val) {
    struct Optional self = {
        .val = val
    };

    return self;
}

void *
get__Optional(struct Optional self)
{
    RUNTIME_ASSERT(self.val != NULL);

    return self.val;
}