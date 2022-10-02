#ifndef LILY_MEM_H
#define LILY_MEM_H

#include <assert.h>
#include <base/types.h>

#define BAD_ALLOC() assert(0 && "bad alloc")

#define BAD_REALLOC() assert(0 && "bad realloc")

/**
 *
 * @return a allocated ptr.
 */
void *
lily_malloc(Usize size);

/**
 *
 * @return a reallocated ptr.
 */
void *
lily_realloc(void *ptr, Usize size);

#endif // LILY_MEM_H
