#include "Array.h"
#include "generic.h"
#include <stdarg.h>
#include <string.h>

LILY_API struct Array
new__Array()
{
    struct Array arr = { .items = malloc(sizeof(struct Generic) * 8),
                         .len = 0,
                         .capacity = 8 };

    return arr;
}

LILY_API struct Array
init__Array(Usize n, ...)
{
    struct Array arr = new__Array();
    va_list vl;

    va_start(vl, n);

    for (MutUsize i = 0; i < n; i++)
        push__Array(&arr, va_arg(vl, struct Generic *));

    va_end(vl);

    return arr;
}

LILY_API Unit
push__Array(struct Array *arr, struct Generic *item)
{
    if (arr->len == arr->capacity) {
        arr->capacity *= 2;
        arr->items =
          realloc(arr->items, sizeof(struct Generic *) * arr->capacity);
    }

    arr->items[arr->len++] = item;
}

LILY_API struct Array *
copy__Array(struct Array arr)
{
    struct Array *copy = malloc(sizeof(struct Array));

    memcpy(&arr, copy, sizeof(struct Array));

    return copy;
}

LILY_API Usize
len__Array(struct Array arr)
{
    return arr.len;
}

LILY_API Usize
capacity__Array(struct Array arr)
{
    return arr.capacity;
}
