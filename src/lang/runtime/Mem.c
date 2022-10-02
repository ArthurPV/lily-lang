#include "Mem.h"
#include <stdio.h>

LILY_API void *
malloc__Mem(size_t n)
{
    void *p = malloc(n);

    if (!p) {
        RUNTIME_ERROR("bad alloc from malloc");
    }

    return p;
}

LILY_API inline Unit
free__Mem(void *p)
{
    free(p);
}
