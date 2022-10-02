#ifndef LILY_TUPLE_H
#define LILY_TUPLE_H

#include <base/types.h>

typedef struct Tuple
{
    Usize size;
    void **items;
} Tuple;

/**
 *
 * @return new Tuple instance.
 */
struct Tuple *
__new__Tuple(Usize size, ...);

/**
 *
 * @return get value at index.
 */
void *
get__Tuple(struct Tuple self, Usize idx);

/**
 *
 * @brief Free Tuple type.
 */
void
__free__Tuple(struct Tuple *self);

#endif // LILY_TUPLE_H