#include <base/mem.h>
#include <base/assert.h>

void *lily_malloc(Usize size) {
    void *ptr = malloc(size);

    if (!ptr)
        BAD_ALLOC();

    return ptr;
}

void *lily_realloc(void *ptr, Usize size) {
    ptr = realloc(ptr, size);

    if (!ptr)
        BAD_REALLOC();

    return ptr;
}