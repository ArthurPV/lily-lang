#include <lang/runtime/runtime.h>
#include <limits.h>

LILY_API struct u128_t
__new__u128_t(uint64_t x, uint64_t y)
{
    struct u128_t self = { .x = x, .y = y };

    return self;
}

LILY_API U128
add__u128_t(U128 self, U128 v)
{
    RUNTIME_ASSERT(!(self.x == ULONG_MAX && self.y == ULONG_MAX));

    if (self.x == ULONG_MAX) {
        struct u128_t u = { .x = self.x, .y = self.y + v.x + v.y };

        RUNTIME_ASSERT(u.y > v.x + v.y);

        return u;
    }

    struct u128_t u = { .x = self.x + v.x + v.y, .y = 0 };

    if (u.x >= ULONG_MAX) {
        u.y = u.x;
        u.x = ULONG_MAX;
    }

    RUNTIME_ASSERT(u.y <= ULONG_MAX);

    return u;
}

#define abs(x) -x

LILY_API U128
sub__u128_t(U128 self, U128 v)
{
    RUNTIME_ASSERT(!(self.x == ULONG_MAX && self.y == ULONG_MAX));

    if (self.x == ULONG_MAX) {
        struct u128_t u = { .x = self.x, .y = abs(self.y - v.x) };
        u.y -= self.y;

        RUNTIME_ASSERT(u.y < v.x && u.y < v.y);

        return u;
    }

    struct u128_t u = { .x = self.x - v.x, .y = 0 };
    u.y -= self.y;

    RUNTIME_ASSERT(u.x >= 0);

    return u;
}

LILY_API U128
mul__u128_t(U128 self, U128 v)
{
    RUNTIME_ASSERT(!(self.x == ULONG_MAX && self.y == ULONG_MAX));

    struct u128_t u = { .x = self.x * v.x, .y = 0 };

    RUNTIME_ASSERT(u.x >= 0);

    return u;
}

LILY_API struct Exception
__new__Exception(void *val, void *ext)
{
    RUNTIME_ASSERT(val != NULL || ext != NULL);

    struct Exception self = { .ext = ext, .val = val };

    return self;
}

LILY_API struct Optional
__new__Optional(void *val)
{
    struct Optional self = { .val = val };

    return self;
}

LILY_API void *
get__Optional(struct Optional self)
{
    RUNTIME_ASSERT(self.val != NULL);

    return self.val;
}

LILY_API struct Range
__new__Range(void *start, void *end)
{
    struct Range self = { .start = start, .end = end };

    return self;
}
