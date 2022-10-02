#ifndef LILY_MAP_H
#define LILY_MAP_H

#include <base/tuple.h>
#include <base/vec.h>

typedef struct HashMap
{
    struct Vec *keys;
    struct Vec *values;
} HashMap;

/**
 *
 * @brief Construct HashMap type.
 * @return the HashMap instance.
 */
struct HashMap *
__new__HashMap(Usize key_size, Usize value_size);

/**
 *
 * @return the value of specific key.
 */
void *
get__HashMap(struct HashMap self, void *key);

/**
 *
 * @return a Tuple which contains key with value associated.
 */
struct Tuple *
get_key_value__HashMap(struct HashMap self, void *key);

/**
 *
 * @brief Insert a new value couple (key, value) in HashMap.
 */
void
insert__HashMap(struct HashMap *self, void *key, void *value);

/**
 *
 * @brief Free HashMap type.
 */
void
__free__HashMap(struct HashMap *self);

#endif // LILY_MAP_H