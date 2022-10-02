#ifndef LILY_RUNTIME_ARRAY_H
#define LILY_RUNTIME_ARRAY_H

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

    struct Array new__Array();
    struct Array init__Array(Usize n, ...);
    Unit push__Array(struct Array *arr, struct Generic *item);
    struct Array *copy__Array(struct Array arr);
    Usize len__Array(struct Array arr);
    Usize capacity__Array(struct Array arr);

#if defined(__cplusplus)
}
#endif

#endif // LILY_RUNTIME_ARRAY_H
