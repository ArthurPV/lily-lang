#include <lang/runtime/runtime.h>

LILY_API struct Exception
__new__Exception(void *val, void *ext) {
    RUNTIME_ASSERT(val != NULL || ext != NULL);

    struct Exception self = {
        .ext = ext,
        .val = val
    };

    return self;
}

LILY_API struct Optional 
__new__Optional(void *val) {
    struct Optional self = {
        .val = val
    };

    return self;
}

LILY_API void *
get__Optional(struct Optional self)
{
    RUNTIME_ASSERT(self.val != NULL);

    return self.val;
}

LILY_API struct Range __new__Range(void *start, void *end)
{
    struct Range self = {
        .start = start,
        .end = end
    };

    return self;
}
