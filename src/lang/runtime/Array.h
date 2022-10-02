#ifndef LILY_RUNTIME_ARRAY_H
#define LILY_RUNTIME_ARRAY_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct Array
    {
        struct Generic **items;
        MutUsize len;
        MutUsize capacity;
    } Array;

    LILY_API struct Array new__Array();
    LILY_API struct Array init__Array(Usize n, ...);
    LILY_API Unit push__Array(struct Array *arr, struct Generic *item);
    LILY_API struct Array *copy__Array(struct Array arr);
    LILY_API Usize len__Array(struct Array arr);
    LILY_API Usize capacity__Array(struct Array arr);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ARRAY_H
