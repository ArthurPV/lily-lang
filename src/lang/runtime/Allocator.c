#include "Allocator.h"
#include "Uint64.h"
#include <stdio.h>

LILY_API void *
allocate__Allocator(Usize n, Usize t_size)
{
    if (n == 0)
        return NULL;

    CHECK_U64_RES_OVERFLOW(n, n, 0);

    void *const p = malloc(n * t_size);

    if (!p) {
        RUNTIME_ERROR("bad alloc from allocator");
    }

    return p;
}

inline LILY_API void
deallocate__Allocator(void *const p)
{
    free(p);
}
