#include <base/u128.h>

struct UInt128
__new__UInt128(uint64_t lo, uint64_t hi)
{
    struct UInt128 u = { .lo = lo, .hi = hi };

    return u;
}

struct UInt128
zero__UInt128()
{
    struct UInt128 u = { .lo = 0, .hi = 0 };

    return u;
}
